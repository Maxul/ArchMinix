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

/*
 *  @file  _csl_int.h
 *
 *  @brief  File for functional layer of INTC CSL
 *
 *  PATH  $(CSLPATH)\inc
 */
 
/* =============================================================================
 *  Revision History
 *  ===============
 *  12-Jun-2004 Ruchika Kharwar File Created
 *  
 * =============================================================================
 */

#ifndef __CSL_INTC_H_
#define __CSL_INTC_H_

#include <ti/csl/src/intc/csl_intc.h>

void _CSL_intcNmiDummy();

typedef struct CSL_IntcVect {
    Uint32 currentVectId;
    void (*resv0)();
    void (*nmiIsr)();
    void (*resv2)();
    void (*resv3)();
    void (*isr4)();
    void (*isr5)();
    void (*isr6)();
    void (*isr7)();
    void (*isr8)();
    void (*isr9)();
    void (*isr10)();
    void (*isr11)();
    void (*isr12)();
    void (*isr13)();
    void (*isr14)();
    void (*isr15)();
} CSL_IntcVect;
extern CSL_IntcVect _CSL_intcCpuIntrTable;

/* These declarations are meant for computing the ISR jump location. */
void _CSL_intcIvpSet();
interrupt void _CSL_intcDispatcher (void);
interrupt void _CSL_intcEvent0Dispatcher (void);
interrupt void _CSL_intcEvent1Dispatcher (void);
interrupt void _CSL_intcEvent2Dispatcher (void);
interrupt void _CSL_intcEvent3Dispatcher (void);

#endif /* __CSL_INTC_H_ */
