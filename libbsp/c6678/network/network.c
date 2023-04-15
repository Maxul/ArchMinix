/*
 *  TMS320C6678 Ethernet driver
 *
 *  Copyright (c) 2016
 *  Written by Maxul Lee <lmy2010lmy@gamil.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems.h>
#include <rtems/irq-extension.h>

#include <rtems/rtems_bsdnet.h>

#include <assert.h>
#include <errno.h>
#include <rtems/bspIo.h>
#include <rtems/error.h>

#include <sys/mbuf.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/sockio.h>

#include <net/if.h>

#include <netinet/if_ether.h>
#include <netinet/in.h>

#include <bsp/irq.h>

#include "network.h"
#include <stdio.h>

rtems_id _gRx_id;

/* function prototypes */
int rtems_c6678_eth_attach(struct rtems_bsdnet_ifconfig* conf, int attaching);
void c6678_eth_init(void* arg);
static void c6678_emac_init_hw(c6678_eth_softc_t* sc);
void c6678_eth_start(struct ifnet* ifp);
void c6678_eth_stop(c6678_eth_softc_t* sc);
void c6678_eth_tx_task(void* arg);
static void c6678_eth_sendpacket(struct ifnet* ifp, struct mbuf* m);
void c6678_eth_rx_task(void* arg);
void c6678_eth_stats(c6678_eth_softc_t* sc);
static int c6678_eth_ioctl(struct ifnet* ifp, ioctl_command_t command, caddr_t data);

int rtems_c6678_eth_attach(struct rtems_bsdnet_ifconfig* config, int attach)
{
    struct ifnet* ifp;

    int mtu;
    int unitnumber;
    char* unitname;

    /*
     * Parse driver name
     */
    unitnumber = rtems_bsdnet_parse_driver_name(config, &unitname);

    /*
     * Is driver free?
     */
    if (unitnumber < 0) {
        printf("Bad c6678 unit number.\n");
        return 0;
    }

    ifp = &softc.arpcom.ac_if;
    if (ifp->if_softc != NULL) {
        printf("Driver already in use.\n");
        return 0;
    }

    if (config->mtu) {
        mtu = config->mtu;
    } else {
        mtu = ETHERMTU;
    }

    if (config->hardware_address != NULL) {
        softc.is_hwaddr_set = 1;
        memcpy(softc.arpcom.ac_enaddr, config->hardware_address, ETHER_ADDR_LEN);
    } else {
        softc.is_hwaddr_set = 0;
    }

    softc.accept_bcast = !config->ignore_broadcast;

    /*
     * Set up network interface values
     */
    ifp->if_softc = &softc;
    ifp->if_unit = unitnumber;
    ifp->if_name = unitname;
    ifp->if_mtu = mtu;
    ifp->if_init = c6678_eth_init;
    ifp->if_ioctl = c6678_eth_ioctl;
    ifp->if_start = c6678_eth_start;
    ifp->if_output = ether_output;
    ifp->if_flags = IFF_BROADCAST;
    if (ifp->if_snd.ifq_maxlen == 0) {
        ifp->if_snd.ifq_maxlen = ifqmaxlen;
    }

    /* Attach the interface */
    if_attach(ifp);
    ether_ifattach(ifp);

    printk("network device '%s' initialized\n", config->name);
    return 0;
}

void c6678_eth_init(void* arg)
{
    c6678_eth_softc_t* sc = arg;
    struct ifnet* ifp = &sc->arpcom.ac_if;

    /*
     *This is for stuff that only gets done once (c6678_eth_init()
     * gets called multiple times
     */
    if (sc->tx_task == 0) {
        /* Set up ENET hardware */
        c6678_emac_init_hw(sc);

        /* Start driver tasks */
        sc->rx_task = rtems_bsdnet_newproc("ENrx",
            4096,
            c6678_eth_rx_task,
            sc);
        sc->tx_task = rtems_bsdnet_newproc("ENtx",
            4096,
            c6678_eth_tx_task,
            sc);
        _gRx_id = sc->rx_task;
    } /* if tx_task */

    /* Configure for promiscuous if needed */
    if (ifp->if_flags & IFF_PROMISC) { }

    /*
     * Tell the world that we're running.
     */
    ifp->if_flags |= IFF_RUNNING;

} /* c6678_eth_init() */

