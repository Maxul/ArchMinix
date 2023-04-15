/*===============================================================*\
| Project: RTEMS TMS320C6678 BSP                                  |
+-----------------------------------------------------------------+
| Partially based on the code references which are named below.   |
| Adaptions, modifications, enhancements and any recent parts of  |
| the code are:                                                   |
|                 Copyright (c) 2016                              |
|                 Maxul Lee <lmy2010lmy@gamil.com>                |
|                                                                 |
+-----------------------------------------------------------------+
| The license and distribution terms for this file may be         |
| found in the file LICENSE in this distribution or at            |
|                                                                 |
| http://www.rtems.org/license/LICENSE.                           |
|                                                                 |
+-----------------------------------------------------------------+
| this file contains the networking driver                        |
\*===============================================================*/

/*
 * nimu_eth.c
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *   @file  nimu_eth.c
 *
 *   @brief
 *  Ethernet Packet Driver rewritten using the NIMU Packet
 *  Architecture guidelines.
 *
 *  Note: The NDK nimu driver interface is built based on the examples from the
 *  PDK. Please refer to PDK examples
 *  (<pdk_install_dir>\packages\ti\drv\pa\example\emacExample) directory to get
 *  the knowledge on the QMSS, CPPI, PA LLD configurations/programs.
 *
 *  Revised for the purpose of rtems porting: Max LEE 2016.12.26
 */

#include <rtems.h>
#include <rtems/irq-extension.h>

#include <KeyStone_common.h>

#include "nimu_internalX.h"

#if NETDBG
#define platform_write printf
#else
#define platform_write
#endif

/* CPPI/QMSS Handles used by the application */
#pragma DATA_SECTION(gPaTxQHnd, ".nimu_eth_ll2");
#pragma DATA_SECTION(gTxReturnQHnd, ".nimu_eth_ll2");
#pragma DATA_SECTION(gTxFreeQHnd, ".nimu_eth_ll2");
#pragma DATA_SECTION(gRxFreeQHnd, ".nimu_eth_ll2");
#pragma DATA_SECTION(gRxQHnd, ".nimu_eth_ll2");
Qmss_QueueHnd gPaTxQHnd[NUM_PA_TX_QUEUES], gTxReturnQHnd, gTxFreeQHnd, gRxFreeQHnd, gRxQHnd, gTxCmdReturnQHnd, gTxCmdFreeQHnd;

/* Queues used */
/* PA command response queue handle */
#pragma DATA_SECTION(gPaCfgCmdRespQHnd, ".nimu_eth_ll2");
Qmss_QueueHnd gPaCfgCmdRespQHnd;

/* CPPI Handles */
#pragma DATA_SECTION(gRxFlowHnd, ".nimu_eth_ll2");
Cppi_FlowHnd gRxFlowHnd;

#pragma DATA_SECTION(gPaL2Handles, ".nimu_eth_ll2");
#pragma DATA_SECTION(gPaL3Handles, ".nimu_eth_ll2");
#pragma DATA_SECTION(gPaL4Handles, ".nimu_eth_ll2");
paHandleL2L3_t gPaL2Handles[MAX_NUM_L2_HANDLES];
paHandleL2L3_t gPaL3Handles[MAX_NUM_L3_HANDLES];
paHandleL4_t gPaL4Handles[MAX_NUM_L4_HANDLES];

#pragma DATA_SECTION(gHiPriAccumList, ".nimu_eth_ll2");
#pragma DATA_ALIGN(gHiPriAccumList, 128)
#define MAX_HI_PRI_ACCUM_LIST_SIZE 32
uint32_t gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE * 2];

#pragma DATA_SECTION(gAccChannelNum, ".nimu_eth_ll2");
uint32_t gAccChannelNum;

/* Various stats  */
#pragma DATA_SECTION(gTxCounter, ".nimu_eth_ll2");
#pragma DATA_SECTION(gRxCounter, ".nimu_eth_ll2");
#pragma DATA_SECTION(gTxDropCounter, ".nimu_eth_ll2");
#pragma DATA_SECTION(gRxDropCounter, ".nimu_eth_ll2");
uint32_t gTxCounter = 0, gRxCounter = 0, gTxDropCounter = 0, gRxDropCounter = 0;

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
/* TX port number */
#pragma DATA_SECTION(gTxPort, ".nimu_eth_ll2");
uint32_t gTxPort;

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
uint32_t coreKey[NUM_CORES];
NETIF_DEVICE* GLOBAL_PTR_NET_DEVICE;

