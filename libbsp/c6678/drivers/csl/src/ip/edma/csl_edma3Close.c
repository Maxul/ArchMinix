/**
 *   @file  csl_edma3Close.c
 *
 *   @brief   
 *      The file implements the API which is used to close the EDMA Module. 
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
 *   @n@b CSL_edma3Close.c
 *
 *   @b Description
 *   @n This is a module level close require to invalidate the module handle.
 *      The module handle must not be used after this API call.
 *      
 *   @b Arguments
 *   @verbatim
            hEdma           Handle to the Edma Instance
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init() and CSL_edma3Open() have to be called in that 
 *       order successfully before calling this function.
 *
 *   <b> Post Condition </b>
 *   @n  The edma CSL APIs can not be called until the edma CSL is reopened 
 *       again using CSL_edma3Open() 
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle     hModule;
            CSL_Edma3Obj        edmaObj;
            CSL_Status          status;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, CSL_EDMA3, NULL, &status);
            ...
            // Close the Module.         
            CSL_edma3Close(hModule);
            ...
    @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3Close, ".text:csl_section:edma3");
CSL_Status CSL_edma3Close 
(
    CSL_Edma3Handle    hEdma
)
{   
    CSL_Status status = CSL_SOK;
    
    if (hEdma != NULL) {
        hEdma->regs = (CSL_Edma3ccRegsOvly) NULL;
        hEdma->instNum = (CSL_InstNum)-1;
    } 
    else {
        status = CSL_ESYS_BADHANDLE;
    }
    
    return (status);
}

