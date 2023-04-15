/*  C6678_cpu.h
 *
 *  This file sets up basic CPU dependency settings based on
 *  compiler settings.  For example, it can determine if
 *  floating point is available.  This particular implementation
 *  is specified to the NO CPU port.
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 */

#ifndef _RTEMS_SCORE_C6678_CPU_H
#define _RTEMS_SCORE_C6678_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

//#define RTEMS_SCORE_OBJECT_ENABLE_STRING_NAMES 1

/*
 *  This file contains the information required to build
 *  RTEMS for a particular member of the NO CPU family.
 *  It does this by setting variables to indicate which
 *  implementation dependent features are present in a particular
 *  member of the family.
 *
 *  This is a good place to list all the known CPU models
 *  that this port supports and which RTEMS CPU model they correspond
 *  to.
 */
#define no_cpu 1
#if defined(rtems_multilib)
/*
 *  Figure out all CPU Model Feature Flags based upon compiler
 *  predefines.
 */

#define CPU_MODEL_NAME  "rtems_multilib"
#define NOCPU_HAS_FPU     1

#elif defined(no_cpu)

#define CPU_MODEL_NAME  "C66_model"
#define NOCPU_HAS_FPU     0

#else

#error "Unsupported CPU Model"

#endif

/*
 *  Define the name of the CPU family.
 */

#define CPU_NAME "C6678"

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_SCORE_NO_CPU_H */
