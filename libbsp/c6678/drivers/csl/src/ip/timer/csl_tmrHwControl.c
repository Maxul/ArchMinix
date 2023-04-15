/**
 *   @file  csl_tmrHwControl.c
 *
 *   @brief   
 *      The file implements the API which is used to control the TIMER Module.
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

/** ============================================================================
 *   @n@b CSL_tmrHwControl
 *
 *   @b Description
 *   @n This function performs various control operations on the timer instance,
 *      based on the command passed.
 *
 *   @b Arguments
 *   @verbatim
            hTmr         Handle to the timer instance
            cmd          Operation to be performed on the timer
            cmdArg       Argument specific to the command
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *   @n Error   - CSL_ESYS_INVCMD    (The command passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Both @a CSL_tmrInit() and @a CSL_tmrOpen() must be called.
 *
 *   <b> Post Condition </b>
 *   @n  Registers of the timer instance are configured according to the command
 *       and the command arguments. The command determines which registers are
 *       modified.
 *
 *   @b Writes
 *   @n TMR_PRDLO_PRDLO;
 *      TMR_PRDHI_PRDHI;
 *      TMR_TGCR_PSCHI;
 *      TMR_TGCR_TIMLORS=1,TMR_TCR_ENAMODE_LO;
 *      TMR_TGCR_TIMHIRS=1,TMR_TCR_ENAMODE_HI;
 *      TMR_TCR_ENAMODE_LO=0;
 *      TMR_TCR_ENAMODE_HI=0;
 *      TMR_TGCR_TIMLORS=0;
 *      TMR_TGCR_TIMHIRS=0;
 *      TMR_TGCR_TIMLORS=1,TMR_TGCR_TIMHIRS=1,
 *      TMR_TCR_ENAMODE_LO,TMR_TCR_ENAMODE_HI;
 *      TMR_TCR_ENAMODE_LO=0,TMR_TCR_ENAMODE_HI=0;
 *      TMR_TGCR_TIMLORS=0,TMR_TGCR_TIMHIRS=0;
 *      TMR_TGCR_TIMLORS=1,TMR_TGCR_TIMHIRS=1,TMR_WDTCR_WDEN=1,TMR_TCR_ENAMODE_LO,TMR_TCR_ENAMODE_HI;
 *      TMR_WDTCR_WDKEY
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
            // Start the LO Timer.
            CSL_tmrHwControl(hTmr, CSL_TMR_CMD_START_TIMLO, NULL);
            ...
     @endverbatim
 * ============================================================================
 */
#pragma CODE_SECTION (CSL_tmrHwControl, ".text:csl_section:tmr");
CSL_Status  CSL_tmrHwControl 
(
    CSL_TmrHandle       hTmr,
    CSL_TmrHwControlCmd cmd,
    void*               arg
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */
    if (hTmr == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (\
         (cmd <= CSL_TMR_CMD_LOAD_WDKEY) && (cmd != CSL_TMR_CMD_STOP_TIMLO) &&\
         (cmd != CSL_TMR_CMD_STOP_TIMHI) && (cmd != CSL_TMR_CMD_RESET_TIMLO)&&\
         (cmd != CSL_TMR_CMD_RESET_TIMHI)&& (cmd != CSL_TMR_CMD_STOP64) &&\
         (cmd != CSL_TMR_CMD_RESET64)    && (arg == NULL)\
         ) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; process the command. */
        switch (cmd) 
        {
            /* Load PRDLO */
            case CSL_TMR_CMD_LOAD_PRDLO:
                CSL_TmrLoadPrdLo(hTmr, (Uint32 *)arg);
                break;

            /* Load PRDHI */
            case CSL_TMR_CMD_LOAD_PRDHI:
                CSL_TmrLoadPrdHi(hTmr, (Uint32 *)arg);
                break;

            /* Load PSCHI */
            case CSL_TMR_CMD_LOAD_PSCHI:
                CSL_TmrLoadPrescalarHi(hTmr, (Uint8 *)arg);
                break;

            /* start the timer Low */
            case CSL_TMR_CMD_START_TIMLO:
                CSL_TmrStartLo(hTmr, *((CSL_TmrEnamode *)arg));
                break;

            /* start the timer Low */
            case CSL_TMR_CMD_START_TIMHI:
                CSL_TmrStartHi(hTmr, *((CSL_TmrEnamode *)arg));
                break;

            /* Stop the Timer Low */
            case CSL_TMR_CMD_STOP_TIMLO:
                CSL_TmrStopLo(hTmr);
                break;

            /* Stop the TimerHi */
            case CSL_TMR_CMD_STOP_TIMHI:
                CSL_TmrStopHi(hTmr);
                break;

            /* Reset the Timer Low */
            case CSL_TMR_CMD_RESET_TIMLO:
                CSL_TmrResetLo(hTmr);
                break;

            /* Reset the TimerHi */
            case CSL_TMR_CMD_RESET_TIMHI:
                CSL_TmrResetHi(hTmr);
                break;

            /* Start the timer in GPtimer64 OR Chained mode */
            case CSL_TMR_CMD_START64:
                CSL_TmrStart64(hTmr, *((CSL_TmrEnamode *)arg));
                break;

            /*Stop the timer of GPtimer64 OR Chained */
            case CSL_TMR_CMD_STOP64:
                CSL_TmrStop64(hTmr);
                break;

            /*Reset the timer of GPtimer64 OR Chained */
            case CSL_TMR_CMD_RESET64:
                CSL_TmrReset64(hTmr);
                break;

            /*Starts the timer in watchdog mode */
            case CSL_TMR_CMD_START_WDT:
                CSL_TmrStartWdt(hTmr, *(CSL_TmrEnamode *)arg);
                break;

            /*loads the watchdog key */
            case CSL_TMR_CMD_LOAD_WDKEY:
                CSL_TmrLoadWdkey(hTmr, *((Uint16 *)arg));
                break;

            default:
                status = CSL_ESYS_INVCMD;
                break;
        }         
    }             
    return status;
}

