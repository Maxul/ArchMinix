#include "nimu_internalX.h"

#include <rtems.h>
#include <rtems/irq-extension.h>

#include <KeyStone_common.h>

/** ============================================================================
 *   @n@b Setup_Tx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for sending data to PASS/Ethernet. It sets up a Tx free descriptor queue,
 *      PASS Tx queues required for send.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Tx(void)
{
    uint8_t isAllocated, i;
    Qmss_Queue qRetInfo;
    Ptr pCppiDesc;
    uint32_t mySWInfo[] = { 0x00000000, 0x00000000, 0x00000000 };

    /* Open all Transmit (Tx) queues.
     *
     * These queues are used to send data to PA PDSP/CPSW.
     */
    for (i = 0; i < NUM_PA_TX_QUEUES; i++) {

        if ((gPaTxQHnd[i] = Qmss_queueOpen(Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
            platform_write("Error opening PA Tx queue \n");
            return -1;
        }
    }

    /* Open a Tx Free Descriptor Queue (Tx FDQ).
     *
     * This queue will be used to hold Tx free decriptors that can be filled
     * later with data buffers for transmission onto wire.
     */
    if ((gTxFreeQHnd = Qmss_queueOpen(Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening Tx Free descriptor queue \n");
        return -1;
    }

    /* Open a Tx Return Descriptor Queue (Tx Transmit CompletionQ).
     *
     * This queue will be used to hold Tx completed decriptors that can be filled
     * later with data buffers for transmission onto wire.
     */
    if ((gTxReturnQHnd = Qmss_queueOpen(Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening Tx Return descriptor queue \n");
        return -1;
    }

    qRetInfo = Qmss_getQueueNumber(gTxReturnQHnd);

    /* Attach some free descriptors to the Tx free queue we just opened. */
    for (i = 0; i < NIMU_NUM_TX_DESC; i++) {
        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(res_mgr_qmss_get_freeq(), QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            break;
        }

        /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the completion q we just
         * setup instead of the global free queue.
         */
        Cppi_setReturnQueue((Cppi_DescType)Cppi_DescType_HOST, pCppiDesc, qRetInfo);

        /* Software info is inteded to hold the PBM Pkt Handle for buffer management */
        mySWInfo[1] = i;
        Cppi_setSoftwareInfo(Cppi_DescType_HOST, pCppiDesc, (uint8_t*)mySWInfo);

        /* Push descriptor to Tx free queue */
        QMSS_QPUSHDESCSIZE(gTxFreeQHnd, pCppiDesc, SIZE_HOST_DESC);
    }
    if (i != NIMU_NUM_TX_DESC) {
        platform_write("Error allocating Tx free descriptors \n");
        return -1;
    }

    /* Open a Tx Command Free Descriptor Queue (Tx CMD FDQ).
     *
     * This queue will be used to hold Tx Command free decriptors
     */
    if ((gTxCmdFreeQHnd = Qmss_queueOpen(Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening Tx Cmd Free descriptor queue \n");
        return -1;
    }

    /* Open a Tx Cmd Return Descriptor Queue (Tx Cmd Completion Queue).
     *
     * This queue will be used to hold Tx command completed decriptors
     */
    if ((gTxCmdReturnQHnd = Qmss_queueOpen(Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening Tx Return descriptor queue \n");
        return -1;
    }

    qRetInfo = Qmss_getQueueNumber(gTxCmdReturnQHnd);

    /* Attach some free descriptors to the Tx CMD free queue we just opened. */
    for (i = 0; i < NIMU_MAX_NUM_TX_CMD_DESC; i++) {
        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(res_mgr_qmss_get_freeq(), QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            break;
        }

        /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the completion q we just
         * setup instead of the global free queue.
         */
        Cppi_setReturnQueue((Cppi_DescType)Cppi_DescType_HOST, pCppiDesc, qRetInfo);

        /* Push descriptor to Tx free queue */
        QMSS_QPUSHDESCSIZE(gTxCmdFreeQHnd, pCppiDesc, SIZE_HOST_DESC);
    }
    if (i != NIMU_MAX_NUM_TX_CMD_DESC) {
        platform_write("Error allocating Tx Command free descriptors \n");
        return -1;
    }

    /* All done with Rx configuration. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Setup_Rx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for receiving data from PASS/Ethernet. It sets up a Rx free descriptor queue
 *      with some empty pre-allocated buffers to receive data, and an Rx queue
 *      to which the Rxed data is streamed for the example application. This API
 *      also sets up the QM high priority accumulation interrupts required to
 *      receive data from the Rx queue.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Rx()
{
    int32_t result;
    // int32_t				vectId;
    uint8_t isAllocated, accChannelNum, i;
    uint16_t numAccEntries, intThreshold, pktLen;
    Qmss_Queue rxFreeQInfo, rxQInfo;
    Ptr pCppiDesc;
    Qmss_AccCmdCfg accCfg;
    Cppi_RxFlowCfg rxFlowCfg;
    // int16_t             eventId;
    Ptr pDataBuffer;
    uint32_t mySWInfo[] = { 0x11112222, 0x33334444 };
    uint32_t ListAddress;

    pktLen = 1500 /*ptr_net_device->mtu*/ + ETHHDR_SIZE + 4; /* ETh Header + 4 bytes of FCS */

    i = MAX_HI_PRI_ACCUM_LIST_SIZE - 1 - (RX_INT_THRESHOLD);
    ListAddress = Convert_CoreLocal2GlobalAddr((uint32_t)&gHiPriAccumList[i]);

    /* Open a Receive (Rx) queue.
     *
     * This queue will be used to hold all the packets received by PASS/CPSW
     *
     * Open the next available High Priority Accumulation queue for Rx.
     */
    if ((gRxQHnd = Qmss_queueOpen(Qmss_QueueType_HIGH_PRIORITY_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening a High Priority Accumulation Rx queue \n");
        return -1;
    }

    rxQInfo = Qmss_getQueueNumber(gRxQHnd);

    /* Setup high priority accumulation interrupts on the Rx queue.
     *
     * Let's configure the accumulator with the following settings:
     *      (1) Interrupt pacing disabled.
     *      (2) Interrupt on every received packet
     */
    intThreshold = RX_INT_THRESHOLD;
    numAccEntries = (intThreshold + 1) * 2;
    accChannelNum = PA_ACC_CHANNEL_NUM;

    /* Initialize the accumulator list memory */
    memset((void*)ListAddress, 0, numAccEntries * 4);

    /* Ensure that the accumulator channel we are programming is not
     * in use currently.
     */
    result = Qmss_disableAccumulator(Qmss_PdspId_PDSP1, accChannelNum);
    if (result != QMSS_ACC_SOK && result != QMSS_ACC_CHANNEL_NOT_ACTIVE) {
        platform_write("Error Disabling high priority accumulator for channel : %d error code: %d\n",
            accChannelNum, result);
        return -1;
    }

    /* Setup the accumulator settings */
    accCfg.channel = accChannelNum;
    accCfg.command = Qmss_AccCmd_ENABLE_CHANNEL;
    accCfg.queueEnMask = 0;
    accCfg.listAddress = ListAddress;
    accCfg.queMgrIndex = gRxQHnd;
    accCfg.maxPageEntries = (intThreshold + 1); /* Add an extra entry for holding the entry count */
    accCfg.timerLoadCount = 40;
    accCfg.interruptPacingMode = Qmss_AccPacingMode_LAST_INTERRUPT;
    accCfg.listEntrySize = Qmss_AccEntrySize_REG_D;
    accCfg.listCountMode = Qmss_AccCountMode_ENTRY_COUNT;
    accCfg.multiQueueMode = Qmss_AccQueueMode_SINGLE_QUEUE;

    /* Program the accumulator */
    if ((result = Qmss_programAccumulator(Qmss_PdspId_PDSP1, &accCfg)) != QMSS_ACC_SOK) {
        platform_write("Error Programming high priority accumulator for channel : %d queue : %d error code : %d\n",
            accCfg.channel, accCfg.queMgrIndex, result);
        return -1;
    }

    /* Register interrupts for the system event corresponding to the
     * accumulator channel we are using.
     */
#if 0
    /* System event n - Accumulator Channel 0 */
    eventId     	=   PLATFORM_ETH_EVENTID;

    /* Pick a interrupt vector id to use */
    vectId          =   PLATFORM_ETH_INTERRUPT;

    platform_write ("Ethernet eventId : %d and vectId (Interrupt) : %d \n", eventId, vectId);

    /* Register our ISR handle for this event */
    EventCombiner_dispatchPlug (eventId, (EventCombiner_FuncPtr)EmacRxPktISR, (UArg)ptr_net_device, TRUE);
    EventCombiner_enableEvent(eventId);

    /* Map the event id to hardware interrupt 7. */
    Hwi_eventMap(vectId, eventId >> 5);

    /* Enable interrupt */
    Hwi_enableInterrupt(vectId);
#endif
    rtems_isr_entry old_handle;

    /* Map the event id to hardware interrupt 7. */
    gpCGEM_regs->INTMUX1 = (CSL_GEM_QM_INT_HIGH_N << CSL_CGEM_INTMUX1_INTSEL7_SHIFT);

    /* Enable interrupt */
    CPU_interrupt_enable(1 << 7);

    /* Register our ISR handle for this event */
    rtems_interrupt_catch(EmacRxPktISR, 7, &old_handle);

    /* Open a Rx Free Descriptor Queue (Rx FDQ).
     *
     * This queue will hold all the Rx free descriptors. These descriptors will be
     * used by the PASS CPDMA to hold data received via CPSW.
     */
    if ((gRxFreeQHnd = Qmss_queueOpen(Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening Rx Free descriptor queue \n");
        return -1;
    }
    rxFreeQInfo = Qmss_getQueueNumber(gRxFreeQHnd);

    /* Attach some free descriptors to the Rx free queue we just opened. */
    for (i = 0; i < NIMU_NUM_RX_DESC; i++) {
        /* Get a free descriptor from the global free queue we setup
         * during initialization.
         */
        if ((QMSS_QPOP(res_mgr_qmss_get_freeq(), QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pCppiDesc)) != NULL) {
            break;
        }

        pktLen = platform_roundup(pktLen, PLATFORM_CACHE_LINE_SIZE);
        if ((pDataBuffer = Osal_nimuMalloc(pktLen, PLATFORM_CACHE_LINE_SIZE)) == NULL) {
            platform_write("Error allocating memory for Rx data buffer \n");
            break;
        }

        /* Populate the Rx free descriptor with the buffer we just allocated. */
        Cppi_setData(Cppi_DescType_HOST, pCppiDesc, (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)pDataBuffer), pktLen);

        /* Save original buffer information */
        Cppi_setOriginalBufInfo(Cppi_DescType_HOST, pCppiDesc, (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)pDataBuffer), pktLen);

        /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the free q we just
         * setup instead of the global free queue.
         */
        Cppi_setReturnQueue(Cppi_DescType_HOST, pCppiDesc, rxFreeQInfo);

        Cppi_setSoftwareInfo(Cppi_DescType_HOST, pCppiDesc, (uint8_t*)mySWInfo);

        Cppi_setPacketLen(Cppi_DescType_HOST, pCppiDesc, pktLen);

        /* Push descriptor to Rx free queue */
        QMSS_QPUSHDESCSIZE(gRxFreeQHnd, pCppiDesc, SIZE_HOST_DESC);
    }
    if (i != NIMU_NUM_RX_DESC) {
        platform_write("Error allocating Rx free descriptors \n");
        return -1;
    }

    /* Setup a Rx Flow.
     *
     * A Rx flow encapsulates all relevant data properties that CPDMA would
     * have to know in order to successfully receive data.
     */
    /* Initialize the flow configuration */
    memset(&rxFlowCfg, 0, sizeof(Cppi_RxFlowCfg));

    /* Let CPPI pick the next available flow */
    rxFlowCfg.flowIdNum = CPPI_PARAM_NOT_SPECIFIED;

    rxFlowCfg.rx_dest_qmgr = rxQInfo.qMgr;
    rxFlowCfg.rx_dest_qnum = rxQInfo.qNum;
    rxFlowCfg.rx_desc_type = Cppi_DescType_HOST;

    rxFlowCfg.rx_ps_location = Cppi_PSLoc_PS_IN_DESC;
    rxFlowCfg.rx_psinfo_present = 1; /* Enable PS info */

    rxFlowCfg.rx_error_handling = 0; /* Drop the packet, do not retry on starvation by default */
    rxFlowCfg.rx_einfo_present = 1; /* EPIB info present */

    rxFlowCfg.rx_dest_tag_lo_sel = 0; /* Disable tagging */
    rxFlowCfg.rx_dest_tag_hi_sel = 0;
    rxFlowCfg.rx_src_tag_lo_sel = 0;
    rxFlowCfg.rx_src_tag_hi_sel = 0;

    rxFlowCfg.rx_size_thresh0_en = 0; /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh1_en = 0; /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh2_en = 0; /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh0 = 0x0;
    rxFlowCfg.rx_size_thresh1 = 0x0;
    rxFlowCfg.rx_size_thresh2 = 0x0;

    rxFlowCfg.rx_fdq0_sz0_qmgr = rxFreeQInfo.qMgr; /* Setup the Receive free queue for the flow */
    rxFlowCfg.rx_fdq0_sz0_qnum = rxFreeQInfo.qNum;
    rxFlowCfg.rx_fdq0_sz1_qnum = 0x0;
    rxFlowCfg.rx_fdq0_sz1_qmgr = 0x0;
    rxFlowCfg.rx_fdq0_sz2_qnum = 0x0;
    rxFlowCfg.rx_fdq0_sz2_qmgr = 0x0;
    rxFlowCfg.rx_fdq0_sz3_qnum = 0x0;
    rxFlowCfg.rx_fdq0_sz3_qmgr = 0x0;

    rxFlowCfg.rx_fdq1_qnum = rxFreeQInfo.qNum; /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq1_qmgr = rxFreeQInfo.qMgr;
    rxFlowCfg.rx_fdq2_qnum = rxFreeQInfo.qNum; /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq2_qmgr = rxFreeQInfo.qMgr;
    rxFlowCfg.rx_fdq3_qnum = rxFreeQInfo.qNum; /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq3_qmgr = rxFreeQInfo.qMgr;

    /* Configure the Rx flow */
    if ((gRxFlowHnd = Cppi_configureRxFlow(res_mgr_cppi_get_passhandle(), &rxFlowCfg, &isAllocated)) == NULL) {
        platform_write("Error configuring Rx flow \n");
        return -1;
    }

    /* All done with Rx configuration. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Init_MAC
 *
 *   @b Description
 *   @n This API initializes the CPGMAC Sliver (MAC Port) port.
 *
 *   @param[in]
 *   @n macPortNum      MAC port number for which the initialization must be done.
 *
 *   @param[in]
 *   @n macAddress      MAC address to configure on this port.
 *
 *   @param[in]
 *   @n mtu             Maximum Frame length to configure on this port.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_MAC(uint32_t macPortNum, uint8_t macAddress[6], uint32_t mtu)
{
    /* Reset MAC Sliver 0 */
    CSL_CPGMAC_SL_resetMac(macPortNum);
    while (CSL_CPGMAC_SL_isMACResetDone(macPortNum) != TRUE)
        ;

    /* Setup the MAC Control Register for this port:
     *      (1) Enable Full duplex
     *      (2) Enable GMII
     *      (3) Enable Gigabit
     *      (4) Enable External Configuration. This enables
     *          the "Full duplex" and "Gigabit" settings to be
     *          controlled externally from SGMII
     *      (5) Don't Enable any control/error frames
     *      (6) Enable short frames
     */
    CSL_CPGMAC_SL_enableFullDuplex(macPortNum);
    CSL_CPGMAC_SL_enableGMII(macPortNum);
    CSL_CPGMAC_SL_enableGigabit(macPortNum);
    CSL_CPGMAC_SL_enableExtControl(macPortNum);

    /* Adding these configurations to allow the switch not to ignore any packets */
    CSL_CPGMAC_SL_enableRxCSF(macPortNum);

    /* Configure the MAC address for this port */
    CSL_CPSW_3GF_setPortMACAddress(macPortNum, macAddress);

    /* Configure VLAN ID/CFI/Priority.
     *
     * For now, we are not using VLANs so just configure them
     * to all zeros.
     */
    CSL_CPSW_3GF_setPortVlanReg(macPortNum, 0, 0, 0);

    /* Configure the Receive Maximum length on this port,
     * i.e., the maximum size the port can receive without
     * any errors.
     *
     * Set the Rx Max length to the MTU configured for the
     * interface.
     */
    CSL_CPGMAC_SL_setRxMaxLen(macPortNum, mtu);

    /* Done setting up the MAC port */
    return;
}

/** ============================================================================
 *   @n@b Init_MDIO
 *
 *   @b Description
 *   @n Not supported at moment. MDIO is not simulated yet.
 *
 *   @param[in]
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_MDIO(void)
{
    /* There is nothing to be done for C6678 EVM */
    return;
}

/** ============================================================================
 *   @n@b Init_Switch
 *
 *   @b Description
 *   @n This API sets up the ethernet switch subsystem and its Address Lookup
 *      Engine (ALE) in "Switch" mode.
 *
 *   @param[in]
 *   @n mtu             Maximum Frame length to configure on the switch.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_Switch(uint32_t mtu)
{
    CSL_CPSW_3GF_PORTSTAT portStatCfg;
    uint32_t rx_max_len = mtu + ETHHDR_SIZE + 4; /* 4 bytes of FCS */

    /* Enable the CPPI port, i.e., port 0 that does all
     * the data streaming in/out of EMAC.
     */
    CSL_CPSW_3GF_enablePort0();
    CSL_CPSW_3GF_disableVlanAware();
    CSL_CPSW_3GF_setPort0VlanReg(0, 0, 0);
    CSL_CPSW_3GF_setPort0RxMaxLen(rx_max_len);

    /* Enable statistics on both the port groups:
     *
     * MAC Sliver ports -   Port 1, Port 2
     * CPPI Port        -   Port 0
     */
    portStatCfg.p0AStatEnable = 1;
    portStatCfg.p0BStatEnable = 1;
    portStatCfg.p1StatEnable = 1;
    portStatCfg.p2StatEnable = 1;
    CSL_CPSW_3GF_setPortStatsEnableReg(&portStatCfg);

    /* Setup the Address Lookup Engine (ALE) Configuration:
     *      (1) Enable ALE.
     *      (2) Clear stale ALE entries.
     *      (3) Disable VLAN Aware lookups in ALE since
     *          we are not using VLANs by default.
     *      (4) No Flow control
     *      (5) Configure the Unknown VLAN processing
     *          properties for the switch, i.e., which
     *          ports to send the packets to.
     */
    CSL_CPSW_3GF_enableAle();
    CSL_CPSW_3GF_clearAleTable();
    CSL_CPSW_3GF_disableAleVlanAware();
    CSL_CPSW_3GF_disableAleTxRateLimit();

    /* Setting the Switch MTU Size to more than needed */
    CSL_CPGMAC_SL_setRxMaxLen(0, rx_max_len);
    CSL_CPGMAC_SL_setRxMaxLen(1, rx_max_len);

    //#ifdef SIMULATOR_SUPPORT
    CSL_CPSW_3GF_enableAleBypass();
    //#endif
    /* Done with switch configuration */
    return;
}

/** ============================================================================
 *   @n@b Switch_update_addr
 *
 *   @b Description
 *   @n This API add/delete entries in the Address Lookup Engine (ALE) in "Switch" mode.
 *
 *   @param[in]
 *   @n portNum         Switch port number.

 *   @param[in]
 *   @n macAddress      MAC address to configure on the switch.
 *
 *   @param[in]
 *   @n add             0:add; 1:delete.
 *
 *   @return
 *   @n None
 *
 *   @Note  It supports "add" operation only now.
 * =============================================================================
 */
int Switch_update_addr(uint32_t portNum, uint8_t macAddress[6], uint16_t add)
{
    CSL_CPSW_3GF_ALE_PORTCONTROL alePortControlCfg;
#ifdef SIMULATOR_SUPPORT
    uint32_t i;
    CSL_CPSW_3GF_ALE_UNICASTADDR_ENTRY ucastAddrCfg;
#endif

    /* Configure the address in "Learning"/"Forward" state */
    alePortControlCfg.portState = ALE_PORTSTATE_FORWARD;
    alePortControlCfg.dropUntaggedEnable = 0;
    alePortControlCfg.vidIngressCheckEnable = 0;
    alePortControlCfg.noLearnModeEnable = 0;
    alePortControlCfg.mcastLimit = 0;
    alePortControlCfg.bcastLimit = 0;

    CSL_CPSW_3GF_setAlePortControlReg(portNum, &alePortControlCfg);

#ifdef SIMULATOR_SUPPORT
    /* Program the ALE with the MAC address.
     *
     * The ALE entries determine the switch port to which any
     * matching received packet must be forwarded to.
     */
    /* Get the next free ALE entry to program */
    for (i = 0; i < CSL_CPSW_3GF_NUMALE_ENTRIES; i++) {
        if (CSL_CPSW_3GF_getALEEntryType(i) == ALE_ENTRYTYPE_FREE) {
            /* Found a free entry */
            break;
        }
    }
    if (i == CSL_CPSW_3GF_NUMALE_ENTRIES) {
        /* No free ALE entry found. return error. */
        return -1;
    } else {
        /* Found a free ALE entry to program our MAC address */
        memcpy(ucastAddrCfg.macAddress, macAddress, 6); // Set the MAC address
        ucastAddrCfg.ucastType = ALE_UCASTTYPE_UCAST_NOAGE; // Add a permanent unicast address entryALE_UCASTTYPE_UCAST_NOAGE.
        ucastAddrCfg.secureEnable = FALSE;
        ucastAddrCfg.blockEnable = FALSE;
        ucastAddrCfg.portNumber = portNum; // Add the ALE entry for this port

        /* Setup the ALE entry for this port's MAC address */
        CSL_CPSW_3GF_setAleUnicastAddrEntry(i, &ucastAddrCfg);
    }
#endif
    /* Done with upading address */
    return 0;
}

/** ============================================================================
 *   @n@b Verify_Init
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PASS CPDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Verify_Init(void)
{
    int32_t count, returnVal = 0, i;
    int32_t num_tx_desc = NIMU_NUM_TX_DESC;
    int32_t num_rx_desc = NIMU_NUM_RX_DESC;
    int32_t max_queue_number = QMSS_MAX_LOW_PRIORITY_QUEUE + QMSS_MAX_PASS_QUEUE + QMSS_MAX_LOW_PRIORITY_QUEUE + QMSS_MAX_PASS_QUEUE + QMSS_MAX_INTC_QUEUE + QMSS_MAX_SRIO_QUEUE + QMSS_MAX_HIGH_PRIORITY_QUEUE + QMSS_MAX_STARVATION_COUNTER_QUEUE + QMSS_MAX_INFRASTRUCTURE_QUEUE + QMSS_MAX_TRAFFIC_SHAPING_QUEUE + QMSS_MAX_GENERAL_PURPOSE_QUEUE;

    /*Verify if we got NIMU_NUM_TX_DESC Tx Free Q*/
    if ((count = Qmss_getQueueEntryCount(gTxFreeQHnd)) != num_tx_desc) {
        platform_write("Verify_Init: Expected %d entry count for gTxFreeQHnd queue %d, found %d entries\n", num_tx_desc, gTxFreeQHnd, count);
        returnVal = -1;
    }

    /* Verify if we got NIMU_NUM_RX_DESC Rx FDQ */
    if ((count = Qmss_getQueueEntryCount(gRxFreeQHnd)) != num_rx_desc) {
        platform_write("Verify_Init: Expected %d entry count for gRxFreeQHnd queue %d, found %d entries\n", num_rx_desc, gRxFreeQHnd, count);
        returnVal = -1;
    }

    /* Verify if we got empty Tx completion Q*/
    if ((count = Qmss_getQueueEntryCount(gTxReturnQHnd)) != 0) {
        platform_write("Verify_Init: Expected 0 entry count for gTxReturnQHnd queue %d, found %d entries\n", gTxReturnQHnd, count);
        returnVal = -1;
    }

    /* Verify if we got NIMU_NUM_RX_DESC Rx FDQ */
    if ((count = Qmss_getQueueEntryCount(gRxQHnd)) != 0) {
        platform_write("Verify_Init: Expected 0 entry count for gRxQHnd= %d, found %d entries\n", gRxQHnd, count);
        returnVal = -1;
    }

    for (i = 0; i < max_queue_number; i++) {
        if ((i == gRxFreeQHnd) || (i == gTxFreeQHnd) || (i == gTxCmdFreeQHnd))
            continue;

        count = Qmss_getQueueEntryCount(i);
        if (count != 0) {
            platform_write("Verify_Init: Expected 0 entry count for Queue number = %d, found %d entries\n", i, count);
        }
    }

    /* Verify_Init Done. Return success */
    return (returnVal);
}

/** ============================================================================
 *   @n@b Init_Cpsw
 *
 *   @b Description
 *   @n This API sets up the entire ethernet subsystem and all its associated
 *      components.
 *
 *   @param[in]
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
int32_t Init_Cpsw(uint32_t mtu, uint8_t* myMACAddress)
{
#ifdef SIMULATOR_SUPPORT

    uint8_t bcMACAddress[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    extern uint8_t clientMACAddress[6];

    /* Initialize the SGMII/Sliver submodules for the MAC port. */
    Init_SGMII(0);
    Init_MAC(0, myMACAddress, mtu);

    Init_SGMII(1);
    Init_MAC(1, myMACAddress, mtu);

    /* Setup the Ethernet switch finally. */
    Init_Switch(mtu);

    /* Update the ALE Entries and ensure that these are correctly configured.
     * There are 2 Entries created here:
     *  Entry1: My OWN MAC Address goes to Port 0
     *  Entry2: Destination MAC Address is forwarded to Port1
     * If there are more destination MAC Addresses to which packets need to be sent
     * than additional entries need to be configured. */

    /* This is needed only for testing in Simulator*/
    Switch_update_addr(0, myMACAddress, 0);
    Switch_update_addr(1, clientMACAddress, 0);
    Switch_update_addr(1, myMACAddress, 0); // testing a hybrid between cooked up ping and the original app (cooked up raw message)

    // checking out if adding bc message still work for unicast
    Switch_update_addr(1, bcMACAddress, 0); // verified needed for BCAST tx

    // add this to see if BC packet response can come into the PA
    Switch_update_addr(0, bcMACAddress, 0); // verfied needed for BCast Rx

#else

    uint32_t i;
    uint8_t backplaneMac[6] = { 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 }; /* Mask for creating mac address by flipping LSB */
    uint8_t cppiMac[6] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }; /* MAC address for (CPPI) Port 0 - we made it up*/

    /* we need to create a mac address for the Backplane Ethernet port
     * using the Ethernet MAC Adress
     */
    for (i = 0; i < 6; i++) {
        backplaneMac[i] |= myMACAddress[i];
    }
    Init_MAC(0, backplaneMac, mtu);

    /* set the silver port to the stacks ethernet address */
    Init_MAC(1, myMACAddress, mtu);

    /* Setup the Phys by initializing the MDIO - not needed for Simulator*/
    Init_MDIO();

    /* Setup the Ethernet switch finally. */
    Init_Switch(mtu);

    /* This is a little confusing but different APIs use different numbering */
    Switch_update_addr(0, cppiMac, 0);
    Switch_update_addr(1, backplaneMac, 0);
    Switch_update_addr(2, myMACAddress, 0);
#endif

    /* CPSW subsystem setup done. Return success */
    return 0;
}

/* ============================================================================
 *   @n@b Init_PASS
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP and opens a queue that the application
 *      can use to receive command responses from the PASS.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Init_PASS(void)
{
    uint8_t isAllocated;

    /* Open a PA Command Response Queue.
     *
     * This queue will be used to hold responses from the PA PDSP for all the
     * commands issued by NIMU.
     *
     * This queue is used only when configuring the PA PDSP. We will use it when adding our mac address.
     */
    if ((gPaCfgCmdRespQHnd = Qmss_queueOpen(Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0) {
        platform_write("Error opening a PA Command Response queue \n");
        return -1;
    }

    /* Init done. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Add_MACAddress
 *
 *   @b Description
 *   @n This API adds the switch MAC address to the PA PDSP Lookup table. This
 *      ensures that all packets destined for this MAC address get processed
 *      for forwarding to the host.
 *
 *   @param[in]
 *   @n ethInfo         Pointer to PA Ethernet Info table.
 *
 *   @param[in]
 *   @n routeInfo       Pointer to PA Route Info table.
 *
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t
Add_MACAddress(
    paEthInfo_t* ethInfo,
    paRouteInfo_t* routeInfo)
{
    int32_t j;
    uint16_t cmdSize;
    Qmss_Queue cmdReplyQInfo;

    paRouteInfo_t nFailInfo = {
        pa_DEST_DISCARD, /* Toss the packet  */
        0, /* Flow Id = dont care */
        0, /* queue = dont care */
        0, /* mutli route = dont care */
        0, /* swinfo0 = dont care */
        0, /* swinfo1 = dont care */
        0, /* customType = pa_CUSTOM_TYPE_NONE */
        0, /* customIndex: not used */
        0, /* pkyType: for SRIO only */
        NULL /* No commands */
    };
    paCmdReply_t cmdReplyInfo = {
        pa_DEST_HOST, /* Replies go to the host */
        0, /* User chosen ID to go to swinfo0 */
        0, /* Destination queue */
        0 /* Flow ID */
    };
    paReturn_t retVal;
    paEntryHandle_t retHandle;
    int32_t handleType, cmdDest;
    uint32_t psCmd = ((uint32_t)(4 << 5) << 24);
    uint32_t myswinfo[] = { 0x11112222, 0x33334444 };
    uint32_t myswinfo_orig[] = { 0x00000000, 0x00000000, 0x00000000 };
    ;
    uint8_t* pCmdDataBuffer;
    Cppi_HostDesc* pHostDesc = NULL;
    Qmss_Queue rxQInfo;
    uint32_t count, cmdbuf_len = 320;
    int32_t ret_val = 0;

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((QMSS_QPOP(gTxCmdFreeQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pHostDesc)) != NULL) {
        platform_write("Error obtaining a Tx free descriptor \n");
        return -1;
    }

    /* Allocate a Tx buffer and attach the command info to it. */
    cmdbuf_len = platform_roundup(cmdbuf_len, PLATFORM_CACHE_LINE_SIZE);
    if ((pCmdDataBuffer = (Ptr)Osal_nimuMalloc(cmdbuf_len, PLATFORM_CACHE_LINE_SIZE)) == NULL) {
        platform_write("Error allocating memory for PA Command data buffer \n");
        return -1;
    }

    /* Populate the Tx free descriptor with the buffer we just allocated. */
    Cppi_setData(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)pCmdDataBuffer), cmdbuf_len);

    /* Save original buffer information */
    Cppi_setOriginalBufInfo(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, (uint8_t*)Convert_CoreLocal2GlobalAddr((uint32_t)pCmdDataBuffer), cmdbuf_len);

    cmdSize = pHostDesc->buffLen;
    cmdReplyInfo.replyId = 0x11111111; /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo = Qmss_getQueueNumber(gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue = cmdReplyQInfo.qNum;

    /* Setup the Rx queue as destination for the packets */
    rxQInfo = Qmss_getQueueNumber(gRxQHnd);
    routeInfo->queue = rxQInfo.qNum;

    retVal = Pa_addMac(res_mgr_get_painstance(),
        pa_LUT1_INDEX_NOT_SPECIFIED,
        ethInfo,
        routeInfo,
        &nFailInfo,
        &gPaL2Handles[0],
        (paCmd_t)pHostDesc->buffPtr,
        &cmdSize,
        &cmdReplyInfo,
        &cmdDest);
    if (retVal != pa_OK) {
        platform_write("Pa_addMac returned error %d\n", retVal);
        ret_val = -1;
        goto return_fail;
    }

    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot
     */
    Cppi_setSoftwareInfo(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, (uint8_t*)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned
     */
    Cppi_setPacketLen(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, cmdSize);
    pHostDesc->buffLen = cmdSize;

    /* Mark the packet as a configuration packet */
    Cppi_setPSData(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, (uint8_t*)&psCmd, 4);

    /* Send the command to the PA and wait for the return */
    QMSS_QPUSH(gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0],
        pHostDesc,
        pHostDesc->buffLen,
        SIZE_HOST_DESC,
        Qmss_Location_TAIL);

    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++) {
        platform_delaycycles(1000);

        /* Verify if we got empty Tx completion Q*/
        count = Qmss_getQueueEntryCount(gTxCmdReturnQHnd);

        if (count != 0) {

            if ((QMSS_QPOP(gTxCmdReturnQHnd, QHANDLER_QPOP_FDQ_ATTACHEDBUF, (Cppi_HostDesc**)&pHostDesc)) != NULL) {
                ret_val = -1;
                goto return_fail;
            }

            /* Restore the states */
            Cppi_setSoftwareInfo(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, (uint8_t*)myswinfo_orig);
            Cppi_setPSData(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, NULL, NULL);
            Cppi_setData(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, NULL, NULL);
            Cppi_setOriginalBufInfo(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, NULL, NULL);
            Cppi_setPacketLen(Cppi_DescType_HOST, (Cppi_Desc*)pHostDesc, NULL);
            pHostDesc->buffLen = 0;

            QMSS_QPUSHDESCSIZE(gTxCmdFreeQHnd, pHostDesc, SIZE_HOST_DESC);
        }

        if (Qmss_getQueueEntryCount(gPaCfgCmdRespQHnd) > 0) {

            /* We have a response from PA PDSP for the command we submitted earlier for
             * MAC address addition.
             */
            if ((QMSS_QPOP(gPaCfgCmdRespQHnd, QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF, (Cppi_HostDesc**)&pHostDesc)) != NULL) {
                ret_val = -1;
                goto return_fail;
            }

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId) {
                platform_write("Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n",
                    pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen = pHostDesc->origBufferLen;
                QMSS_QPUSH(gTxCmdFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
                ret_val = -1;
                goto return_fail;
            }

            retVal = Pa_forwardResult(res_mgr_get_painstance(), (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK) {
                platform_write("PA sub-system rejected Pa_addMac command\n");
                ret_val = -1;
                goto return_fail;
            }

            /* Reset the buffer length and put the descriptor back on the Rx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            QMSS_QPUSH(gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
            break;
        }
    }

    if (j == 100) {
        platform_write("Timeout waiting for reply from PA to Pa_addMac command\n");
        Osal_nimuFree(pCmdDataBuffer, cmdbuf_len);
        ret_val = -1;
    }

return_fail:

    Osal_nimuFree(pCmdDataBuffer, cmdbuf_len);

    return (ret_val);
}

/**
 *  @b Description
 *  @n
 *      Returns the number of received packet drops, most likely due to NDK buffer starvation.
 *
 *  @retval
 *      None.
 */
uint32_t nimu_getrxdrops(void)
{
    return gRxDropCounter;
}

/**
 *  @b Description
 *  @n
 *      Returns the number of transmit packet drops due to Tx descriptor starvation.
 *
 *  @retval
 *      None.
 */
uint32_t nimu_gettxdrops(void)
{
    return gTxDropCounter;
}
