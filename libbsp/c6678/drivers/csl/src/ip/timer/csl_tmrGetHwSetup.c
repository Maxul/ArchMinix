/**
 *   @file  csl_tmrGetHwSetup.c
 *
 *   @brief   
 *      The file implements the API which is used to get a snapshot of all 
 *      the registers in the TIMER Module  
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
 *   @n@b CSL_tmrGetHwSetup
 *
 *   @b Description
 *   @n It retrives the hardware setup parameters
 *
 *   @b Arguments
 *   @verbatim
            hTmr            Handle to the timer instance
            hwSetup         Pointer to hardware setup structure populated by this API
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
 *   @n  The hardware set up structure will be populated with values from
 *       the registers
 *
 *   @b Reads
 *   @n TMR_PRDLO_PRDLO,TMR_PRDHI_PRDHI,TMR_CNTLO_CNT,TMR_CNT_CNTHI,TMR_TCR_TIEN_HI,TMR_TCR_CLKSRC_HI,
 *   TMR_TCR_PWID_HI,TMR_TCR_CP_HI,TMR_TCR_INVINP_HI,TMR_TCR_INVOUTP_HI,TMR_TCR_TIEN_LO,TMR_TCR_CLKSRC_LO,
 *   TMR_TCR_PWID_LO,TMR_TCR_CP_LO,TMR_TCR_INVINP_LO,TMR_TCR_INVOUTP_LO,TMR_TGCR_PSCHI,TMR_TGCR_TIMMODE,
 *
 *   @b Example
 *   @verbatim
            CSL_status      status;
            CSL_TmrObj      tmrObj;
            CSL_TmrHandle   hTmr;
            CSL_TmrContext  context;
            CSL_TmrConfig   config;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
            // Open TIMER 0
            hTmr = CSL_tmrOpen(&tmrObj, 0, NULL, &status);
            ...
            // Get the current timer configuration the timer with the 'defaults'
            CSL_tmrGetHwsetup (hTmr, &config);
            ...
     @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION (CSL_tmrGetHwSetup, ".text:csl_section:tmr");
CSL_Status CSL_tmrGetHwSetup 
(
    CSL_TmrHandle   hTmr,
    CSL_TmrHwSetup* hwSetup
)
{
    Uint32 tmpReg;

    if (hTmr == NULL)
        return CSL_ESYS_BADHANDLE;

    /* Get the Period register's values */
    hwSetup->tmrTimerPeriodLo = hTmr->regs->PRDLO;
    hwSetup->tmrTimerPeriodHi = hTmr->regs->PRDHI;

    /* Get the counter register's  values*/
    hwSetup->tmrTimerCounterLo = hTmr->regs->CNTLO;
    hwSetup->tmrTimerCounterHi = hTmr->regs->CNTHI;

    tmpReg = hTmr->regs->TCR;
    
    /* Get the TCR register  configurations */
    hwSetup->tmrIpGateHi = 
        (CSL_TmrIpGate)CSL_FEXT(tmpReg, TMR_TCR_TIEN_HI);

	hwSetup->tmrClksrcHi =
        (CSL_TmrClksrc)CSL_FEXT(tmpReg, TMR_TCR_CLKSRC_HI);

	hwSetup->tmrPulseWidthHi =
			(CSL_TmrPulseWidth)CSL_FEXT(tmpReg, TMR_TCR_PWID_HI);

	hwSetup->tmrClockPulseHi =
			(CSL_TmrClockPulse)CSL_FEXT(tmpReg, TMR_TCR_CP_HI);

	hwSetup->tmrInvInpHi =
			(CSL_TmrInvInp)CSL_FEXT(tmpReg, TMR_TCR_INVINP_HI);

	hwSetup->tmrInvOutpHi =
			(CSL_TmrInvOutp)CSL_FEXT(tmpReg, TMR_TCR_INVOUTP_HI);

	/* Get the TCR register  configurations */
	hwSetup->tmrIpGateLo =
			(CSL_TmrIpGate)CSL_FEXT(tmpReg, TMR_TCR_TIEN_LO);

	hwSetup->tmrClksrcLo =
			(CSL_TmrClksrc)CSL_FEXT(tmpReg, TMR_TCR_CLKSRC_LO);

	hwSetup->tmrPulseWidthLo =
			(CSL_TmrPulseWidth)CSL_FEXT(tmpReg, TMR_TCR_PWID_LO);

	hwSetup->tmrClockPulseLo =
			(CSL_TmrClockPulse)CSL_FEXT(tmpReg, TMR_TCR_CP_LO);

	hwSetup->tmrInvInpLo =
			(CSL_TmrInvInp)CSL_FEXT(tmpReg, TMR_TCR_INVINP_LO);

	hwSetup->tmrInvOutpLo =
			(CSL_TmrInvOutp)CSL_FEXT(tmpReg, TMR_TCR_INVOUTP_LO);

    tmpReg = hTmr->regs->TGCR;
	
    /* Get the TGCR register  configurations */
	hwSetup->tmrPreScalarCounterHi = (Uint8)CSL_FEXT(tmpReg, TMR_TGCR_PSCHI);

	/* Get the operation mode */
	hwSetup->tmrTimerMode = (CSL_TmrMode)CSL_FEXT(tmpReg, TMR_TGCR_TIMMODE);

    return CSL_SOK;
}

