/**
 *   @file  csl_tmrInit.c
 *
 *   @brief   
 *      The file implements the Timer Init API which is used to initialize and
 *      provide context for the CSL Timer Module.
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

#include <ti/csl/csl_tmr.h>

/** ============================================================================
 *   @n@b CSL_tmrInit
 *
 *   @b Description
 *   @n This is the initialization function for the General purpose timer CSL.
 *      The function must be called before calling any other API from this CSL.
 *      This function is idem-potent. Currently, the function just return
 *      status CSL_SOK, without doing anything.
 *
 *   @b Arguments
 *   @verbatim
        pContext    Pointer to module-context. As General purpose timer doesn't
                    have any context based information user is expected to pass
                    NULL.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Always returns Success - CSL_SOK
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  The CSL for gptimer is initialized
 *
 *   @b Writes
 *   @n  None
 *
 *   @b Example
 *   @verbatim
            CSL_TmrContext  context;

            // Initialize the Timer Module.
            CSL_tmrInit(&context);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION(CSL_tmrInit, ".text:csl_section:tmr");
CSL_Status CSL_tmrInit (CSL_TmrContext *pContext)
{
    return CSL_SOK;
}

