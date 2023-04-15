/**
 *   @file  csl_tmrGetHwStatus.c
 *
 *   @brief   
 *      The file implements the API which is used to get the status 
 *      of the TIMER Module.
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
#include <ti/csl/csl_tmrAux.h>

/** ===========================================================================
 *   @n@b CSL_tmrGetHwStatus
 *
 *   @b Description
 *   @n This function is used to get the value of various parameters of the
 *      timer instance. The value returned depends on the query passed.
 *
 *   @b Arguments
 *   @verbatim
            hTmr            Handle to the timer instance
            query           Query to be performed
            response        Pointer to buffer to return the data requested by
                            the query passed
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVQUERY  (The query passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Both @a CSL_tmrInit() and @a CSL_tmrOpen() must be called.
 *
 *   <b> Post Condition </b>
 *       Data requested by the query is returned through the variable "response"
 *
 *   @b Reads
 *   @n  TMR_CNTLO_CNT;TMR_CNTHI_CNT;TMR_TCR_TSTAT_LO;TMR_TCR_TSTAT_HI;TMR_WDTCR_WDFLAG
 *
 *   @b Example
 *   @verbatim
            CSL_status      status;
            CSL_TmrObj      tmrObj;
            CSL_TmrHandle   hTmr;
            CSL_TmrContext  context;
            Uint32          response;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
            // Open TIMER 0
            hTmr = CSL_tmrOpen(&tmrObj, 0, NULL, &status);
            ...
            // Get the value of LOW Count for Timer 0.
            CSL_tmrGetHwStatus(hTmr, CSL_TMR_QUERY_COUNT_LO, &response);
     @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION (CSL_tmrGetHwStatus, ".text:csl_section:tmr");
CSL_Status CSL_tmrGetHwStatus 
(
    CSL_TmrHandle        hTmr,
    CSL_TmrHwStatusQuery query,
    void*                response
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hTmr == NULL)  {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (response == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; process the query. */
        switch (query) 
        {
            case CSL_TMR_QUERY_COUNT_LO:
                CSL_tmrGetTimLoCount(hTmr, (Uint32 *)response);
                break;

            case CSL_TMR_QUERY_COUNT_HI:
                CSL_tmrGetTimHiCount(hTmr, (Uint32 *)response);
                break;

            case CSL_TMR_QUERY_TSTAT_LO:
                *((CSL_TmrTstat *)response) = CSL_tmrGetTstatLoStatus(hTmr);
                break;

            case CSL_TMR_QUERY_TSTAT_HI:
                *((CSL_TmrTstat *)response) = CSL_tmrGetTstatHiStatus(hTmr);
                break;

            case CSL_TMR_QUERY_WDFLAG_STATUS:
                *((CSL_TmrWdflagBitStatus*)response) = CSL_tmrGetWdflagStatus(hTmr);
                break;

            default:
                status = CSL_ESYS_INVQUERY;
                break;
        }
    }
    return status;
}

