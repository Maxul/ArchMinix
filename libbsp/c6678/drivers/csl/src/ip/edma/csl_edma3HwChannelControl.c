/**
 *   @file  csl_edma3HwChannelControl.c
 *
 *   @brief   
 *      The file implements the API which is used to configure the various 
 *      properties of an opened channel.
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
 *   @n@b CSL_edma3HwChannelControl
 *
 *   @b Description
 *   @n This API is used to control the EDMA Channel. The function accepts a
 *      command as an argument on the type of operation which is to be performed
 *      on the specific channel.  
 *
 *   @b Arguments
 *   @verbatim
            hCh             Channel Handle
            cmd             Command/Action to be performed on the channel handle.
            cmdArg          Pointer to the argument specific to the command                             
      @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen()
 *       must be called successfully in that order before this API can be 
 *       invoked. If a Shadow region is used then care of the DRAE settings
 *       must be taken care of.
 *
 *   <b> Post Condition </b>
 *   @n  Edma registers are configured according to the command and
 *       the command arguements. The command determines which registers are
 *       modified.
 *
 *   @b Writes
 *   @n TPCC_TPCC_QEESR_QEESR0=1;TPCC_TPCC_QEESR_QEESR1=1;TPCC_TPCC_QEESR_QEESR2=1;
 *      TPCC_TPCC_QEESR_QEESR3=1;TPCC_TPCC_QEESR_QEESR4=1;TPCC_TPCC_QEESR_QEESR5=1;
 *      TPCC_TPCC_QEESR_QEESR6=1;TPCC_TPCC_QEESR_QEESR7=1;
 *   @n TPCC_TPCC_EESR_E0=1;TPCC_TPCC_EESR_E1=1;TPCC_TPCC_EESR_E2=1;
 *      TPCC_TPCC_EESR_E3=1;TPCC_TPCC_EESR_E4=1;TPCC_TPCC_EESR_E5=1;
 *      TPCC_TPCC_EESR_E6=1;TPCC_TPCC_EESR_E7=1;TPCC_TPCC_EESR_E8=1;
 *      TPCC_TPCC_EESR_E9=1;TPCC_TPCC_EESR_E10=1;TPCC_TPCC_EESR_E11=1;
 *      TPCC_TPCC_EESR_E12=1;TPCC_TPCC_EESR_E13=1;TPCC_TPCC_EESR_E14=1;
 *      TPCC_TPCC_EESR_E15=1;TPCC_TPCC_EESR_E16=1;TPCC_TPCC_EESR_E17=1;
 *      TPCC_TPCC_EESR_E18=1;TPCC_TPCC_EESR_E19=1;TPCC_TPCC_EESR_E20=1;
 *      TPCC_TPCC_EESR_E21=1;TPCC_TPCC_EESR_E22=1;TPCC_TPCC_EESR_E23=1;
 *      TPCC_TPCC_EESR_E24=1;TPCC_TPCC_EESR_E25=1;TPCC_TPCC_EESR_E26=1;
 *      TPCC_TPCC_EESR_E27=1;TPCC_TPCC_EESR_E28=1;TPCC_TPCC_EESR_E29=1;
 *      TPCC_TPCC_EESR_E30=1;TPCC_TPCC_EESR_E31=1;
 *   @n TPCC_TPCC_EESRH_E32=1;TPCC_TPCC_EESRH_E33=1;TPCC_TPCC_EESRH_E34=1;
 *      TPCC_TPCC_EESRH_E35=1;TPCC_TPCC_EESRH_E36=1;TPCC_TPCC_EESRH_E37=1;
 *      TPCC_TPCC_EESRH_E38=1;TPCC_TPCC_EESRH_E39=1;TPCC_TPCC_EESRH_E40=1;
 *      TPCC_TPCC_EESRH_E41=1;TPCC_TPCC_EESRH_E42=1;TPCC_TPCC_EESRH_E43=1;
 *      TPCC_TPCC_EESRH_E44=1;TPCC_TPCC_EESRH_E45=1;TPCC_TPCC_EESRH_E46=1;
 *      TPCC_TPCC_EESRH_E47=1;TPCC_TPCC_EESRH_E48=1;TPCC_TPCC_EESRH_E49=1;
 *      TPCC_TPCC_EESRH_E50=1;TPCC_TPCC_EESRH_E51=1;TPCC_TPCC_EESRH_E52=1;
 *      TPCC_TPCC_EESRH_E53=1;TPCC_TPCC_EESRH_E54=1;TPCC_TPCC_EESRH_E55=1;
 *      TPCC_TPCC_EESRH_E56=1;TPCC_TPCC_EESRH_E57=1;TPCC_TPCC_EESRH_E58=1;
 *      TPCC_TPCC_EESRH_E59=1;TPCC_TPCC_EESRH_E60=1;TPCC_TPCC_EESRH_E61=1;
 *      TPCC_TPCC_EESRH_E62=1;TPCC_TPCC_EESRH_E63=1;
 *   @n TPCC_TPCC_QEECR_QEECR0=1;TPCC_TPCC_QEECR_QEECR1=1;TPCC_TPCC_QEECR_QEECR2=1;
 *      TPCC_TPCC_QEECR_QEECR3=1;TPCC_TPCC_QEECR_QEECR4=1;TPCC_TPCC_QEECR_QEECR5=1;
 *      TPCC_TPCC_QEECR_QEECR6=1;TPCC_TPCC_QEECR_QEECR7=1;
 *   @n TPCC_TPCC_EECR_E0=1;TPCC_TPCC_EECR_E1=1;TPCC_TPCC_EECR_E2=1;
 *      TPCC_TPCC_EECR_E3=1;TPCC_TPCC_EECR_E4=1;TPCC_TPCC_EECR_E5=1;
 *      TPCC_TPCC_EECR_E6=1;TPCC_TPCC_EECR_E7=1;TPCC_TPCC_EECR_E8=1;
 *      TPCC_TPCC_EECR_E9=1;TPCC_TPCC_EECR_E10=1;TPCC_TPCC_EECR_E11=1;
 *      TPCC_TPCC_EECR_E12=1;TPCC_TPCC_EECR_E13=1;TPCC_TPCC_EECR_E14=1;
 *      TPCC_TPCC_EECR_E15=1;TPCC_TPCC_EECR_E16=1;TPCC_TPCC_EECR_E17=1;
 *      TPCC_TPCC_EECR_E18=1;TPCC_TPCC_EECR_E19=1;TPCC_TPCC_EECR_E20=1;
 *      TPCC_TPCC_EECR_E21=1;TPCC_TPCC_EECR_E22=1;TPCC_TPCC_EECR_E23=1;
 *      TPCC_TPCC_EECR_E24=1;TPCC_TPCC_EECR_E25=1;TPCC_TPCC_EECR_E26=1;
 *      TPCC_TPCC_EECR_E27=1;TPCC_TPCC_EECR_E28=1;TPCC_TPCC_EECR_E29=1;
 *      TPCC_TPCC_EECR_E30=1;TPCC_TPCC_EECR_E31=1;
 *   @n TPCC_TPCC_EECRH_E32=1;TPCC_TPCC_EECRH_E33=1;TPCC_TPCC_EECRH_E34=1;
 *      TPCC_TPCC_EECRH_E35=1;TPCC_TPCC_EECRH_E36=1;TPCC_TPCC_EECRH_E37=1;
 *      TPCC_TPCC_EECRH_E38=1;TPCC_TPCC_EECRH_E39=1;TPCC_TPCC_EECRH_E40=1;
 *      TPCC_TPCC_EECRH_E41=1;TPCC_TPCC_EECRH_E42=1;TPCC_TPCC_EECRH_E43=1;
 *      TPCC_TPCC_EECRH_E44=1;TPCC_TPCC_EECRH_E45=1;TPCC_TPCC_EECRH_E46=1;
 *      TPCC_TPCC_EECRH_E47=1;TPCC_TPCC_EECRH_E48=1;TPCC_TPCC_EECRH_E49=1;
 *      TPCC_TPCC_EECRH_E50=1;TPCC_TPCC_EECRH_E51=1;TPCC_TPCC_EECRH_E52=1;
 *      TPCC_TPCC_EECRH_E53=1;TPCC_TPCC_EECRH_E54=1;TPCC_TPCC_EECRH_E55=1;
 *      TPCC_TPCC_EECRH_E56=1;TPCC_TPCC_EECRH_E57=1;TPCC_TPCC_EECRH_E58=1;
 *      TPCC_TPCC_EECRH_E59=1;TPCC_TPCC_EECRH_E60=1;TPCC_TPCC_EECRH_E61=1;
 *      TPCC_TPCC_EECRH_E62=1;TPCC_TPCC_EECRH_E63=1;
 *   @n TPCC_TPCC_ESR_E0=1;TPCC_TPCC_ESR_E1=1;TPCC_TPCC_ESR_E2=1;TPCC_TPCC_ESR_E3=1;
 *      TPCC_TPCC_ESR_E4=1;TPCC_TPCC_ESR_E5=1;TPCC_TPCC_ESR_E6=1;TPCC_TPCC_ESR_E7=1;
 *      TPCC_TPCC_ESR_E8=1;TPCC_TPCC_ESR_E9=1;TPCC_TPCC_ESR_E10=1;TPCC_TPCC_ESR_E11=1;
 *      TPCC_TPCC_ESR_E12=1;TPCC_TPCC_ESR_E13=1;TPCC_TPCC_ESR_E14=1;TPCC_TPCC_ESR_E15=1;
 *      TPCC_TPCC_ESR_E16=1;TPCC_TPCC_ESR_E17=1;TPCC_TPCC_ESR_E18=1;TPCC_TPCC_ESR_E19=1;
 *      TPCC_TPCC_ESR_E20=1;TPCC_TPCC_ESR_E21=1;TPCC_TPCC_ESR_E22=1;TPCC_TPCC_ESR_E23=1;
 *      TPCC_TPCC_ESR_E24=1;TPCC_TPCC_ESR_E25=1;TPCC_TPCC_ESR_E26=1;TPCC_TPCC_ESR_E27=1;
 *      TPCC_TPCC_ESR_E28=1;TPCC_TPCC_ESR_E29=1;TPCC_TPCC_ESR_E30=1;TPCC_TPCC_ESR_E31=1;
 *   @n TPCC_TPCC_ESRH_E32=1;TPCC_TPCC_ESRH_E33=1;TPCC_TPCC_ESRH_E34=1;TPCC_TPCC_ESRH_E35=1;
 *      TPCC_TPCC_ESRH_E36=1;TPCC_TPCC_ESRH_E37=1;TPCC_TPCC_ESRH_E38=1;TPCC_TPCC_ESRH_E39=1;
 *      TPCC_TPCC_ESRH_E40=1;TPCC_TPCC_ESRH_E41=1;TPCC_TPCC_ESRH_E42=1;TPCC_TPCC_ESRH_E43=1;
 *      TPCC_TPCC_ESRH_E44=1;TPCC_TPCC_ESRH_E45=1;TPCC_TPCC_ESRH_E46=1;TPCC_TPCC_ESRH_E47=1;
 *      TPCC_TPCC_ESRH_E48=1;TPCC_TPCC_ESRH_E49=1;TPCC_TPCC_ESRH_E50=1;TPCC_TPCC_ESRH_E51=1;
 *      TPCC_TPCC_ESRH_E52=1;TPCC_TPCC_ESRH_E53=1;TPCC_TPCC_ESRH_E54=1;TPCC_TPCC_ESRH_E55=1;
 *      TPCC_TPCC_ESRH_E56=1;TPCC_TPCC_ESRH_E57=1;TPCC_TPCC_ESRH_E58=1;TPCC_TPCC_ESRH_E59=1;
 *      TPCC_TPCC_ESRH_E60=1;TPCC_TPCC_ESRH_E61=1;TPCC_TPCC_ESRH_E62=1;TPCC_TPCC_ESRH_E63=1;
 *   @n TPCC_TPCC_ECR_E0=1;TPCC_TPCC_ECR_E1=1;TPCC_TPCC_ECR_E2=1;TPCC_TPCC_ECR_E3=1;
 *      TPCC_TPCC_ECR_E4=1;TPCC_TPCC_ECR_E5=1;TPCC_TPCC_ECR_E6=1;TPCC_TPCC_ECR_E7=1;
 *      TPCC_TPCC_ECR_E8=1;TPCC_TPCC_ECR_E9=1;TPCC_TPCC_ECR_E10=1;TPCC_TPCC_ECR_E11=1;
 *      TPCC_TPCC_ECR_E12=1;TPCC_TPCC_ECR_E13=1;TPCC_TPCC_ECR_E14=1;TPCC_TPCC_ECR_E15=1;
 *      TPCC_TPCC_ECR_E16=1;TPCC_TPCC_ECR_E17=1;TPCC_TPCC_ECR_E18=1;TPCC_TPCC_ECR_E19=1;
 *      TPCC_TPCC_ECR_E20=1;TPCC_TPCC_ECR_E21=1;TPCC_TPCC_ECR_E22=1;TPCC_TPCC_ECR_E23=1;
 *      TPCC_TPCC_ECR_E24=1;TPCC_TPCC_ECR_E25=1;TPCC_TPCC_ECR_E26=1;TPCC_TPCC_ECR_E27=1;
 *      TPCC_TPCC_ECR_E28=1;TPCC_TPCC_ECR_E29=1;TPCC_TPCC_ECR_E30=1;TPCC_TPCC_ECR_E31=1;
 *   @n TPCC_TPCC_ECRH_E32=1;TPCC_TPCC_ECRH_E33=1;TPCC_TPCC_ECRH_E34=1;TPCC_TPCC_ECRH_E35=1;
 *      TPCC_TPCC_ECRH_E36=1;TPCC_TPCC_ECRH_E37=1;TPCC_TPCC_ECRH_E38=1;TPCC_TPCC_ECRH_E39=1;
 *      TPCC_TPCC_ECRH_E40=1;TPCC_TPCC_ECRH_E41=1;TPCC_TPCC_ECRH_E42=1;TPCC_TPCC_ECRH_E43=1;
 *      TPCC_TPCC_ECRH_E44=1;TPCC_TPCC_ECRH_E45=1;TPCC_TPCC_ECRH_E46=1;TPCC_TPCC_ECRH_E47=1;
 *      TPCC_TPCC_ECRH_E48=1;TPCC_TPCC_ECRH_E49=1;TPCC_TPCC_ECRH_E50=1;TPCC_TPCC_ECRH_E51=1;
 *      TPCC_TPCC_ECRH_E52=1;TPCC_TPCC_ECRH_E53=1;TPCC_TPCC_ECRH_E54=1;TPCC_TPCC_ECRH_E55=1;
 *      TPCC_TPCC_ECRH_E56=1;TPCC_TPCC_ECRH_E57=1;TPCC_TPCC_ECRH_E58=1;TPCC_TPCC_ECRH_E59=1;
 *      TPCC_TPCC_ECRH_E60=1;TPCC_TPCC_ECRH_E61=1;TPCC_TPCC_ECRH_E62=1;TPCC_TPCC_ECRH_E63=1;
 *   @n TPCC_TPCC_EMCR_EMCR0;TPCC_TPCC_EMCR_EMCR1;TPCC_TPCC_EMCR_EMCR2;
 *      TPCC_TPCC_EMCR_EMCR3;TPCC_TPCC_EMCR_EMCR4;TPCC_TPCC_EMCR_EMCR5;
 *      TPCC_TPCC_EMCR_EMCR6;TPCC_TPCC_EMCR_EMCR7;TPCC_TPCC_EMCR_EMCR8;
 *      TPCC_TPCC_EMCR_EMCR9;TPCC_TPCC_EMCR_EMCR10;TPCC_TPCC_EMCR_EMCR11;
 *      TPCC_TPCC_EMCR_EMCR12;TPCC_TPCC_EMCR_EMCR13;TPCC_TPCC_EMCR_EMCR14;
 *      TPCC_TPCC_EMCR_EMCR15;TPCC_TPCC_EMCR_EMCR16;TPCC_TPCC_EMCR_EMCR17;
 *      TPCC_TPCC_EMCR_EMCR18;TPCC_TPCC_EMCR_EMCR19;TPCC_TPCC_EMCR_EMCR20;
 *      TPCC_TPCC_EMCR_EMCR21;TPCC_TPCC_EMCR_EMCR22;TPCC_TPCC_EMCR_EMCR23;
 *      TPCC_TPCC_EMCR_EMCR24;TPCC_TPCC_EMCR_EMCR25;TPCC_TPCC_EMCR_EMCR26;
 *      TPCC_TPCC_EMCR_EMCR27;TPCC_TPCC_EMCR_EMCR28;TPCC_TPCC_EMCR_EMCR29;
 *      TPCC_TPCC_EMCR_EMCR30;TPCC_TPCC_EMCR_EMCR31;
 *   @n TPCC_TPCC_EMCRH_EMCR32;TPCC_TPCC_EMCRH_EMCR33;TPCC_TPCC_EMCRH_EMCR34
 *      TPCC_TPCC_EMCRH_EMCR35;TPCC_TPCC_EMCRH_EMCR36;TPCC_TPCC_EMCRH_EMCR37
 *      TPCC_TPCC_EMCRH_EMCR38;TPCC_TPCC_EMCRH_EMCR39;TPCC_TPCC_EMCRH_EMCR40
 *      TPCC_TPCC_EMCRH_EMCR41;TPCC_TPCC_EMCRH_EMCR42;TPCC_TPCC_EMCRH_EMCR43
 *      TPCC_TPCC_EMCRH_EMCR44;TPCC_TPCC_EMCRH_EMCR45;TPCC_TPCC_EMCRH_EMCR46
 *      TPCC_TPCC_EMCRH_EMCR47;TPCC_TPCC_EMCRH_EMCR48;TPCC_TPCC_EMCRH_EMCR49
 *      TPCC_TPCC_EMCRH_EMCR50;TPCC_TPCC_EMCRH_EMCR51;TPCC_TPCC_EMCRH_EMCR52
 *      TPCC_TPCC_EMCRH_EMCR53;TPCC_TPCC_EMCRH_EMCR54;TPCC_TPCC_EMCRH_EMCR55
 *      TPCC_TPCC_EMCRH_EMCR56;TPCC_TPCC_EMCRH_EMCR57;TPCC_TPCC_EMCRH_EMCR58
 *      TPCC_TPCC_EMCRH_EMCR59;TPCC_TPCC_EMCRH_EMCR60;TPCC_TPCC_EMCRH_EMCR61
 *      TPCC_TPCC_EMCRH_EMCR62;TPCC_TPCC_EMCRH_EMCR63,
 *   @n TPCC_TPCC_SECR_SECR0=1;TPCC_TPCC_SECR_SECR1=1;TPCC_TPCC_SECR_SECR2=1;
 *      TPCC_TPCC_SECR_SECR3=1;TPCC_TPCC_SECR_SECR4=1;TPCC_TPCC_SECR_SECR5=1;
 *      TPCC_TPCC_SECR_SECR6=1;TPCC_TPCC_SECR_SECR7=1;TPCC_TPCC_SECR_SECR8=1;
 *      TPCC_TPCC_SECR_SECR9=1;TPCC_TPCC_SECR_SECR10=1;TPCC_TPCC_SECR_SECR11=1;
 *      TPCC_TPCC_SECR_SECR12=1;TPCC_TPCC_SECR_SECR13=1;TPCC_TPCC_SECR_SECR14=1;
 *      TPCC_TPCC_SECR_SECR15=1;TPCC_TPCC_SECR_SECR16=1;TPCC_TPCC_SECR_SECR17=1;
 *      TPCC_TPCC_SECR_SECR18=1;TPCC_TPCC_SECR_SECR19=1;TPCC_TPCC_SECR_SECR20=1;
 *      TPCC_TPCC_SECR_SECR21=1;TPCC_TPCC_SECR_SECR22=1;TPCC_TPCC_SECR_SECR23=1;
 *      TPCC_TPCC_SECR_SECR24=1;TPCC_TPCC_SECR_SECR25=1;TPCC_TPCC_SECR_SECR26=1;
 *      TPCC_TPCC_SECR_SECR27=1;TPCC_TPCC_SECR_SECR28=1;TPCC_TPCC_SECR_SECR29=1;
 *      TPCC_TPCC_SECR_SECR30=1;TPCC_TPCC_SECR_SECR31=1;
 *   @n TPCC_TPCC_SECRH_SECR32=1;TPCC_TPCC_SECRH_SECR33=1;TPCC_TPCC_SECRH_SECR34=1;
 *      TPCC_TPCC_SECRH_SECR35=1;TPCC_TPCC_SECRH_SECR36=1;TPCC_TPCC_SECRH_SECR37=1;
 *      TPCC_TPCC_SECRH_SECR38=1;TPCC_TPCC_SECRH_SECR39=1;TPCC_TPCC_SECRH_SECR40=1;
 *      TPCC_TPCC_SECRH_SECR41=1;TPCC_TPCC_SECRH_SECR42=1;TPCC_TPCC_SECRH_SECR43=1;
 *      TPCC_TPCC_SECRH_SECR44=1;TPCC_TPCC_SECRH_SECR45=1;TPCC_TPCC_SECRH_SECR46=1;
 *      TPCC_TPCC_SECRH_SECR47=1;TPCC_TPCC_SECRH_SECR48=1;TPCC_TPCC_SECRH_SECR49=1;
 *      TPCC_TPCC_SECRH_SECR50=1;TPCC_TPCC_SECRH_SECR51=1;TPCC_TPCC_SECRH_SECR52=1;
 *      TPCC_TPCC_SECRH_SECR53=1;TPCC_TPCC_SECRH_SECR54=1;TPCC_TPCC_SECRH_SECR55=1;
 *      TPCC_TPCC_SECRH_SECR56=1;TPCC_TPCC_SECRH_SECR57=1;TPCC_TPCC_SECRH_SECR58=1;
 *      TPCC_TPCC_SECRH_SECR59=1;TPCC_TPCC_SECRH_SECR60=1;TPCC_TPCC_SECRH_SECR61=1;
 *      TPCC_TPCC_SECRH_SECR62=1;TPCC_TPCC_SECRH_SECR63=1;
 *   @n TPCC_TPCC_QEMCR_QEMCR0;TPCC_TPCC_QEMCR_QEMCR1;TPCC_TPCC_QEMCR_QEMCR2;
 *      TPCC_TPCC_QEMCR_QEMCR3;TPCC_TPCC_QEMCR_QEMCR4;TPCC_TPCC_QEMCR_QEMCR5;
 *      TPCC_TPCC_QEMCR_QEMCR6;TPCC_TPCC_QEMCR_QEMCR7,
 *   @n TPCC_TPCC_QSECR_QSECR0=1;TPCC_TPCC_QSECR_QSECR1=1;TPCC_TPCC_QSECR_QSECR2=1;
 *      TPCC_TPCC_QSECR_QSECR3=1;TPCC_TPCC_QSECR_QSECR4=1;TPCC_TPCC_QSECR_QSECR5=1;
 *      TPCC_TPCC_QSECR_QSECR6=1;TPCC_TPCC_QSECR_QSECR7=1;
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, CSL_EDMA3, &chAttr, &status);
            ...               
            // Enable Channel 
            CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_ENABLE,NULL);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3HwChannelControl, ".text:csl_section:edma3");
CSL_Status  CSL_edma3HwChannelControl 
(
    CSL_Edma3ChannelHandle          hCh,
    CSL_Edma3HwChannelControlCmd    cmd,
    void*                           cmdArg
)
{ 
    CSL_Status status = CSL_SOK;
 
    /* Validate the arguments. */   
    if (hCh == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if ((cmd == CSL_EDMA3_CMD_CHANNEL_CLEARERR) && (cmdArg == NULL)) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good; Process the command... */
        switch(cmd) 
        {
            case CSL_EDMA3_CMD_CHANNEL_ENABLE:
            {
                /* Determine if this is a request for a DMA or QDMA Channel? */
                if (hCh->chaNum >= hCh->cfgInfo.numDMAChannel)
                {
                    /* QDMA Channel is being enabled. */
                    CSL_edma3QDMAChannelEnable ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum - hCh->cfgInfo.numDMAChannel);
                }
                else
                {
                    /* DMA Channel is being enabled. */
                    CSL_edma3DMAChannelEnable ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum);
                }
                break;
            }
            case CSL_EDMA3_CMD_CHANNEL_DISABLE:
            {
                /* Determine if this is a request for a DMA or QDMA Channel? */
                if (hCh->chaNum >= hCh->cfgInfo.numDMAChannel)
                {
                    /* QDMA Channel is being disabled. */
                    CSL_edma3QDMAChannelDisable ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum - hCh->cfgInfo.numDMAChannel);
                }
                else
                {
                    /* DMA Channel is being disabled. */
                    CSL_edma3DMAChannelDisable ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum);
                }
                break;
            }            
            case CSL_EDMA3_CMD_CHANNEL_SET:
            {
                /* Determine if this is a request for a DMA or QDMA Channel? */
                if (hCh->chaNum < hCh->cfgInfo.numDMAChannel)
                {
                    /* Only DMA channels can be set through this... */
                    CSL_edma3SetDMAChannelEvent ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum);
                }
                else
                {
                    /* QDMA Channels are invalid. */
                    status = CSL_ESYS_INVPARAMS;
                }   
                break;
            }            
            case CSL_EDMA3_CMD_CHANNEL_CLEAR:
            {
                /* Determine if this is a request for a DMA or QDMA Channel? */
                if (hCh->chaNum < hCh->cfgInfo.numDMAChannel)
                { 
                    /* Only DMA Channels can be cleared. */
                    CSL_edma3ClearDMAChannelEvent ((CSL_Edma3Handle)hCh, hCh->region, hCh->chaNum);
                }
                else
                {
                    /* QDMA Channels are invalid. */
                    status = CSL_ESYS_INVPARAMS;
                }                                       
                break;
            }                      
            case CSL_EDMA3_CMD_CHANNEL_CLEARERR:
            {
                CSL_Edma3ChannelErr* ptrChannelErr = (CSL_Edma3ChannelErr *)cmdArg;
                
                if (ptrChannelErr->missed == TRUE) 
                {
                    /* Determine if this is a request for a DMA or QDMA Channel? */
                    if (hCh->chaNum < hCh->cfgInfo.numDMAChannel) 
                    {
                        /* DMA Channel */
                        CSL_edma3ClearDMAMissedEvent ((CSL_Edma3Handle)hCh, hCh->chaNum);
                        if (ptrChannelErr->secEvt == TRUE)
                            CSL_edma3ClearDMAChannelSecondaryEvents ((CSL_Edma3Handle)hCh, hCh->chaNum);
                    }                    
                    else 
                    {
                        /* QDMA Channel. */                            
                        CSL_edma3ClearQDMAMissedEvent ((CSL_Edma3Handle)hCh, hCh->chaNum - hCh->cfgInfo.numDMAChannel);
                        if (ptrChannelErr->secEvt == TRUE)
                            CSL_edma3ClearQDMAChannelSecondaryEvents((CSL_Edma3Handle)hCh, 
                                                                     hCh->chaNum - hCh->cfgInfo.numDMAChannel);
                    }
                }
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