static void
c6678_emac_init_hw(c6678_eth_softc_t* sc)
{
    rtems_status_code status = RTEMS_SUCCESSFUL;

    // rtems_task_wake_after(1);

    c6678_emac_hw_init();
    /* Install the interrupt handler */
    c6678_emac_init();
    /* Enable TX/RX */
    c6678_emac_start();

    /* Set our ethernet address */
    if (softc.is_hwaddr_set == 0) {

        /* Taken from "pdk_c667x_2_0_1/packages/ti/platform/evmc6678l/platform_lib/src" */
        uint32_t mac_addr2, mac_addr1;

        CSL_BootCfgGetMacIdentifier(&mac_addr1, &mac_addr2);
        softc.arpcom.ac_enaddr[0] = ((mac_addr2 & 0x0000ff00) >> 8);
        softc.arpcom.ac_enaddr[1] = (mac_addr2 & 0x000000ff);

        softc.arpcom.ac_enaddr[2] = ((mac_addr1 & 0xff000000) >> 24);
        softc.arpcom.ac_enaddr[3] = ((mac_addr1 & 0x00ff0000) >> 16);
        softc.arpcom.ac_enaddr[4] = ((mac_addr1 & 0x0000ff00) >> 8);
        softc.arpcom.ac_enaddr[5] = (mac_addr1 & 0x000000ff);

        printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
            softc.arpcom.ac_enaddr[0],
            softc.arpcom.ac_enaddr[1],
            softc.arpcom.ac_enaddr[2],
            softc.arpcom.ac_enaddr[3],
            softc.arpcom.ac_enaddr[4],
            softc.arpcom.ac_enaddr[5]);
    }

    assert(status == RTEMS_SUCCESSFUL);

} /* c6678_emac_init_hw() */

void c6678_eth_start(struct ifnet* ifp)
{
    c6678_eth_softc_t* sc = ifp->if_softc;

    /* wake up the send daemon */
    rtems_bsdnet_event_send(sc->tx_task, START_TRANSMIT_EVENT);

    ifp->if_flags |= IFF_OACTIVE;
}

void c6678_eth_stop(c6678_eth_softc_t* sc)
{
    struct ifnet* ifp = &sc->arpcom.ac_if;

    ifp->if_flags &= ~IFF_RUNNING;

    /* Stop the transmitter and receiver. */
    c6678_emac_stop();
}

/*
 * Driver transmit daemon
 */
void c6678_eth_tx_task(void* arg)
{
    c6678_eth_softc_t* sc = (c6678_eth_softc_t*)arg;
    struct ifnet* ifp = &sc->arpcom.ac_if;
    struct mbuf* m;
    rtems_event_set events;

    for (;;) {
        rtems_bsdnet_event_receive(
            START_TRANSMIT_EVENT,
            RTEMS_EVENT_ANY | RTEMS_WAIT,
            RTEMS_NO_TIMEOUT,
            &events);

        /* Send packets till queue is empty */
        for (;;) {
            /* Get the next mbuf chain to transmit. */
            IF_DEQUEUE(&ifp->if_snd, m);
            if (!m) {
                break;
            }
            c6678_eth_sendpacket(ifp, m);
            softc.stats.tx_packets++;
        }
        ifp->if_flags &= ~IFF_OACTIVE;
    }
}

/* On our board, at least 60-byte packet is required */
#define C6678_ETH_MIN_LEN 60
/* Send packet */
int c6678_emac_send(void* buffer, int length);
static uint8_t tx_buf[ETHER_MAX_LEN];

static void
c6678_eth_sendpacket(struct ifnet* ifp, struct mbuf* m)
{
    c6678_eth_softc_t* sc = ifp->if_softc; // for drop TX stats
    struct mbuf* l = NULL;

    int size = 0, pkt_offset;

    uint8_t* data;
    uint8_t* pkt_data = &tx_buf[0];

    /* How big is the whole packet ? */
    for (l = m; l != NULL; l = l->m_next)
        size += l->m_len;

    /*
     * Packet from stack is smaller than required, for C6678 target
     * it should contain at least 60 bytes
     */
    if (size < C6678_ETH_MIN_LEN) {
        memset(pkt_data, 0, C6678_ETH_MIN_LEN);
        size = C6678_ETH_MIN_LEN; /* Adjust this size to be qualified */
    }

    /* Copy the mbuf chain into the transmit buffer */
    pkt_offset = 0;
    l = m;
    while (l != NULL) {
        memcpy(((char*)tx_buf + pkt_offset), /* offset into pkt for mbuf */
            (char*)mtod(l, void*), /* cast to void */
            l->m_len); /* length of this mbuf */

        pkt_offset += l->m_len; /* update offset */
        l = l->m_next; /* get next mbuf, if any */
    }
#if 0
	static int xx = 0;
	printf("#%d\tc6678_eth_sendpacket %d\n", ++xx, size);

    	uint16_t d;
    	for (d = 0; d < size; d++)
    		printf("%02x, ", pkt_data[d]);
    	puts("");
#endif
    c6678_emac_send((char*)pkt_data, size);

    /* free the mbuf chain we just copied */
    m_freem(m);

} /* c6678_eth_sendpacket () */

