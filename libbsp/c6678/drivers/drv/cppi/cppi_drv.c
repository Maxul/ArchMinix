/**
 *   @file  cppi_drv.c
 *
 *   @brief   
 *      This is the CPPI Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2012, Texas Instruments, Inc.
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
 *  \par
*/

/* CPPI Types includes */
#include <ti/drv/cppi/cppi_types.h>

/* CPPI LLD includes */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/include/cppi_listlib.h>
#include <ti/drv/cppi/include/cppi_pvt.h>

/* CPPI OSAL layer */
#include <ti/drv/cppi/cppi_osal.h>

/* RM LLD includes */
#include <ti/drv/rm/rm_public_lld.h>

/**********************************************************************
 ************************** Globals ***********************************
 **********************************************************************/

/* CPPI object */
#pragma DATA_ALIGN (cppiObject, CPPI_MAX_CACHE_ALIGN)
#pragma DATA_SECTION (cppiObject, ".cppi");
Cppi_Obj     cppiObject;

/* CPPI Local object */
Cppi_LocalObj    cppiLObj = 
{
    NULL,  /* Initialize to NULL */
};

/** @brief Global Variable which describes the CPPI LLD Version Information */
const char   cppiLldVersionStr[] = CPPI_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/** @addtogroup CPPI_LLD_FUNCTION
@{ 
*/

/**********************************************************************
 ************************** APIs **************************************
 **********************************************************************/

/**
 *  @b Description
 *  @n  
 *      This function initializes the CPPI low level driver
 *      This function is called once in the system to setup the CPPI 
 *      low level driver with information pertaining to maximum supported Rx priority, 
 *      Tx priority, Rx channels, Tx channels, Rx flows and memory mapped address for each CPPI CPDMA. 
 *
 *  @param[in]  cppiGblCfgParams
 *      Initialization structure that contains the CPPI device specific information.
 *  @param[in]  initCfg
 *      Initialization structure that contains the heap config information which can be NULL.
 *
 *  @post  
 *      CPPI instance is created.
 *
 *  @retval
 *      Success -   CPPI_SOK
 */
Cppi_Result Cppi_initCfg (Cppi_GlobalConfigParams *cppiGblCfgParams, Cppi_InitCfg *initCfg)
{
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t          i;
    void             *key;
    Cppi_HeapParams  *heapParams;

    /* Zero overhead check that CPPI_BLOCK_CHUNK_SIZE is good */
    CPPI_BLOCK_CHUNK_SIZE_CHECK;

    /* Begin Critical Section before accessing Shared resources */
    key = Cppi_osalCsEnter ();

    /* Invalidate Global Object */
    Cppi_osalBeginMemAccess ((void *) &cppiObject, sizeof (Cppi_Obj));

    memset ((void *) &cppiObject, 0, sizeof (Cppi_Obj));

    for (i = 0; i < CPPI_MAX_CPDMA; i++)
    {
        /* Get the pointer to dmaobject */
        dmaObjPtr = &(cppiObject.obj.dmaCfg[cppiGblCfgParams[i].dmaNum]);

        /* Store CPDMA related configuration */
        dmaObjPtr->dmaNum = cppiGblCfgParams[i].dmaNum;
        dmaObjPtr->maxRxCh = cppiGblCfgParams[i].maxRxCh;
        dmaObjPtr->maxTxCh = cppiGblCfgParams[i].maxTxCh;
        dmaObjPtr->maxRxFlow = cppiGblCfgParams[i].maxRxFlow;
        dmaObjPtr->rxPriority = cppiGblCfgParams[i].rxPriority;
        dmaObjPtr->txPriority = cppiGblCfgParams[i].txPriority;

        /* Store the register base address */
        dmaObjPtr->gblCfgRegs = cppiGblCfgParams[i].gblCfgRegs;
        dmaObjPtr->rxChRegs = cppiGblCfgParams[i].rxChRegs;
        dmaObjPtr->txChRegs = cppiGblCfgParams[i].txChRegs;
        dmaObjPtr->txSchedRegs = cppiGblCfgParams[i].txSchedRegs;
        dmaObjPtr->rxFlowRegs = cppiGblCfgParams[i].rxFlowRegs;
    }

    /* Create static memory if provided */
    cppiObject.obj.heapDesc.alignPow2 = CPPI_BLOCK_ALIGN_POW2;
    cppiObject.obj.heapDesc.blockSize = CPPI_BLOCK_SIZE;
    if (initCfg)
    {
        heapParams = &initCfg->heapParams;
        if (heapParams->heapAlignPow2)
        {
            cppiObject.obj.heapDesc.alignPow2 = heapParams->heapAlignPow2;
        }
        if (heapParams->dynamicHeapBlockSize)
        {
            if (heapParams->dynamicHeapBlockSize > 0)
            {
                cppiObject.obj.heapDesc.blockSize = (uint32_t)heapParams->dynamicHeapBlockSize;
            }
            else
            {
                /* Disable dynamic allocation */
                cppiObject.obj.heapDesc.blockSize = 0;
            }
        }
        if (heapParams->staticHeapBase && heapParams->staticHeapSize)
        {
            cppi_internal_heap_add (&cppiObject.obj.heapDesc, heapParams->staticHeapBase, 
                                    heapParams->staticHeapSize);
        }
    }

    /* Writeback Global Object */
    Cppi_osalEndMemAccess ((void *) &cppiObject, sizeof (Cppi_Obj));

    /* Initialize the rmHandle in the local object to NULL */
    cppiLObj.cppiRmHandle = NULL;

    /* End Critical Section */
    Cppi_osalCsExit (key);
    return CPPI_SOK;
}


/**
 *  @b Description
 *  @n  
 *      This function initializes the CPPI low level driver
 *      This function is called once in the system to setup the CPPI 
 *      low level driver with information pertaining to maximum supported Rx priority, 
 *      Tx priority, Rx channels, Tx channels, Rx flows and memory mapped address for each CPPI CPDMA. 
 *
 *      This works by calling @ref Cppi_initCfg with initCfg = NULL.
 *
 *  @param[in]  cppiGblCfgParams
 *      Initialization structure that contains the CPPI device specific information.
 *
 *  @post  
 *      CPPI instance is created.
 *
 *  @retval
 *      Success -   CPPI_SOK
 */
