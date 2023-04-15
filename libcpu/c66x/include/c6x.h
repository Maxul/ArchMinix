

#ifndef _INCLUDE_C6X_h
#define _INCLUDE_C6X_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/*
 *  This file contains the information required to build
 *  RTOS for a particular member of the "no cpu"
 *  family when executing in protected mode.  It does
 *  this by setting variables to indicate which implementation
 *  dependent features are present in a particular member
 *  of the family.
 */

#if defined(_C62)
#define CPU_MODEL_NAME  "C62"

#elif defined(_C64)
#define CPU_MODEL_NAME  "C64"

#elif defined(_C67)
#define CPU_MODEL_NAME  "C67"

#elif defined(_C66)
#define CPU_MODEL_NAME  "C66"

#else

#error "Unsupported CPU Model"

#endif

/*
 *  Define the name of the CPU family.
 */

#define CPU_NAME "Texas Instruments C6X"

/*
 *  This port is a little unusual in that even though there are "floating
 *  point registers", the notion of floating point is very inherent to
 *  applications.  In addition, the calling conventions require that
 *  only a few extended registers be preserved across subroutine calls.
 *  The overhead of including these few registers in the basic
 *  context is small compared to the overhead of managing the notion
 *  of separate floating point contexts.  So we decided to pretend that
 *  there is no FPU on the C3x or C4x.
 */

#define C6X_HAS_FPU  0

/*
 *  Routines to manipulate the bits in the Status Word (ST).
 */

#define C6X_ST_C      0x0001
#define C6X_ST_V      0x0002
#define C6X_ST_Z      0x0004
#define C6X_ST_N      0x0008
#define C6X_ST_UF     0x0010
#define C6X_ST_LV     0x0020
#define C6X_ST_LUF    0x0040
#define C6X_ST_OVM    0x0080
#define C6X_ST_RM     0x0100
#define C6X_ST_CF     0x0400
#define C6X_ST_CE     0x0800
#define C6X_ST_CC     0x1000
#define C6X_ST_GIE    0x2000

#define C6X_CSR_GIE    0x00000001
#define C6X_CSR_PGIE   0x00000002

#ifndef ASM

/*
 *  A nop macro.
 */

#define c6x_nop() \
  __asm(" nop");      /* zhou */


extern unsigned int GetCSR(void);   /* zhou */
extern void SetCSRBit(int bit_value);
extern void CleanCSRBit(int bit_value);
extern cregister volatile unsigned int ISTP; 	/* Interrupt Service Tbl Ptr */



/*
 *  Routines to set and clear individual bits in the ST (status word).
 *
 *  cpu_st_bit_clear  - clear bit in ST
 *  cpu_st_bit_set    - set bit in ST
 *  cpu_st_get        - obtain entire ST
 */

#define c6x_gie_nop() { c6x_nop(); c6x_nop(); }

#define cpu_st_bit_clear(_st_bit) \
  do { \
    CleanCSRBit(~_st_bit); \
    c6x_gie_nop(); \
  } while (0)

#define cpu_st_bit_set(_st_bit) \
  do { \
    SetCSRBit(_st_bit); \
    c6x_gie_nop(); \
  } while (0)

static inline unsigned int cpu_st_get(void)
{
    register unsigned int st_value;

    st_value = GetCSR();
    return st_value;
}

/*
 *  Routines to manipulate the Global Interrupt Enable (GIE) bit in
 *  the Status Word (ST).
 *
 *  c6x_global_interrupts_get      - returns current GIE setting
 *  c6x_global_interrupts_disable  - disables global interrupts
 *  c6x_global_interrupts_enable   - enables global interrupts
 *  c6x_global_interrupts_restore  - restores GIE to pre-disable state
 *  c6x_global_interrupts_flash    - temporarily enable global interrupts
 */

#define c6x_global_interrupts_get() \
  (cpu_st_get() & C6X_CSR_GIE)

#ifdef _C67
#define c6x_global_interrupts_disable() \
  cpu_st_bit_clear(C6X_CSR_GIE)
#endif

#ifdef _C66
#define c6x_global_interrupts_disable() \
	__asm("   DINT")
#endif

#define c6x_global_interrupts_enable() \
  cpu_st_bit_set(C6X_CSR_GIE)

#define c6x_global_interrupts_restore(_old_level) \
  cpu_st_bit_set(_old_level)

#define c6x_global_interrupts_flash(_old_level) \
  do { \
    cpu_st_bit_set(_old_level); \
    c6x_global_interrupts_disable(); \
  } while (0)

/* end of C6x specific interrupt flag routines */

/*
 *  Routines to access the Interrupt Trap Table Pointer
 *
 *  c6x_get_ittp    - get ITTP
 *  c6x_set_ittp    - set ITTP
 */

static inline void *c6x_get_ittp(void)
{
#if 0
    register unsigned int _if_value;

    __asm__( "ldi if, %0" : "=r" (_if_value) );
    return (void *)((_if_value & 0xffff0000) >> 8);
#endif
    /*	return (void *)0x809fc0;
    *    changed by ww,20130415
    */
    return (void *)ISTP;
}

static inline void c6x_set_ittp(void *_ittp_value)
{
	ISTP = (unsigned int)_ittp_value;
}

#endif /* ifndef ASM */

#ifdef __cplusplus
}
#endif

#endif /* ! _INCLUDE_C6X_h */
/* end of include file */
