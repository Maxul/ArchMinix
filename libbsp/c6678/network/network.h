/* BootCfg module include */
#include <ti/csl/csl_bootcfg.h>
#include <ti/csl/csl_bootcfgAux.h> /* CSL_BootCfgGetMacIdentifier() */

#include <rtems.h>
#include <rtems/rtems_bsdnet.h>

#include <sys/mbuf.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/sockio.h>

#include <net/if.h>

#include <netinet/if_ether.h>
#include <netinet/in.h>

typedef struct {
    unsigned long rx_packets; /* total packets received         */
    unsigned long tx_packets; /* total packets transmitted      */
    unsigned long rx_bytes; /* total bytes received           */
    unsigned long tx_bytes; /* total bytes transmitted        */
    unsigned long interrupts; /* total number of interrupts     */
    unsigned long rx_interrupts; /* total number of rx interrupts  */
    unsigned long tx_interrupts; /* total number of tx interrupts  */
    unsigned long txerr_interrupts; /* total number of tx error interrupts  */

} eth_stats_t;

/*
 * Hardware-specific storage
 */
typedef struct
{
    /*
     * Connection to networking code
     * This entry *must* be the first in the sonic_softc structure.
     */
    struct arpcom arpcom;

    int accept_bcast;

    /* Tasks waiting for interrupts */
    rtems_id rx_task;
    rtems_id tx_task;

    eth_stats_t stats;

    bool is_hwaddr_set;

} c6678_eth_softc_t;

static c6678_eth_softc_t softc;

/*  RTEMS event used by interrupt handler to signal receive daemon. */
#define START_RECEIVE_EVENT RTEMS_EVENT_1

/* RTEMS event used to start transmit daemon. */
#define START_TRANSMIT_EVENT RTEMS_EVENT_2