Cppi_Result Cppi_init (Cppi_GlobalConfigParams *cppiGblCfgParams)
{
    return Cppi_initCfg (cppiGblCfgParams, NULL);
}

/**
 *  @b Description
 *  @n  
 *      This function returns the worst-case heap requirements for the CPPI LLD given
 *      a cppiGblCfgParams set of managed resources.
 *
 *      This can be used to optionally statically allocate all of the heap memory, 
 *      which is then passed in trhough @ref Cppi_initCfg.  As long as the static heap
 *      is at least the size returned by this function then no dynamic allocations
 *      via Cppi_osalMalloc will ever be made.
 *
 *      Use of this API is completely optional.  A static heap can be used of any
 *      size.  If it is too small then dynamic allocation will occur.  The system
 *      can also be used with only dynamic allocation, where no static memory is
 *      provided through Cppi_initCfg.
 *
 *  @param[in]  cppiGblCfgParams
 *      Initialization structure that contains the CPPI device specific information.
 *
 *  @param[out] size
 *      Total bytes of memory that would be used if all flows and channels specified
 *      in cppiGblCfgParams are actually used.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *      Failure -   CPPI_INVALID_PARAM: NULL passsed for cppiGblCfgParmas or size
 */
Cppi_Result Cppi_getHeapReq (Cppi_GlobalConfigParams *cppiGblCfgParams, uint32_t *size)
{
    int i;

    if (cppiGblCfgParams && size)
    {
        uint32_t totalFlows = 0;
        uint32_t totalChannels = 0;

        for (i = 0; i < CPPI_MAX_CPDMA; i++)
        {
            totalFlows += cppiGblCfgParams[i].maxRxFlow;
            totalChannels += cppiGblCfgParams[i].maxRxCh +
                             cppiGblCfgParams[i].maxTxCh;
        }

        *size = CPPI_BLOCK_CHUNK_SIZE * (totalFlows + totalChannels);

        return CPPI_SOK;
    }

    return CPPI_INVALID_PARAM;
}

/**
 *  @b Description
 *  @n  
 *      This function initializes any CPPI start configurations such as the Resource Manager handle
 */
void Cppi_startCfg (Cppi_StartCfg *startCfg)
{
    /* Copy RM handle into local object */
    cppiLObj.cppiRmHandle = startCfg->rmHandle;
}

/**
 *  @b Description
 *  @n  
 *      This function deinitializes the CPPI low level driver.
 *      The LLD is deinitialized only if all the CPDMA instances are closed.
 *      This function should be called before re-initializing the CPPI low level driver.
 *
 *  @pre  
 *      Cppi_init function should be called before calling this function.
 *
 *  @post  
 *      CPPI instance is closed.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_CLOSED
 */
Cppi_Result Cppi_exit (void)
{
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        i;
    void            *key;
    Cppi_Result     retVal;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate the global object */
    Cppi_osalBeginMemAccess ((void *) &cppiObject, sizeof (cppiObject));

    for (i = 0; i < CPPI_MAX_CPDMA; i++)
    {
        dmaObjPtr = &(cppiObject.obj.dmaCfg[i]);
        
        if (dmaObjPtr->refCnt > 0)
        {
            retVal = CPPI_CPDMA_NOT_CLOSED;                
            goto end;
        }
    }

    /* Return all allocated memory */
    cppi_internal_heap_release(&cppiObject.obj.heapDesc);

    /* Writeback the global object */
    Cppi_osalEndMemAccess ((void *) &cppiObject, sizeof (cppiObject));

    retVal = CPPI_SOK; 
end:
    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      This function opens the CPPI CPDMA instance. 
 *      This function is called by the CPPI CPDMA driver, application to initialize the 
 *      global configuration pertaining to CPPI CPDMA that includes rx priority, 
 *      tx priority, write arbitration FIFO depth, receive starvation timeout and QM base addresses. 
 *      The registers are configured only once when the function 
 *      is called for the first time. Any future calls to this function returns the CPDMA 
 *      object handle created during the first call.
 *
 *  @param[in]  initCfg
 *      Initialization structure that contains the global configuration parameters 
 *      pertaining to CPPI CPDMA. 
 *
 *  @pre  
 *      Cppi_init function should be called before calling this function.
 *
 *  @post  
 *      Global configuration registers are configured with input parameters
 *
 *  @retval
 *      Success -   Handle to CPPI CPDMA object. Used as an input parameter to all other CPPI LLD APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_Handle Cppi_open (Cppi_CpDmaInitCfg *initCfg)
{
    Cppi_DMAObj     *dmaObjPtr;
    void             *key;

    if (initCfg == NULL)
        return NULL;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = &(cppiObject.obj.dmaCfg[initCfg->dmaNum]);

    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if (dmaObjPtr->refCnt > 0)
    {
        dmaObjPtr->refCnt++;

        /* Writeback Global Object */
        Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj)); 

        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return (Cppi_Handle) dmaObjPtr;
    }

    if (initCfg->writeFifoDepth > 32)
    {
        Cppi_osalCsExit (key);
        return NULL;
    }
    dmaObjPtr->writeFifoDepth = initCfg->writeFifoDepth;

    if (initCfg->writeFifoDepth != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->PERF_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_WARB_FIFO_DEPTH, initCfg->writeFifoDepth);

    dmaObjPtr->timeoutCount = initCfg->timeoutCount;
    CSL_FINS (dmaObjPtr->gblCfgRegs->PERF_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PERF_CONTROL_REG_TIMEOUT_CNT, initCfg->timeoutCount);

    CSL_FINS (dmaObjPtr->gblCfgRegs->PRIORITY_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_TX_PRIORITY, dmaObjPtr->txPriority);
    CSL_FINS (dmaObjPtr->gblCfgRegs->PRIORITY_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_PRIORITY_CONTROL_REG_RX_PRIORITY, dmaObjPtr->rxPriority);

    dmaObjPtr->qm0BaseAddress = initCfg->qm0BaseAddress;
    dmaObjPtr->qm1BaseAddress = initCfg->qm1BaseAddress;
    dmaObjPtr->qm2BaseAddress = initCfg->qm2BaseAddress;
    dmaObjPtr->qm3BaseAddress = initCfg->qm3BaseAddress;

    if (initCfg->qm0BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[0], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm0BaseAddress);
    if (initCfg->qm1BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[1], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm1BaseAddress);
    if (initCfg->qm2BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[2], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm2BaseAddress);
    if (initCfg->qm3BaseAddress != 0)
        CSL_FINS (dmaObjPtr->gblCfgRegs->QM_BASE_ADDRESS_REG[3], 
                    CPPIDMA_GLOBAL_CONFIG_QM_BASE_ADDRESS_REG_QM_BASE, initCfg->qm3BaseAddress);
 
    dmaObjPtr->refCnt++;

    /* Writeback Global Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_Handle) dmaObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function closes the CPPI CPDMA instance. 
 *      The instance reference count is decremented. 
 *      CPPI CPDMA instance is closed only if the reference count is zero.
 *      CPPI CPDMA instance is closed only if all the Rx, Tx channels and Rx flows are closed.
 *      This function should be called to close all instances before re-opening the CPPI CPDMA instance. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      CPPI CPDMA object is freed if the reference count is zero.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 *  @retval
 *      Failure -   CPPI_TX_CHANNELS_NOT_CLOSED
 *  @retval
 *      Failure -   CPPI_RX_CHANNELS_NOT_CLOSED
 *  @retval
 *      Failure -   CPPI_RX_FLOWS_NOT_CLOSED
 */
