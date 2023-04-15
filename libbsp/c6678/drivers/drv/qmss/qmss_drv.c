/**
 *   @file  qmss_drv.c
 *
 *   @brief   
 *      This is the Queue Manager Low Level Driver file.
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

/* QMSS Types includes */
#include <ti/drv/qmss/qmss_types.h>
#include <string.h>

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/include/qmss_pvt.h>
#include <ti/drv/qmss/include/qmss_intd.h>

/* QMSS OSAL layer */
#include <ti/drv/qmss/qmss_osal.h>

/* RM LLD includes */
#include <ti/drv/rm/rm_public_lld.h>

/**********************************************************************
 ************************** Globals ***********************************
 **********************************************************************/

/* Maintain status of queues */
#pragma DATA_SECTION (queueFree, ".qmss");
#pragma DATA_ALIGN (queueFree, QMSS_MAX_CACHE_ALIGN)
uint8_t                     queueFree[8192];

/* QMSS Global object */
#pragma DATA_SECTION (qmssGObj, ".qmss");
#pragma DATA_ALIGN (qmssGObj, QMSS_MAX_CACHE_ALIGN)
Qmss_GlobalObj              qmssGObj;

/* QMSS Local object */
Qmss_GlobalConfigParams     qmssLObj;

/* QMSS Local Object is valid flag */
uint8_t                     qmssLObjIsValid = 0;

/** @brief Global Variable which describes the QMSS LLD Version Information */
const char   qmssLldVersionStr[] = QMSS_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/**********************************************************************
 ************************** Functions *********************************
 **********************************************************************/

static Qmss_QueueHnd Qmss_internalQueueOpen (Qmss_QueueType queType, int queNum, uint8_t *isAllocated);
static Qmss_Result Qmss_internaldownloadFirmware (Qmss_PdspId pdspId, void *image, uint32_t size);

/** @addtogroup QMSS_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function initializes the Queue Manager subsystem low level driver
 *      This function is called once in the system to setup the queue manager 
 *      low level driver with information pertaining to linking RAM and descriptor 
 *      memory region.
 *
 *  @param[in]  initCfg
 *      Initialization structure that contains memory region configuration information.
 *      Linking RAM memory address should be a global address  
 *
 *  @param[in]  qmssGblCfgParams
 *      Initialization structure that contains the QMSS device specific information.
 *
 *  @post  
 *      QMSS instance is created.
 *
 *  @retval
 *      Success -   QMSS_SOK
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_RESOURCE_INIT_DENIED
 *  @retval
 *      Failure -   QMSS_RESOURCE_LINKING_RAM0_INIT_DENIED
 *  @retval
 *      Failure -   QMSS_RESOURCE_LINKING_RAM1_INIT_DENIED
 */

Qmss_Result Qmss_init (Qmss_InitCfg *initCfg, Qmss_GlobalConfigParams *qmssGblCfgParams)
{
    Qmss_GlobalObj_Unpadded *qmss_objPtr = &qmssGObj.obj;
    uint32_t        index;
    void            *key;
    Qmss_Result retVal = QMSS_SOK;
    Qmss_StartCfg startCfg;
    Rm_LldPermCallouts *rmPermCallTbl;
    Rm_ResourceInfo resourceData;
    
    if (initCfg == NULL)
    {
        return QMSS_INVALID_PARAM;
    }

    for (index = 0; index < QMSS_MAX_PDSP; index++)
    {
        if ((initCfg->pdspFirmware[index].firmware != NULL) && (initCfg->pdspFirmware[index].size == 0))
            return QMSS_INVALID_PARAM;
    }

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Initialize private data structures */
    memset ((void *) &queueFree, 0, sizeof (queueFree));
    Qmss_osalEndMemAccess ((void *) queueFree, sizeof (queueFree));

    memset (qmss_objPtr, 0, sizeof (Qmss_GlobalObj));
    for (index = 0; index < QMSS_MAX_MEM_REGIONS; index++)
        qmss_objPtr->descQueue[index] = -1;

    /* Store a local copy of the initial memory region configuration */
    memcpy ((void *) &qmss_objPtr->initCfg, initCfg, sizeof (Qmss_InitCfg));

    /* Check if LinkingRAM0 can hold all the descriptors if LinkingRAM1 is NULL */
    if (initCfg->linkingRAM1Base == 0) 
    {
        if (initCfg->maxDescNum > (QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE + 1)) 
        {
            /* End Critical Section */
            Qmss_osalCsExit (key);
            return QMSS_INVALID_PARAM;
        }
    }

    /* Copy global configuration parameters */
    memcpy ((void *) &qmss_objPtr->qmssGblCfgParams, qmssGblCfgParams, sizeof (Qmss_GlobalConfigParams));

    startCfg.rmHandle = qmssGblCfgParams->qmRmHandle;
    Qmss_startCfg (&startCfg);

    /* Populate RM call table after Qmss_startCfg which initializes the qmssLObj */
    rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;

    if (initCfg->linkingRAM0Size == 0)
    {
        qmss_objPtr->initCfg.linkingRAM0Size = QMSS_LINKING_RAM_REGION_0_DEFAULT_SIZE;
    }
    else
    {
        qmss_objPtr->initCfg.linkingRAM0Size = initCfg->linkingRAM0Size;
    }

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_LINKING_RAM_CONTROL;

        /* Check the Linking RAM 0 and RAM 1 configuration values if RM is plugged */
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            /* Set the return value to signify setting base address registers was denied and set the
              * HW_INIT_COMPLETE to be true in the global config.  This signifies another entity has
              * configured the QMSS and some entries of the qmssGObj may not be valid, such as
              * the initCfg.maxDescNum field */
            retVal = QMSS_RESOURCE_LINKING_RAM_INIT_DENIED;
            qmssGObj.obj.initCfg.qmssHwStatus = QMSS_HW_INIT_COMPLETE;
        }
    }

    if (qmssGObj.obj.initCfg.qmssHwStatus != QMSS_HW_INIT_COMPLETE)
    {
        if (initCfg->linkingRAM0Base == 0)
            qmssLObj.qmConfigReg->LINKING_RAM_REGION_0_BASE_ADDRESS_REG = QMSS_LINKING_RAM_OFFSET;
        else
            qmssLObj.qmConfigReg->LINKING_RAM_REGION_0_BASE_ADDRESS_REG =
                (uint32_t)Qmss_osalVirtToPhy((void *)(initCfg->linkingRAM0Base));

        qmssLObj.qmConfigReg->LINKING_RAM_REGION_0_SIZE_REG = qmss_objPtr->initCfg.linkingRAM0Size;

        qmssLObj.qmConfigReg->LINKING_RAM_REGION_1_BASE_ADDRESS_REG =
            (uint32_t)Qmss_osalVirtToPhy((void *)(initCfg->linkingRAM1Base));
    }

    /* Download Firmware */
    for (index = 0; index < QMSS_MAX_PDSP; index++)
    {
        if (initCfg->pdspFirmware[index].firmware != NULL)
        {
            retVal = Qmss_internaldownloadFirmware (initCfg->pdspFirmware[index].pdspId, initCfg->pdspFirmware[index].firmware, 
                                    initCfg->pdspFirmware[index].size);
        }
    }

    /* Global initializations are done. */

    /* Writeback Global Object */
    Qmss_osalEndMemAccess ((void *) &qmssGObj, sizeof (Qmss_GlobalObj));

    /* End Critical Section */
    Qmss_osalCsExit (key);
    return (retVal);
}

