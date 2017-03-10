/**
 * @file
 * 
 * @brief No-CPU Type Definitions
 * 
 * This include file contains type definitions pertaining to the Intel
 * no_cpu processor family. 
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_TYPES_H
#define _RTEMS_SCORE_TYPES_H

#include <rtems/score/basedefs.h>

#ifndef ASM

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This section defines the basic types for this processor.
 */

/** Type that can store a 32-bit integer or a pointer. */
typedef uintptr_t CPU_Uint32ptr;

/** This defines the type for a priority bit map entry. */
typedef uint16_t Priority_bit_map_Word;

#ifdef __cplusplus
}
#endif

#endif  /* !ASM */

#endif