Cppi_Result Cppi_close (Cppi_Handle hnd)
{
    Cppi_DMAObj     *dmaObjPtr;
    void            *key;
    Cppi_Result      retVal;

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    if (dmaObjPtr->refCnt == 0)
    {
        retVal = CPPI_CPDMA_NOT_INITIALIZED;
        goto end;
    }

    /* If channels or flows are still open then don't close CPDMA instance */
    if (dmaObjPtr->txChCnt)
    {
        retVal = CPPI_TX_CHANNELS_NOT_CLOSED;
        goto end;
    }
    if (dmaObjPtr->rxChCnt)
    {
        retVal = CPPI_RX_CHANNELS_NOT_CLOSED;
        goto end;
    }
    if (dmaObjPtr->rxFlowCnt)
    {
        retVal = CPPI_RX_FLOWS_NOT_CLOSED;
        goto end;
    }
    dmaObjPtr->refCnt--;

    /* Writeback reference count */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    retVal = dmaObjPtr->refCnt;
end:
    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      This function opens a CPPI transmit channel. 
 *      The channel can be opened in two ways:
 *          1) If channel number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available channel is allocated.
 *          2) If channel number in cfg structure is a valid channel i.e., >= 0, 
 *               then the channel is allocated if free 
 *               else
 *               a handle to a previously opened channel is returned.
 *      The channel is configured only if it is a new channel allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  cfg
 *      Tx channel configuration specifying scheduler priority for the channel, channel number, 
 *      channel enable, PS and EPIB filtering control, special AIF 
 *      Monolithic Mode.
 *
 *  @param[out]  isAllocated
 *      Indicates whether the requested channel number is a new channel allocation(1).
 *      or was already allocated. If the channel was previously allocated this parameter returns 
 *      the reference count.
 *      
 * 
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      Transmit channel is allocated and configured.
 *
 *  @retval
 *      Success -   Channel Handle. Used as a input parameter in all other channel related APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_ChHnd Cppi_txChannelOpen (Cppi_Handle hnd, Cppi_TxChInitCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = NULL;
    uint32_t          index=0, mask, bitPos;
    int32_t           chNum = -1;
    void             *key;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) cppiLObj.cppiRmHandle;
    Rm_ResourceInfo resourceData;      

    resourceData.resourceType = Rm_resource_CPPI_TX_CH;    

    *isAllocated = 0;

    if ((hnd == NULL) || (cfg == NULL))
    {
        return NULL;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));
    /* Invalidate the heap descriptor, in case cppi_internal_heap_malloc is called */
    cppi_internal_heap_cache_begin (&cppiObject.obj.heapDesc);

    if ((cfg->channelNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->channelNum >= dmaObjPtr->maxTxCh))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }

    if (cfg->channelNum >= 0)
    { 
        /* Find the channel object.  No need for permissions check since non-NULL objects will already
          * have had their permissions verified */
        chObjPtr = (Cppi_ChObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->txChHnd);
        while (chObjPtr != NULL)
        {
            /* Invalidate channel Object */
            Cppi_osalBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

            if (chObjPtr->channelNum == cfg->channelNum)
            {
                chObjPtr->refCnt++;
                dmaObjPtr->txChCnt++;
                *isAllocated = chObjPtr->refCnt;
            
                /* Writeback channel Object */
                Cppi_osalEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

                /* Writeback CPDMA Object */
                Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_ChHnd) chObjPtr;
            }
            /* Get next list entry */
            chObjPtr = (Cppi_ChObj *) cppi_list_get_next ((Cppi_ListNode *) chObjPtr);
        }
    }
    if (chObjPtr == NULL)
    {
        if (cfg->channelNum >= 0)
        {
            chNum = cfg->channelNum;

            /* Check permissions for channel */
            if (rmPermCallTbl)
            {
                resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                resourceData.res_info.cpDmaData.cppiChNumOrFlowId = chNum;
                
                if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
            }   
        }
        else
        {
            for (chNum = 0; chNum < dmaObjPtr->maxTxCh; chNum++)
            {
                index = chNum >> 5;
                bitPos  = chNum & 0x1f;
                mask = dmaObjPtr->txChMask[index];

                if (mask == 0xffffffff) 
                {
                  /* There are no available channels in this mask, so immediately
                   * skip to next mask to save cycles */
                  chNum += 31;
                  continue;
                }
                if ((mask & (1 << bitPos)) == 0) 
                {
                     /* channel is available, check if we have permissions to use the channel */
                    resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                    resourceData.res_info.cpDmaData.cppiChNumOrFlowId = chNum;
                    if ((!rmPermCallTbl) ||
                         (rmPermCallTbl &&
                          (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_APPROVED)))
                    {
                        break; /* Found channel in chNum */
                    }
                }
            }
            if (chNum == dmaObjPtr->maxTxCh) 
            {
                /* Failed to allocate a channel -- all are used or permission denied */
                Cppi_osalCsExit (key);
                return NULL;
            }
        }

        /* We now have a valid channel number in chNum which was specified
         *  by user or found via search */
        chObjPtr = (Cppi_ChObj *) cppi_internal_heap_malloc (&cppiObject.obj.heapDesc, sizeof (Cppi_ChObj));
        if (chObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Need to invalidate the new pointer, because it could be in same cache
         * line as "other CPPI data" since CPPI has dedicated heap.  Otherwise, old
         * data could be written on top of the "other CPPI data" if this line is
         * already in cache.
         * It is also critical at this point that no other items from the heap
         * are dirty, otherwise the new data would be destroyed.
         */
        Cppi_osalBeginMemAccess (chObjPtr, sizeof(Cppi_ChObj));
        /* Invalidate the whole list, to ensure no old data is left when linking
         * in cppi_list_cat.  Also, must be before chObjPtr is dirtied, else
         * new changes would be lost
         */
        cppi_list_cache_invalidate ((Cppi_ListNode *)dmaObjPtr->txChHnd);

        /* Initialize the allocated channel object */
        memset ((void *) chObjPtr, 0, sizeof (Cppi_ChObj));

        chObjPtr->refCnt++;
        dmaObjPtr->txChCnt++;

        index = chNum / 32;
        bitPos = chNum % 32;
        CSL_FINSR (dmaObjPtr->txChMask[index], bitPos, bitPos, 1);

        chObjPtr->channelNum = chNum;
        chObjPtr->dmaNum = dmaObjPtr->dmaNum;
        chObjPtr->chType = Cppi_ChType_TX_CHANNEL;
        chObjPtr->dmaObjHnd = (Cppi_DMAObj *) dmaObjPtr;
        *isAllocated = chObjPtr->refCnt;

        /* Add the channel object to channel list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->txChHnd, (Cppi_ListNode **) &chObjPtr);

    }

    if (*isAllocated == 1)
    {
        CSL_FINS (dmaObjPtr->txSchedRegs->TX_CHANNEL_SCHEDULER_CONFIG_REG[chObjPtr->channelNum], 
                CPPIDMA_TX_SCHEDULER_CONFIG_TX_CHANNEL_SCHEDULER_CONFIG_REG_PRIORITY, cfg->priority);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_EINFO, cfg->filterEPIB);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_FILT_PSWORDS, cfg->filterPS);

        CSL_FINS (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_B, 
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_B_TX_AIF_MONO_MODE, cfg->aifMonoMode);

        if (cfg->txEnable)
        {
            uint32_t value = 0;
            CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, cfg->txEnable);
            dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
        }
    }     

    /* Writeback channel Object */
    Cppi_osalEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

    /* Writeback CPDMA Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_ChHnd) chObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function opens a CPPI receive channel. 
 *      The channel can be opened in two ways:
 *          1) If channel number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available channel is allocated.
 *          2) If channel number in cfg structure is a valid channel i.e., >= 0, 
 *               then the channel is allocated if free 
 *               else
 *               a handle to a previously opened channel is returned.
 *      The channel is configured only if it is a new channel allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  cfg
 *      Rx channel configuration specifying channel number, channel enable. 
 *
 *  @param[out]  isAllocated
 *      Indicates whether the requested channel number is a new channel allocation(1).
 *      or was already allocated. If the channel was previously allocated this parameter returns 
 *      the reference count.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @post  
 *      Receive channel is allocated and configured.
 *
 *  @retval
 *      Success -   Channel Handle. Used as a input parameter in all other channel related APIs.
 *  @retval
 *      Failure -   NULL
 */
Cppi_ChHnd Cppi_rxChannelOpen (Cppi_Handle hnd, Cppi_RxChInitCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = NULL;
    uint32_t          index = 0, mask, bitPos;
    int32_t           chNum = -1;
    void             *key;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) cppiLObj.cppiRmHandle;
    Rm_ResourceInfo resourceData;      

    resourceData.resourceType = Rm_resource_CPPI_RX_CH;        
    
    *isAllocated = 0;

    if ((hnd == NULL) || (cfg == NULL))
    {
        return NULL;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));
    /* Invalidate the heap descriptor, in case cppi_internal_heap_malloc is called */
    cppi_internal_heap_cache_begin (&cppiObject.obj.heapDesc);

    if ((cfg->channelNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->channelNum >= dmaObjPtr->maxRxCh))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }    

    if (cfg->channelNum >= 0)
    {
        /* Find the channel object.  No need for permissions check since non-NULL objects will already
          * have had their permissions verified */
        chObjPtr = (Cppi_ChObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->rxChHnd);
        while (chObjPtr != NULL)
        {
            /* Invalidate channel Object */
            Cppi_osalBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

            if (chObjPtr->channelNum == cfg->channelNum)
            {
                chObjPtr->refCnt++;
                dmaObjPtr->rxChCnt++;
                *isAllocated = chObjPtr->refCnt;
                
                /* Writeback channel Object */
                Cppi_osalEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

                /* Writeback CPDMA Object */
                Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_ChHnd) chObjPtr;
            }
            /* Get next list entry */
            chObjPtr = (Cppi_ChObj *) cppi_list_get_next ((Cppi_ListNode *) chObjPtr);
        }
    }
    if (chObjPtr == NULL)
    {
        if (cfg->channelNum >= 0)
        {
            chNum = cfg->channelNum;

            /* Check permissions for channel */
            if (rmPermCallTbl)
            {
                resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                resourceData.res_info.cpDmaData.cppiChNumOrFlowId = chNum;
                
                if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
            }   
        }
        else
        {
            for (chNum = 0; chNum < dmaObjPtr->maxRxCh; chNum++)
            {
                index = chNum >> 5;
                bitPos  = chNum & 0x1f;
                mask = dmaObjPtr->rxChMask[index];

                if (mask == 0xffffffff) 
                {
                  /* There are no available channels in this mask, so immediately
                   * skip to next mask to save cycles */
                  chNum += 31;
                  continue;
                }
                if ((mask & (1 << bitPos)) == 0) 
                {
                     /* channel is available, check if we have permissions to use the channel */
                    resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                    resourceData.res_info.cpDmaData.cppiChNumOrFlowId = chNum;
                    if ((!rmPermCallTbl) ||
                         (rmPermCallTbl &&
                          (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_APPROVED)))
                    {
                        break; /* Found channel in chNum */
                    }
                }
            }
            if (chNum == dmaObjPtr->maxRxCh) 
            {
                /* Failed to allocate a channel -- all are used or permission denied */
                Cppi_osalCsExit (key);
                return NULL;
            }
        }

        /* We now have a valid channel number in chNum which was specified
         *  by user or found via search */
        chObjPtr = (Cppi_ChObj *) cppi_internal_heap_malloc (&cppiObject.obj.heapDesc, sizeof (Cppi_ChObj));
        if (chObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Need to invalidate the new pointer, because it could be in same cache
         * line as "other CPPI data" since CPPI has dedicated heap.  Otherwise, old
         * data could be written on top of the "other CPPI data" if this line is
         * already in cache.
         * It is also critical at this point that no other items from the heap
         * are dirty, otherwise the new data would be destroyed.
         */
        Cppi_osalBeginMemAccess (chObjPtr, sizeof(Cppi_ChObj));
        /* Invalidate the whole list, to ensure no old data is left when linking
         * in cppi_list_cat.  Also, must be before chObjPtr is dirtied, else
         * new changes would be lost
         */
        cppi_list_cache_invalidate ((Cppi_ListNode *)&dmaObjPtr->txChHnd);

        /* Initialize the allocated channel object */
        memset ((void *) chObjPtr, 0, sizeof (Cppi_ChObj));

        chObjPtr->refCnt++;
        dmaObjPtr->rxChCnt++;

        index = chNum / 32;
        bitPos = chNum % 32;

        CSL_FINSR (dmaObjPtr->rxChMask[index], bitPos, bitPos, 1);

        chObjPtr->channelNum = chNum;
        chObjPtr->dmaNum = dmaObjPtr->dmaNum;
        chObjPtr->chType = Cppi_ChType_RX_CHANNEL;
        chObjPtr->dmaObjHnd = (Cppi_DMAObj *) dmaObjPtr;
        *isAllocated = chObjPtr->refCnt;
        
        /* Add the channel object to channel list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->rxChHnd, (Cppi_ListNode **) &chObjPtr);
    }

    if (*isAllocated == 1)
    {
        if (cfg->rxEnable)
        {
            uint32_t value = 0;
            CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, cfg->rxEnable);
            dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
        }
    }        

    /* Writeback channel Object */
    Cppi_osalEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));

    /* Writeback CPDMA Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_ChHnd) chObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function enables a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelEnable (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        value = 0;
    void            *key;

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* No RM permissions check since this was done during channel open */

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function disables a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelDisable (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    void            *key;

    /* No RM permissions check since this was done during channel open */

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = chObjPtr->dmaObjHnd;
    
    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = 0;
    else
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = 0;
  
    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function tears down a transmit or a receive channel by writing 
 *      to the channel teardown bit. 
 *      This function can do a blocking wait for the teardown to complete OR
 *      return immediately and it is up to the callee to check for channel 
 *      teardown status. The behavior is based on the "wait" input parameter
 *
 *  @param[in]  hnd
 *      Channel handle.
 *
 *  @param[in]  wait
 *      Specifies Wait or No Wait for teardown complete.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM 
 */
Cppi_Result Cppi_channelTeardown (Cppi_ChHnd hnd, Cppi_Wait wait)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        value = 0;
    void            *key;

    /* No RM permissions check since this was done during channel open */

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    
    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_TEARDOWN, 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_TEARDOWN, 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }
 
    if (wait)
    {
        uint32_t  enable;
        do
        {
            if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
                enable = CSL_FEXT (dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG, 
                    CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE);
            else
                enable = CSL_FEXT (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A, 
                    CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE);
        } while (enable);
    }

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function closes a transmit or a receive channel.
 *      The channel reference count is decremented. The channel is freed only if the 
 *      reference count is zero. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @post
 *      Channel is freed if the reference count is zero and available for reallocation.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CHANNEL_NOT_OPEN
 */
