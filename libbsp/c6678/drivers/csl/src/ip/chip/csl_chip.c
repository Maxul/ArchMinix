/* ============================================================================
 * Copyright (c) Texas Instruments Incorporated 2008, 2009
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
 * @file csl_chip.c
 *
 * @brief 
 *  Implementation for APIs to read / write into the CPU Control Registers.
 * 
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2008, 2009, Texas Instruments, Inc.
 *  @n   Use of this software is controlled by the terms and conditions found 
 *  @n   in the license agreement under which this software has been supplied.
 *  ===========================================================================
 *  \par  
 */

#include <ti/csl/csl_chip.h>

/** @addtogroup CSL_CHIP_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b CSL_chipReadReg
 *
 *   @b Description
 *   @n This API reads the specified control register. 
 *
 *   @b Arguments
 *   @verbatim      
        reg     Specifies the control register to be read
     @endverbatim
 *
 *
 *   <b> Return Value  </b>  Uint32
 *      @li     The control register value read
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n AMR;
 *      CSR;
 *      IFR;
 *      IER;
 *      ISTP;
 *      IRP;
 *      NRP;
 *      TSCL;
 *      TSCH;
 *      ARP;
 *      ILC;
 *      RILC;
 *      REP;
 *      PCE1;
 *      DNUM;
 *      SSR;
 *      GPLYA;
 *      GPLYB;
 *      GFPGFR;
 *      DIER;
 *      TSR;
 *      ITSR;
 *      NTSR;
 *      EFR;
 *      IERR
 *
 *  <b> Usage Constraints: </b>
 *  @n Please refer to the C64x+ user guide for constraints while accessing 
 *     registers in different privilege levels.
 *
 *   @b Example
 *   @verbatim
 
        Uint32 amr;
        amr = CSL_chipReadReg (AMR);

    @endverbatim
 * ===========================================================================
 */
Uint32  CSL_chipReadReg(
    CSL_ChipReg               reg
)
{
   switch (reg)
   {
        case CSL_CHIP_AMR:
        {
            /** Addressing Mode Register */
            return AMR;
        }
        case CSL_CHIP_CSR:
        {
            /** Control Status Register */                
            return CSR;
        }
        case CSL_CHIP_IFR:
        case CSL_CHIP_ICR:
        {
            /** Interrupt Flag/Set/Clear Registers */
            return IFR;                
        }
        case CSL_CHIP_IER:
        {
            /** Interrupt Enable Register */
            return IER;
        }
        case CSL_CHIP_ISTP:
        {
            /** Interrupt Service Table Pointer Register */
            return ISTP;                    
        }
        case CSL_CHIP_IRP:
        {
            /** Interrupt Return Pointer Register */                
            return IRP;                
        }
        case CSL_CHIP_NRP:
        {
            /** Nonmaskable Interrupt (NMI) / Exception Return Pointer Registers */                
            return NRP;                
        }
        case CSL_CHIP_TSCL:
        {
            /** Time Stamp Counter Register - Low */
            return TSCL;                
        }
        case CSL_CHIP_TSCH:
        {
            /** Time Stamp Counter Registers - High */
            return TSCH;                
        }
        case CSL_CHIP_ARP:
        {
            /** Analysis Return Pointer */
            return ARP;                
        }
        case CSL_CHIP_ILC:
        {
            /** SPLOOP Inner Loop Count Register */
            return ILC;                
        }
        case CSL_CHIP_RILC:
        {
            /** SPLOOP Reload Inner Loop Count Register */                
            return RILC;                
        }
        case CSL_CHIP_REP:
        {
            /** Restricted Entry Point Address Register */
            return REP;                
        }
        case CSL_CHIP_PCE1:
        {
            /** E1 Phase Program Counter */
            return PCE1;
        }
        case CSL_CHIP_DNUM:
        {
            /** DSP Core Number Register */
            return DNUM;                
        }
        case CSL_CHIP_SSR:
        {
            /** Saturation Status Register */
            return SSR;                
        }
        case CSL_CHIP_GPLYA:
        {
            /** GMPY Polynomial.A Side Register */
            return GPLYA;                
        }
        case CSL_CHIP_GPLYB:
        {
            /** GMPY Polynomial.B Side Register */
            return GPLYB;                
        }
        case CSL_CHIP_GFPGFR:
        {
            /** Galois Field Polynomial Generator Function Register */
            return GFPGFR;                
        }
        case CSL_CHIP_DIER:
        {
            /* Debug Interrupt Enable Register */                
            return DIER;
        }
        case CSL_CHIP_TSR:
        {
            /** Task State Register */
            return TSR;                
        }
        case CSL_CHIP_ITSR:
        {
            /** Interrupt Task State Register */
            return ITSR;                
        }
        case CSL_CHIP_NTSR:
        {
            /** NMI/Exception Task State Register */
            return NTSR;                
        }
        case CSL_CHIP_EFR:
        {
            /** Exception Flag and Clear Registers */
            return EFR;                
        }
        case CSL_CHIP_IERR:
        {
            /** Internal Exception Report Register */
            return IERR;
        }
        default:
        {
            break;
        }
   }

   /* We reach here only if the control register specified
    * is not supported by this CPU.
    */
   return 0;
}

