/**
 *   @file  csl_edma3GetHwSetup.c
 *
 *   @brief   
 *      The file implements the API which to get the setup information for all
 *      DMA and QDMA channels which are in the EDMA module
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
 *   @n@b CSL_edma3GetHwSetup
 *
 *   @b Description
 *   @n The function gets the hardware setup for all DMA/QDMA Channels.
 *
 *   @b Arguments
 *   @verbatim
            hMod            EDMA Handle
            setup           Setup structure which is populated by this API.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() must be called successfully
 *       in that order before CSL_edma3GetHwSetup() can be called. 
 *
 *   <b> Post Condition </b>
 *   @n  The hardware setup structure is populated with the hardware setup
 *       parameters
 *
 *   @b Reads
 *   @n TPCC_TPCC_DCHMAP_PAENTRY,
 *   @n TPCC_TPCC_DMAQNUM_E0;TPCC_TPCC_DMAQNUM_E1;TPCC_TPCC_DMAQNUM_E2;
 *      TPCC_TPCC_DMAQNUM_E3;TPCC_TPCC_DMAQNUM_E4;TPCC_TPCC_DMAQNUM_E5;
 *      TPCC_TPCC_DMAQNUM_E6;TPCC_TPCC_DMAQNUM_E7;
 *   @n TPCC_TPCC_QCHMAP_PAENTRY,
 *   @n TPCC_TPCC_QCHMAP_TRWORD,
 *   @n TPCC_TPCC_QDMAQNUM_E0;TPCC_TPCC_QDMAQNUM_E1;TPCC_TPCC_QDMAQNUM_E2;
 *      TPCC_TPCC_QDMAQNUM_E3;TPCC_TPCC_QDMAQNUM_E4;TPCC_TPCC_QDMAQNUM_E5;
 *      TPCC_TPCC_QDMAQNUM_E6;TPCC_TPCC_QDMAQNUM_E7
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3QueryInfo          info;
            CSL_Edma3HwSetup            setup;
            CSL_Edma3HwDmaChannelSetup  dmahwSetup;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Get DMA Channel Setup for all channels.
            setup.dmaChaSetup  = &dmahwSetup[0];
            setup.qdmaChaSetup = NULL;
            CSL_edma3GetHwSetup(hModule,&setup);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwSetup, ".text:csl_section:edma3");
CSL_Status  CSL_edma3GetHwSetup 
(
    CSL_Edma3Handle        hMod,
    CSL_Edma3HwSetup*      setup
)
{
    Uint8         numCha;
    CSL_Status    status = CSL_SOK;  

    /* Validate the arguments. */    
    if (hMod == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (setup == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good. Do we need to get the DMA Channel Information? */
        if (setup->dmaChaSetup != NULL) 
        {
            /* YES. Cycle through all the DMA Channels. */
            for (numCha = 0; numCha < hMod->cfgInfo.numDMAChannel; numCha++)
            {
                /* Get the DMA Channel to PARAM Block Mapping. */
                setup->dmaChaSetup[numCha].paramNum = CSL_edma3GetDMAChannelToParamBlockMapping (hMod, numCha);

                /* Get the DMA Channel to Event Queue Mapping. */
                setup->dmaChaSetup[numCha].que = (CSL_Edma3Que)CSL_edma3GetDMAChannelToEventQueueMapping (hMod, numCha);
            }
        }
        
        /* Do we need to get the QDMA Channel Information? */
        if (setup->qdmaChaSetup != NULL) 
        {
            /* Cycle through all the QDMA Channels. */
            for (numCha = 0 ; numCha < hMod->cfgInfo.numQDMAChannel; numCha++) 
            {
                /* Get the QDMA Channel to PARAM Block Mapping. */
                setup->qdmaChaSetup[numCha].paramNum =  CSL_edma3GetQDMAChannelToParamBlockMapping(hMod, numCha);

                /* Get the QDMA Channel Trigger Word. */
                CSL_edma3GetQDMATriggerWord(hMod, numCha, &setup->qdmaChaSetup[numCha].triggerWord);

                /* Get the QDMA Channel to Event Queue Mapping. */
                setup->qdmaChaSetup[numCha].que = (CSL_Edma3Que)CSL_edma3GetQDMAChannelToEventQueueMapping (hMod, numCha);
            }
        }
    }
    return (status); 
}