Cppi_Result Cppi_channelClose (Cppi_ChHnd hnd)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    uint32_t        refCount;
    void            *key;

    /* No RM check because permissions were checked at open */
    
    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    
    /* Invalidate channel Object */
    Cppi_osalBeginMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));
    /* Invalidate the heap descriptor, in case cppi_internal_heap_free is called */
    cppi_internal_heap_cache_begin (&cppiObject.obj.heapDesc);

    if (chObjPtr->refCnt == 0)
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return CPPI_CHANNEL_NOT_OPEN;
    }
    else
    {
        dmaObjPtr = chObjPtr->dmaObjHnd;

        /* Invalidate CPDMA Object */
        Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));
       
        chObjPtr->refCnt--;
        refCount = chObjPtr->refCnt;

        /* Writeback channel Object */
        Cppi_osalEndMemAccess ((void *) chObjPtr, sizeof (Cppi_ChObj));
        
        if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        {
            dmaObjPtr->rxChCnt--;
            /* If reference count is zero then free the object */
            if (refCount == 0)
            {
                uint32_t  index, bitPos;
                
                index = chObjPtr->channelNum / 32;
                bitPos = chObjPtr->channelNum % 32;

                /* Invalidate list for remove_node precondition */
                /* chObjPtr was written back to avoid loss during invalidate */
                cppi_list_cache_invalidate ((Cppi_ListNode *)dmaObjPtr->rxChHnd);

                CSL_FINSR (dmaObjPtr->rxChMask[index], bitPos, bitPos, 0);
                cppi_list_remove_node ((Cppi_ListNode **) &dmaObjPtr->rxChHnd, (Cppi_ListNode *) chObjPtr);
                cppi_internal_heap_free (&cppiObject.obj.heapDesc, chObjPtr);
            }      
        }
        else
        {
            dmaObjPtr->txChCnt--;
            /* If reference count is zero then free the object */
            if (refCount == 0)
            {
                uint32_t index, bitPos;

                index = chObjPtr->channelNum / 32;
                bitPos = chObjPtr->channelNum % 32;
                
                /* Invalidate list for remove_node precondition */
                /* chObjPtr was written back to avoid loss during invalidate */
                cppi_list_cache_invalidate ((Cppi_ListNode *)dmaObjPtr->txChHnd);

                CSL_FINSR (dmaObjPtr->txChMask[index], bitPos, bitPos, 0);
                cppi_list_remove_node ((Cppi_ListNode**) &dmaObjPtr->txChHnd, (Cppi_ListNode*) chObjPtr);
                cppi_internal_heap_free (&cppiObject.obj.heapDesc, chObjPtr);
            } 
        }
    }

    /* Writeback CPDMA Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return refCount;
}

/**
 *  @b Description
 *  @n  
 *      This function pauses a transmit or a receive channel. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelPause (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t          value = 0;
    void            *key;

    /* No RM permissions check since this was done during channel open */

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
    {
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_PAUSE, 1);
        dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG = value;
    }
    else
    {
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE, (uint32_t) 1);
        CSL_FINS (value, CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_PAUSE, 1);
        dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A = value;
    }  

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return CPPI_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function returns the enable or disable channel status. 
 *
 *  @param[in]  hnd
 *      Channel handle.
 * 
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Success -   Channel Status
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 */
Cppi_Result Cppi_channelStatus (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    void            *key;
    Cppi_Result      retVal;

    /* No RM check since this is a read only operation */

    if (chObjPtr == NULL)
        return CPPI_INVALID_PARAM;

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    dmaObjPtr = chObjPtr->dmaObjHnd;

    if (chObjPtr->chType == Cppi_ChType_RX_CHANNEL)
        retVal = (CSL_FEXT (dmaObjPtr->rxChRegs->RX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].RX_CHANNEL_GLOBAL_CONFIG_REG, 
            CPPIDMA_RX_CHANNEL_CONFIG_RX_CHANNEL_GLOBAL_CONFIG_REG_RX_ENABLE));
    else
        retVal = (CSL_FEXT (dmaObjPtr->txChRegs->TX_CHANNEL_GLOBAL_CONFIG[chObjPtr->channelNum].TX_CHANNEL_GLOBAL_CONFIG_REG_A,
            CPPIDMA_TX_CHANNEL_CONFIG_TX_CHANNEL_GLOBAL_CONFIG_REG_A_TX_ENABLE));

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      This function configures a receive flow. 
 *      The flow can be configured in two ways:
 *          1) If flow ID number in cfg structure is set to CPPI_PARAM_NOT_SPECIFIED, then a new 
 *              available flow is allocated.
 *          2) If flow ID number is cfg structure is a valid flow i.e., >= 0, 
 *               then the flow is allocated if free 
 *               else
 *               a handle to a previously opened flow is returned.

 *      The flow is configured only if it is a new flow allocation. 
 *
 *  @param[in]  hnd
 *      handle returned by Cppi_open API.
 *      
 *  @param[in]  cfg
 *      Rx flow configuration
 * 
 *  @param[out]  isAllocated
 *      Indicates whether the requested flow is a new flow allocation(1).
 *      or was already allocated. If the flow was previously allocated this parameter returns 
 *      the reference count.
 *       
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *       
 *  @post  
 *      Rx flow is allocated and configured.
 *
 *  @retval
 *      Success -   Flow Handle.
 *  @retval
 *      Failure -   NULL
 */
