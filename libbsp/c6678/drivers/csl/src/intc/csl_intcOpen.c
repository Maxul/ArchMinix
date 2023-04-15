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
 *  @file  csl_intcOpen.c
 *
 *  @brief    File for functional layer of CSL API CSL_intcOpen()
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

#pragma CODE_SECTION (CSL_intcOpen, ".text:csl_section:intc");
CSL_IntcHandle  CSL_intcOpen (
    CSL_IntcObj          *intcObj,
    CSL_IntcEventId      eventId,
    CSL_IntcParam        *param,
    CSL_Status           *status
)
{
    CSL_IntcHandle  hIntc = (CSL_IntcHandle)CSL_INTC_BADHANDLE;
    CSL_Status      openStatus;
    Uint32          evtId;      
    Uint32          _yCo;
    Uint32          _xCo;
    
    evtId = (Uint32)(eventId);
    _yCo = evtId >> 5;          /* _yCo co-ordinate */
    _xCo = 1 << (evtId & 0x1f); /* _xCo co-ordinate */
    
    asm(" dint");
    if (!(_CSL_intcAllocMask[_yCo] & _xCo)) {
        _CSL_intcAllocMask[_yCo] |= _xCo; /* set bit -> used */
        hIntc = intcObj;
    }
    asm(" rint");
    
    if (hIntc != (CSL_IntcHandle)CSL_INTC_BADHANDLE) {
        intcObj->eventId    = eventId;
        intcObj->vectId     = *((CSL_IntcVectId*)param);
        openStatus          = CSL_SOK;
        
        /* Do the Mapping for the Event -> CPU Interrupt */
        if (*((CSL_IntcVectId*)param) < ((Uint32)(CSL_INTC_VECTID_COMBINE))) {
            CSL_intcMapEventVector(eventId,*param); 
            if (eventId < 4) {
                switch (eventId) {
                    case 0:
                        CSL_intcHookIsr(*((CSL_IntcVectId*)param), \
                                        _CSL_intcEvent0Dispatcher);
                        break;
                    case 1:
                        CSL_intcHookIsr(*((CSL_IntcVectId*)param), \
                                       _CSL_intcEvent1Dispatcher);
                        break;
                    case 2:
                        CSL_intcHookIsr(*((CSL_IntcVectId*)param), \
                                       _CSL_intcEvent2Dispatcher);
                        break;
                    case 3:
                        CSL_intcHookIsr(*((CSL_IntcVectId*)param), \
                                       _CSL_intcEvent3Dispatcher);
                        break;                  
                }           
            }
            
        }        
    } 
    else 
        openStatus = CSL_ESYS_FAIL;

    if (status) {
        *status = openStatus;
    }
    
    return hIntc;
}
