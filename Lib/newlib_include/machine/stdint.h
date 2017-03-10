/*
 *
 */
#ifndef _MACHINE_STDINT_H
#define _MACHINE_STDINT_H

#ifndef _STDINT_H
#error machine/stdint.h is an internal file and  must not be directly included
#endif

#if defined(__sparc__) \
  || defined(__powerpc__) || defined(__PPC__) \
  || defined(__mips__) \
  || defined(__sh__) \
  || defined(__AVR__) \
  || defined(_C4x) || defined(_C3x) \
  || defined(__H8300__)
/* PTRDIFF_TYPE = int */
#define __ptrint_t_int_defined 1
#endif

#if defined(__i386__) \
  || defined(__m68k__) \
  || defined(__bfin__) \
  || defined(__arm__) \
  || defined(__H8300S__) || defined(__H8300H__)
/* PTRDIFF_TYPE = long */
#define __ptrint_t_long_defined 1
#endif

#endif
