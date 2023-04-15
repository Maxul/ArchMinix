/**
 *   @file  csl_edma3GetBaseAddress.c
 *
 *   @brief   
 *      The file contains routines which are EDMA3 Device specific
 *      and need to be modified for each new device which has the 
 *      EDMA3 IP module. 
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
#include <ti/csl/soc.h>
#include <ti/csl/csl_edma3.h>

/** ============================================================================
 *   @n@b CSL_edma3ccGetModuleBaseAddr
 *
 *   @b Description
 *   @n  This function is used for getting the base-address of the peripheral
 *       instance. This function will be called inside the @ CSL_edma3Open()/
 *       CSL_edma3ChannelOpen() function call. 
 *
 *       Note: This function is open for re-implementing if the user wants to 
 *       modify the base address of the peripheral object to point to a 
 *       different location and there by allow CSL initiated write/reads into 
 *       peripheral MMR's go to an alternate location.
 *
 *   @b Arguments
 *   @verbatim      
            edmaNum         Specifies the instance of the edma to be opened.
            pParam          Module specific parameters.
            pBaseAddress    Pointer to baseaddress structure containing base 
                            address details.
            pCfgInfo        Pointer to the EDMA CC Instance information populated
                            by this API.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_FAIL (Timer Instance is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Base Address structure is populated
 *
 *   @b Affects
 *   @n None.
 *
 *   @b Example
 *   @verbatim
        CSL_Status                   status;
        CSL_Edma3ModuleBaseAddress   baseAddress;
        CSL_Edma3CfgInfo             cfgInfo;

        // Get the EDMA CC Instance 0 Information 
        status = CSL_edma3ccGetModuleBaseAddr(0, NULL, &baseAddress, &cfgInfo);

    @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION (CSL_edma3ccGetModuleBaseAddr, ".text:csl_section:edma");
CSL_Status CSL_edma3ccGetModuleBaseAddr 
(
    CSL_InstNum                     edmaNum,
    CSL_Edma3ModuleAttr*            pParam,
    CSL_Edma3ModuleBaseAddress*     pBaseAddress,
    CSL_Edma3CfgInfo*               pCfgInfo
)
{
    CSL_Status status = CSL_SOK;

    if (pBaseAddress == NULL) {
        status = CSL_ESYS_INVPARAMS;
	}
    else {
        switch (edmaNum) {
            case CSL_TPCC_0:
            {
                /* Instance 0: Populate the Base Address... */
                pBaseAddress->regs = (CSL_Edma3ccRegsOvly)CSL_EDMA0CC_REGS;

                /* Populate the Configuration Information for this instance. */
                pCfgInfo->numDMAChannel      = CSL_EDMA3_TPCC0_NUM_DMACH;
                pCfgInfo->numQDMAChannel     = CSL_EDMA3_TPCC0_NUM_QDMACH;
                pCfgInfo->numINTChannel      = CSL_EDMA3_TPCC0_NUM_INTCH;
                pCfgInfo->numParamsets       = CSL_EDMA3_TPCC0_NUM_PARAMSETS;
                pCfgInfo->numEvque           = CSL_EDMA3_TPCC0_NUM_EVQUE;
                pCfgInfo->numRegions         = CSL_EDMA3_TPCC0_NUM_REGIONS;
                pCfgInfo->IsChannelMapping   = TRUE;
                pCfgInfo->IsMemoryProtection = TRUE;
                break;
            }
            case CSL_TPCC_1:
            {
                /* Instance 1: Populate the Base Address... */
                pBaseAddress->regs = (CSL_Edma3ccRegsOvly)CSL_EDMA1CC_REGS;

                /* Populate the Configuration Information for this instance. */
                pCfgInfo->numDMAChannel      = CSL_EDMA3_TPCC1_NUM_DMACH;
                pCfgInfo->numQDMAChannel     = CSL_EDMA3_TPCC1_NUM_QDMACH;
                pCfgInfo->numINTChannel      = CSL_EDMA3_TPCC1_NUM_INTCH;
                pCfgInfo->numParamsets       = CSL_EDMA3_TPCC1_NUM_PARAMSETS;
                pCfgInfo->numEvque           = CSL_EDMA3_TPCC1_NUM_EVQUE;
                pCfgInfo->numRegions         = CSL_EDMA3_TPCC1_NUM_REGIONS;
                pCfgInfo->IsChannelMapping   = TRUE;
                pCfgInfo->IsMemoryProtection = TRUE;
                break;
            }
            case CSL_TPCC_2:
            {
                /* Instance 2: Populate the Base Address... */
                pBaseAddress->regs = (CSL_Edma3ccRegsOvly)CSL_EDMA2CC_REGS;

                /* Populate the Configuration Information for this instance. */
                pCfgInfo->numDMAChannel      = CSL_EDMA3_TPCC2_NUM_DMACH;
                pCfgInfo->numQDMAChannel     = CSL_EDMA3_TPCC2_NUM_QDMACH;
                pCfgInfo->numINTChannel      = CSL_EDMA3_TPCC2_NUM_INTCH;
                pCfgInfo->numParamsets       = CSL_EDMA3_TPCC2_NUM_PARAMSETS;
                pCfgInfo->numEvque           = CSL_EDMA3_TPCC2_NUM_EVQUE;
                pCfgInfo->numRegions         = CSL_EDMA3_TPCC2_NUM_REGIONS;
                pCfgInfo->IsChannelMapping   = TRUE;
                pCfgInfo->IsMemoryProtection = TRUE;
                break;
            }
            default:
                pBaseAddress->regs = (CSL_Edma3ccRegsOvly)NULL;
                status = CSL_ESYS_FAIL;
        }
    }
    return (status);
}

