/**
 *   @file  csl_tmrClose.c
 *
 *   @brief   
 *      The file implements the API which is used to close an instance of the Timer.
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
 *   @n@b CSL_tmrClose
 *
 *   @b Description
 *   @n This function marks that CSL for the GP timer instance is closed.
 *      CSL for the GP timer instance need to be reopened before using any
 *      GP timer CSL API.
 *
 *   @b Arguments
 *   @verbatim
            hTmr         Timer handle to be closed.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Both @a CSL_tmrInit() and @a CSL_tmrOpen() must be called.
 *
 *   <b> Post Condition </b>
 *   @n  1. The GP timer CSL APIs can not be called until the GP timer
 *          CSL is reopened again using CSL_tmrOpen()
 *
 *   @b Writes
 *   @n  None
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
            // Close the timer.
            CSL_tmrClose(hTmr);
    @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION (CSL_tmrClose, ".text:csl_section:tmr");
CSL_Status CSL_tmrClose (CSL_TmrHandle hTmr)
{
    CSL_Status st;

    if (hTmr != NULL) 
    {
        hTmr->regs = (CSL_TmrRegsOvly)NULL;
        hTmr->tmrNum = (CSL_InstNum)-1;

        st = CSL_SOK;
    }
    else 
    {
        st = CSL_ESYS_BADHANDLE;
    }

    return st;
}

