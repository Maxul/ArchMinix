/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2002, 2003, 2004, 2005, 2008                 
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

/**
 *  @file  csl_intcExcep.c
 *
 *  @brief File for functional layer of CSL Exceptions APIs
 *
 *  PATH  $(CSLPATH)\src\intc
 */
 
/* =============================================================================
 *  Revision History
 *  ===============
 *  12-Jun-2004 Ruchika Kharwar File Created
 *
 *  16-Mar-2005 brn modified for doxygen documentation
 *  19-Nov-2008 Updated for Doxygen
 * =============================================================================
 */

#include <ti/csl/src/intc/csl_intc.h>

#pragma CODE_SECTION (CSL_intcExcepAllEnable, ".text:csl_section:intc");
CSL_Status CSL_intcExcepAllEnable (
        CSL_IntcExcepEn     excepMask,
        CSL_BitMask32       excVal,
        CSL_BitMask32       *prevState
        
)
{
    if (prevState) 
        *prevState = ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EXPMASK[excepMask];
        
    ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EXPMASK[excepMask] &= ~excVal;
    
    return CSL_SOK;
}


#pragma CODE_SECTION (CSL_intcExcepAllDisable, ".text:csl_section:intc");
CSL_Status CSL_intcExcepAllDisable (
        CSL_IntcExcepEn             excepMask,
        CSL_BitMask32               excVal,
        CSL_BitMask32               *prevState
)
{
    if (prevState)
        *prevState = ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EXPMASK[excepMask];
        
    ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EXPMASK[excepMask] |= excVal;
    
    return CSL_SOK;
}

#pragma CODE_SECTION (CSL_intcExcepAllRestore, ".text:csl_section:intc");
CSL_Status CSL_intcExcepAllRestore (
        CSL_IntcExcepEn             excepMask,
        CSL_IntcGlobalEnableState   restoreVal
)
{
    ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EXPMASK[excepMask] = restoreVal;
    
    return CSL_SOK;
}

#pragma CODE_SECTION (CSL_intcExcepAllClear, ".text:csl_section:intc");
CSL_Status CSL_intcExcepAllClear (
        CSL_IntcExcepEn  excepMask,
        CSL_BitMask32    excVal
)
{
    ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->EVTCLR[excepMask] = excVal;
    return CSL_SOK;
}

#pragma CODE_SECTION (CSL_intcExcepAllStatus, ".text:csl_section:intc");
CSL_Status CSL_intcExcepAllStatus (
        CSL_IntcExcepEn  excepMask,
        CSL_BitMask32    *status     
)
{
    if (status)
        *status = ((CSL_IntcRegsOvly)CSL_CGEM0_5_REG_BASE_ADDRESS_REGS)->MEXPFLAG[excepMask] ;
        
    return CSL_SOK; 
}