/**
 *  @b c6678_emac_start
 *  @n
 *  The function is used to initialize and start the EMAC
 *  controller and device.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int c6678_emac_start()
{
    NETIF_DEVICE* ptr_net_device = GLOBAL_PTR_NET_DEVICE;
    EMAC_DATA* ptr_pvt_data;
    paMacAddr_t broadcast_mac_addr = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    paEthInfo_t ethInfo = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* Src mac = dont care */
        { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 }, /* Default Dest mac */
        0, /* vlan = dont care */
        0, /* ignore ether type */
        0 /* MPLS tag = don't care */
    };
    paRouteInfo_t routeInfo = {
        pa_DEST_HOST, /* Route a match to the host */
        0, /* Flow ID 0 */
        0, /* Destination queue */
        -1, /* Multi route disabled */
        0xaaaaaaaa, /* SwInfo 0 */
        0, /* SwInfo 1 is dont care */
        0, /* customType = pa_CUSTOM_TYPE_NONE */
        0, /* customIndex: not used */
        0, /* pkyType: for SRIO only */
        NULL /* No commands */
    };

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;

    /* Setup Tx */
    if (Setup_Tx() != 0) {
        platform_write("Tx setup failed \n");
        return -1;
    }

    /* Setup Rx */
    if (Setup_Rx() != 0) {
        platform_write("Rx setup failed \n");
        return -1;
    }

    memcpy(&ethInfo.dst[0], ptr_pvt_data->pdi.bMacAddr, sizeof(paMacAddr_t));

    /* Set up the MAC Address LUT*/
    if (Add_MACAddress(&ethInfo, &routeInfo) != 0) {
        platform_write("Add_MACAddress failed \n");
        return -1;
    }

    memcpy(&ethInfo.dst[0], broadcast_mac_addr, sizeof(paMacAddr_t));
    /* Set up the MAC Address LUT for Broadcast */
    if (Add_MACAddress(&ethInfo, &routeInfo) != 0) {
        platform_write("Add_MACAddress failed \n");
        return -1;
    }
    /* Verify the Tx and Rx Initializations */
    if (Verify_Init() != 0) {
        platform_write("Warning:Queue handler Verification failed \n");
    }

    /* Copy the MAC Address into the network interface object here. */
    memcpy(&ptr_net_device->mac_address[0], &ptr_pvt_data->pdi.bMacAddr[0], 6);

    /* Set the 'initial' Receive Filter */
    ptr_pvt_data->pdi.Filter = ETH_PKTFLT_MULTICAST;
    ptr_pvt_data->pdi.TxFree = 1;

    ///////////////////////////////////////////////////////////////////////
    // /* 2017/3/1 */ Open the packet buffer manager
    PBM_open();
    ///////////////////////////////////////////////////////////////////////

    return 0;
}

/**
 *  @b c6678_emac_stop
 *  @n
 *  The function is used to de-initialize and stop the EMAC
 *  controller and device.
 *
 *  @param[in]  ptr_net_device
 *      NETIF_DEVICE structure pointer.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int c6678_emac_stop(NETIF_DEVICE* ptr_net_device)
{
    EMAC_DATA* ptr_pvt_data;
    uint16_t count, i;
    Cppi_Desc* pCppiDesc;
    uint8_t* bufaddr;
    uint32_t bufLen;

    ///////////////////////////////////////////////////////////////////////
    ISR_Level level;
    /* Disable event, interrupt */
#if 0
    EventCombiner_disableEvent(PLATFORM_ETH_EVENTID);
    Hwi_disableInterrupt(PLATFORM_ETH_INTERRUPT);
