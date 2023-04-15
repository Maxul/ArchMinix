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
 *  @file  csl_intcClose.c
 *
 *  @brief   File for functional layer of CSL API CSL_intcClose()
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
#include <ti/csl/src/intc/_csl_intc.h>
#include <ti/csl/src/intc/csl_intcAux.h>

#pragma CODE_SECTION (CSL_intcClose, ".text:csl_section:intc");
CSL_Status  CSL_intcClose (
    CSL_IntcHandle      hIntc
)
{
    
    CSL_Status  closeStatus = CSL_ESYS_BADHANDLE;
    Uint32      evt;
    Uint32      x;
    Uint32      y;
    
    if (hIntc != (CSL_IntcHandle) CSL_INTC_BADHANDLE) {
        evt = (Uint32)(hIntc->eventId);
        y = evt >> 5;
        x = 1 << (evt & 0x1f);
        
        asm(" dint");
        _CSL_intcAllocMask[y] &= ~x; /* clear bit -> unused */
        if (_CSL_intcEventOffsetMap[hIntc->eventId] != CSL_INTC_MAPPED_NONE) {
            _CSL_intcEventHandlerRecord
            [_CSL_intcEventOffsetMap[hIntc->eventId]].handler = 
                                                       CSL_INTC_EVTHANDLER_NONE;
            _CSL_intcEventOffsetMap[hIntc->eventId] = CSL_INTC_MAPPED_NONE;
        }   
        hIntc = (CSL_IntcHandle) CSL_INTC_BADHANDLE;
        asm(" rint");
        
        closeStatus = CSL_SOK;      
    }
    
    return closeStatus;
}
