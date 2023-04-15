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
 *  @file  csl_intcHwControl.c
 *
 *  @brief File for functional layer of CSL API CSL_intcHwControl()
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

#pragma CODE_SECTION (CSL_intcHwControl, ".text:csl_section:intc");
CSL_Status  CSL_intcHwControl(
    CSL_IntcHandle            hIntc,
    CSL_IntcHwControlCmd      controlCommand,
    void*                     commandArg
)

{
    CSL_Status status = CSL_SOK;
    
    if (hIntc == NULL)
        return CSL_ESYS_BADHANDLE;
        
    switch(controlCommand) {
        case CSL_INTC_CMD_EVTENABLE: 
            if (hIntc->vectId < CSL_INTC_VECTID_EXCEP) {
                if (hIntc->vectId < CSL_INTC_VECTID_COMBINE) {
                    if (hIntc->eventId < 4) {
                        if (commandArg)
                            CSL_intcCombinedEventEnable(hIntc->eventId,\
                                                  *(CSL_BitMask32*)commandArg);
                    }
                    CSL_intcInterruptEnable(hIntc->vectId);             
                    CSL_intcEventDisable(hIntc->eventId);                   
                }       
                else 
                    CSL_intcEventEnable(hIntc->eventId);                    
            } 
            else          
                CSL_intcExcepEnable(hIntc->eventId);
            break;
        
        case CSL_INTC_CMD_EVTDISABLE:
            if (hIntc->vectId < CSL_INTC_VECTID_EXCEP) {
                if (hIntc->vectId < CSL_INTC_VECTID_COMBINE) {
                    if (hIntc->eventId < 4) {
                        if (commandArg)
                            CSL_intcCombinedEventDisable(hIntc->eventId, \
                                                  *(CSL_BitMask32*)commandArg);
                    }
                    CSL_intcInterruptDisable(hIntc->vectId);                
                }       
                else 
                    CSL_intcEventDisable(hIntc->eventId);
            }
            else          
                CSL_intcExcepDisable(hIntc->eventId);
            break;
            
        case CSL_INTC_CMD_EVTSET:
            CSL_intcEventSet(hIntc->eventId);               
            break;
                
        case CSL_INTC_CMD_EVTCLEAR:  
            if (hIntc->vectId < CSL_INTC_VECTID_EXCEP) {
                if (hIntc->eventId < 4) {
                    if (commandArg)
                        CSL_intcCombinedEventClear(hIntc->eventId, \
                                                  *(CSL_BitMask32*)commandArg);
                } 
                else                    
                    CSL_intcEventClear(hIntc->eventId);
            } 
            else          
                CSL_intcExcepClear(hIntc->eventId);

            break;
        
        case CSL_INTC_CMD_EVTDROPENABLE:  
            if (hIntc->vectId < CSL_INTC_VECTID_COMBINE)
                CSL_intcInterruptDropEnable(1 << hIntc->vectId); 
            else 
                status = CSL_ESYS_INVCMD;
            break;
        
        case CSL_INTC_CMD_EVTDROPDISABLE:   
            if (hIntc->vectId < CSL_INTC_VECTID_COMBINE)
                CSL_intcInterruptDropDisable(1 << hIntc->vectId); 
            else 
                status = CSL_ESYS_INVCMD;   
            break;
        case CSL_INTC_CMD_EVTINVOKEFUNCTION:    
            CSL_intcInvokeEventHandle (hIntc->eventId);
            break;
        default:
            status = CSL_ESYS_INVCMD;
            break;

    }
    return status;
}
