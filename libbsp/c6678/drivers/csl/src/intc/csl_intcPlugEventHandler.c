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
 *  @file  csl_intcPlugEventHandler.c
 *
 *  @brief  File for functional layer of CSL API @a CSL_intcPlugEventHandler()
 *
 *  PATH  $(CSLPATH)\src\intc
 *  
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

#pragma CODE_SECTION (CSL_intcPlugEventHandler, ".text:csl_section:intc");
CSL_Status CSL_intcPlugEventHandler (
    CSL_IntcHandle                 hIntc,
    CSL_IntcEventHandlerRecord     *eventHandlerRecord
)
{
    volatile Uint16  i;
    CSL_Status status = CSL_ESYS_FAIL;

    asm(" dint");

    /* Search for free entry and plug in handler */
    
    for (i = 0; i < _CSL_intcNumEvents; i++) {
        if (_CSL_intcEventHandlerRecord[i].handler == CSL_INTC_EVTHANDLER_NONE){
            /* Plug in Handler */
            _CSL_intcEventHandlerRecord[i].handler =
                eventHandlerRecord->handler;
            _CSL_intcEventHandlerRecord[i].arg =
                eventHandlerRecord->arg;    
            /* Establish mapping */
            _CSL_intcEventOffsetMap[hIntc->eventId] = i;
            status = CSL_SOK;
            break;          
        }
    
    }
    asm(" rint");
    
    return status;
}
