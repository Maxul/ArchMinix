/**
 *   @file  csl_edma3HwControl.c
 *
 *   @brief   
 *      The file implements the API's which is used to configure the various
 *      entities in the EDMA Module.
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

#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h> 

/** ============================================================================
 *   @n@b CSL_edma3HwControl
 *
 *   @b Description
 *   @n This API takes a command with an optional argument and performs the
 *      appropriate action. This can be used to perform various operations 
 *      on the EDMA module.  
 *
 *   @b Arguments
 *   @verbatim
            hMod        EDMA Module Handle 
            cmd         Command which needs to be executed.
            cmdArg      Pointer to the argument specific to the command
      @endverbatim      
 *
 *   @sa
 *      CSL_Edma3HwControlCmd 
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVCMD    (The command passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() must be called successfully 
 *       in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  EDMA registers are configured according to the command and
 *       the command arguments. The command determines which registers are
 *       modified.
 *
 *   @b Writes
 *   @n TPCC_TPCC_MPPAG;TPCC_TPCC_MPPA;
 *   @n TPCC_TPCC_MPFCR_MPFCLR=1;
 *   @n TPCC_TPCC_DRA_DRAE_E0=1;TPCC_TPCC_DRA_DRAE_E1=1;TPCC_TPCC_DRA_DRAE_E2=1;
 *      TPCC_TPCC_DRA_DRAE_E3=1;TPCC_TPCC_DRA_DRAE_E4=1;TPCC_TPCC_DRA_DRAE_E5=1;
 *      TPCC_TPCC_DRA_DRAE_E6=1;TPCC_TPCC_DRA_DRAE_E7=1;TPCC_TPCC_DRA_DRAE_E8=1;
 *      TPCC_TPCC_DRA_DRAE_E9=1;TPCC_TPCC_DRA_DRAE_E10=1;TPCC_TPCC_DRA_DRAE_E11=1;
 *      TPCC_TPCC_DRA_DRAE_E12=1;TPCC_TPCC_DRA_DRAE_E13=1;TPCC_TPCC_DRA_DRAE_E14=1;
 *      TPCC_TPCC_DRA_DRAE_E15=1;TPCC_TPCC_DRA_DRAE_E16=1;TPCC_TPCC_DRA_DRAE_E17=1;
 *      TPCC_TPCC_DRA_DRAE_E18=1;TPCC_TPCC_DRA_DRAE_E19=1;TPCC_TPCC_DRA_DRAE_E20=1;
 *      TPCC_TPCC_DRA_DRAE_E21=1;TPCC_TPCC_DRA_DRAE_E22=1;TPCC_TPCC_DRA_DRAE_E23=1;
 *      TPCC_TPCC_DRA_DRAE_E24=1;TPCC_TPCC_DRA_DRAE_E25=1;TPCC_TPCC_DRA_DRAE_E26=1;
 *      TPCC_TPCC_DRA_DRAE_E27=1;TPCC_TPCC_DRA_DRAE_E28=1;TPCC_TPCC_DRA_DRAE_E29=1;
 *      TPCC_TPCC_DRA_DRAE_E30=1;TPCC_TPCC_DRA_DRAE_E31=1;
 *   @n TPCC_TPCC_DRA_DRAEH_E32=1;TPCC_TPCC_DRA_DRAEH_E33=1;TPCC_TPCC_DRA_DRAEH_E34=1;
 *      TPCC_TPCC_DRA_DRAEH_E35=1;TPCC_TPCC_DRA_DRAEH_E36=1;TPCC_TPCC_DRA_DRAEH_E37=1;
 *      TPCC_TPCC_DRA_DRAEH_E38=1;TPCC_TPCC_DRA_DRAEH_E39=1;TPCC_TPCC_DRA_DRAEH_E40=1;
 *      TPCC_TPCC_DRA_DRAEH_E41=1;TPCC_TPCC_DRA_DRAEH_E42=1;TPCC_TPCC_DRA_DRAEH_E43=1;
 *      TPCC_TPCC_DRA_DRAEH_E44=1;TPCC_TPCC_DRA_DRAEH_E45=1;TPCC_TPCC_DRA_DRAEH_E46=1;
 *      TPCC_TPCC_DRA_DRAEH_E47=1;TPCC_TPCC_DRA_DRAEH_E48=1;TPCC_TPCC_DRA_DRAEH_E49=1;
 *      TPCC_TPCC_DRA_DRAEH_E50=1;TPCC_TPCC_DRA_DRAEH_E51=1;TPCC_TPCC_DRA_DRAEH_E52=1;
 *      TPCC_TPCC_DRA_DRAEH_E53=1;TPCC_TPCC_DRA_DRAEH_E54=1;TPCC_TPCC_DRA_DRAEH_E55=1;
 *      TPCC_TPCC_DRA_DRAEH_E56=1;TPCC_TPCC_DRA_DRAEH_E57=1;TPCC_TPCC_DRA_DRAEH_E58=1;
 *      TPCC_TPCC_DRA_DRAEH_E59=1;TPCC_TPCC_DRA_DRAEH_E60=1;TPCC_TPCC_DRA_DRAEH_E61=1;
 *      TPCC_TPCC_DRA_DRAEH_E62=1;TPCC_TPCC_DRA_DRAEH_E63=1;
 *   @n TPCC_TPCC_DRA_DRAE_E0=0;TPCC_TPCC_DRA_DRAE_E1=0;TPCC_TPCC_DRA_DRAE_E2=0;
 *      TPCC_TPCC_DRA_DRAE_E3=0;TPCC_TPCC_DRA_DRAE_E4=0;TPCC_TPCC_DRA_DRAE_E5=0;
 *      TPCC_TPCC_DRA_DRAE_E6=0;TPCC_TPCC_DRA_DRAE_E7=0;TPCC_TPCC_DRA_DRAE_E8=0;
 *      TPCC_TPCC_DRA_DRAE_E9=0;TPCC_TPCC_DRA_DRAE_E10=0;TPCC_TPCC_DRA_DRAE_E11=0;
 *      TPCC_TPCC_DRA_DRAE_E12=0;TPCC_TPCC_DRA_DRAE_E13=0;TPCC_TPCC_DRA_DRAE_E14=0;
 *      TPCC_TPCC_DRA_DRAE_E15=0;TPCC_TPCC_DRA_DRAE_E16=0;TPCC_TPCC_DRA_DRAE_E17=0;
 *      TPCC_TPCC_DRA_DRAE_E18=0;TPCC_TPCC_DRA_DRAE_E19=0;TPCC_TPCC_DRA_DRAE_E20=0;
 *      TPCC_TPCC_DRA_DRAE_E21=0;TPCC_TPCC_DRA_DRAE_E22=0;TPCC_TPCC_DRA_DRAE_E23=0;
 *      TPCC_TPCC_DRA_DRAE_E24=0;TPCC_TPCC_DRA_DRAE_E25=0;TPCC_TPCC_DRA_DRAE_E26=0;
 *      TPCC_TPCC_DRA_DRAE_E27=0;TPCC_TPCC_DRA_DRAE_E28=0;TPCC_TPCC_DRA_DRAE_E29=0;
 *      TPCC_TPCC_DRA_DRAE_E30=0;TPCC_TPCC_DRA_DRAE_E31=0;
 *   @n TPCC_TPCC_DRA_DRAEH_E32=0;TPCC_TPCC_DRA_DRAEH_E33=0;TPCC_TPCC_DRA_DRAEH_E34=0;
 *      TPCC_TPCC_DRA_DRAEH_E35=0;TPCC_TPCC_DRA_DRAEH_E36=0;TPCC_TPCC_DRA_DRAEH_E37=0;
 *      TPCC_TPCC_DRA_DRAEH_E38=0;TPCC_TPCC_DRA_DRAEH_E39=0;TPCC_TPCC_DRA_DRAEH_E40=0;
 *      TPCC_TPCC_DRA_DRAEH_E41=0;TPCC_TPCC_DRA_DRAEH_E42=0;TPCC_TPCC_DRA_DRAEH_E43=0;
 *      TPCC_TPCC_DRA_DRAEH_E44=0;TPCC_TPCC_DRA_DRAEH_E45=0;TPCC_TPCC_DRA_DRAEH_E46=0;
 *      TPCC_TPCC_DRA_DRAEH_E47=0;TPCC_TPCC_DRA_DRAEH_E48=0;TPCC_TPCC_DRA_DRAEH_E49=0;
 *      TPCC_TPCC_DRA_DRAEH_E50=0;TPCC_TPCC_DRA_DRAEH_E51=0;TPCC_TPCC_DRA_DRAEH_E52=0;
 *      TPCC_TPCC_DRA_DRAEH_E53=0;TPCC_TPCC_DRA_DRAEH_E54=0;TPCC_TPCC_DRA_DRAEH_E55=0;
 *      TPCC_TPCC_DRA_DRAEH_E56=0;TPCC_TPCC_DRA_DRAEH_E57=0;TPCC_TPCC_DRA_DRAEH_E58=0;
 *      TPCC_TPCC_DRA_DRAEH_E59=0;TPCC_TPCC_DRA_DRAEH_E60=0;TPCC_TPCC_DRA_DRAEH_E61=0;
 *      TPCC_TPCC_DRA_DRAEH_E62=0;TPCC_TPCC_DRA_DRAEH_E63=0;
 *   @n TPCC_TPCC_QRAE_E0=1;TPCC_TPCC_QRAE_E1=1;TPCC_TPCC_QRAE_E2=1;
 *      TPCC_TPCC_QRAE_E3=1;TPCC_TPCC_QRAE_E4=1;TPCC_TPCC_QRAE_E5=1;
 *      TPCC_TPCC_QRAE_E6=1;TPCC_TPCC_QRAE_E7=1;
 *   @n TPCC_TPCC_QRAE_E0=0;TPCC_TPCC_QRAE_E1=0;TPCC_TPCC_QRAE_E2=0;
 *      TPCC_TPCC_QRAE_E3=0;TPCC_TPCC_QRAE_E4=0;TPCC_TPCC_QRAE_E5=0;
 *      TPCC_TPCC_QRAE_E6=0;TPCC_TPCC_QRAE_E7=0;
 *   @n TPCC_TPCC_QUEPRI_PRIQ0;TPCC_TPCC_QUEPRI_PRIQ1;TPCC_TPCC_QUEPRI_PRIQ2;
 *      TPCC_TPCC_QUEPRI_PRIQ3;TPCC_TPCC_QUEPRI_PRIQ4;TPCC_TPCC_QUEPRI_PRIQ5;
 *      TPCC_TPCC_QUEPRI_PRIQ6;TPCC_TPCC_QUEPRI_PRIQ7;
 *   @n TPCC_TPCC_QWMTHRA_Q0;TPCC_TPCC_QWMTHRA_Q1;TPCC_TPCC_QWMTHRA_Q2;
 *      TPCC_TPCC_QWMTHRA_Q3;
 *   @n TPCC_TPCC_QWMTHRB_Q4;TPCC_TPCC_QWMTHRB_Q1;TPCC_TPCC_QWMTHRB_Q2;
 *      TPCC_TPCC_QWMTHRB_Q3;
 *   @n TPCC_TPCC_EEVAL_EVAL=1;
 *   @n TPCC_TPCC_ICR,TPCC_TPCC_ICRH;
 *   @n TPCC_TPCC_IESR_IESR0=1;TPCC_TPCC_IESR_IESR1=1;TPCC_TPCC_IESR_IESR2=1;
 *      TPCC_TPCC_IESR_IESR3=1;TPCC_TPCC_IESR_IESR4=1;TPCC_TPCC_IESR_IESR5=1;
 *      TPCC_TPCC_IESR_IESR6=1;TPCC_TPCC_IESR_IESR7=1;TPCC_TPCC_IESR_IESR8=1;
 *      TPCC_TPCC_IESR_IESR9=1;TPCC_TPCC_IESR_IESR10=1;TPCC_TPCC_IESR_IESR11=1;
 *      TPCC_TPCC_IESR_IESR12=1;TPCC_TPCC_IESR_IESR13=1;TPCC_TPCC_IESR_IESR14=1;
 *      TPCC_TPCC_IESR_IESR15=1;TPCC_TPCC_IESR_IESR16=1;TPCC_TPCC_IESR_IESR17=1;
 *      TPCC_TPCC_IESR_IESR18=1;TPCC_TPCC_IESR_IESR19=1;TPCC_TPCC_IESR_IESR20=1;
 *      TPCC_TPCC_IESR_IESR21=1;TPCC_TPCC_IESR_IESR22=1;TPCC_TPCC_IESR_IESR23=1;
 *      TPCC_TPCC_IESR_IESR24=1;TPCC_TPCC_IESR_IESR25=1;TPCC_TPCC_IESR_IESR26=1;
 *      TPCC_TPCC_IESR_IESR27=1;TPCC_TPCC_IESR_IESR28=1;TPCC_TPCC_IESR_IESR29=1;
 *      TPCC_TPCC_IESR_IESR30=1;TPCC_TPCC_IESR_IESR31=1;
 *   @n TPCC_TPCC_IESRH_IESR32=1;TPCC_TPCC_IESRH_IESR33=1;TPCC_TPCC_IESRH_IESR34=1;
 *      TPCC_TPCC_IESRH_IESR35=1;TPCC_TPCC_IESRH_IESR36=1;TPCC_TPCC_IESRH_IESR37=1;
 *      TPCC_TPCC_IESRH_IESR38=1;TPCC_TPCC_IESRH_IESR39=1;TPCC_TPCC_IESRH_IESR40=1;
 *      TPCC_TPCC_IESRH_IESR41=1;TPCC_TPCC_IESRH_IESR42=1;TPCC_TPCC_IESRH_IESR43=1;
 *      TPCC_TPCC_IESRH_IESR44=1;TPCC_TPCC_IESRH_IESR45=1;TPCC_TPCC_IESRH_IESR46=1;
 *      TPCC_TPCC_IESRH_IESR47=1;TPCC_TPCC_IESRH_IESR48=1;TPCC_TPCC_IESRH_IESR49=1;
 *      TPCC_TPCC_IESRH_IESR50=1;TPCC_TPCC_IESRH_IESR51=1;TPCC_TPCC_IESRH_IESR52=1;
 *      TPCC_TPCC_IESRH_IESR53=1;TPCC_TPCC_IESRH_IESR54=1;TPCC_TPCC_IESRH_IESR55=1;
 *      TPCC_TPCC_IESRH_IESR56=1;TPCC_TPCC_IESRH_IESR57=1;TPCC_TPCC_IESRH_IESR58=1;
 *      TPCC_TPCC_IESRH_IESR59=1;TPCC_TPCC_IESRH_IESR60=1;TPCC_TPCC_IESRH_IESR61=1;
 *      TPCC_TPCC_IESRH_IESR62=1;TPCC_TPCC_IESRH_IESR63=1;
 *   @n TPCC_TPCC_IECR_IECR0=1;TPCC_TPCC_IECR_IECR1=1;TPCC_TPCC_IECR_IECR2=1;
 *      TPCC_TPCC_IECR_IECR3=1;TPCC_TPCC_IECR_IECR4=1;TPCC_TPCC_IECR_IECR5=1;
 *      TPCC_TPCC_IECR_IECR6=1;TPCC_TPCC_IECR_IECR7=1;TPCC_TPCC_IECR_IECR8=1;
 *      TPCC_TPCC_IECR_IECR9=1;TPCC_TPCC_IECR_IECR10=1;TPCC_TPCC_IECR_IECR11=1;
 *      TPCC_TPCC_IECR_IECR12=1;TPCC_TPCC_IECR_IECR13=1;TPCC_TPCC_IECR_IECR14=1;
 *      TPCC_TPCC_IECR_IECR15=1;TPCC_TPCC_IECR_IECR16=1;TPCC_TPCC_IECR_IECR17=1;
 *      TPCC_TPCC_IECR_IECR18=1;TPCC_TPCC_IECR_IECR19=1;TPCC_TPCC_IECR_IECR20=1;
 *      TPCC_TPCC_IECR_IECR21=1;TPCC_TPCC_IECR_IECR22=1;TPCC_TPCC_IECR_IECR23=1;
 *      TPCC_TPCC_IECR_IECR24=1;TPCC_TPCC_IECR_IECR25=1;TPCC_TPCC_IECR_IECR26=1;
 *      TPCC_TPCC_IECR_IECR27=1;TPCC_TPCC_IECR_IECR28=1;TPCC_TPCC_IECR_IECR29=1;
 *      TPCC_TPCC_IECR_IECR30=1;TPCC_TPCC_IECR_IECR31=1;
 *   @n TPCC_TPCC_IECRH_IECR32=1;TPCC_TPCC_IECRH_IECR33=1;TPCC_TPCC_IECRH_IECR34=1;
 *      TPCC_TPCC_IECRH_IECR35=1;TPCC_TPCC_IECRH_IECR36=1;TPCC_TPCC_IECRH_IECR37=1;
 *      TPCC_TPCC_IECRH_IECR38=1;TPCC_TPCC_IECRH_IECR39=1;TPCC_TPCC_IECRH_IECR40=1;
 *      TPCC_TPCC_IECRH_IECR41=1;TPCC_TPCC_IECRH_IECR42=1;TPCC_TPCC_IECRH_IECR43=1;
 *      TPCC_TPCC_IECRH_IECR44=1;TPCC_TPCC_IECRH_IECR45=1;TPCC_TPCC_IECRH_IECR46=1;
 *      TPCC_TPCC_IECRH_IECR47=1;TPCC_TPCC_IECRH_IECR48=1;TPCC_TPCC_IECRH_IECR49=1;
 *      TPCC_TPCC_IECRH_IECR50=1;TPCC_TPCC_IECRH_IECR51=1;TPCC_TPCC_IECRH_IECR52=1;
 *      TPCC_TPCC_IECRH_IECR53=1;TPCC_TPCC_IECRH_IECR54=1;TPCC_TPCC_IECRH_IECR55=1;
 *      TPCC_TPCC_IECRH_IECR56=1;TPCC_TPCC_IECRH_IECR57=1;TPCC_TPCC_IECRH_IECR58=1;
 *      TPCC_TPCC_IECRH_IECR59=1;TPCC_TPCC_IECRH_IECR60=1;TPCC_TPCC_IECRH_IECR61=1;
 *      TPCC_TPCC_IECRH_IECR62=1;TPCC_TPCC_IECRH_IECR63=1;
 *   @n TPCC_TPCC_IEVAL_EVAL=1;
 *   @n TPCC_TPCC_CCERRCLR_QTHRXD0;TPCC_TPCC_CCERRCLR_QTHRXD1;TPCC_TPCC_CCERRCLR_QTHRXD2;
 *      TPCC_TPCC_CCERRCLR_QTHRXD3;TPCC_TPCC_CCERRCLR_QTHRXD4;TPCC_TPCC_CCERRCLR_QTHRXD5;
 *      TPCC_TPCC_CCERRCLR_QTHRXD6;TPCC_TPCC_CCERRCLR_QTHRXD7,
 *      TPCC_TPCC_CCERR_TCCERR;
 *   @n TPCC_TPCC_EMCR,TPCC_TPCC_EMCRH,TPCC_TPCC_QEMCR
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3CmdDrae            regionAccess;

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // DRAE Enable(Bits 0-15) for the Shadow Region 0.
            regionAccess.region = CSL_EDMA3_REGION_0 ;
            regionAccess.drae =   0xFFFF ;   
            regionAccess.draeh =  0x0000 ;
            CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_DMAREGION_ENABLE, &regionAccess);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwControl, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwControl 
(
    CSL_Edma3Handle         hMod,
    CSL_Edma3HwControlCmd   cmd,
    void*                   cmdArg
)
{ 
    CSL_Status  status = CSL_SOK; 
   
    /* Validate the arguments that have been passed. */ 
    if (hMod == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (((cmd >= 0) && (cmd <= CSL_EDMA3_CMD_EVENTMISSED_CLEAR)) && 
              ((cmd != CSL_EDMA3_CMD_MEMFAULT_CLEAR) && 
               (cmd != CSL_EDMA3_CMD_ERROR_EVAL)) && (cmdArg == NULL)) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; process the command. */
        switch(cmd) 
        {
            case CSL_EDMA3_CMD_MEMPROTECT_SET:
            {
                CSL_edma3SetMemoryProtectionAttrib(hMod, \
                                     ((CSL_Edma3CmdRegion*)cmdArg)->region,\
                                     ((CSL_Edma3CmdRegion*)cmdArg)->regionVal);
                break;
            }
            case CSL_EDMA3_CMD_MEMFAULT_CLEAR:
            {
                CSL_edma3MemFaultClear(hMod);
                break;
            }
            case CSL_EDMA3_CMD_DMAREGION_ENABLE:
            {
                CSL_edma3DmaRegionAccessEnable(hMod, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->region, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->drae, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->draeh);
                break;
            }   
            case CSL_EDMA3_CMD_DMAREGION_DISABLE:
            {
                CSL_edma3DmaRegionAccessDisable(hMod, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->region, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->drae, \
                                     ((CSL_Edma3CmdDrae*)cmdArg)->draeh);
                break;
            }    
            case CSL_EDMA3_CMD_QDMAREGION_ENABLE:
            {
                CSL_edma3QdmaRegionAccessEnable(hMod, \
                                     ((CSL_Edma3CmdQrae*)cmdArg)->region,\
                                     ((CSL_Edma3CmdQrae*)cmdArg)->qrae);
                break;
            }    
            case CSL_EDMA3_CMD_QDMAREGION_DISABLE:
            {
                CSL_edma3QdmaRegionAccessDisable(hMod, \
                                     ((CSL_Edma3CmdQrae*)cmdArg)->region, \
                                     ((CSL_Edma3CmdQrae*)cmdArg)->qrae);
                break;  
            }
            case CSL_EDMA3_CMD_QUEPRIORITY_SET:
            {
                CSL_edma3SetEventQueuePriority(hMod, ((CSL_Edma3CmdQuePri*)cmdArg)->que, \
                                                     ((CSL_Edma3CmdQuePri*)cmdArg)->pri);
                break;
            }   
            case CSL_EDMA3_CMD_QUETHRESHOLD_SET:
            {
                CSL_edma3EventQueueThresholdSet(hMod,\
                                     ((CSL_Edma3CmdQueThr*)cmdArg)->que, \
                                     ((CSL_Edma3CmdQueThr*)cmdArg)->threshold);
                break;          
            }    
            case CSL_EDMA3_CMD_ERROR_EVAL:
            {
                CSL_edma3ErrorEval(hMod);
                break;          
            }    
            case CSL_EDMA3_CMD_INTRPEND_CLEAR:
            {
                CSL_Edma3Obj*   ptrEdma3Obj = (CSL_Edma3Obj *)hMod;

                /* Clear the low pending interrupts. */
                CSL_edma3ClearLoPendingInterrupts (hMod,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->intr);

                /* Clear the high pending interrupts only if they are supported*/
                if (ptrEdma3Obj->cfgInfo.numINTChannel > 31)
                {
                    CSL_edma3ClearHiPendingInterrupts (hMod,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->intrh);
                }
                break;          
            }    
            case CSL_EDMA3_CMD_INTR_ENABLE:
            {
                CSL_Edma3Obj*   ptrEdma3Obj = (CSL_Edma3Obj *)hMod;

                /* Enable the low interrupts. */
                CSL_edma3InterruptLoEnable(hMod,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->intr);

                /* Enable the high pending interrupts only if they are supported*/
                if (ptrEdma3Obj->cfgInfo.numINTChannel > 31)
                {
                    CSL_edma3InterruptHiEnable(hMod,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->intrh);
                }
                break;          
            }    
            case CSL_EDMA3_CMD_INTR_DISABLE:
            {
                CSL_Edma3Obj*   ptrEdma3Obj = (CSL_Edma3Obj *)hMod;

                /* Disable the low interrupts */
                CSL_edma3InterruptLoDisable(hMod,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                     ((CSL_Edma3CmdIntr*)(cmdArg))->intr);

                /* Disable the high pending interrupts only if they are supported */
                if (ptrEdma3Obj->cfgInfo.numINTChannel > 31)
                {
                    /* Disable the high interrupts */
                    CSL_edma3InterruptHiDisable(hMod,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->region,
                                         ((CSL_Edma3CmdIntr*)(cmdArg))->intrh);
                }
                break;
            }    
            case CSL_EDMA3_CMD_INTR_EVAL:
            {
                CSL_edma3InterruptEval(hMod,*((Int*)(cmdArg)));                        
                break;  
            }           
            case CSL_EDMA3_CMD_CTRLERROR_CLEAR:
            {
                CSL_edma3ClearControllerError(hMod, ((CSL_Edma3CtrlErrStat*)(cmdArg)));
                break;
            }    
            case CSL_EDMA3_CMD_EVENTMISSED_CLEAR:
            {
                CSL_edma3EventsMissedClear(hMod,((CSL_BitMask32*)cmdArg)[0], \
                                           ((CSL_BitMask32*)cmdArg)[1],
                                           ((CSL_BitMask32*)cmdArg)[2]);
                break;
            }
            default:
            {
                 status = CSL_ESYS_INVCMD;
                 break;
            }
        }
    }

    return (status);
}

