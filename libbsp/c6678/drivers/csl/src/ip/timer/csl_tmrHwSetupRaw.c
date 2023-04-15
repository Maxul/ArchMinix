/**
 *   @file  csl_tmrHwSetupRaw.c
 *
 *   @brief   
 *      The file implements the Timer Hadware Setup RAW API which is used to 
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
 *   @n@b CSL_tmrHwSetupRaw
 *
 *   @b Description
 *   @n This function initializes the device registers with the register-values
 *  	provided through the config data structure.
 *
 *   @b Arguments
 *   @verbatim
            hTmr            Pointer to the object that holds reference to the
                            instance of TIMER requested after the call
            config          Pointer to the config structure containing the
                            device register values
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
 *   @n  The registers of the specified General purpose timer instance will be setup
 *       according to the values passed through the config structure
 *
 *   @b Writes
 *   @n TMR_CNTLO_CNT,TMR_CNTHI_CNT,TMR_PRDLO_PRDLO,TMR_PRDHI_PRDHI,TMR_TCR_TCR,TMR_TGCR_TGCR,
 *   TMR_WDTCR_WDTCR
 *
 *   @b Example
 *   @verbatim
            CSL_status      status;
            CSL_TmrObj      tmrObj;
            CSL_TmrHandle   hTmr;
            CSL_TmrContext  context;
            CSL_TmrConfig   config = CSL_TMR_CONFIG_DEFAULTS;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
            // Open TIMER 0
            hTmr = CSL_tmrOpen(&tmrObj, 0, NULL, &status);
            ...
            // Configure the timer with the 'defaults'
            CSL_tmrHwSetupRaw (hTmr, &config);
            ...
     @endverbatim
 * ===========================================================================
 */
#pragma CODE_SECTION(CSL_tmrHwSetupRaw, ".text:csl_section:tmr");
CSL_Status  CSL_tmrHwSetupRaw 
(
    CSL_TmrHandle   hTmr,
    CSL_TmrConfig*  config
)
{
    if (hTmr == NULL)
        return CSL_ESYS_BADHANDLE;

    if (config == NULL)
        return CSL_ESYS_INVPARAMS;

    /* General purpose timer Timer Counter Register Low */
    hTmr->regs->CNTLO = config->CNTLO;

    /* General purpose timer Timer Counter Register High */
    hTmr->regs->CNTHI = config->CNTHI;

    /* General purpose timer Timer Period Register Low */
    hTmr->regs->PRDLO = config->PRDLO;

    /* General purpose timer Timer Period Register High */
    hTmr->regs->PRDHI = config->PRDHI;

    /* General purpose timer Timer Control Register */
    hTmr->regs->TCR = config->TCR;

    /* General purpose timer Timer Global Control Register */
    hTmr->regs->TGCR = config->TGCR;

    /* General purpose timer General purpose Timer Control Register */
    hTmr->regs->WDTCR = config->WDTCR;

    return CSL_SOK;
}