Cppi_FlowHnd Cppi_configureRxFlow (Cppi_Handle hnd, Cppi_RxFlowCfg *cfg, uint8_t *isAllocated)
{
    Cppi_DMAObj     *dmaObjPtr = (Cppi_DMAObj *) hnd;
    Cppi_FlowObj    *flowObjPtr = NULL;
    uint32_t          index = 0, mask, bitPos;
    int32_t           flowId = -1;
    void             *key;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) cppiLObj.cppiRmHandle;
    Rm_ResourceInfo resourceData;      

    resourceData.resourceType = Rm_resource_CPPI_RX_FLOW;      

    *isAllocated = 0;

    if ((dmaObjPtr == NULL) || (cfg == NULL))
        return NULL;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* Invalidate the heap descriptor, in case cppi_internal_heap_malloc is called */
    cppi_internal_heap_cache_begin (&cppiObject.obj.heapDesc);

    if ((cfg->flowIdNum != CPPI_PARAM_NOT_SPECIFIED) && (cfg->flowIdNum >= dmaObjPtr->maxRxFlow))
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return NULL;
    }
    
    if (cfg->flowIdNum >= 0)
    {
        /* Find the flow object.  No need for permissions check since non-NULL objects will already
          * have had their permissions verified */
        flowObjPtr = (Cppi_FlowObj *) cppi_list_get_head ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd);
        while (flowObjPtr != NULL)
        {
            /* Invalidate flow Object */
            Cppi_osalBeginMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

            if (flowObjPtr->flowId == cfg->flowIdNum)
            {
                flowObjPtr->refCnt++;
                dmaObjPtr->rxFlowCnt++;
                *isAllocated = flowObjPtr->refCnt;

                /* Writeback flow Object */
                Cppi_osalEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

                /* Writeback CPDMA Object */
                Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

                /* End Critical Section. */
                Cppi_osalCsExit (key);
                return (Cppi_FlowHnd) flowObjPtr;
            }
            /* Get next list entry */
            flowObjPtr = (Cppi_FlowObj *) cppi_list_get_next ((Cppi_ListNode *) flowObjPtr);
        }
    }

    /* New flow allocation - either specified or next available one */
    if (flowObjPtr == NULL)
    {
        if (cfg->flowIdNum >= 0)
        {
            flowId = cfg->flowIdNum;

            /* Check permissions for flow */
            if (rmPermCallTbl)
            {
                resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                resourceData.res_info.cpDmaData.cppiChNumOrFlowId = flowId;
                
                if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
                {
                    /* End Critical Section. */
                    Cppi_osalCsExit (key);
                    return NULL;
                }
            }   
        }
        else
        {
            for (flowId = 0; flowId < dmaObjPtr->maxRxFlow; flowId++)
            {
                index = flowId >> 5;
                bitPos  = flowId & 0x1f;
                mask = dmaObjPtr->rxFlowMask[index];

                if (mask == 0xffffffff) 
                {
                  /* There are no available flows in this mask, so immediately
                   * skip to next mask to save cycles */
                  flowId += 31;
                  continue;
                }
                if ((mask & (1 << bitPos)) == 0)
                {
                     /* channel is available, check if we have permissions to use the channel */
                    resourceData.res_info.cpDmaData.dmaNum = dmaObjPtr->dmaNum;
                    resourceData.res_info.cpDmaData.cppiChNumOrFlowId = flowId;
                    if ((!rmPermCallTbl) ||
                         (rmPermCallTbl &&
                          (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_APPROVED)))
                    {
                        break; /* Found channel in flowId */
                    }
                }
            }
            if (flowId == dmaObjPtr->maxRxFlow) 
            {
                /* Failed to allocate a flow -- all are used or permission denied */
                Cppi_osalCsExit (key);
                return NULL;
            }
        }

        /* We now have a valid channel number in chNum which was specified
         *  by user or found via search */
        flowObjPtr = (Cppi_FlowObj *) cppi_internal_heap_malloc (&cppiObject.obj.heapDesc, sizeof (Cppi_FlowObj));
        if (flowObjPtr == NULL)
        {
            /* End Critical Section. */
            Cppi_osalCsExit (key);
            return NULL;
        }

        /* Need to invalidate the new pointer, because it could be in same cache
         * line as "other CPPI data" since CPPI has dedicated heap.  Otherwise, old
         * data could be written on top of the "other CPPI data" if this line is
         * already in cache.
         * It is also critical at this point that no other items from the heap
         * are dirty, otherwise the new data would be destroyed.
         */
        Cppi_osalBeginMemAccess (flowObjPtr, sizeof(Cppi_FlowObj));
        /* Invalidate the whole list, to ensure no old data is left when linking
         * in cppi_list_cat.  Also, must be before chObjPtr is dirtied, else
         * new changes would be lost
         */
        cppi_list_cache_invalidate ((Cppi_ListNode *)dmaObjPtr->rxFlowHnd);

        /* Initialize the allocated flow object */
        memset ((void *) flowObjPtr, 0, sizeof (Cppi_FlowObj));

        flowObjPtr->refCnt++;
        dmaObjPtr->rxFlowCnt++;
        
        index = flowId / 32;
        bitPos = flowId % 32;
        CSL_FINSR (dmaObjPtr->rxFlowMask[index], bitPos, bitPos, 1);

        flowObjPtr->dmaNum = dmaObjPtr->dmaNum;
        flowObjPtr->flowId = flowId;
        flowObjPtr->dmaObjHnd = dmaObjPtr;
        *isAllocated = flowObjPtr->refCnt;
                
        /* Add the flow object to flow list */
        cppi_list_cat ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd, (Cppi_ListNode **) &flowObjPtr);

    }

    if (*isAllocated == 1)
    {
            uint32_t      reg, temp = 0;

            index = flowObjPtr->flowId;
            reg = 0;
                    /* Rx flow configuration register A */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QNUM, cfg->rx_dest_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DEST_QMGR, cfg->rx_dest_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_SOP_OFFSET, cfg->rx_sop_offset);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PS_LOCATION, cfg->rx_ps_location);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_DESC_TYPE, cfg->rx_desc_type);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_ERROR_HANDLING, cfg->rx_error_handling);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_PSINFO_PRESENT, cfg->rx_psinfo_present);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_A_RX_EINFO_PRESENT, cfg->rx_einfo_present);
                    dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_A = reg;

            reg = 0;
                    /* Rx flow configuration register B */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_LO, cfg->rx_dest_tag_lo);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_DEST_TAG_HI, cfg->rx_dest_tag_hi);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_LO, cfg->rx_src_tag_lo);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_B_RX_SRC_TAG_HI, cfg->rx_src_tag_hi);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_B = reg;

            reg = 0;
            /* Rx flow configuration register C */                
            temp = ((cfg->rx_size_thresh0_en) | (cfg->rx_size_thresh1_en << 1) | (cfg->rx_size_thresh2_en << 2));

            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SIZE_THRESH_EN, temp);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_LO_SEL, cfg->rx_dest_tag_lo_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_DEST_TAG_HI_SEL, cfg->rx_dest_tag_hi_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_LO_SEL, cfg->rx_src_tag_lo_sel);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_C_RX_SRC_TAG_HI_SEL, cfg->rx_src_tag_hi_sel);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_C = reg;

            reg = 0;
                        /* Rx flow configuration register D */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QNUM, cfg->rx_fdq1_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ1_QMGR, cfg->rx_fdq1_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QNUM, cfg->rx_fdq0_sz0_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_D_RX_FDQ0_SZ0_QMGR, cfg->rx_fdq0_sz0_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_D = reg;

            reg = 0;
                        /* Rx flow configuration register E */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QNUM, cfg->rx_fdq3_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ3_QMGR, cfg->rx_fdq3_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QNUM, cfg->rx_fdq2_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_E_RX_FDQ2_QMGR, cfg->rx_fdq2_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_E = reg;

            reg = 0;
                        /* Rx flow configuration register F */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH1, (cfg->rx_size_thresh1 >> 5));
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_F_RX_SIZE_THRESH0, (cfg->rx_size_thresh0  >> 5));
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_F = reg;
            
            reg = 0;
            /* Rx flow configuration register G */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QNUM, cfg->rx_fdq0_sz1_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_FDQ0_SZ1_QMGR, cfg->rx_fdq0_sz1_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_G_RX_SIZE_THRESH2, (cfg->rx_size_thresh2) >> 5);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_G = reg;

            reg = 0;
            /* Rx flow configuration register H */
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QNUM, cfg->rx_fdq0_sz3_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ3_QMGR, cfg->rx_fdq0_sz3_qmgr);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QNUM, cfg->rx_fdq0_sz2_qnum);
            CSL_FINS (reg, CPPIDMA_RX_FLOW_CONFIG_RX_FLOW_CONFIG_REG_H_RX_FDQ0_SZ2_QMGR, cfg->rx_fdq0_sz2_qmgr);
            dmaObjPtr->rxFlowRegs->RX_FLOW_CONFIG[index].RX_FLOW_CONFIG_REG_H = reg;

    }

    /* Writeback flow Object */
    Cppi_osalEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    /* Writeback CPDMA Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return (Cppi_FlowHnd) flowObjPtr;
}

/**
 *  @b Description
 *  @n  
 *      This function closes a receive flow. 
 *      The flow reference count is decremented. The flow is freed only if the 
 *      reference count is zero. 
 *
 *  @param[in]  hnd
 *      flow handle.
 * 
 *  @pre  
 *      Cppi_configureRxFlowfunction should be called before calling this function.
 *
 *  @post
 *      Flow is freed if the reference count is zero and available for reallocation.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_FLOW_NOT_OPEN
 */
