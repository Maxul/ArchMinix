/**
 *   @file  csl_tmrHwSetup.c
 *
 *   @brief   
 *      The file implements the Timer Hardware Setup API which is used to 
 *      configure all the timer registers
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
 *   @n@b CSL_tmrHwSetup
 *
 *   @b Description
 *   @n It configures the  timer instance registers as per the values passed
 *      in the hardware setup structure.
 *
 *   @b Arguments
 *   @verbatim
            htmr        Handle to the timer instance
            setup       Pointer to hardware setup structure populated by 
                        this API
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Both @a CSL_tmrInit() and @a CSL_tmrOpen() must be called.
 *
 *   <b> Post Condition </b>
 *   @n  The specified instance will be setup according to value passed
 *
 *   @b Writes
 *   @n TMR_PRDLO_PRDLO,TMR_PRDHI_PRDHI,TMR_CNTLO_CNT,TMR_CNTHI_CNT,
 *	TMR_TCR_TIEN_HI,TMR_TCR_CLKSRC_HI,TMR_TCR_PWID_HI,TMR_TCR_CP_HI,TMR_TCR_INVINP_HI, 
 *	TMR_TCR_INVOUTP_HI,TMR_TCR_TIEN_LO,TMR_TCR_CLKSRC_LO,TMR_TCR_PWID_LO,TMR_TCR_CP_LO,
 *	TMR_TCR_INVINP_LO, TMR_TCR_INVOUTP_LO,TMR_TGCR_PSCHI,TMR_TGCR_TIMMODE,
 *
 *   @b Example
 *   @verbatim
            CSL_status      status;
            CSL_TmrObj      tmrObj;
            CSL_TmrHandle   hTmr;
            CSL_TmrContext  context;
            CSL_tmrHwSetup  hwSetup;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
            // Open TIMER 0
            hTmr = CSL_tmrOpen(&tmrObj, 0, NULL, &status);
            ...
            // Get the Hardware Setup for Timer 0.        
            CSL_tmrHwSetup(htmr, &hwSetup);
            ...
     @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION(CSL_tmrHwSetup, ".text:csl_section:tmr");
CSL_Status  CSL_tmrHwSetup 
(
    CSL_TmrHandle   hTmr,
    CSL_TmrHwSetup* hwSetup
)
{
    Uint32 tmpReg;

    /* Validate the arguments. */
    if (hTmr == NULL)
        return CSL_ESYS_BADHANDLE;

    if (hwSetup == NULL)
        return CSL_ESYS_INVPARAMS;

	/* Load the Period register's */
	hTmr->regs->PRDLO = hwSetup->tmrTimerPeriodLo;
	hTmr->regs->PRDHI = hwSetup->tmrTimerPeriodHi;

	/* Load the counter register's */
	hTmr->regs->CNTLO = hwSetup->tmrTimerCounterLo;
	hTmr->regs->CNTHI = hwSetup->tmrTimerCounterHi;

	tmpReg = hTmr->regs->TCR;

    /* Setup the TCR register  configurations */
	CSL_FINS(tmpReg, TMR_TCR_TIEN_HI, hwSetup->tmrIpGateHi);
	CSL_FINS(tmpReg, TMR_TCR_CLKSRC_HI, hwSetup->tmrClksrcHi);
	CSL_FINS(tmpReg, TMR_TCR_PWID_HI, hwSetup->tmrPulseWidthHi);
	CSL_FINS(tmpReg, TMR_TCR_CP_HI, hwSetup->tmrClockPulseHi);
	CSL_FINS(tmpReg, TMR_TCR_INVINP_HI, hwSetup->tmrInvInpHi);
	CSL_FINS(tmpReg, TMR_TCR_INVOUTP_HI, hwSetup->tmrInvOutpHi);

	/* Setup the TCR register  configurations */
	CSL_FINS(tmpReg, TMR_TCR_TIEN_LO, hwSetup->tmrIpGateLo);
	CSL_FINS(tmpReg, TMR_TCR_CLKSRC_LO, hwSetup->tmrClksrcLo);
	CSL_FINS(tmpReg, TMR_TCR_PWID_LO, hwSetup->tmrPulseWidthLo);
	CSL_FINS(tmpReg, TMR_TCR_CP_LO, hwSetup->tmrClockPulseLo);
	CSL_FINS(tmpReg, TMR_TCR_INVINP_LO, hwSetup->tmrInvInpLo);
	CSL_FINS(tmpReg, TMR_TCR_INVOUTP_LO, hwSetup->tmrInvOutpLo);

    hTmr->regs->TCR = tmpReg;
	
    /* Setup the TGCR register  configurations and the operation mode */
	tmpReg = hTmr->regs->TGCR;

    CSL_FINS(tmpReg, TMR_TGCR_PSCHI, hwSetup->tmrPreScalarCounterHi);
	/* set the operation mode */
    CSL_FINS(tmpReg, TMR_TGCR_TIMMODE, hwSetup->tmrTimerMode);

    hTmr->regs->TGCR = tmpReg;

    return CSL_SOK;
}

