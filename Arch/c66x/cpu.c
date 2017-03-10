/**
 *  @file
 *
 *  @brief No CPU Dependent Source
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/wkspace.h>

#ifdef __TI_EABI__
extern int __TI_STATIC_BASE;
#else
extern int __bss__;
#endif

#define C6X_BASE_ST   0x0

extern unsigned int Normally_Enabled_Interrupts;

void _CPU_Initialize(void)
{
  /*
   *  If there is not an easy way to initialize the FP context
   *  during Context_Initialize, then it is usually easier to
   *  save an "uninitialized" FP context here and copy it to
   *  the task's during Context_Initialize.
   */

  /* FP context initialization support goes here */
}

#if 0
uint32_t   _CPU_ISR_Get_level( void )
{
  /*
   *  This routine returns the current interrupt level.
   */

  return 0;
}
#endif

void _CPU_Context_Initialize(
    Context_Control *_the_context,
    void *_stack_base,
    uint32_t _size,
    uint32_t _isr,
    void (*_entry_point)(void),
	int _is_fp,
    void *_tls_area
)
{
  unsigned int *_stack;

  _stack = (unsigned int *)(((unsigned int)_stack_base + (unsigned int)_size - 16) & ~7);

  _the_context->REG_B3 = (unsigned int) _entry_point;
#ifdef __TI_EABI__
  _the_context->REG_B14 = (unsigned int) &__TI_STATIC_BASE;
#else
  _the_context->REG_B14 = &__bss__;
#endif
  _the_context->AMR = 0x0;

  _the_context->IER = Normally_Enabled_Interrupts;
  _the_context->REG_B15 = (unsigned int) _stack;
  _the_context->CSR = C6X_BASE_ST;

  if ( _isr == 0 )
  _the_context->CSR |= (C6X_CSR_GIE | C6X_CSR_PGIE);
}

/*
 *  _CPU_ISR_install_raw_handler
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */

void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
  /*
   *  This is where we install the interrupt handler into the "raw" interrupt
   *  table used by the CPU to dispatch interrupt handlers.
   */
}

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
   *old_handler = _ISR_Vector_table[ vector ];

   /*
    *  If the interrupt vector table is a table of pointer to isr entry
    *  points, then we need to install the appropriate RTEMS interrupt
    *  handler for this vector number.
    */

   _CPU_ISR_install_raw_handler( vector, new_handler, old_handler );

   /*
    *  We put the actual user ISR address in '_ISR_vector_table'.  This will
    *  be used by the _ISR_Handler so the user gets control.
    */

    _ISR_Vector_table[ vector ] = new_handler;
}

/*
 *  _CPU_Install_interrupt_stack
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */

void _CPU_Install_interrupt_stack( void )
{
}

/*
 *  _CPU_Thread_Idle_body
 *
 *  NOTES:
 *
 *  1. This is the same as the regular CPU independent algorithm.
 *
 *  2. If you implement this using a "halt", "idle", or "shutdown"
 *     instruction, then don't forget to put it in an infinite loop.
 *
 *  3. Be warned. Some processors with onboard DMA have been known
 *     to stop the DMA if the CPU were put in IDLE mode.  This might
 *     also be a problem with other on-chip peripherals.  So use this
 *     hook with caution.
 *
 *  NO_CPU Specific Information:
 *
 *  XXX document implementation including references if appropriate
 */

void *_CPU_Thread_Idle_body( uintptr_t ignored )
{

  for( ; ; ) {
    __asm( " idle" );
    __asm( " nop" );
    __asm( " nop" );
    __asm( " nop" );
  }
    /* insert your "halt" instruction here */ ;
}