Cppi_Result Cppi_closeRxFlow (Cppi_FlowHnd hnd)
{
    Cppi_FlowObj    *flowObjPtr = (Cppi_FlowObj *) hnd;
    Cppi_DMAObj     *dmaObjPtr;
    uint32_t        refCount;
    void            *key;

    /* No RM permissions check since it was done at open */

    if (hnd == NULL)
        return CPPI_INVALID_PARAM;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    
    /* Invalidate flow Object */
    Cppi_osalBeginMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    /* Invalidate the heap descriptor, in case cppi_internal_heap_free is called */
    cppi_internal_heap_cache_begin (&cppiObject.obj.heapDesc);

    if (flowObjPtr->refCnt == 0)
    {
        /* End Critical Section. */
        Cppi_osalCsExit (key);
        return CPPI_FLOW_NOT_OPEN;
    }
    dmaObjPtr = flowObjPtr->dmaObjHnd;
 
    /* Invalidate CPDMA Object */
    Cppi_osalBeginMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* This dirties flowObjPtr, so need to clean it before deleting! */
    flowObjPtr->refCnt--;
    refCount = flowObjPtr->refCnt;

    /* If reference count is zero then free the object */
    if (refCount == 0)
    {
        uint32_t  index, bitPos;

        index = flowObjPtr->flowId / 32;
        bitPos = flowObjPtr->flowId % 32;

        /* Writeback all dirty lines that belong to heap before calling cppi_list_cache_invalidate */
        Cppi_osalEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));
        /* Invalidate list for remove_node precondition */
        cppi_list_cache_invalidate ((Cppi_ListNode *)dmaObjPtr->rxFlowHnd);

        CSL_FINSR (dmaObjPtr->rxFlowMask[index], bitPos, bitPos, 0);
        cppi_list_remove_node ((Cppi_ListNode **) &dmaObjPtr->rxFlowHnd, (Cppi_ListNode *) flowObjPtr);
        cppi_internal_heap_free (&cppiObject.obj.heapDesc, flowObjPtr);
    }

    dmaObjPtr->rxFlowCnt--;

    /* Writeback flow Object */
    Cppi_osalEndMemAccess ((void *) flowObjPtr, sizeof (Cppi_FlowObj));

    /* Writeback CPDMA Object */
    Cppi_osalEndMemAccess ((void *) dmaObjPtr, sizeof (Cppi_DMAObj));

    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return refCount;
}