/** ============================================================================
 *   @n@b CSL_chipWriteReg
 *
 *   @b Description
 *   @n This API writes specified control register with the specified 
 *      value 'val'. On successful writes, it returns the old value from
 *      the register. When the specified control register is a "Read-Only"
 *      register, this API just returns a 0.
 *
 *   @b Arguments
 *   @verbatim      
        reg     Specifies the control register to be written to
        val     Value to be written.
     @endverbatim
 *
 *
 *   <b> Return Value  </b> Uint32
 *      @li     New programmed value or Zero when Register is Read-Only
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  The reg control register is written with the value passed and
 *       the previous contents of the register are returned to the calling
 *       application. 
 *
 *   @b Writes
 *   @n AMR;
 *      CSR;
 *      ISR;
 *      ICR;
 *      IER;
 *      ISTP;
 *      IRP;
 *      NRP;
 *      TSCL;
 *      ARP;
 *      ILC;
 *      RILC;
 *      REP;
 *      SSR;
 *      GPLYA;
 *      GPLYB;
 *      GFPGFR;
 *      DIER;
 *      TSR;
 *      ITSR;
 *      NTSR;
 *      ECR;
 *      IERR
 *
 * <b> Usage Constraints: </b>
 *  @n Please refer to the C64x+ user guide for constraints while accessing 
 *     registers in different privilege levels.
 *
 *   @b Example
 *   @verbatim
 
        Uint32 oldamr;
        oldamr = CSL_chipWriteReg (AMR, 56);

    @endverbatim
 * ===========================================================================
 */
Uint32  CSL_chipWriteReg (
    CSL_ChipReg               reg,
    CSL_Reg32                 val
)
{
    Uint32      oldVal = 0;

    switch (reg)
   	{
		case CSL_CHIP_AMR:
        {
            /** Addressing Mode Register */
            oldVal  = AMR;
            AMR     = val;
            break;
        }
        case CSL_CHIP_CSR:
        {
            /** Control Status Register */                
            oldVal  = CSR;
            CSR     = val;
            break;
        }
        case CSL_CHIP_ISR:
        {
            /** Read Interrupt Flag Register and write to the Interrupt Set Register */
            oldVal  = IFR;
            ISR     = val;
            break;
        }
        case CSL_CHIP_ICR:
        {
            /** Read Interrupt Flag Register and write to the Interrupt Clear Register */
            oldVal  = IFR;
            ICR     = val;
            break;
        }
        case CSL_CHIP_IER:
        {
            /** Interrupt Enable Register */
            oldVal  = IER;
            IER     = val;
            break;
        }
        case CSL_CHIP_ISTP:
        {
            /** Interrupt Service Table Pointer Register */
            oldVal  = ISTP;
            ISTP    = val;
            break;
        }
        case CSL_CHIP_IRP:
        {
            /** Interrupt Return Pointer Register */                
            oldVal  = IRP;
            IRP     = val;
            break;
        }
        case CSL_CHIP_NRP:
        {
            /** Nonmaskable Interrupt (NMI) / Exception Return Pointer Registers */                
            oldVal  = NRP;
            NRP     = val;
            break;
        }
        case CSL_CHIP_TSCL:
        {
            /** Time Stamp Counter Register - Low */
            oldVal  = TSCL;
            TSCL    = val;
            break;
        }
        case CSL_CHIP_TSCH:
        {
            /** Time Stamp Counter Registers - High */
            break;
        }
        case CSL_CHIP_ARP:
        {
            /** Analysis Return Pointer */
            oldVal  = ARP;                
            ARP     = val;
            break;
        }        
        case CSL_CHIP_ILC:
        {
            /** SPLOOP Inner Loop Count Register */
            oldVal  = ILC;
            ILC     = val;
            break;
        }
        case CSL_CHIP_RILC:
        {
            /** SPLOOP Reload Inner Loop Count Register */                
            oldVal  = RILC;
            RILC    = val;
            break;
        }
        case CSL_CHIP_REP:
        {
            /** Restricted Entry Point Address Register */
            oldVal  = REP;
            REP     = val;
            break;
        }
        case CSL_CHIP_PCE1:
        {
            /** E1 Phase Program Counter */
            break;
        }
        case CSL_CHIP_DNUM:
        {
            /** DSP Core Number Register */
            break;
        }
        case CSL_CHIP_SSR:
        {
            /** Saturation Status Register */
            oldVal  = SSR;
            SSR     = val;
            break;
        }
        case CSL_CHIP_GPLYA:
        {
            /** GMPY Polynomial.A Side Register */
            oldVal  = GPLYA;
            GPLYA   = val;
            break;
        }
        case CSL_CHIP_GPLYB:
        {
            /** GMPY Polynomial.B Side Register */
            oldVal  = GPLYB;
            GPLYB   = val;
            break;
        }
        case CSL_CHIP_GFPGFR:
        {
            /** Galois Field Polynomial Generator Function Register */
            oldVal  = GFPGFR;           
			GFPGFR  = val;                   
            break;
        }
        case CSL_CHIP_DIER:
        {
            /** Debug Interrupt Enable Register */
            oldVal  = DIER;
            DIER    = val;
            break;
        }        
        case CSL_CHIP_TSR:
        {
            /** Task State Register */
            oldVal  = TSR;
            TSR     = val;
            break;
        }
        case CSL_CHIP_ITSR:
        {
            /** Interrupt Task State Register */
            oldVal  = ITSR;
            ITSR    = val;
            break;
        }
        case CSL_CHIP_NTSR:
        {
            /** NMI/Exception Task State Register */
            oldVal  = NTSR;
            NTSR    = val;
            break;
        }
        case CSL_CHIP_ECR:
        {
            /** Exception Flag and Clear Registers */
            oldVal  = EFR;
            ECR     = val;
            break;
        }
        case CSL_CHIP_IERR:
        {
            /** Internal Exception Report Register */
            oldVal  = IERR;
            IERR    = val;
            break;
        }
        default:
        {
            break;
        }
	}

    /* Return the old value from the register */
   	return oldVal;        
}

/**
@}
*/

