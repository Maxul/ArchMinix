/**
 *   @file  csl_edma3GetHwChannelSetup.c
 *
 *   @brief   
 *      The file implements API's which are used to retreive setup information
 *      specific to an opened EDMA Channel.
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
 *   @n@b CSL_edma3GetHwChannelSetupParam
 *
 *   @b Description
 *   @n The function gets the DMA/QDMA Channel to PARAM Entry Mapping.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            paramNum        Pointer to parameter entry to which the channel 
                            is mapped.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameters passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen()
 *       must be called successfully in that order before 
 *       CSL_edma3GetHwChannelSetupParam() can be invoked. 
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n TPCC_TPCC_DCHMAP_PAENTRY;
 *   @n TPCC_TPCC_QCHMAP_PAENTRY
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
            Uint16                      paramNum;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, 0, NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...               
            // Get the parameter entry number to which a channel is mapped to 
            CSL_edma3GetHwChannelSetupParam(hChannel,&paramNum);            
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwChannelSetupParam, ".text:csl_section:edma3");
CSL_Status  CSL_edma3GetHwChannelSetupParam 
(
    CSL_Edma3ChannelHandle    hEdma,
    Uint16                    *paramNum
)
{
    CSL_Status    status = CSL_SOK;

    /* Validate the arguments. */
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;        
    }
    else if (paramNum == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good... Is this a DMA/QDMA Channel? */
        if (hEdma->chaNum  < hEdma->cfgInfo.numDMAChannel)
        {
            /* DMA Channel: Get the PARAM Mapping... */
            *paramNum = CSL_edma3GetDMAChannelToParamBlockMapping ((CSL_Edma3Handle)hEdma, hEdma->chaNum);
        }
        else
        {
            /* QDMA Channel: Get the PARAM Mapping... */
            *paramNum = CSL_edma3GetQDMAChannelToParamBlockMapping ((CSL_Edma3Handle)hEdma, 
                                                                    hEdma->chaNum - hEdma->cfgInfo.numDMAChannel);
        }
    }
    return (status); 
}

/** ============================================================================
 *   @n@b CSL_edma3GetHwChannelSetupTriggerWord
 *
 *   @b Description
 *   @n The function gets the Trigger word for a specific QDMA Channel.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle (Only QDMA Channels)
            triggerWord     Pointer to Trigger word populated by this API.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameters passed is invalid)
 *                                                
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen()
 *       must be called successfully in that order before 
 *       CSL_edma3GetHwChannelSetupTriggerWord() can be called.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n TPCC_TPCC_QCHMAP0_TRWORD
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
            Uint8                       triggerWord;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, 0, NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...
            // Get the trigger word programmed for the channel
            CSL_edma3GetHwChannelSetupTriggerWord(hChannel,&triggerWord);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwChannelSetupTriggerWord, ".text:csl_section:edma3");
CSL_Status  CSL_edma3GetHwChannelSetupTriggerWord 
(
    CSL_Edma3ChannelHandle  hEdma,
    Uint8*                  triggerWord  
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else 
    {
        /* Arguments look good; check is this a DMA or QDMA Channel? */
        if (hEdma->chaNum < hEdma->cfgInfo.numDMAChannel)
        {
            /* DMA Channel: This is not supported and API is valid for QDMA Channels only. */
            status = CSL_ESYS_INVPARAMS;
        } 
        else
        {
            /* QDMA Channel: Get the trigger word. */
            CSL_edma3GetQDMATriggerWord ((CSL_Edma3Handle)hEdma, 
                                         hEdma->chaNum-hEdma->cfgInfo.numDMAChannel, 
                                         triggerWord);
        }
    }
    return (status); 
}

/** ============================================================================
 *   @n@b CSL_edma3GetHwChannelSetupQue
 *
 *   @b Description
 *   @n The function gets the event queue to which the specific DMA/QDMA 
 *   channel is mapped.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle 
            evtQue          Event Queue to which the DMA/QDMA channel is mapped.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameters passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen()
 *       must be called successfully in that order before 
 *       CSL_edma3GetHwChannelSetupQue() can be called.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
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
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
            CSL_Edma3Que                evtQue;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, 0, NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...
            // Get the trigger word programmed for the channel
            CSL_edma3GetHwChannelSetupQue(hChannel, &evtQue);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwChannelSetupQue, ".text:csl_section:edma3");
CSL_Status  CSL_edma3GetHwChannelSetupQue 
(
    CSL_Edma3ChannelHandle  hEdma,
    CSL_Edma3Que*           evtQue   
) 
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */    
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }    
    else if (evtQue == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good. Is this a DMA or QDMA Channel? */
        if (hEdma->chaNum >= hEdma->cfgInfo.numDMAChannel) 
        {
            /* QDMA Channel: Get the Event Queue Mapping  */
            *evtQue = (CSL_Edma3Que)CSL_edma3GetQDMAChannelToEventQueueMapping ((CSL_Edma3Handle)hEdma, 
                                        hEdma->chaNum - hEdma->cfgInfo.numDMAChannel);
        }
        else 
        {
            /* DMA Channel: Get the Event Queue Mapping  */
            *evtQue = (CSL_Edma3Que)CSL_edma3GetDMAChannelToEventQueueMapping ((CSL_Edma3Handle)hEdma, 
                                        hEdma->chaNum);
        }
    }
    return (status);
}

