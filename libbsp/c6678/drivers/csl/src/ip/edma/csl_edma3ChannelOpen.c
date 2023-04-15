/**
 *   @file  csl_edma3ChannelOpen.c
 *
 *   @brief   
 *      The file implements the API which is used to open an EDMA channel. 
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

/** ============================================================================
 *   @n@b CSL_edma3ChannelOpen
 *
 *   @b Description
 *   @n The API returns a handle for the specified EDMA Channel for use. The 
 *      channel can be re-opened anytime after it has been normally closed if so
 *      required. The handle returned by this call is input as an essential 
 *      argument for many of the APIs described for this module.
 *
 *   @b Arguments
 *   @verbatim
            pEdmaObj         pointer to the object that holds reference to the
                             channel instance of the Specified DMA 
            edmaNum          Instance of EDMA whose channel is requested
            pChAttr          Channel Attributes which describe the channel to
                             be opened.
            status           Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - Channel Handle
 *   @n  Error   - NULL
 *
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() must be invoked successfully in that 
 *       order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *       EDMA channel object structure is populated
 *
 *   @b Affects
 *   @n  None  
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, 0, NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ... 
    @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3ChannelOpen, ".text:csl_section:edma3");
CSL_Edma3ChannelHandle  CSL_edma3ChannelOpen 
(
    CSL_Edma3ChannelObj*    pEdmaObj,
    CSL_InstNum             edmaNum,
    CSL_Edma3ChannelAttr*   pChAttr,
    CSL_Status*             pStatus
)
{
    CSL_Edma3ChannelHandle        hEdma = (CSL_Edma3ChannelHandle)NULL;
    CSL_Edma3ModuleBaseAddress    baseAddress;
    CSL_Edma3CfgInfo              cfgInfo;
    
    /* Validate the arguments. */
    if (pStatus == NULL) {
        /* Do nothing : Module handle already initialised to NULL */
    }
    else if (pEdmaObj == NULL) {
        *pStatus = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        if (pChAttr != NULL) 
        {
            /* Arguments look good; get the instance information... */
            *pStatus = CSL_edma3ccGetModuleBaseAddr(edmaNum, NULL, &baseAddress, &cfgInfo);
            if (*pStatus == CSL_SOK) 
            {
                /* Populate the instance information. */
                pEdmaObj->regs    = baseAddress.regs;
                pEdmaObj->edmaNum = edmaNum;
                pEdmaObj->chaNum  = pChAttr->chaNum;
                pEdmaObj->region  = pChAttr->regionNum;                        
                pEdmaObj->cfgInfo = cfgInfo; 
                
                hEdma = (CSL_Edma3ChannelHandle)pEdmaObj;
            }
            else 
            {
                /* Error: Instance Number is not supported. */
                pEdmaObj->regs = (CSL_Edma3ccRegsOvly)NULL;
                pEdmaObj->edmaNum = (CSL_InstNum) -1;
                pEdmaObj->chaNum = 0xFF;
                pEdmaObj->region = 0xFF;
            } 
        }
        else {
            *pStatus = CSL_ESYS_INVPARAMS;
        }        
    }

    return (hEdma);
}

