/**
 *   @file  pa_osal.h
 *
 *   @brief   
 *      This is the sample OS Adaptation layer which is used by the PA low level
 *      driver. 
 *    
 *   @details The OSAL layer can be ported in either of the following manners to a native OS:
 *
 *      <b> Approach 1: </b>
 *      @n  Use Prebuilt Libraries
 *           - Ensure that the LLD users provide an implementation of all 
 *             Osal_XXX API for their native OS.
 *           - Link the prebuilt libraries with their application.
 *           - Refer to the "test" directory for an example of this
 *       @n <b> Pros: </b>
 *              - Customers can reuse prebuilt TI provided libraries
 *       @n <b> Cons: </b>
 *              - Level of indirection in the API to get to the actual OS call
 *              
 *      <b> Approach 2: </b>
 *      @n  Rebuilt Library 
 *           - Create a copy of this file and modify it to directly 
 *             inline the native OS calls
 *           - Rebuild the PA low level driver library; ensure that the Include 
 *             path points to the directory where the copy of this file 
 *             has been provided.
 *           - Please refer to the "test" directory for an example of this 
 *       @n <b> Pros: </b>
 *              - Optimizations can be done to remove the level of indirection
 *       @n <b> Cons: </b>
 *              - PA LLD Libraries need to be rebuilt by the customer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009-2011 Texas Instruments, Inc.
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
#ifndef _PA_OSAL_H
#define _PA_OSAL_H

/** @addtogroup PA_LLD_OSAL
 @{ */
 
/**********************************************************************
 ************************* Extern Declarations ************************
 **********************************************************************/
extern void Osal_paBeginMemAccess (void* addr, uint32_t size);
extern void Osal_paEndMemAccess   (void* addr, uint32_t size);
extern void Osal_paMtCsEnter (uint32_t *key);
extern void Osal_paMtCsExit (uint32_t key);

/**
 * @brief  This macro is used to alert the application that the PA is
 *         going to access a block of memory. The application must ensure
 *         cache coherency for multi-core applications
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_paBeginMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  The address of the table to be accessed
 *  @n  The number of bytes in the table
 *
 *  @note PA will make nested calls to this function for memory access
 *        protection of different memory tables. 
 */

#define Pa_osalBeginMemAccess   Osal_paBeginMemAccess

/**
 * @brief  This macro is used to alert the application that the PA
 *         has completed access to a block of memory. If the memory block is 
 *         cached then the application would need to ensure that the contents 
 *         of the cache are updated immediately to the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_paEndMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n The address of the table to be accessed
 *  @n The number of bytes in the table
 *
 *  @note PA will make nested calls to this function for memory access
 *        protection of different memory tables. 
 */
 
#define Pa_osalEndMemAccess   Osal_paEndMemAccess

/**
 * @brief   The macro is used by the PA LLD to provide critical section to 
 *          protect its global and shared resources access from multiple cores. 
 *          If the PALLD operates in single-core environment then these macros can 
 *          be defined to be NOP. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_paMtCsEnter (uint32_t *key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the multi-core critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 *
 *  @note The multiple-core protection area is usually pretty small. To avoid the scenario that the
 *        interrupt occurs after the multi-core lock is achieved, the interrupt may be disabled in 
 *        this function and re-enabled at the function Osal_saMtCsExit.
 */
#define Pa_osalMtCsEnter          Osal_paMtCsEnter

/**
 * @brief   The macro is used by the PA LLD to exit a critical section 
 *      protected using the Osal_paMtCsEnter() API.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_paMtCsExit (uint32_t key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the multi-core critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Pa_osalMtCsExit           Osal_paMtCsExit

/**
@}
*/
#endif /* _PA_OSAL_H */