#endif
    _ISR_Disable(level);
    ///////////////////////////////////////////////////////////////////////

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;
    Osal_nimuFree((Ptr)ptr_pvt_data, platform_roundup(sizeof(EMAC_DATA), PLATFORM_CACHE_LINE_SIZE));

    count = Qmss_getQueueEntryCount(gRxQHnd);

    /* Free the packet data buffer associated with the Rx Descriptor */
    for (i = 0; i < count; i++) {
        /* Need to free up the PBM handle */
        /* free the PBM packet */

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(gRxQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            _ISR_Enable(level);
            return -1;
        }

        /* Get the Address and Length for Free */
        Cppi_getOriginalBufInfo(Cppi_DescType_HOST, pCppiDesc, &bufaddr, &bufLen);
        Osal_nimuFree((Ptr)bufaddr, bufLen);
    }

    count = Qmss_getQueueEntryCount(gRxFreeQHnd);
    /* Free the packet data buffer associated with the Rx Descriptor */
    for (i = 0; i < count; i++) {
        /* Need to free up the PBM handle */
        /* free the PBM packet */

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(gRxFreeQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            _ISR_Enable(level);
            return -1;
        }

        /* Get the Address and Length for Free */
        Cppi_getOriginalBufInfo(Cppi_DescType_HOST, pCppiDesc, &bufaddr, &bufLen);
        Osal_nimuFree((Ptr)bufaddr, bufLen);
    }

    /* tear down queues and dmas */
    res_mgr_stop_qmss();
    Qmss_queueClose(gPaCfgCmdRespQHnd);
    Qmss_queueClose(gTxFreeQHnd);
    Qmss_queueClose(gRxFreeQHnd);
    Qmss_queueClose(gRxQHnd);
    Qmss_queueClose(gTxReturnQHnd);

    for (i = 0; i < NUM_PA_TX_QUEUES; i++)
        Qmss_queueClose(gPaTxQHnd[i]);

    res_mgr_stop_cppi(CPPI_CFG_PASS);

    ///////////////////////////////////////////////////////////////////////
    _ISR_Enable(level);
    ///////////////////////////////////////////////////////////////////////
    /* EMAC Controller has been stopped. */
    return 0;
}
#ifndef SIMULATOR_SUPPORT
/**
 *  @b emac_display_linkstatus
 *  @n
 *      This function is called whenever there is a change in link state on
 *      master core.
 *
 *  @param[in]  port_num
 *      EMAC port number.
 *  @param[in]  link_status
 *      Status of the link.
 *
 *  @retval
 *      void
 */
void emac_display_linkstatus(
    uint32_t port_num,
    uint32_t link_status)
{
    /* This string array corresponds to link state as defined in csl_mdio.h */
    char* LinkStr[] = { "No Link",
        "10Mb/s Half Duplex",
        "10Mb/s Full Duplex",
        "100Mb/s Half Duplex",
        "100Mb/s Full Duplex",
        "1000Mb/s Full Duplex" };

    platform_write("Port %d Link Status: %s on PHY %d\n",
        port_num, LinkStr[link_status],
        platform_get_phy_addr(port_num));
}
#endif

#include "network.h"

static Bool gIsPingListUsed = 0;
rtems_isr EmacRxPktISR(rtems_vector_number vector)
{
    NETIF_DEVICE* ptr_net_device = GLOBAL_PTR_NET_DEVICE;
    uint32_t protocol, pktLen;
    uint8_t* pBuffer;
    Cppi_HostDesc* pHostDesc;
    PBM_Handle hPkt;
    Cppi_Desc* pCppiDesc;
    uint32_t count, i;
    uint32_t* ListAddress;
    EMAC_DATA* ptr_pvt_data;
    PBM_Pkt* rx_pbm_pkt;
    void* key;
    void* accum_list_ptr;

#if 0
    /* Disable the interrupt */
    coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] =  Hwi_disableInterrupt(PLATFORM_ETH_INTERRUPT); //Hwi_disable();
