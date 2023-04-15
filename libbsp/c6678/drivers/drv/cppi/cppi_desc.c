/**
 *   @file  cppi_desc.c
 *
 *   @brief   
 *      This is the CPPI descriptor management APIs file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
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
#include <ti/drv/cppi/cppi_desc.h>
#include <ti/drv/cppi/include/cppi_pvt.h>

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_drv.h>

/**********************************************************************
 ************************** APIs **************************************
 **********************************************************************/

/** @addtogroup CPPI_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function is used to obtain previously allocated descriptors.
 *      The function opens an destination queue as specified in the input parameters 
 *      and pushes the requested number of descriptors from specified memory region, if available onto it.
 *      The host and monolithic descriptors are configured with values specified in "descCfg"
 *      before pushing them to the destination queue.
 *      The configuration for host descriptors are return policy, return push policy, return queue.
 *      The configuration for monolithic descriptors is the return queue.
 *      If the return queue is set to QMSS_PARAM_NOT_SPECIFIED then the destination queue specified in input
 *      parameter is configured to as return queue.
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
 *      Failure -   CPPI_INVALID_PARAM
 *  @retval
 *      Failure -   CPPI_QMSS_MEMREGION_NOT_INITIALIZED
 *  @retval
 *      Failure -   CPPI_QUEUE_OPEN_ERROR
 */
Qmss_QueueHnd Cppi_initDescriptor (Cppi_DescCfg *descCfg, uint32_t *numAllocated)
{
    uint32_t                count;
    Cppi_HostDesc           *descPtr;
    uint8_t                 isAllocated;
    int32_t                 srcQueNum, srcQueHnd, destQueHnd;

    /* Get the descriptor from the source queue */
    
	if (descCfg == NULL)
	{
		return CPPI_INVALID_PARAM;			
	}	
	if ((descCfg->descType != Cppi_DescType_HOST) && (descCfg->descType != Cppi_DescType_MONOLITHIC))
        return CPPI_INVALID_PARAM;			

	/* Find the source queue given the memory region */
	if ((srcQueNum = Qmss_getMemRegQueueHandle(descCfg->memRegion)) < 0)
	{
		return CPPI_QMSS_MEMREGION_NOT_INITIALIZED;
	}	

	/* Find the descriptor size in the given the memory region */
	if ((Qmss_getMemRegDescSize(descCfg->memRegion)) == 0)
	{
		return CPPI_QMSS_MEMREGION_NOT_INITIALIZED;
	}	

	/* Open destination queue specified in the configuration */
    if ((destQueHnd = Qmss_queueOpen (descCfg->queueType, descCfg->destQueueNum, &isAllocated)) < 0)
	{
		return CPPI_QUEUE_OPEN_ERROR;
	}

	/* Open general purpose queue on which descriptors are stored */
    if ((srcQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, srcQueNum, &isAllocated)) < 0)
	{
		return CPPI_QUEUE_OPEN_ERROR;
	}

    *numAllocated = 0;
	for (count = 0; count < descCfg->descNum; count++)
	{
		/* Pop descriptor from source queue */
		if ((descPtr = (Cppi_HostDesc *) Qmss_queuePop (srcQueHnd)) == NULL)
		{
			break;
		}

        /* Check if descriptor should be configured or not */
        if (descCfg->initDesc == Cppi_InitDesc_INIT_DESCRIPTOR)
        {
            /* Configure the descriptors with parameters provided */

            CSL_FINSR (descPtr->descInfo, 31, 30, descCfg->descType);
            if (descCfg->descType == Cppi_DescType_HOST)
            {
                CSL_FINSR (descPtr->descInfo, 22, 22, descCfg->cfg.host.psLocation);
                CSL_FINSR (descPtr->packetInfo, 15, 15, descCfg->cfg.host.returnPolicy);
                CSL_FINSR (descPtr->packetInfo, 14, 14, descCfg->returnPushPolicy);
                CSL_FINSR (descPtr->packetInfo, 31, 31, descCfg->epibPresent);
                descPtr->nextBDPtr = 0;
            }
            else if (descCfg->descType == Cppi_DescType_MONOLITHIC)
            {
                CSL_FINSR (descPtr->packetInfo, 14, 14, descCfg->returnPushPolicy);
                CSL_FINSR (descPtr->descInfo, 24, 16, descCfg->cfg.mono.dataOffset);
                CSL_FINSR (descPtr->packetInfo, 31, 31, descCfg->epibPresent);
            }
            if ((descCfg->returnQueue.qMgr == QMSS_PARAM_NOT_SPECIFIED) && 
                (descCfg->returnQueue.qNum == QMSS_PARAM_NOT_SPECIFIED))
            {
                Qmss_Queue              queue;
                
                queue = Qmss_getQueueNumber(destQueHnd);
                CSL_FINSR (descPtr->packetInfo, 13, 12, queue.qMgr);
                CSL_FINSR (descPtr->packetInfo, 11, 0, queue.qNum);
            }
            else
            {
                CSL_FINSR (descPtr->packetInfo, 13, 12, descCfg->returnQueue.qMgr);
                CSL_FINSR (descPtr->packetInfo, 11, 0, descCfg->returnQueue.qNum);
            }
        }
        /* Push descriptor to the specified destination queue */
		Qmss_queuePushDesc (destQueHnd, descPtr);

        *numAllocated += 1;
	}

    return destQueHnd;
}


/**
@}
*/