/** 
 *  @b Description
 *  @n  
 *      Given the channel handle the function returns the channel number
 *      corresponding to the handle
 *
 *  @param[in]  hnd      
 *      Channel handle
 *
 *  @pre  
 *      Cppi_rxChannelOpen or Cppi_txChannelOpen function should be called before 
 *      calling this function.
 *
 *  @retval
 *      Channel number
 */
uint32_t Cppi_getChannelNumber (Cppi_ChHnd hnd)
{
    Cppi_ChObj      *chObjPtr = (Cppi_ChObj *) hnd;

    /* No RM permissions check since this is a read only operation */
    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */
    /* No critical section because already atomic operation & shared object not written */

    return (chObjPtr->channelNum);
}

/** 
 *  @b Description
 *  @n  
 *      Given the flow handle the function returns the flow ID
 *      corresponding to the handle
 *
 *  @param[in]  hnd      
 *      Flow handle
 *
 *  @pre  
 *      Cppi_configureRxFlowfunction should be called before calling this function.
 *
 *  @retval
 *      Flow ID
 */
uint32_t Cppi_getFlowId (Cppi_FlowHnd hnd)
{
    Cppi_FlowObj    *flowObjPtr = (Cppi_FlowObj *) hnd;

    /* No RM permissions check since this is a read only operation */
    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */
    /* No critical section because already atomic operation & shared object not written */

    return (flowObjPtr->flowId);
}

