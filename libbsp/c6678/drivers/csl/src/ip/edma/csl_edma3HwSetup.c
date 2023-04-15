/**
 *   @file  csl_edma3HwSetup.c
 *
 *   @brief   
 *      The file implements the API's which is used to configure all the DMA/QDMA
 *      channels in the EDMA module with the specified configuration.
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
 *   @n@b CSL_edma3HwSetup
 *
 *   @b Description
 *   @n This function does the setup for all DMA/QDMA channels by doing the 
 *      following operations:-
 *          - Mapping DMA/QDMA Channels to PARAM Blocks.
 *          - Mapping DMA/QDMA Channels to Event Queues.
 *          - Setting up the trigger word for QDMA channels.
 *
 *   @b Arguments
 *   @verbatim
            hMod            Edma module Handle
            setup           Pointer to the setup structure
      @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   @sa
 *      CSL_Edma3HwSetup
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() must be called successfully in that
 *       order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  EDMA registers are configured according to the hardware setup parameters
 *
 *   @b Writes
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
            CSL_Edma3HwSetup            hwSetup;
            CSL_Edma3HwDmaChannelSetup  dmahwSetup[64];
            CSL_Edma3HwQdmaChannelSetup qdmahwSetup[8];

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Module Setup
            hwSetup.dmaChaSetup  = &dmahwSetup[0];
            hwSetup.qdmaChaSetup = &qdmaSetup[0];
            CSL_edma3HwSetup(hModule,&hwSetup);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwSetup, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwSetup
(
    CSL_Edma3Handle        hMod,
    CSL_Edma3HwSetup       *setup
)
{
    Uint32        numCha;
    CSL_Status    status = CSL_SOK;
    
    if (hMod == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }    
    else if (setup == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Parameters looks good; check if the DMA Setup information has been provided or not? */
        if (setup->dmaChaSetup != NULL) 
        {
            /* DMA Setup Information has been provided; cycle through all the DMA Channels */             
            for (numCha = 0 ; numCha < hMod->cfgInfo.numDMAChannel; numCha++) 
            {
                /* Map DMA Channel to the specified PARAM Entry. */
                CSL_edma3MapDMAChannelToParamBlock(hMod, numCha, setup->dmaChaSetup[numCha].paramNum);

                /* Map QDMA Channel to the specified Event Queue */
                CSL_edma3MapDMAChannelToEventQueue (hMod, numCha, setup->dmaChaSetup[numCha].que);
            }
        }

        /* Check if QDMA Setup information has been provided? */
        if (setup->qdmaChaSetup != NULL) 
        {
            /* QDMA Setup Information has been provided; cycle through all the QDMA Channels */
            for (numCha = 0 ; numCha < hMod->cfgInfo.numQDMAChannel; numCha++) 
            {
                /* Map QDMA Channel to the specified PARAM Entry. */
                CSL_edma3MapQDMAChannelToParamBlock(hMod, numCha, setup->qdmaChaSetup[numCha].paramNum);

                /* Setup the Trigger word for the QDMA Channel. */
                CSL_edma3SetQDMATriggerWord(hMod, numCha, setup->qdmaChaSetup[numCha].triggerWord);

                /* Map QDMA Channel to the specified Event Queue */
                CSL_edma3MapQDMAChannelToEventQueue (hMod, numCha, setup->qdmaChaSetup[numCha].que);
            }
        }
    }
    return (status); 
}