#endif
    /* Disable the interrupt */
    // coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)] = _ISR_Get_level();
    _ISR_Disable(coreKey[CSL_chipReadReg(CSL_CHIP_DNUM)]);

    /* Begin Critical Section before accessing shared resources. */
    key = Osal_qmssCsEnter();

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;

    if (!gIsPingListUsed) {
        accum_list_ptr = (void*)&gHiPriAccumList[0];
    } else {
        accum_list_ptr = (void*)&gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE];
    }

    /* Invalidate cache if needed --
     *   if accumulator is in DDR then INV L2.
     *   if accumulator is in shared RAM (MSMC) invalidate L1
     */
    if ((uint32_t)(gHiPriAccumList)&EMAC_EXTMEM) {
        CACHE_invL2(accum_list_ptr, sizeof(gHiPriAccumList) / 2, CACHE_WAIT);
    }

    if ((uint32_t)(gHiPriAccumList)&EMAC_MSMCSRAM) {
        CACHE_invL1d(accum_list_ptr, sizeof(gHiPriAccumList) / 2, CACHE_WAIT);
    }

    i = MAX_HI_PRI_ACCUM_LIST_SIZE - 1 - (RX_INT_THRESHOLD);
    ListAddress = (uint32_t*)Convert_CoreLocal2GlobalAddr((uint32_t)&gHiPriAccumList[i]);

    /* Process ISR.
     *
     * Get the number of entries in accumulator list.
     * The hardware enqueues data alternatively to Ping/Pong buffer lists in
     * the accumulator. Hence, we need to track which list (Ping/Pong)
     * we serviced the last time and accordingly process the other one
     * this time around.
     */
    if (!gIsPingListUsed) {
        /* Serviced Pong list last time. So read off the Ping list now */
        count = ListAddress[0];
    } else {
        /* Serviced Pong list last time. So read off the Ping list now */
        count = ListAddress[RX_INT_THRESHOLD + 1];
    }

    /* Nothing to receive, so return... */
    if (count == 0) {
        /* End Critical Section */
        Osal_qmssCsExit(key);
#if 0
        Hwi_restoreInterrupt(PLATFORM_ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);
#endif
        _ISR_Enable(coreKey[CSL_chipReadReg(CSL_CHIP_DNUM)]);
        /* Clear INTD */
        Qmss_ackInterrupt(PA_ACC_CHANNEL_NUM, 1);
        Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);
        return; /* Not enough packets are received */
    }

    /* Process all the Results received
     *
     * Skip the first entry in the list that contains the
     * entry count and proceed processing results.
     */
    for (i = 1; i <= count; i++) {
        gRxCounter++;

        /* Get the result descriptor.
         * The hardware enqueues data alternatively to Ping/Pong buffer lists in
         * the accumulator. Hence, we need to track which list (Ping/Pong)
         * we serviced the last time and accordingly process the other one
         * this time around.
         */
        if (!gIsPingListUsed) {
            /* Serviced Pong list last time. So read off the Ping list now */
            pCppiDesc = (Cppi_Desc*)ListAddress[i];
        } else {
            /* Serviced Ping list last time. So read off the Pong list now
             * Skip over Ping list length to arrive at Pong list start.
             */
            pCppiDesc = (Cppi_Desc*)ListAddress[i + RX_INT_THRESHOLD + 1];
        }

        /* Descriptor size appended to the address in the last 4 bits.
         * To get the true descriptor size, always mask off the last
         * 4 bits of the address.
         */
        pCppiDesc = (Ptr)((uint32_t)pCppiDesc & 0xFFFFFFF0);
        pHostDesc = (Cppi_HostDesc*)pCppiDesc;

        /* Invalidate cache based on where the memory is */
        if ((uint32_t)(pHostDesc)&EMAC_EXTMEM) {
            CACHE_invL2((void*)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
        }
        if ((uint32_t)(pHostDesc)&EMAC_MSMCSRAM) {
            CACHE_invL1d((void*)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
        }
        if ((uint32_t)(pHostDesc->buffPtr) & EMAC_EXTMEM) {
            CACHE_invL2((void*)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
        }
        if ((uint32_t)(pHostDesc->buffPtr) & EMAC_MSMCSRAM) {
            CACHE_invL1d((void*)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
        }

        /*
         * We should not see packets too large but check anyways ...
         * Note that we are subtracting off the FCS the switch added to the frame.
         * If its too large then return it to the free queue.
         */
        if ((pHostDesc->buffLen - 4) > (ptr_net_device->mtu + ETHHDR_SIZE)) {
            /* lets try the next one... we should record this as a too large.... */
            gRxDropCounter++;
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            QMSS_QPUSH(gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
            continue;
        }

        /* Allocate the PBM packet for the Max MTU size*/
        hPkt = PBM_alloc(1514);
        if (NULL == hPkt) {
            /* could not get a free NDK packet, maybe the next time around we can... */
            gRxDropCounter++;
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            QMSS_QPUSH(gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
            continue;
        }

        rx_pbm_pkt = (PBM_Pkt*)hPkt;

        PBM_setDataOffset((PBM_Handle)hPkt, 0);

        /* removing the FCS length the EMAC switch adds  here */
        pktLen = (pHostDesc->buffLen - 4);

        /* Set to minimum packet size */
        if (pktLen < 60) {
            pktLen = 64;
        }

        PBM_setValidLen((PBM_Handle)hPkt, pktLen);

        /* Handle raw frames separately, i.e. check the
         * Ethernet Protocol type in this packet and see
         * if its a well known ether type. If so, this is normal
         * IP stream, enqueue this is in usual Rx queue and let the
         * stack know that a packet has arrived for it. However, if
         * the Ethernet type in the packet is not a well known one,
         * this could be a custom raw Ethernet packet, enqueue it
         * separately in the Raw Rx queue and notify stack. The Raw
         * Ethernet packets when being handed over are given
         * preferential treatment and are serviced before the normal
         * IP traffic. Hence the 2 queues.
         */
        pBuffer = (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)pHostDesc->buffPtr);

        /* Extract the Ethernet type from the packet. */
        protocol = (pBuffer[12] << 8) | pBuffer[13];
        protocol = (protocol & 0xFFFFu);

        PBM_setIFRx((PBM_Handle)hPkt, (HANDLE)protocol);

        if (0xff != pBuffer[14 + 20 - 1]) ////////////////////////
            /* Copy the data buffer received to the allocated PBM packet */
            memcpy((uint8_t*)rx_pbm_pkt->pDataBuffer, (uint8_t*)pBuffer, pktLen);

#if 0
        if (0xff != pBuffer[14+20-1]) {
        	static int xx = 0;
        	printf("#%d\tethernet frame length: %d\n", ++xx, pktLen);
            dbgprintf(pBuffer, 12);
            dbgprintf(pBuffer+14+20-8, 8);
            dbgprintf(pBuffer+14+20, 4);
            stack_main(pBuffer);
        }
#endif

#if 0
        /* Is it a standard ethernet type? */
        if (protocol != ETHERTYPE_IP && protocol != ETHERTYPE_IPV6 /*&& protocol != ETHERTYPE_VLAN
            && protocol != ETHERTYPE_PPPOECTL && protocol != ETHERTYPE_PPPOEDATA*/ )
        {
            /* This is a raw packet, enqueue in Raw Rx Queue */
            PBMQ_enq( &ptr_pvt_data->pdi.PBMQ_rawrx, (PBM_Handle) hPkt);
        }
        else
        {   /* This is a normal IP packet. Enqueue in Rx Queue */
            PBMQ_enq( &ptr_pvt_data->pdi.PBMQ_rx, (PBM_Handle) hPkt );
        }
#endif
        PBMQ_enq(&ptr_pvt_data->pdi.PBMQ_rx, (PBM_Handle)hPkt);

        /* Free the packet back to the Rx FDQ */
        pHostDesc->buffLen = pHostDesc->origBufferLen;
        QMSS_QPUSH(gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
    }

    ListAddress = (uint32_t*)Convert_CoreLocal2GlobalAddr((uint32_t)&gHiPriAccumList[0]);

    /* Clear the accumulator list and save whether we used Ping/Pong
     * list information for next time around.
     */
    if (!gIsPingListUsed) {
        /* Just processed Ping list */
        gIsPingListUsed = 1;

        i = sizeof(gHiPriAccumList) / 2;

        /* Clear the accumulator list after processing */
        memset((void*)&gHiPriAccumList[0], 0, i);

        if (
            ((uint32_t)(&gHiPriAccumList[0]) & EMAC_EXTMEM) || ((uint32_t)(&gHiPriAccumList[0]) & EMAC_MSMCSRAM)) {
            /* This needs to be enabled if gHiPriAccumList is in External Memory or MSMCMEM */
            CACHE_wbL2((void*)&gHiPriAccumList[0], i, CACHE_WAIT);
        }
    } else {
        /* Just processed Pong list */
        gIsPingListUsed = 0;
        i = sizeof(gHiPriAccumList) / 2;

        /* Clear the accumulator list after processing */
        memset((void*)&gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE], 0, i);

        if (
            ((uint32_t)(&gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE]) & EMAC_EXTMEM) || ((uint32_t)(&gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE]) & EMAC_MSMCSRAM)) {
            /* This needs to be enabled if gHiPriAccumList is in External Memory or MSMCMEM */
            CACHE_wbL2((void*)&gHiPriAccumList[MAX_HI_PRI_ACCUM_LIST_SIZE], i, CACHE_WAIT);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    /* Notify RX daemon of pending Rx Ethernet packet */
    // STKEVENT_signal( ptr_pvt_data->pdi.hEvent, STKEVENT_ETHERNET, 1 );
    extern rtems_id _gRx_id;
    rtems_bsdnet_event_send(_gRx_id, START_RECEIVE_EVENT);
    ///////////////////////////////////////////////////////////////////////

    /* End Critical Section */
    Osal_qmssCsExit(key);
#if 0
    Hwi_restoreInterrupt(PLATFORM_ETH_INTERRUPT, coreKey [CSL_chipReadReg (CSL_CHIP_DNUM)]);
#endif
    _ISR_Enable(coreKey[CSL_chipReadReg(CSL_CHIP_DNUM)]);

    /* Clear INTD */
    Qmss_ackInterrupt(PA_ACC_CHANNEL_NUM, 1);
    Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);

    return;
}

/**
 *  @b c6678_emac_init
 *  @n
 *  The function is used to initialize and register the EMAC
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int c6678_emac_init(void)
{
    NETIF_DEVICE* ptr_device;
    EMAC_DATA* ptr_pvt_data;
    char names[6][5] = { "eth0", "eth1", "eth2", "eth3", "eth4", "eth5" };

    PLATFORM_EMAC_EXT_info emac_info;

    /* Get the core number. */
    uint32_t coreNum = platform_get_coreid();

    uint32_t i;

    /* Allocate memory for the private data */
    ptr_pvt_data = Osal_nimuMalloc(platform_roundup(sizeof(EMAC_DATA),
                                       PLATFORM_CACHE_LINE_SIZE),
        PLATFORM_CACHE_LINE_SIZE);

    if (ptr_pvt_data == NULL) {
        platform_write("Error: Unable to allocate private memory data\n");
        return -1;
    }

    /* Initialize the allocated memory block. */
    mmZeroInit(ptr_pvt_data, sizeof(EMAC_DATA));

    /* Initialize the RX Queue */
    PBMQ_init(&ptr_pvt_data->pdi.PBMQ_rx);
    PBMQ_init(&ptr_pvt_data->pdi.PBMQ_rawrx);

    /* Initialize the private data */
    mmZeroInit(&ptr_pvt_data->pdi, sizeof(PDINFO));

    /* Set physical index */
    ptr_pvt_data->pdi.PhysIdx = coreNum;
    // ptr_pvt_data->pdi.hEvent  = hEvent;

    /* MCast List is EMPTY */
    ptr_pvt_data->pdi.MCastCnt = 0;

#ifndef SIMULATOR_SUPPORT
    for (i = 0; i < PLATFORM_MAX_EMAC_PORT_NUM; i++) {
        platform_get_emac_info(i, &emac_info);
        if (emac_info.mode == PLATFORM_EMAC_PORT_MODE_PHY) {
            break;
        }
    }
    if (i < PLATFORM_MAX_EMAC_PORT_NUM) {
        gTxPort = emac_info.port_num;
        memcpy(ptr_pvt_data->pdi.bMacAddr, emac_info.mac_address, 6);
    } else {
        platform_write("Error: Unable to find a TX EMAC port\n");
        return -1;
    }
#else
    {
        gTxPort = 1;
        platform_get_emac_info(1, &emac_info);
        memcpy(ptr_pvt_data->pdi.bMacAddr, emac_info.mac_address, 6);
    }
#endif

    /* Init Logical Device */
    /* ptr_pvt_data->pdi.hEther = hEther; */

    /* Allocate memory for the EMAC. */
    /*  ptr_device needs to be allocated via mmAlloc since NDK frees it during shutdown */
    ptr_device = mmAlloc(sizeof(NETIF_DEVICE));

    if (ptr_device == NULL) {
        platform_write("Error: Unable to allocate memory for the EMAC\n");
        return -1;
    }

    /* Initialize the allocated memory block. */
    mmZeroInit(ptr_device, sizeof(NETIF_DEVICE));

    /* Populate the Network Interface Object. */
    strcpy(ptr_device->name, names[coreNum]);
    ptr_device->mtu = ETH_MAX_PAYLOAD - ETHHDR_SIZE;
    ptr_device->pvt_data = (void*)ptr_pvt_data;
#if 0
    /* Populate the Driver Interface Functions. */
    ptr_device->start       = EmacStart;
    ptr_device->stop        = EmacStop;
    ptr_device->poll        = EmacPoll;
    ptr_device->send        = EmacSend;
    ptr_device->pkt_service = EmacPktService;
    ptr_device->ioctl       = Emacioctl;
    ptr_device->add_header  = NIMUAddEthernetHeader;
#endif
    /* Init PA LLD */
    if (Init_PASS() != 0) {
        platform_write("PASS init failed \n");
        return -1;
    } else {
        platform_write("PASS successfully initialized \n");
    }

    /* Initialize the CPSW switch */
    if (Init_Cpsw((uint32_t)ptr_device->mtu, ptr_pvt_data->pdi.bMacAddr) != 0) {
        platform_write("Ethernet subsystem init failed \n");
        return -1;
    } else {
        platform_write("Ethernet subsystem successfully initialized \n");
    }
#if 0
    /* Register the device with NIMU */
    if (NIMURegister (ptr_device) < 0)
    {
    	platform_write ("Error: Unable to register the EMAC\n");
        return -1;
    }
#endif
    GLOBAL_PTR_NET_DEVICE = ptr_device;
    platform_write("Registration of the EMAC Successful, waiting for link up ..\n");

    return 0;
}

#include "c6678_emac_setup.h"

///////////////////////////////////////////////////////////////////////
/*
 * 最小化发射装置，只要把内存地址和长度发到这里就可以发出去！！！
 * 完成！！！Max Lee 2017/3/2
 * 注意：调用者必须要自己负责内存管理。
 */
int c6678_emac_send(/*register*/ uint8_t* buffer, /*register*/ uint32_t length)
{
    NETIF_DEVICE* ptr_net_device = GLOBAL_PTR_NET_DEVICE;

    ISR_Level level;
    EMAC_DATA* ptr_pvt_data;
    Cppi_Desc* pCppiDesc;
    uint32_t dataBufferSize, i, count = 0;

    Cppi_HostDesc* pHostDesc;
    uint32_t coreNum;
    Ptr key;

    /* Make sure the driver does not transmit packet less than min. as per the
     * Ethernet standards. */
    if (length < 60)
        return -1;

    /* Fill out MAC address. */
    // memcpy (buffer + 6, GLOBAL_PTR_NET_DEVICE->mac_address, 6);

    /* Disable all interrupts. */
    _ISR_Disable(level);

    /* Begin Critical Section before accessing shared resources. */
    key = Osal_cppiCsEnter();

    /* Get the core number. */
    coreNum = platform_get_coreid();

    count = Qmss_getQueueEntryCount(gTxReturnQHnd);

    for (i = 0; i < count; i++) {
        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(gTxReturnQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            goto error_cond;
        }
        pHostDesc = (Cppi_HostDesc*)pCppiDesc;

        /* Clear the PBM Handle set in the descriptor  */
        pHostDesc->softwareInfo0 = NULL;

        /* Push descriptor to Tx free queue */
        QMSS_QPUSH(gTxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
    }

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;

    /* We do not do any packet size checks here. If the packet
     * is too big to fit in the MTU configured on the peripheral,
     * then the driver/CSL layer should catch the error.
     */

    ptr_pvt_data->pdi.PhysIdx = coreNum;

    /* Pass the packet to the controller if the transmitter is free. */
    if (ptr_pvt_data->pdi.TxFree) {
        // TODO flush
        /* Clean the cache for external/L2 addresses */
        if (((uint32_t)buffer & EMAC_EXTMEM) || ((uint32_t)buffer & EMAC_LL2SRAM)) {
#if 0
              OEMCacheClean( (void *)buffer, length );
#endif
        }

        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(gTxFreeQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            gTxDropCounter++;
            goto error_cond;
        }

        pHostDesc = (Cppi_HostDesc*)pCppiDesc;
        pHostDesc->softwareInfo0 = (uint32_t)buffer; // hPkt;
        dataBufferSize = length;

        Cppi_setData(Cppi_DescType_HOST,
            (Cppi_Desc*)pCppiDesc,
            (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)buffer),
            dataBufferSize);
        Cppi_setPacketLen(Cppi_DescType_HOST, (Cppi_Desc*)pCppiDesc, dataBufferSize);

        count = Qmss_getQueueEntryCount(gTxReturnQHnd);

#ifdef PA_LOOPBACK
        QMSS_QPUSH(gPaTxQHnd[0], pCppiDesc, dataBufferSize, SIZE_HOST_DESC, Qmss_Location_TAIL);
#else
        Cppi_setPSFlags(Cppi_DescType_HOST, (Cppi_Desc*)pCppiDesc, (1 << gTxPort));
        /* Send the packet out the mac. It will loop back to PA if the mac/switch
         * have been configured properly
         */
        // printf("c6678_emac_send %d\n", dataBufferSize);
        QMSS_QPUSH(gPaTxQHnd[8], pCppiDesc, dataBufferSize, SIZE_HOST_DESC, Qmss_Location_TAIL);
#endif
        /* Increment the application transmit counter */
        gTxCounter++;
    }
    /* Packet has been successfully transmitted. */
    /* End Critical Section */
    Osal_cppiCsExit(key);
    _ISR_Enable(level);
    return 0;

error_cond:
    /* End Critical Section */
    Osal_cppiCsExit(key);
    _ISR_Enable(level);
    return -1;
}

#include <sys/mbuf.h>
int c6678_eth_count()
{
    NETIF_DEVICE* ptr_net_device = GLOBAL_PTR_NET_DEVICE;

    EMAC_DATA* ptr_pvt_data;
    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;

    return PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rx);
}

void c6678_eth_recvpacket(int* plen, struct mbuf* m)
{
    NETIF_DEVICE* ptr_net_device = GLOBAL_PTR_NET_DEVICE;

    EMAC_DATA* ptr_pvt_data;
    PBM_Handle hPacket;
    PBM_Pkt* ptr_pkt;
    int pktlen;

    /* Get the pointer to the private data */
    ptr_pvt_data = (EMAC_DATA*)ptr_net_device->pvt_data;
#if 0
retry:
    if (PBMQ_count(&ptr_pvt_data->pdi.PBMQ_rx) <= 0) {
    	//usleep(10);
    	goto retry;
    }
#endif
    /* Dequeue a packet from the driver receive queue. */
    hPacket = PBMQ_deq(&ptr_pvt_data->pdi.PBMQ_rx);

    /* Prepare the packet so that it can be passed up the networking stack.
     * If this 'step' is not done the fields in the packet are not correct
     * and the packet will eventually be dropped.  */
    PBM_setIFRx(hPacket, ptr_net_device);

    ptr_pkt = (PBM_Pkt*)hPacket;

    // TODO: just return???/////////////////////////////////////////////////////////////////
    /* Make sure we received a valid packet. */
    if (ptr_pkt == NULL)
        return;

    /* Basic validations: The received packet should conform to the Ethernet standards */
    if ((ptr_pkt->ValidLen < ETH_MIN_PAYLOAD) || (ptr_pkt->ValidLen > ETH_MAX_PAYLOAD)) {
        printf("c6678_eth_recvpacket: Bad Size; Payload is %d bytes", ptr_pkt->ValidLen);
        goto recvpacket_out;
    }

    *plen = ptr_pkt->ValidLen;
    pktlen = ptr_pkt->ValidLen;

    /* Copy the received packet into an mbuf */
    memcpy((char*)m->m_data, (char*)(ptr_pkt->pDataBuffer + ptr_pkt->DataOffset), *plen);

recvpacket_out:
    /* Release the packets memory */
    PBM_free(ptr_pkt);
}

///////////////////////////////////////////////////////////////////////

/* Nothing past this point */