/***********************************************************************
 *  Function:   c6678_eth_rx_task
 *
 *  Description: Receiver task
 *
 *  Algorithm: Extract the packet from PBMQ, and place into an mbuf chain.
 *             Place the mbuf chain in the network task queue.
 *
 ***********************************************************************/
void c6678_eth_recvpacket(int* plen, struct mbuf* m);
void c6678_eth_rx_task(void* arg)
{
    c6678_eth_softc_t* sc = (c6678_eth_softc_t*)arg;
    struct ifnet* ifp = &sc->arpcom.ac_if;
    struct mbuf* m;
    struct ether_header* eh;
    rtems_event_set events;
    int pktlen;

    // static uint8_t rx_buf[ETHER_MAX_LEN - 4];

    /* Input packet handling loop */
    while (1) {

        rtems_bsdnet_event_receive(
            START_RECEIVE_EVENT,
            RTEMS_EVENT_ANY | RTEMS_WAIT,
            RTEMS_NO_TIMEOUT,
            &events);
#if 0
    	static int xx = 0;
    	printf("#%d\tc6678_eth_rx_task\n", ++xx);
#endif
        for (;;) {
            /*
             * Allocate an mbuf to give to the stack.
             * The format of the data portion of the RFD is:
             * <ethernet header, payload, FRAME CHECK SEQUENCE / CRC>.
             * This is to be optimized later.... should not have to memcopy!
             */

            /* get an mbuf for this packet */
            MGETHDR(m, M_WAIT, MT_DATA);

            /* now get a cluster pointed to by the mbuf */
            /* since an mbuf by itself is too small */
            MCLGET(m, M_WAIT);
            m_adj(m, 4); /* so the payload is 4-byte aligned */

            /* set the type of mbuf to ifp (ethernet I/F) */
            m->m_pkthdr.rcvif = ifp;
            m->m_nextpkt = 0;

            /*
             * The receive buffer must be aligned with a cache line
             * boundary.
             */
            {
                // uint32_t *p = mtod(m, uint32_t *);
                //*p = (mtod(m, uint32_t) + 0x1f) & ~0x1f;
            }

            ///////////////////////////////////////////////////////////////////////
            /* copy the received packet into an mbuf */
            c6678_eth_recvpacket(&pktlen, m);
#if 0
        puts("Network Layer");
        dbgprintf((char *)(m->m_data+14+20-8), 8);
        dbgprintf((char *)(m->m_data+14+20), 4);
#endif
            ///////////////////////////////////////////////////////////////////////

            /* set the length of the mbuf */
            /* Adjust the mbuf pointers to skip the header and set eh to point to it */
            m->m_len = pktlen - (sizeof(struct ether_header));
            m->m_pkthdr.len = m->m_len;

            /* strip off the ethernet header from the mbuf */
            /* but save the pointer to it */
            eh = mtod(m, struct ether_header*);
            m->m_data += sizeof(struct ether_header);

            softc.stats.rx_packets++;

            /* give all this stuff to the stack */
            ether_input(ifp, eh, m);

            if (c6678_eth_count() == 0)
                break;
        }
    }
} /* c6678_eth_rx_task */

/* Show interface statistics */
void c6678_eth_stats(c6678_eth_softc_t* sc)
{
    printf(" Total Interrupts:%-8lu", sc->stats.interrupts);
    printf("   Rx Interrupts:%-8lu", sc->stats.rx_interrupts);
    printf("   Tx Interrupts:%-8lu\n", sc->stats.tx_interrupts);
    printf(" Tx Error Interrupts:%-8lu\n", sc->stats.txerr_interrupts);
    printf(" Rx Packets:%-8lu", sc->stats.rx_packets);
    printf("   Tx Packets:%-8lu\n", sc->stats.tx_packets);
}

/*  Driver ioctl handler */
static int
c6678_eth_ioctl(struct ifnet* ifp, ioctl_command_t command, caddr_t data)
{
    c6678_eth_softc_t* sc = ifp->if_softc;
    int error = 0;

    switch (command) {
    case SIOCGIFADDR:
    case SIOCSIFADDR:
        error = ether_ioctl(ifp, command, data);
        break;

    case SIOCSIFFLAGS:
        switch (ifp->if_flags & (IFF_UP | IFF_RUNNING)) {
        case IFF_RUNNING:
            c6678_eth_stop(sc);
            break;

        case IFF_UP:
            c6678_eth_init(sc);
            break;

        case IFF_UP | IFF_RUNNING:
            c6678_eth_stop(sc);
            c6678_eth_init(sc);
            break;

        default:
            break;
        } /* switch (if_flags) */
        break;

    case SIO_RTEMS_SHOW_STATS:
        c6678_eth_stats(sc);
        break;

        /*
         * FIXME: All sorts of multicast commands need to be added here!
         */
    default:
        error = EINVAL;
        break;
    } /* switch (command) */
    return error;
}
