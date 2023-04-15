/**
 *   @file  csl_edma3Open.c
 *
 *   @brief   
 *      The file implements the API to open the EDMA Module
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
 *   @n@b CSL_edma3Open
 *
 *   @b Description
 *   @n This function returns the handle to the EDMA instance. This handle is 
 *      passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            pEdmaObj         EDMA Module Object pointer 
            edmaNum          Instance of EDMA                             
            pAttr            EDMA Attribute pointer 
            pStatus          Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n Success - Valid Edma handle will be returned if status value is equal to CSL_SOK.
 *   @n Failure - NULL
 *
 *   <b> Pre Condition </b>
 *   @n  The EDMA must be succesfully initialized via CSL_edma3Init() before 
 *       calling this function.
 *
 *   <b> Post Condition </b>
 *   @n  1. The status is returned in the status variable. If status
 *          returned is
 *   @li            CSL_SOK             Valid EDMA handle is returned
 *   @li            CSL_ESYS_FAIL       The EDMA instance is invalid
 *   @li            CSL_ESYS_INVPARAMS  The Parameter passed is invalid
 *
 *       2. Edma object structure is populated
 *
 *   @b Affects
 *   @n  None  
 * 
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle     hModule;
            CSL_Edma3Obj        edmaObj;
            CSL_Status          status;
            
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
    @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3Open, ".text:csl_section:edma3");
CSL_Edma3Handle  CSL_edma3Open 
(
    CSL_Edma3Obj*           pEdmaObj,
    CSL_InstNum             edmaNum,
    CSL_Edma3ModuleAttr*    pAttr,
    CSL_Status*             pStatus
)
{
    CSL_Edma3ModuleBaseAddress    baseAddress;
    CSL_Edma3CfgInfo              cfgInfo;
    CSL_Edma3Handle               hEdma = (CSL_Edma3Handle)NULL;

    /* Parameter Validation: Ensure that there is a placeholder to hold the status of the API */    
    if (pStatus == NULL) 
        return NULL;
    
    /* Parameter Validation: Ensure that there a valid parameter EDMA object is passed. */
    if (pEdmaObj == NULL)
    {
        *pStatus = CSL_ESYS_INVPARAMS;
         return NULL;
    }
     
    /* Get the EDMA Base Address. */    
    *pStatus = CSL_edma3ccGetModuleBaseAddr(edmaNum, pAttr, &baseAddress, &cfgInfo);
    if (*pStatus == CSL_SOK) 
    {
        /* Success: Initialize the parameters here. */
        pEdmaObj->regs    = baseAddress.regs;
        pEdmaObj->instNum = (CSL_InstNum)edmaNum;
        pEdmaObj->cfgInfo = cfgInfo; 
 
        /* Return the handle */
        hEdma = (CSL_Edma3Handle)pEdmaObj;
    } 
    else 
    {
        /* Error: Failed to get the base address for the Instance; report failure. */
        pEdmaObj->regs = (CSL_Edma3ccRegsOvly)NULL;
        pEdmaObj->instNum = (CSL_InstNum) -1;
    }
    return (hEdma);
}