/** 
 *  @b Description
 *  @n  
 *      This function enables or disables CPDMA loopback 
 *
 *  @param[in]  hnd      
 *      handle returned by Cppi_open API.
 *
 *  @param[in]  loopback      
 *      0 - To disable loopback
 *      1 - To enable loopback
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @retval
 *      Success -   CPPI_SOK 
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 */
Cppi_Result Cppi_setCpdmaLoopback (Cppi_Handle hnd, uint8_t loopback)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_Result      retVal;
    void            *key;

    /* No RM permissions check since this was done during channel open */

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();

    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    if (dmaObjPtr->refCnt == 0)
    {
        retVal = CPPI_CPDMA_NOT_INITIALIZED;
        goto end;
    }
    CSL_FINS (dmaObjPtr->gblCfgRegs->EMULATION_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_EMULATION_CONTROL_REG_LOOPBACK_EN, loopback);

    retVal = CPPI_SOK;
end:
    /* End Critical Section. */
    Cppi_osalCsExit (key);

    return retVal;
}

/** 
 *  @b Description
 *  @n  
 *      This function returns the CPDMA loopback configuration
 *
 *  @param[in]  hnd      
 *      handle returned by Cppi_open API.
 *
 *  @pre  
 *      Cppi_open function should be called before calling this function.
 *
 *  @retval
 *      Success 
 *          0 - loopback is disabled 
 *          1 - loopback is enabled
 *          
 *  @retval
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_CPDMA_NOT_INITIALIZED
 */
Cppi_Result Cppi_getCpdmaLoopback (Cppi_Handle hnd)
{
    Cppi_DMAObj     *dmaObjPtr;
    Cppi_Result      retVal;
    void            *key;

    /* No RM permissions check since this is a read only operation */

    if (hnd == NULL)
    {
        return CPPI_INVALID_PARAM;
    }

    dmaObjPtr = (Cppi_DMAObj *) hnd;

    /* Begin Critical Section before accessing shared resources. */
    key = Cppi_osalCsEnter ();
    /* No cache operation because it was done in Cppi_xxChannelOpen and old data is still valid */

    if (dmaObjPtr->refCnt == 0)
    {
        retVal = CPPI_CPDMA_NOT_INITIALIZED;
        goto end;
    }
    retVal = (CSL_FEXT (dmaObjPtr->gblCfgRegs->EMULATION_CONTROL_REG, 
                    CPPIDMA_GLOBAL_CONFIG_EMULATION_CONTROL_REG_LOOPBACK_EN));

end:
    /* End Critical Section. */
    Cppi_osalCsExit (key);
    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version information of the CPPI LLD.
 *
 *  @retval
 *      Version Information.
 */
uint32_t Cppi_getVersion (void)
{
    return CPPI_LLD_VERSION_ID;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version string for the CPPI LLD.
 *
 *  @retval
 *      Version String.
 */
const char* Cppi_getVersionStr (void)
{
    return cppiLldVersionStr;
}

/**
@}
*/

