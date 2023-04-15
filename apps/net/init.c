/*
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <rtems/error.h>
#include <rtems/rtems_bsdnet.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <bsp.h>

const char rtems_test_name[] = "LOOPBACK";

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_SEMAPHORES 20
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 20
#define CONFIGURE_MAXIMUM_TASKS 20

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 50

#define CONFIGURE_INIT_TASK_STACK_SIZE (10 * 1024)
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_NO_ASR | RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_INIT
rtems_task Init(rtems_task_argument argument);

#include <rtems/confdefs.h>

#include <errno.h>
#include <netinet/in.h>
#include <rtems/error.h>
#include <rtems/rtems_bsdnet.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

extern int rtems_c6678_eth_attach(struct rtems_bsdnet_ifconfig* conf, int attaching);

/*
 * Network configuration
 */
static struct rtems_bsdnet_ifconfig netdriver_config = {
    "c6678", /* name */
    rtems_c6678_eth_attach, /* attach function */
    NULL, /* No more interfaces */
    "192.168.2.163", /* IP address */
    "255.255.255.0", /* IP net mask */
    NULL, /* Driver supplies hardware address */
    0, /* Use default driver parameters */
    0, /* default efficiency multiplier */
    0, /* default udp TX socket buffer size */
    0, /* default udp RX socket buffer size */
    0, /* default tcp TX socket buffer size */
    0, /* default tcp RX socket buffer size */
};

struct rtems_bsdnet_config rtems_bsdnet_config = {
    &netdriver_config, /* Network interface */
    NULL, /* Use fixed network configuration */
    1, /* Default network task priority */
    0, /* Default mbuf capacity */
    0, /* Default mbuf cluster capacity */
    "testSystem", /* Host name */
    "nowhere.com", /* Domain name */
    "192.168.2.255", /* Gateway */
    "192.168.2.1", /* Log host */
    { "192.168.2.1" }, /* Name server(s) */
    { "192.168.2.1" }, /* NTP server(s) */
    0,
    0,
    0,
    0,
    0
};

/*
 * Thread-safe output routines
 */
static rtems_id printMutex;
static void printSafe(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    rtems_semaphore_obtain(printMutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    vprintf(fmt, args);
    rtems_semaphore_release(printMutex);
    va_end(args);
}
#define printf printSafe

/*
 * Spawn a task
 */
static void spawnTask(rtems_task_entry entryPoint, rtems_task_priority priority, rtems_task_argument arg)
{
    rtems_status_code sc;
    rtems_id tid;

    sc = rtems_task_create(rtems_build_name('t', 'a', 's', 'k'),
        priority,
        RTEMS_MINIMUM_STACK_SIZE + (8 * 1024),
        RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_NO_ASR,
        RTEMS_LOCAL,
        &tid);
    if (sc != RTEMS_SUCCESSFUL)
        rtems_panic("Can't create task: %s", rtems_status_text(sc));
    sc = rtems_task_start(tid, entryPoint, arg);
    if (sc != RTEMS_SUCCESSFUL)
        rtems_panic("Can't start task: %s", rtems_status_text(sc));
}

static rtems_task serverTask(rtems_task_argument arg)
{
    int s, s1;
    socklen_t addrlen;
    struct sockaddr_in myAddr, farAddr;

    printf("Create socket.\n");
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
        rtems_panic("Can't create socket: %s\n", strerror(errno));
    memset(&myAddr, 0, sizeof myAddr);
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(8088);
    myAddr.sin_addr.s_addr = inet_addr("192.168.2.163");
    printf("Bind socket.\n");
    if (bind(s, (struct sockaddr*)&myAddr, sizeof myAddr) < 0)
        rtems_panic("Can't bind socket: %s\n", strerror(errno));
    if (listen(s, 5) < 0)
        printf("Can't listen on socket: %s\n", strerror(errno));

    for (;;) {
        addrlen = sizeof farAddr;
        s1 = accept(s, (struct sockaddr*)&farAddr, &addrlen);
        if (s1 < 0)
            if (errno == ENXIO)
                rtems_task_delete(RTEMS_SELF);
            else
                rtems_panic("Can't accept connection: %s", strerror(errno));
        else
            printf("ACCEPTED:%s\n", inet_ntoa(farAddr.sin_addr));

        char buffer[1600];
        int i, sum = 0;

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            i = recv(s1, buffer, sizeof(buffer), 0);
            // printf("Content: %s\n", buffer);
            if (i == 0)
                break;
            sum += send(s1, buffer, i, 0);
            // printf("shoot %d\n", sum);
            if (sum < 0)
                printf("Couldn't send: %s\n", strerror(errno));
        }
        printf("shoot %d\n", sum);
    }
}

void do_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen)
{
#define MAXLINE 2000
    int n;
    socklen_t len;
    char mesg[MAXLINE];
    for (;;) {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}
static rtems_task udp(rtems_task_argument arg)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.2.163");
    servaddr.sin_port = htons(2017);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        goto out;

    do_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

    printf("Client closing connection.\n");
out:
    printf("Test task terminating.\n");

    rtems_task_delete(RTEMS_SELF);
}

static rtems_task net_manager(rtems_task_argument arg)
{
    /*
     * Wait for characters from console terminal
     */
    for (;;) {
        rtems_task_wake_after(50 * rtems_clock_get_ticks_per_second());
        switch (getchar()) {
        case '\004':
            return;

        case 's':
            rtems_bsdnet_show_inet_routes();
            rtems_bsdnet_show_mbuf_stats();
            rtems_bsdnet_show_if_stats();
            rtems_bsdnet_show_ip_stats();
            rtems_bsdnet_show_icmp_stats();
            rtems_bsdnet_show_udp_stats();
            rtems_bsdnet_show_tcp_stats();
            break;
        }
    }
}

/*
 * RTEMS Startup Task
 */
rtems_task
Init(rtems_task_argument ignored)
{
    rtems_status_code sc;

    sc = rtems_semaphore_create(rtems_build_name('P', 'm', 't', 'x'), 1,
        RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_NO_PRIORITY_CEILING | RTEMS_LOCAL, 0, &printMutex);
    if (sc != RTEMS_SUCCESSFUL)
        rtems_panic("Can't create printf mutex:", rtems_status_text(sc));

    printf("\"Network\" initializing!\n");
    rtems_bsdnet_initialize_network();
    printf("\"Network\" initialized!\n");

    spawnTask(serverTask, 2, 1);
    spawnTask(udp, 2, 1);
    // spawnTask(net_manager, 100, 1);

    rtems_task_delete(RTEMS_SELF);
}
