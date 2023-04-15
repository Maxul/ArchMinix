/**
 *   @file  csl_cpIntcGetBaseAddress.c
 *
 *   @brief   
 *      The file contains routines which are CPINTC Device specific
 *      and need to be modified for each new device which has the 
 *      CPINTC IP module. 
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2008, 2009, Texas Instruments, Inc.
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

#include <ti/csl/soc.h>
#include <ti/csl/csl_cpIntc.h>

/** ============================================================================
 *   @n@b CSL_CPINTC_open
 *
 *   @b Description
 *   @n This API opens the interrupt controller instance. This should always
 *      be the first call to the CSL CPINTC Functional layer. The handle 
 *      which is returned from this call should be passed in all subsequent
 *      CSL API's.
 *      
 *   <b> Return Value </b> 
 *   @n  CSL_CPINTC_HANDLE  - Handle to the CPINTC Module
 *   @n  0                  - Error. 
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None 
 *    
 *   @b  Affects
 *   @n  None.
 *
 *   @b  Example
 *   @verbatim
        CSL_CPINTC_HANDLE   hnd;
        ...
        hnd = CSL_CPINTC_open (0);    // Opens CPINTC Instance 0
        ...
     @endverbatim
 * ===========================================================================
 */
CSL_CPINTC_Handle CSL_CPINTC_open (Int32 instNum)
{
    /* Return the handle matching the instance number which is being opened. */
    switch (instNum)
    {
        case CSL_CP_INTC_0:
            return (CSL_CPINTC_Handle)CSL_CP_INTC_0_REGS;
        case CSL_CP_INTC_1:
            return (CSL_CPINTC_Handle)CSL_CP_INTC_1_REGS;
        case CSL_CP_INTC_2:
            return (CSL_CPINTC_Handle)CSL_CP_INTC_2_REGS;
        case CSL_CP_INTC_3:
            return (CSL_CPINTC_Handle)CSL_CP_INTC_3_REGS;            
    }
    
    /* Control comes here implies that an invalid instance number was passed */
    return (CSL_CPINTC_Handle)0;
}
