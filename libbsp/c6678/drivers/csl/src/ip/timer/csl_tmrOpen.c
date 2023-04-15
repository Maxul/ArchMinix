/**
 *   @file  csl_tmrOpen.c
 *
 *   @brief   
 *      The file implements the Timer Open API which opens and initializes the
 *      Timer handle used subsequently in all Timer API's.
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

#include <ti/csl/csl_tmr.h>

/** ============================================================================
 *   @n@b CSL_tmrOpen
 *
 *   @b Description
 *   @n This function populates the peripheral data object for the TIMER instance
 *      and returns a handle to the instance. The open call sets up the data 
 *      structures for the particular instance of TIMER device. The 
 *      device can be re-opened anytime after it has been normally closed 
 *      if so required. The handle returned by this call is input as an essential 
 *      argument for rest of the APIs described for this module.
 *
 *   @b Arguments
 *   @verbatim
            tmrObj          Pointer to GPTIMER object.
            tmrNum          Instance of GPTIMER CSL to be opened.
            pTmrParam       Module specific parameters.
            status          Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - Valid Timer Handle
 *   @n Error   - CSL_ESYS_FAIL (The Timer Instance is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  @a CSL_tmrInit() must be called.
 *
 *   <b> Post Condition </b>
 *   @n None
 *
 *   @b Writes
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_status      status;
            CSL_TmrObj      tmrObj;
            CSL_TmrHandle   hTmr;
            CSL_TmrContext  context;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
            // Open TIMER 0
            hTmr = CSL_tmrOpen(&tmrObj, 0, NULL, &status);
            ...
    @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION(CSL_tmrOpen, ".text:csl_section:tmr");
CSL_TmrHandle CSL_tmrOpen 
(
    CSL_TmrObj   *pTmrObj,
    CSL_InstNum  tmrNum,
    CSL_TmrParam *pTmrParam,
    CSL_Status   *pStatus
)
{
    CSL_Status          st;
    CSL_TmrHandle       hTmr = NULL;
    CSL_TmrBaseAddress  baseAddress;

    /* Validate the arguments... */
    if (pStatus == NULL) {
        /* do nothing */
    }
    else if (pTmrObj == NULL) {
        *pStatus = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; get the base address information for the device. */    
        st = CSL_tmrGetBaseAddress(tmrNum, pTmrParam, &baseAddress);
        if (st == CSL_SOK) 
        {
            pTmrObj->regs = baseAddress.regs;
            pTmrObj->tmrNum = (CSL_InstNum)tmrNum;
            hTmr = (CSL_TmrHandle)pTmrObj;
        }
        else 
        {
            pTmrObj->regs = (CSL_TmrRegsOvly)NULL;
            pTmrObj->tmrNum = (CSL_InstNum)-1;
            hTmr = (CSL_TmrHandle)NULL;
        }
        *pStatus = st;
    }
    return hTmr;
}