/**
 *  @b Description
 *  @n  
 *      This function initializes the Queue Manager SubsSystem local object blocks in memory 
 *      local to each core and initializes any start parameters like the RM instance. 
 *      
 *      This function must be called atleast once for every core.  It can either be called directly
 *      through the application if RM is to be used or it will be called via Qmss_start if RM is
 *      not to be used.
 *
 *  @pre  
 *      Qmss_init function should be called before calling this function.
 *
 *  @retval
 *      Success -   QMSS_SOK
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 */
Qmss_Result Qmss_startCfg (Qmss_StartCfg *startCfg)
{
    if (qmssLObjIsValid == 0)
    {
        memset ((void *) &qmssLObj, 0, sizeof (Qmss_GlobalConfigParams));
        memcpy ((void *) &qmssLObj, &qmssGObj.obj.qmssGblCfgParams, sizeof (Qmss_GlobalConfigParams));
        qmssLObj.qmRmHandle = startCfg->rmHandle;
        qmssLObjIsValid = 1;
    }

    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function initializes the Queue Manager SubsSystem local object blocks in memory 
 *      local to each core. 
 *      
 *      This function must be called atleast once for every core.
 *
 *  @pre  
 *      Qmss_init function should be called before calling this function.
 *
 *  @retval
 *      Success -   QMSS_SOK
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 */
Qmss_Result Qmss_start (void)
{
    Qmss_StartCfg startCfg;

    /* For backwards compatability make the RM handle NULL */
    startCfg.rmHandle = NULL;
    
    return(Qmss_startCfg(&startCfg));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to obtain the QMSS memory region configuration.
 *      The function returns the descriptor base address, descriptor size and
 *      the number of descriptors for each configured memory region.
 *
 *  @param[out]  memRegInfo
 *      Pointer to the memory region configuration structure allocated by caller.
 * 
 *  @pre  
 *      Qmss_init function should be called before calling this function.
 *       
 *  @retval
 *      Success -   QMSS_SOK
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 */
Qmss_Result Qmss_getMemoryRegionCfg (Qmss_MemRegCfg *memRegInfo)
{
    uint32_t          i;            

    if (memRegInfo == NULL)
        return QMSS_INVALID_PARAM;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* No RM check since readonly operation */

    memset ((void *) memRegInfo, 0, sizeof (Qmss_MemRegCfg));

    /* Invalidate Global Object */
    Qmss_osalBeginMemAccess ((void *) &qmssGObj, sizeof (Qmss_GlobalObj));

    for (i = 0; i < QMSS_MAX_MEM_REGIONS; i++)
    {
        memRegInfo->memRegInfo[i].descBase = qmssGObj.obj.memRegInfo[i].descBase;
        memRegInfo->memRegInfo[i].descSize = qmssGObj.obj.memRegInfo[i].descSize;
        memRegInfo->memRegInfo[i].descNum = qmssGObj.obj.memRegInfo[i].descNum;
        memRegInfo->memRegInfo[i].startIndex = qmssGObj.obj.memRegInfo[i].startIndex;
        memRegInfo->memRegInfo[i].manageDescFlag = qmssGObj.obj.memRegInfo[i].manageDescFlag;
        memRegInfo->memRegInfo[i].memRegion = qmssGObj.obj.memRegInfo[i].memRegion;
    } 
    memRegInfo->currDescCnt = qmssGObj.obj.currDescCnt; 
    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure memory region at runtime.
 *      The function configures specified memory region with descriptor base address, 
 *      descriptor size and the number of descriptors.
 *
 *  @param[out]  memRegCfg
 *      Pointer to the memory region configuration structure allocated by caller.
 *      Descriptor memory address should be a global address
 * 
 *  @pre  
 *      Qmss_init function should be called before calling this function.
 *      Memory Region specified should not have been configured before.
 *       
 *  @retval
 *      Success -   Inserted Memory region index. Range is 0 to 19. 
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure -   QMSS_MEMREGION_ALREADY_INITIALIZED
 *  @retval
 *      Failure -   QMSS_MEMREGION_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_MAX_DESCRIPTORS_CONFIGURED
 *  @retval
 *      Failure -   QMSS_MEMREGION_INVALID_INDEX
 *  @retval
 *      Failure -   QMSS_MEMREGION_OVERLAP
 *  @retval
 *      Failure -   QMSS_QUEUE_OPEN_ERROR
 *  @retval
 *      Failure -   QMSS_RESOURCE_MEMORY_REGION_INIT_DENIED 
 *  @retval
 *      Failure -   QMSS_RESOURCE_LINKING_RAM_INIT_DENIED
 */
Qmss_Result Qmss_insertMemoryRegion (Qmss_MemRegInfo *memRegCfg)
{
    uint32_t          count, powRegSize, reg = 0;
    uint32_t          i, startIndex, nextIndex; 
    uint32_t          descNum, oneBits;
    uint8_t           *descPtr, isAllocated;
    int32_t           queueHnd, index = -1, prevIndex;
    void             *key;
    uint32_t         *descBasePhy;
    Qmss_Result       result;
    uint8_t          *newRegBegAddr, *newRegEndAddr;
    uint32_t          newRegBegIndex, newRegEndIndex;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;

    if (memRegCfg == NULL)
        return QMSS_INVALID_PARAM;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    if ((memRegCfg->memRegion != Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED) &&
         rmPermCallTbl)
    {
        /* If RM permissions call table exists check the init permissions for the memory region */
        resourceData.resourceType = Rm_resource_QMSS_MEMORY_REGION;
        resourceData.res_info.memRegion = memRegCfg->memRegion;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_MEM_REGION_INIT_DENIED;
        }
    }

    /* Descriptor size should be a multiple of 16 */
    if ((memRegCfg->descSize % 16) != 0)
    {
        return QMSS_INVALID_PARAM;
    }

    /* Number of descriptors should be a power of 2 >= 2^5.  The upper limit is checked
     * below inside Qmss_osalCsEnter() */
    descNum = memRegCfg->descNum;
    if (descNum < 32)
    {
        return QMSS_INVALID_PARAM;
    }

    /* count the # of 1 bits, in order to see if its a power of 2 */
    for (oneBits = 0, descNum = memRegCfg->descNum; descNum; descNum >>=1)
    {
        if (descNum & 1) {
            oneBits++;
        }
    }

    /* Check if # of descriptors is a power of 2 */
    if (oneBits != 1) {
        return QMSS_INVALID_PARAM;
    }

    descBasePhy = (uint32_t*)Qmss_osalVirtToPhy((void *)(memRegCfg->descBase));
    if (!descBasePhy) {
        return QMSS_INVALID_PARAM;
    }

    /* The region base address must be aligned to 16 bytes.  It is suggested
     * to align to 64 bytes if the descriptor size > 32 bytes for performance
     * reasons.
     */
    if (((uint32_t)descBasePhy) & 0xf) {
        return QMSS_INVALID_PARAM;
    }

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();
    
    /* Invalidate Global Object */
    Qmss_osalBeginMemAccess ((void *) &qmssGObj, sizeof (Qmss_GlobalObj));
 
    /* Check if maximum descriptors already configured */
    if (qmssGObj.obj.currDescCnt + memRegCfg->descNum > qmssGObj.obj.initCfg.maxDescNum)
    {
        result = QMSS_MAX_DESCRIPTORS_CONFIGURED;
        goto exitCs; /* common exit to close CsEnter and MemAccess */
    }

    /* Memory region to configure is not specified. Find the next available one */
    if (memRegCfg->memRegion == Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED)
    {
        for (i = 0; i < QMSS_MAX_MEM_REGIONS; i++)
        {
            if (qmssGObj.obj.memRegInfo[i].descNum == 0)
            {
                resourceData.resourceType = Rm_resource_QMSS_MEMORY_REGION;
                resourceData.res_info.memRegion = (Qmss_MemRegion) i;

                if ((!rmPermCallTbl) || 
                    (rmPermCallTbl && 
                     (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == 
                       RM_INIT_PERMISSION_APPROVED)))
                {
                    index = i;
                    break;                    
                }
            }
        }
        if (index == 0)
            startIndex = 0;
        else
            startIndex = qmssGObj.obj.memRegInfo[index - 1].descNum + qmssGObj.obj.memRegInfo[index - 1].startIndex;
    }
    else
    {
        index = memRegCfg->memRegion;
        startIndex = memRegCfg->startIndex;
    }

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_LINKING_RAM;
        resourceData.res_info.linkRamData.linkRamStartIndex = startIndex;
        resourceData.res_info.linkRamData.linkRamEndIndex = startIndex + memRegCfg->descNum;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            result = QMSS_RESOURCE_LINKING_RAM_INIT_DENIED;
            goto exitCs;
        }
    }
    
    /* Invalid memory region index */
    if (index < 0 || index >= QMSS_MAX_MEM_REGIONS)
    {
        result = QMSS_MEMREGION_INVALID_INDEX;
        goto exitCs; /* common exit to close CsEnter and MemAccess */
    }

    /* If specified memory region is already configured don't overwrite that configuration */
    if (qmssGObj.obj.memRegInfo[index].descNum != 0)
    {
        result = QMSS_MEMREGION_ALREADY_INITIALIZED;
        goto exitCs; /* common exit to close CsEnter and MemAccess */
    }

    /* startIndex should be a multiple of 32 */
    if ((startIndex % 32) != 0)
    {
        result = QMSS_INVALID_PARAM;
        goto exitCs; /* common exit to close CsEnter and MemAccess */
    }
    

   
    /* Compute boundaries of the new region */
    newRegBegAddr  = (uint8_t *)Qmss_osalVirtToPhy((void *)(memRegCfg->descBase));
    newRegEndAddr  = newRegBegAddr + memRegCfg->descNum * memRegCfg->descSize;
    newRegBegIndex = startIndex;
    newRegEndIndex = newRegBegIndex + memRegCfg->descNum;

    /* Check if the new region overlaps any existing regions.  This is done 
     * with a brute force search that doesn't depend on ordering
     *
     * This allows overlap check to be kept if future hardware relaxes the
     * ordering requirements.
     */
    for (i = 0; i < QMSS_MAX_MEM_REGIONS; i++) 
    {
        Qmss_MemRegInfo *checkReg = &qmssGObj.obj.memRegInfo[i];
        uint8_t *checkRegBegAddr  = 0;
        uint8_t *checkRegEndAddr  = 0;
        uint32_t checkRegBegIndex = checkReg->startIndex;
        uint32_t checkRegEndIndex = checkRegBegIndex + checkReg->descNum;
        
        if(checkReg->descBase)
        {
            /* Address translation if active */
            checkRegBegAddr = (uint8_t *)Qmss_osalVirtToPhy((void *)(checkReg->descBase));
        }
        checkRegEndAddr = checkRegBegAddr + (checkReg->descNum * checkReg->descSize);

        if (i == index) 
        {
            continue; /* don't test against myself */
        }

        if (! checkRegBegAddr) {
            continue; /* Check region hasn't been initialized */
        }

        /* Check the physical addresses for overlap */
        /* A) Is the new region's start address overlapping the check region? */
        if ((newRegBegAddr >= checkRegBegAddr) && 
            (newRegBegAddr < checkRegEndAddr)) 
        {   /* The new region starts inside the check region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* B) Is the new region's end address overlapping the check region? */
        if ((newRegEndAddr > checkRegBegAddr) && 
            (newRegEndAddr <= checkRegEndAddr)) 
        {   /* The new region ends inside the check region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* C) Is the check region completely inside the new region? */
        if ((checkRegBegAddr >= newRegBegAddr) && 
            (checkRegEndAddr <= newRegEndAddr)) 
        {   /* The check region is contained within the new region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* It is not necessary to check if the new region is completely inside
         * the check region because that is covered by A and B above since both
         * its start and end addresses are inside the check region.
         */

        /* Check the linking RAM index for overlap */
        /* Is the new region's start index overlapping the check region? */
        if ((newRegBegIndex >= checkRegBegIndex) && 
            (newRegBegIndex < checkRegEndIndex)) 
        {   /* The new region starts inside the check region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* Is the new region's end index overlapping the check region? */
        if ((newRegEndIndex > checkRegBegIndex) && 
            (newRegEndIndex <= checkRegEndIndex)) 
        {   /* The new region ends inside the check region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* Is the check region completely inside the new region? */
        if ((checkRegBegIndex >= newRegBegIndex) && 
            (checkRegEndIndex <= newRegEndIndex)) 
        {   /* The check region is contained within the new region */
            result = QMSS_MEMREGION_OVERLAP;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
        /* It is not necessary to check if the new region is completely inside
         * the check region because that is covered by A and B above since both
         * its start and end addresses are inside the check region.
         */
    }

    /* Find next and previous initialized regions to check for ordering */
    for (prevIndex = index-1; 
         (prevIndex >= 0 && (qmssGObj.obj.memRegInfo[prevIndex].descNum == 0)); 
         prevIndex--);
    for (nextIndex = index+1; 
         ((nextIndex <= (QMSS_MAX_MEM_REGIONS-1) && 
          (qmssGObj.obj.memRegInfo[nextIndex].descNum == 0))); 
         nextIndex++);

    /* If there is a previous region make sure its index and its physical 
     * address is lower than the new region's.
     *
     * It is assumed that the index and address do not overlap the previous 
     * region since that is already verified above.
     */
    if (prevIndex >= 0) 
    {
        Qmss_MemRegInfo *prevReg       = &qmssGObj.obj.memRegInfo[prevIndex];

        /* Check that the linking RAM index is strictly increasing */
        if (prevReg->startIndex >= newRegBegIndex) {
            result = QMSS_MEMREGION_ORDERING;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }

        /* Check that the physical memory address is strictly increasing */
        if (((uint8_t *)Qmss_osalVirtToPhy((void *)(prevReg->descBase))) >= newRegBegAddr) {
            result = QMSS_MEMREGION_ORDERING;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
    }

    /* If there is a next region make sure its index and its physical 
     * address is lower than the new region's.
     *
     * It is assumed that the index and address do not overlap the next 
     * region since that is already verified above.
     */
    if (nextIndex < QMSS_MAX_MEM_REGIONS) 
    {
        Qmss_MemRegInfo *nextReg       = &qmssGObj.obj.memRegInfo[nextIndex];

        /* Check that the linking RAM index is strictly increasing */
        if (nextReg->startIndex <= newRegBegIndex) {
            result = QMSS_MEMREGION_ORDERING;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }

        /* Check that the physical memory address is strictly increasing */
        if (((uint8_t *)Qmss_osalVirtToPhy((void *)(nextReg->descBase))) <= newRegBegAddr) {
            result = QMSS_MEMREGION_ORDERING;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
    }

    /* If the qmssGObj is valid (i.e. HW INIT was performed in the DSP subsystem
      * check that there is sufficient linking RAM */
    if (qmssGObj.obj.initCfg.qmssHwStatus != QMSS_HW_INIT_COMPLETE)
    {
        if (newRegEndIndex > qmssGObj.obj.initCfg.maxDescNum)
        {
            result = QMSS_INVALID_PARAM;
            goto exitCs; /* common exit to close CsEnter and MemAccess */
        }
    }

    /* Configure the QMSS memory regions */
    qmssLObj.qmDescReg->MEMORY_REGION_BASE_ADDRESS_GROUP[index].MEMORY_REGION_BASE_ADDRESS_REG
        = (uint32_t)descBasePhy;

    CSL_FINS (qmssLObj.qmDescReg->MEMORY_REGION_BASE_ADDRESS_GROUP[index].MEMORY_REGION_START_INDEX_REG, 
        QM_DESCRIPTOR_REGION_CONFIG_MEMORY_REGION_START_INDEX_REG_START_INDEX, startIndex);

    CSL_FINS (reg, QM_DESCRIPTOR_REGION_CONFIG_MEMORY_REGION_DESCRIPTOR_SETUP_REG_DESC_SIZE, 
        ((memRegCfg->descSize / 16) - 1));

    for (powRegSize = 0; (32UL << powRegSize) < memRegCfg->descNum; powRegSize++);

    CSL_FINS (reg, QM_DESCRIPTOR_REGION_CONFIG_MEMORY_REGION_DESCRIPTOR_SETUP_REG_REG_SIZE, powRegSize);

    qmssLObj.qmDescReg->MEMORY_REGION_BASE_ADDRESS_GROUP[index].MEMORY_REGION_DESCRIPTOR_SETUP_REG = reg; 

    /* Check if LLD is supposed to manage the descriptors, if so slice up the 
     * descriptors and push to queues. else do nothing the caller will manage 
     * the descriptors in memory region */
    if (memRegCfg->manageDescFlag)
    {
        /* Split and push to general purpose queues */

        /* Open general purpose queue */
        queueHnd = Qmss_internalQueueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated);
        if (queueHnd < 0)
        {            
            result = QMSS_QUEUE_OPEN_ERROR;
            goto exitCs; /* common exit to close osalCsEnter and osalBeginMemAccess */
        }
            
        /* Store the handle */
        qmssGObj.obj.descQueue[index] = queueHnd;
        
        descPtr = (uint8_t *) memRegCfg->descBase;
        for (count = 0; count < memRegCfg->descNum; count ++) 
        {
            /* TODO QueueProxy is not working add this later 
             * Qmss_queuePushDesc (queueHnd, (uint32_t *)descPtr, initCfgPtr->memRegionInfo[index].descSize); */
        
            Qmss_queuePushDesc (queueHnd, (uint32_t *) descPtr);
            descPtr = descPtr + memRegCfg->descSize;
        }
    }

    /* Store in internal data structures */
    qmssGObj.obj.memRegInfo[index].descBase = memRegCfg->descBase;
    qmssGObj.obj.memRegInfo[index].descSize = memRegCfg->descSize;
    qmssGObj.obj.memRegInfo[index].descNum = memRegCfg->descNum;
    qmssGObj.obj.memRegInfo[index].startIndex = startIndex;
    qmssGObj.obj.memRegInfo[index].manageDescFlag = memRegCfg->manageDescFlag;
    qmssGObj.obj.memRegInfo[index].memRegion = (Qmss_MemRegion) index;
    qmssGObj.obj.currDescCnt += memRegCfg->descNum;

    /* Return value */
    result = index;
    
exitCs:
    /* Writeback Global Object */
    Qmss_osalEndMemAccess ((void *) &qmssGObj, sizeof (Qmss_GlobalObj));

    /* End Critical Section */
    Qmss_osalCsExit (key);
    return result;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to obtain previously allocated descriptors.
 *      The function opens a destination queue as specified in the input parameters 
 *      and pushes the requested number of descriptors if available onto it. 
 *
 *  @param[in]  descCfg
 *      Specifies the number of descriptors, memory region from where it should be allocated 
 *      and the destination queue to push to.
 *      
 *  @param[out]  numAllocated
 *      Number of descriptors actually allocated.
 * 
 *  @pre  
 *      Qmss_init function should be called before calling this function.
 *       
 *  @retval
 *      Success -   Destination queue handle on which the allocated descriptors are stored. 
 *                  The queue must be closed by the caller using Qmss_queueClose() when done.

 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_MEMREGION_NOT_INITIALIZED
 *  @retval
 *      Failure -   QMSS_QUEUE_OPEN_ERROR
 *  @retval
 *      Failure -   QMSS_RESOURCE_MEM_REGION_USE_DENIED
 */
Qmss_QueueHnd Qmss_initDescriptor (Qmss_DescCfg *descCfg, uint32_t *numAllocated)
{
    uint32_t              count;
    uint32_t              *descPtr;
    uint8_t               isAllocated;
    int32_t               srcQueNum, srcQueHnd, destQueHnd;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;

    /* Get the descriptor from the source queue */
    
    if (qmssLObjIsValid == 0)
    {
        return QMSS_NOT_INITIALIZED;
    }

	if (descCfg == NULL)
	{
        return QMSS_INVALID_PARAM;			
	}	

    if (rmPermCallTbl)
    {
        /* If RM permissions call table exists check the use permissions for the memory region */
        resourceData.resourceType = Rm_resource_QMSS_MEMORY_REGION;
        resourceData.res_info.memRegion = (Qmss_MemRegion) descCfg->memRegion;
        
        if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_MEM_REGION_USE_DENIED;
        }
    }

	/* Find the source queue given the memory region */
	if ((srcQueNum = Qmss_getMemRegQueueHandle (descCfg->memRegion)) < 0)
	{
        return QMSS_MEMREGION_NOT_INITIALIZED;
	}	

	/* Find the descriptor size in the given the memory region */
	if ((Qmss_getMemRegDescSize (descCfg->memRegion)) == 0)
	{
        return QMSS_MEMREGION_NOT_INITIALIZED;
	}	

	/* Open destination queue specified in the configuration */
    if ((destQueHnd = Qmss_queueOpen (descCfg->queueType, descCfg->destQueueNum, &isAllocated)) < 0)
	{
		return QMSS_QUEUE_OPEN_ERROR;
	}

	/* Open general purpose queue on which descriptors are stored */
    srcQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, srcQueNum, &isAllocated);
    *numAllocated = 0;
	for (count = 0; count < descCfg->descNum; count++)
	{
		/* Pop descriptor from source queue */
		if ((descPtr = Qmss_queuePop (srcQueHnd)) == NULL)
		{
			break;
		}

        /* Push descriptor to the specified destination queue */
        
        /* TODO QueueProxy is not working add this later 
         * Qmss_queuePushDesc (destQueHnd, descPtr, descSize); */
        
        Qmss_queuePushDesc (destQueHnd, descPtr);
        *numAllocated += 1;
	}

    return destQueHnd;
}

/**
 *  @b Description
 *  @n  
 *      This function opens the requested queue.
 *      A queue can be opened in two ways:
 *          1) If "queNum" is set to QMSS_PARAM_NOT_SPECIFIED, then a new
 *          available queue of type "queType" is allocated.
 *          2) If "queNum" is a valid queue number i.e., >= 0, 
 *               then a queue is allocated if free 
 *               else
 *               a handle to a previously opened queue is returned.
 *               In this case "queType" parameter is not used.
 *
 *  @param[in]  queType
 *      Specifies the type of queue that should be opened. 
 * 
 *  @param[in]  queNum
 *      Specifies the queue number that should be opened.
 *      
 *  @param[out]  isAllocated
 *      Indicates whether the requested queue is a new queue allocation(1).
 *      or was already allocated. If the queue was previously allocated this parameter returns 
 *      the reference count.
 *
 *  @retval
 *      Success -   Queue Handle. Used as an input parameter for operation on this queue.
 *  @retval
 *      Failure -   QMSS_RESOURCE_USE_DENIED 
 *  @retval
 *      Failure -   <0
 */
Qmss_QueueHnd Qmss_queueOpen (Qmss_QueueType queType, int32_t queNum, uint8_t *isAllocated)
{
    Qmss_QueueHnd   result;
    void            *key;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    result = Qmss_internalQueueOpen (queType, queNum, isAllocated);

    /* End Critical Section */   
    Qmss_osalCsExit (key);

    return (Qmss_QueueHnd) result;
}

/**
 *  @b Description
 *  @n  
 *      This function opens the requested queue if a free queue is avialable within the given start and end range. 
 *
 *  @param[in]  startQueNum
 *      Specifies the start of the queue number range.
 * 
 *  @param[in]  endQueNum
 *      Specifies the end of the queue number range.
 * 
 *  @param[out]  isAllocated
 *      Indicates whether the requested queue is a new queue allocation(1).
 *      or was already allocated. If the queue was previously allocated this parameter returns 
 *      the reference count.
 *
 *  @retval
 *      Success -   Queue Handle. Used as an input parameter for operation on this queue.
 *
 *  @retval
 *      Failure -   <0
 */
Qmss_QueueHnd Qmss_queueOpenInRange (uint32_t startQueNum, uint32_t endQueNum, uint8_t *isAllocated)
{
    void            *key;
    uint32_t        index;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;    

    resourceData.resourceType = Rm_resource_QMSS_QUEUE;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Invalidate queueFree Object */
    Qmss_osalBeginMemAccess ((void *) &queueFree[startQueNum], endQueNum - startQueNum);

    *isAllocated = 0;

    for (index = startQueNum; index < endQueNum; index++)
    {
        resourceData.res_info.queNum = index;
      
        if ((!rmPermCallTbl) ||
             (rmPermCallTbl &&
              (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_APPROVED)))
        {
            if (queueFree[index] == 0)
            {
                queueFree[index]++;

                /* Writeback queueFree Object */
                Qmss_osalEndMemAccess ((void *) &queueFree[index], QMSS_MAX_CACHE_ALIGN);

                *isAllocated = queueFree[index];

                /* End Critical Section */   
                Qmss_osalCsExit (key);

                return (Qmss_QueueHnd) index;
            }
        }
    }

    /* End Critical Section */   
    Qmss_osalCsExit (key);

    return (Qmss_QueueHnd) -1;
}

/**
 *  @b Description
 *  @n  
 *      This function closes a queue. 
 *      The queue reference count is decremented. The queue is freed only if the 
 *      reference count is zero. 
 *
 *  @param[in]  hnd
 *      Queue handle.
 * 
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @post  
 *      Queue is freed if the reference count is zero and available for reallocation.
 *
 *  @retval
 *      Success -   Current reference count.
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 */
Qmss_Result Qmss_queueClose (Qmss_QueueHnd hnd)
{
    void        *key;

    /* No RM check since check performed in open */

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Invalidate queueFree Object */
    Qmss_osalBeginMemAccess ((void *) &queueFree[hnd], QMSS_MAX_CACHE_ALIGN);
 
    if (queueFree[hnd] == 0)
    {
        /* End Critical Section */   
        Qmss_osalCsExit (key);
        return QMSS_INVALID_PARAM;
    }
    else
    {
        queueFree[hnd]--;

        /* Writeback queueFree Object */
        Qmss_osalEndMemAccess ((void *) &queueFree[hnd], QMSS_MAX_CACHE_ALIGN);

        /* End Critical Section */   
        Qmss_osalCsExit (key);
    }
    return queueFree[hnd];
}

/**
 *  @b Description
 *  @n  
 *      This function returns the threshold at which the queue threshold pin is asserted
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Queue threshold
 */
uint32_t Qmss_getQueueThreshold (Qmss_QueueHnd hnd)
{
    /* No RM check since read only */
  
    return (uint32_t) CSL_FEXT (qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_D,
                    QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_D_THRESHOLD); 
}

/**
 *  @b Description
 *  @n  
 *      This function sets the threshold at which the queue threshold pin is asserted and 
 *      threshold bit in threshold status register is set. 
 *      "hilo" indicates whether the number of items in the queue should be greater than/equal to 
 *      OR less than the confgiured threshold value before the queue ecnt status bit is asserted and
 *      threshold bit in threshold status register is set.
 *      The threshold value is 10'h3ff when it is ten or higher. It is (2^threshold-1)  for other values.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @param[in]  hilo
 *      1 - High
 *      0 - Low.
 *
 *  @param[in]  threshold
 *      Threshold value.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      Qmss_Result
 */
Qmss_Result Qmss_setQueueThreshold (Qmss_QueueHnd hnd, uint16_t hilo, uint8_t threshold)
{
    uint32_t  temp = 0;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;   

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QUEUE;
        resourceData.res_info.queNum = hnd;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
    
    if (threshold > 10)
        threshold = 10;
    CSL_FINS (temp, QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_D_THRESHOLD_HILO, hilo);
    CSL_FINS (temp, QM_QUEUE_STATUS_CONFIG_QUEUE_STATUS_CONFIG_REG_D_THRESHOLD, threshold);
    
    qmssLObj.qmQueStatReg->QUEUE_STATUS_CONFIG_GROUP[hnd].QUEUE_STATUS_CONFIG_REG_D = temp; 
    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      The starvation count is incremented every time the Free Descriptor/Buffer 
 *      queue is read when empty. This function returns the starvation count of queue.
 * 
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      8 bit Starvation count
 */
uint32_t Qmss_getStarvationCount (Qmss_QueueHnd hnd)
{
    uint32_t queNum, regIndex, bitField;

    /* No RM check since read only */

    queNum = hnd - qmssLObj.maxQueueNum[Qmss_QueueType_STARVATION_COUNTER_QUEUE].startIndex;
    regIndex = queNum / 4;
    bitField = (queNum % 4) * 8;
  
    return (uint32_t) CSL_FEXTR (qmssLObj.qmConfigReg->FREE_DESCRIPTOR_STARVE_COUNT_REG[regIndex], bitField + 7, bitField);
}

/**
 *  @b Description
 *  @n  
 *      This function returns the threshold status of the queue. There are 2 conditions under which the 
 *      threshold bit is set
 *
 *      The threshold bit is set for a queue if the number of element in a queue is above or below 
 *      a certain threshold number of items configured using Qmss_setQueueThreshold() API. 
 *
 *      The threshold bit is set for a queue if there is atleast 1 element on the queue when the threshold 
 *      is not set Qmss_setQueueThreshold() API
 *
 *  @param[in]  hnd
 *      Queue handle.
 *
 *  @pre  
 *      Qmss_queueOpen function should be called before calling this function.
 *
 *  @retval
 *      0 - Configured threshold is not met
 *  @retval
 *      1 - Configured threshold is met
 */
uint16_t Qmss_getQueueThresholdStatus (Qmss_QueueHnd hnd)
{
    uint32_t regIndex, bitField;

    /* No RM check since read only */

    regIndex = hnd / 32;
    bitField = hnd % 32;

    return (uint16_t) CSL_FEXTR (qmssLObj.qmStatusRAM->QUEUE_THRESHOLD_STATUS_REG[regIndex], bitField, bitField);
}

/** 
 *  @b Description
 *  @n  
 *      Given the queue handle the function returns the queue manager and queue number within
 *      the queue manager corresponding to the handle
 *
 *  @param[in]  hnd      
 *      Queue handle
 *
 *  @retval
 *      Queue Manager and Queue Number within the Queue Manager
 */
Qmss_Queue Qmss_getQueueNumber (Qmss_QueueHnd hnd)
{
    Qmss_Queue              queue;

    /* No RM check since read only */

    queue.qMgr = CSL_FEXTR (hnd, 13, 12);
    queue.qNum = CSL_FEXTR (hnd, 11, 0);
    return (queue);
}

/** 
 *  @b Description
 *  @n  
 *      Given the queue manager and queue number within the queue manager 
 *      the function returns the corresponding queue handle
 *
 *  @param[in]  queue      
 *      Queue Manager number and Queue Number within the Queue Manager
 *
 *  @retval
 *      Queue handle corresponding to the queue manager and queue number
 */
Qmss_QueueHnd Qmss_getQueueHandle (Qmss_Queue queue)
{
    Qmss_QueueHnd           hnd = 0;

    /* No RM check since read only */

    CSL_FINSR (hnd, 13, 12, queue.qMgr);
    CSL_FINSR (hnd, 11, 0, queue.qNum);
    return (hnd);
}

/** 
 *  @b Description
 *  @n  
 *      Downloads the PDSP firmware to PDSP. Not supported in simulator
 *      
 *      **Not tested**.
 *
 *  @param[in]  pdspId      
 *      PDSP number where the firmware should be downloaded
 *
 *  @param[in]  image      
 *      Pointer to the firmware image
 *
 *  @param[in]  size      
 *      Size of firmware image in bytes
 *
 *  @retval
 *      Success -   Firmware is downloaded to PDSP
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure -   QMSS_INVALID_PARAM
 *  @retval
 *      Failure -   QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_downloadFirmware (Qmss_PdspId pdspId, void *image, uint32_t size)
{
    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    if ((image == NULL) || (size == 0))
        return QMSS_INVALID_PARAM;

    return (Qmss_internaldownloadFirmware (pdspId, image, size));
}

static Qmss_Result Qmss_internaldownloadFirmware (Qmss_PdspId pdspId, void *image, uint32_t size)
{
    uint32_t            i, count;
    volatile uint32_t   enable;
    uint32_t            *data = (uint32_t *) image;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_FIRMWARE_PDSP;
        resourceData.res_info.pdspNum = pdspId;
        
        if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_USE_DENIED;
        }
        else if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            /* Use but don't init - Check firmware revision */
        }
    }

    /* Reset the PDSP */
    CSL_FINS (qmssLObj.qmPdspCtrlReg[pdspId]->PDSP_CONTROL_REG, PDSP_PDSP_CONTROL_REG_PDSP_ENABLE, 0);

    /* Confirm PDSP has halted */
    do
    { 
        enable = CSL_FEXT (qmssLObj.qmPdspCtrlReg[pdspId]->PDSP_CONTROL_REG, PDSP_PDSP_CONTROL_REG_PDSP_STATE);
    }while (enable);

    count = size / 4;

    /* Download the firmware */
    for(i = 0; i < count; i++)
        qmssLObj.qmPdspIRamReg[pdspId][i] = data[i];

    /* Use the command register to sync the PDSP */
    *qmssLObj.qmPdspCmdReg[pdspId] = 0xFFFFFFFF;

    /* Wait to the memory write to land */
    for(i = 0; i < 20000 && *qmssLObj.qmPdspCmdReg[pdspId] != 0xFFFFFFFF; i++);
   
    /* Reset program counter to zero */
	CSL_FINS (qmssLObj.qmPdspCtrlReg[pdspId]->PDSP_CONTROL_REG, PDSP_PDSP_CONTROL_REG_PCOUNTER_RST_VAL, 0);
    
	/* Set soft reset to zero to load the program counter */
	CSL_FINS (qmssLObj.qmPdspCtrlReg[pdspId]->PDSP_CONTROL_REG, PDSP_PDSP_CONTROL_REG_SOFT_RST_N, 0);
    
    /* Enable the PDSP */
    CSL_FINS (qmssLObj.qmPdspCtrlReg[pdspId]->PDSP_CONTROL_REG, PDSP_PDSP_CONTROL_REG_PDSP_ENABLE, 1);

    /* Wait for the command register to clear */
    while (*qmssLObj.qmPdspCmdReg[pdspId]);

    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function sets the end of interrupt vector value for the specified interrupt. 
 *      This API is used by the host to indicate 
 *          - The end of interrupt processing to the accumulator. The accumulator 
 *              is then free to write to the accumulator ping/pong page released by the host.
 *          - The end of interrupt processing for buffer descriptor starvation event on 
 *              receive SOP and MOP for any of the RX DMA units in the CDMA.
 *
 *  @param[in]  type
 *      Interrupt  to program. 
 *          Qmss_IntdInterruptType_HIGH selects the interrupts corresponding to the high priority accumulator
 *          Qmss_IntdInterruptType_LOW selects the interrupts corresponding to the low priority accumulator
 *          Qmss_IntdInterruptType_CDMA selects the interrupts corresponding to the low priority accumulator
 *
 *  @param[in]  accumCh
 *      Accumulator channel which is mapped to interrupt number via Qmss_xxxEoiVector mapping 
 *      table.  The resulting interrupt number is set. 
 *
 *      **No validation is done to check if the interrupt number is within range to reduce overhead.**
 *
 *  @post  
 *      EOI vector value is set indicating end of host interrupt processing.
 *
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - QMSS_RESOURCE_INIT_DENIED
 */

Qmss_Result Qmss_setEoiVector (Qmss_IntdInterruptType type, uint8_t accumCh)
{ 
    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    if (type == Qmss_IntdInterruptType_HIGH)
        CSL_FINS (qmssLObj.qmQueIntdReg->EOI_REG, QM_INTD_EOI_REG_EOI_VECTOR, Qmss_highEoiVector[accumCh]);
    else if (type == Qmss_IntdInterruptType_LOW)
        CSL_FINS (qmssLObj.qmQueIntdReg->EOI_REG, QM_INTD_EOI_REG_EOI_VECTOR, Qmss_lowEoiVector[accumCh]);
    else 
        CSL_FINS (qmssLObj.qmQueIntdReg->EOI_REG, QM_INTD_EOI_REG_EOI_VECTOR, Qmss_cdmaEoiVector[accumCh]);
    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      This function decrements the interrupt count. The Interrupt Count Registers contains a 
 *      count of the interrupts that have triggered and not been processed. Writing a non-zero 
 *      value to the register subtracts that value from the count. Writing a 0 clears the count. 
 *      The count saturates at 3.
 *
 *      Note: Clearing the count does not clear the interrupt. The EOI Vector must be set using Qmss_setEoiVector API.
 *  
 *  @param[in]  interruptNum
 *      Interrupt Number that should be acknowledged. 
 *
 *  @param[in]  value
 *      Value to decrement by or zero to clear. 
 *      
 *      **No validation is done to check if the interrupt number is within range to reduce overhead.**
 *
 *  @post  
 *      Count is decremented or cleared depending on input parameter. 
 *
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - QMSS_RESOURCE_INIT_DENIED 
 */

Qmss_Result Qmss_ackInterrupt (uint8_t interruptNum, uint8_t value)
{
    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;
    
    qmssLObj.qmQueIntdReg->INTCNT_REG[interruptNum] = value;
    return QMSS_SOK;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version information of the QMSS LLD.
 *
 *  @retval
 *      Version Information.
 */
uint32_t Qmss_getVersion (void)
{
    return QMSS_LLD_VERSION_ID;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version string for the QMSS LLD.
 *
 *  @retval
 *      Version String.
 */
const char* Qmss_getVersionStr (void)
{
    return qmssLldVersionStr;
}

/**
@}
*/

/** 
 *  @b Description
 *  @n  
 *      The function return the source queue handle on which the descriptors configured for 
 *      the specified memory region are stored.
 *
 *      This is for *internal* LLD usage.
 *
 *  @param[in]  memRegion      
 *      Memory region number
 *
 *  @retval
 *      Success -   Queue Handle
 *  @retval
 *      Failure -   QMSS_NOT_INITIALIZED
 */
int32_t Qmss_getMemRegQueueHandle(uint32_t memRegion)
{
    /* No RM check since internal LLD usage */
  
    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;
    else
    {
        /* Invalidate Global Object */
        Qmss_osalBeginMemAccess ((void *) &qmssGObj, sizeof (Qmss_GlobalObj));
        return (qmssGObj.obj.descQueue[memRegion]);
    }
}

/** 
 *  @b Description
 *  @n  
 *      The function returns size of the descriptors in the specified memory region.
 *
 *      This is for *internal* LLD usage.
 *
 *  @param[in]  memRegion      
 *      Memory region number.
 *
 *  @retval
 *      Descriptor size
 */
uint32_t Qmss_getMemRegDescSize (uint32_t memRegion)
{
    uint32_t  descVal;

    /* No RM check since internal LLD usage */
        
    descVal = CSL_FEXT (qmssLObj.qmDescReg->MEMORY_REGION_BASE_ADDRESS_GROUP[memRegion].MEMORY_REGION_DESCRIPTOR_SETUP_REG, 
            QM_DESCRIPTOR_REGION_CONFIG_MEMORY_REGION_DESCRIPTOR_SETUP_REG_DESC_SIZE);
    return ((descVal + 1) * 16);
}

/**
 *  @b Description
 *  @n  
 *      Internal queue open function; is called to allocate a queue when critical section is provided 
 *      by the caller. 
 *
 *      A queue can be opened in two ways:
 *          1) If "queNum" is set to QMSS_PARAM_NOT_SPECIFIED, then a new
 *          available queue of type "queType" is allocated.
 *          2) If "queNum" is a valid queue number i.e., >= 0, 
 *               then a queue is allocated if free 
 *               else
 *               a handle to a previously opened queue is returned.
 *               In this case "queType" parameter is not used.
 *
 *  @param[in]  queType
 *      Specifies the type of queue that should be opened. 
 * 
 *  @param[in]  queNum
 *      Specifies the queue number that should be opened.
 *      
 *  @param[out]  isAllocated
 *      Indicates whether the requested queue is a new queue allocation(1).
 *      or was already allocated. If the queue was previously allocated this parameter returns 
 *      the reference count.
 *
 *  *  @pre  
 *      Appropriate Critical section protection should be provided by caller.
 *
 *  @retval
 *      Success -   Queue Handle. Used as an input parameter for operation on this queue.
 *  @retval
 *      Failure -   QMSS_RESOURCE_USE_DENIED
 *  @retval
 *      Failure -   <0
 */
static Qmss_QueueHnd Qmss_internalQueueOpen (Qmss_QueueType queType, int queNum, uint8_t *isAllocated)
{
    uint32_t      index, startQueue, endQueue;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;   

    resourceData.resourceType = Rm_resource_QMSS_QUEUE;
   
    *isAllocated = 0;

    if (queNum >= 0)
    {
        if (rmPermCallTbl)
        {
            resourceData.res_info.queNum = queNum;
            
            if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED)
            {
                return QMSS_RESOURCE_USE_DENIED;
            }
        }
      
        /* Invalidate queueFree Object */
        Qmss_osalBeginMemAccess ((void *) &queueFree[queNum], QMSS_MAX_CACHE_ALIGN);

        queueFree[queNum]++;

        /* Writeback queueFree Object */
        Qmss_osalEndMemAccess ((void *) &queueFree[queNum], QMSS_MAX_CACHE_ALIGN);

        *isAllocated = queueFree[queNum];
        return (Qmss_QueueHnd) queNum;
    }

    /* Get the range to allocate from */
    startQueue = qmssLObj.maxQueueNum[queType].startIndex;
    endQueue = qmssLObj.maxQueueNum[queType].startIndex + qmssLObj.maxQueueNum[queType].maxNum;

    /* Invalidate queueFree Object */
    Qmss_osalBeginMemAccess ((void *) &queueFree[startQueue], qmssLObj.maxQueueNum[queType].maxNum);

    for (index = startQueue; index < endQueue; index++)
    {
        resourceData.res_info.queNum = index;
          
        if ((!rmPermCallTbl) ||
             (rmPermCallTbl &&
              (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_APPROVED)))
        {
            if (queueFree[index] == 0)
            {
                queueFree[index]++;

                /* Writeback queueFree Object */
                Qmss_osalEndMemAccess ((void *) &queueFree[index], QMSS_MAX_CACHE_ALIGN);

                *isAllocated = queueFree[index];
                return (Qmss_QueueHnd) index;
            }
        }
    }
    return (Qmss_QueueHnd) -1;
}

