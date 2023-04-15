/**
 *   @file  csl_edma3ChannelClose.c
 *
 *   @brief   
 *      The file implements the API which is used to close a previously opened
 *      EDMA Channel.
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
 *   @n@b csl_edma3ChannelClose.c
 *
 *   @b Description
 *   @n This function marks the channel cannot be accessed anymore using 
 *      the handle. CSL for the EDMA channel need to be reopened before using 
 *      any edma channel.
 *      
 *   @b Arguments
 *   @verbatim
            hEdma           Handle to the channel to be closed.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open(), CSL_edma3ChannelOpen()
 *       must be invoked successfully in that order before this API can 
 *       be invoked.
 *
 *   <b> Post Condition </b>
 *   @n  The edma channel related CSL APIs can not be called until the edma 
 *       channel is reopened again using CSL_edma3ChannelOpen() 
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
            // Close Channel
            CSL_edma3ChannelClose(hChannel);
            ... 
    @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3ChannelClose, ".text:csl_section:edma3");
CSL_Status  CSL_edma3ChannelClose 
(
    CSL_Edma3ChannelHandle    hEdma
)
{
    CSL_Status    status = CSL_SOK;
        
    if(hEdma != NULL) {
        hEdma->regs    = (CSL_Edma3ccRegsOvly) NULL;
        hEdma->edmaNum = (CSL_InstNum) - 1;
        hEdma->region  = -2;    /* -2 specifies the invalid region number */
        hEdma->chaNum  = 0xFF;  /* -1 specifies the invalid channel */
    } 
    else 
    {
        status = CSL_ESYS_BADHANDLE;
    }
    return (status);
}


