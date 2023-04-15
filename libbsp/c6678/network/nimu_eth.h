/*
 * nimu_eth.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *   @file  nimu.h
 *
 *   @brief
 *      This file defines the interface applications can use when
 *      integrating the Network Interface Module Ethernet driver adaptation layer
 *
 */

#ifndef _NIMU_ETH_H
#define _NIMU_ETH_H /* #defined if this .h file has been included */

/* NIMU Library Version */
#define NIMU_LIB_VERSION "1.00.00.08"

/**
 * ============================================================================
 *  @n@b Osal_nimuMalloc
 *
 *  @b  brief
 *  @n  This routine implements the memory handler for the NIMU library. The API
 *      allocates a memory block of a given size specified by input parameter 'num_bytes'.
 *
 *      This API should allocate memory from shared memory.
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @return
 *      Allocated block address
 * =============================================================================
 */
Ptr Osal_nimuMalloc(uint32_t num_bytes, uint32_t alignment);

/**
 * ============================================================================
 *  @n@b Osal_nimuFree
 *
 *  @b  brief
 *  @n  This API frees and restores a given memory location
 *      pointer 'dataPtr' of size 'num_bytes' to its
 *      original heap location. Frees up memory allocated using
 *      @a Osal_nimuMalloc ()
 *
 *  @param[in]  dataPtr
 *      Pointer to the memory block to be cleaned up.
 *
 *  @param[in]  num_bytes
 *      Size of the memory block to be cleaned up.
 *
 *  @return
 *      Not Applicable
 * =============================================================================
 */
void Osal_nimuFree(Ptr dataPtr, uint32_t num_bytes);

#include "c6678_emac_types.h"
#include "etherif.h"
#include <net/ethernet.h> // ETHERTYPE_*

#endif
