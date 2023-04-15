/**
 *   @file  csl_edma3HwChannelSetup.c
 *
 *   @brief   
 *      The file implements the API's which are used to setup the properties
 *      of the channel.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2002, 2003, 2004, 2005, 2008, 2009, Texas Instruments, Inc.
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

#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>

/** ============================================================================
 *   @n@b CSL_edma3HwChannelSetupParam
 *
 *   @b Description
 *   @n The function maps a DMA/QDMA channel to the specific PARAM Entry.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            paramNum        Parameter Entry
      @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen() must be 
 *       called successfully in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  Channel is mapped to the PARAM Entry.
 *
 *   @b Writes
 *   @n TPCC_TPCC_DCHMAP_PAENTRY;
 *   @n TPCC_TPCC_QCHMAP_PAENTRY
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...
            // Map the channel to PARAM Entry 1
            CSL_edma3HwChannelSetupParam(hChannel, 1);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwChannelSetupParam, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwChannelSetupParam 
(
    CSL_Edma3ChannelHandle        hEdma,
    Uint16                        paramNum  
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (paramNum >= hEdma->cfgInfo.numParamsets) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; setup the Mapping; check if this is a DMA or QDMA Channel? */
        if (hEdma->chaNum  < hEdma->cfgInfo.numDMAChannel)
        {
            /* DMA Channel: Map the channel to PARAM Entry. */
            CSL_edma3MapDMAChannelToParamBlock ((CSL_Edma3Handle)hEdma, hEdma->chaNum, paramNum);
        } 
        else
        {
            /* QDMA Channel: Map the channel to PARAM Entry. */
            CSL_edma3MapQDMAChannelToParamBlock ((CSL_Edma3Handle)hEdma, hEdma->chaNum - hEdma->cfgInfo.numDMAChannel,
                                                 paramNum);
        }
    }
    return (status); 
}

/** ============================================================================
 *   @n@b CSL_edma3HwChannelSetupTriggerWord
 *
 *   @b Description
 *   @n The function is used to setup the trigger word for QDMA Channels.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            triggerWord     Trigger Word to be configured.                            
      @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen() must be 
 *       called successfully in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  Sets up the QDMA Channel to trigger Word
 *
 *   @b Writes
 *   @n TPCC_TPCC_QCHMAP_TRWORD
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Channel 16 Open in context of Shadow region 0 (This is a QDMA Channel)
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum = 16;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...        
            // Sets up the QDMA Channel 0 trigger Word to the 3rd trigger word.
            CSL_edma3HwChannelSetupTriggerWord(hChannel,3);
            ...     
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwChannelSetupTriggerWord, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwChannelSetupTriggerWord 
(
    CSL_Edma3ChannelHandle      hEdma,
    Uint8                       triggerWord   
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (triggerWord > 7) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Check if this is a DMA or QDMA Channel? */
        if (hEdma->chaNum < hEdma->cfgInfo.numDMAChannel)
        {
            /* Error: Only QDMA Channels can be configured for Trigger Word. */ 
            status = CSL_ESYS_INVPARAMS;
        } 
        else
        {
            /* QDMA Channel: Configure the Trigger Word. */
            CSL_edma3SetQDMATriggerWord ((CSL_Edma3Handle)hEdma, hEdma->chaNum - hEdma->cfgInfo.numDMAChannel, triggerWord);  
        }    
    }
    return (status); 
}

/** ============================================================================
 *   @n@b CSL_edma3HwChannelSetupQue
 *
 *   @b Description
 *   @n The function maps the DMA/QDMA channels to the specified event queue.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            evtQue          Queue Setup
      @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen() must be 
 *       called successfully in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  Sets up the channel to Queue mapping
 *
 *   @b Writes
 *   @n TPCC_TPCC_QDMAQNUM_E0;TPCC_TPCC_QDMAQNUM_E1;TPCC_TPCC_QDMAQNUM_E2;
 *      TPCC_TPCC_QDMAQNUM_E3;TPCC_TPCC_QDMAQNUM_E4;TPCC_TPCC_QDMAQNUM_E5;
 *      TPCC_TPCC_QDMAQNUM_E6;TPCC_TPCC_QDMAQNUM_E7;
 *   @n TPCC_TPCC_DMAQNUM_E0;TPCC_TPCC_DMAQNUM_E1;TPCC_TPCC_DMAQNUM_E2;
 *      TPCC_TPCC_DMAQNUM_E3;TPCC_TPCC_DMAQNUM_E4;TPCC_TPCC_DMAQNUM_E5;
 *      TPCC_TPCC_DMAQNUM_E6;TPCC_TPCC_DMAQNUM_E7
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Channel 1 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum = 16;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...
            // Map the channel 1 to Event Queue 3.
            CSL_edma3HwChannelSetupQue(hChannel,CSL_EDMA3_QUE_3);
            ...                
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwChannelSetupQue, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwChannelSetupQue 
(
    CSL_Edma3ChannelHandle       hEdma,
    CSL_Edma3Que                 evtQue 
) 
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else 
    {
        /* Arguments look good. Check is this a DMA or QDMA Channel? */
        if (hEdma->chaNum >= hEdma->cfgInfo.numDMAChannel)
        {
            /* QDMA Channel: Map the channel to Event Queue. */
            CSL_edma3MapQDMAChannelToEventQueue ((CSL_Edma3Handle)hEdma, hEdma->chaNum - hEdma->cfgInfo.numDMAChannel, 
                                                 evtQue);
        } 
        else 
        {
            /* DMA Channel: Map the channel to Event Queue. */
            CSL_edma3MapDMAChannelToEventQueue ((CSL_Edma3Handle)hEdma, hEdma->chaNum, evtQue);
        }
    }

    return (status);
}

