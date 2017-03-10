#include <rtems/system.h>
#include <rtems/score/cpu.h>
#include <rtems/score/percpu.h>
#include <rtems/score/threaddispatch.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>

#include <ti/csl/csl_xmcAux.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_cache.h>

extern volatile cregister unsigned int DNUM;

/*
 *  This routine provides the RTOS interrupt management.
 */

#if( CPU_HAS_SOFTWARE_INTERRUPT_STACK == TRUE)
unsigned int    *_old_stack_ptr;
#endif

/*
register unsigned long  *stack_ptr asm("sp");
*/

/*register*/
unsigned int  *stack_ptr; /* __asm(" B15"); */

int intr_disable_flag;

#pragma DATA_SECTION(intr_occur_count, ".msmc_data");
int intr_occur_count[8];

extern void Shm_isr(void);

void __ISR_Handler(uint32_t vector, CPU_Interrupt_frame *pContext)
{
  ISR_Level level;

  _ISR_Disable( level );

  _Thread_Dispatch_disable();

#if (CPU_HAS_SOFTWARE_INTERRUPT_STACK == TRUE)
  if ( _ISR_Nest_level == 0 )
    {
      /* Install irq stack */
      _old_stack_ptr = stack_ptr;
      stack_ptr = _CPU_Interrupt_stack_high;
    }

#endif

  _ISR_Nest_level++;

  _ISR_Enable( level );

  /* call isp */
  if ( _ISR_Vector_table[ vector])
    (*_ISR_Vector_table[ vector ])( vector,pContext );

  _ISR_Disable( level );

  _Thread_Unnest_dispatch( /*_Per_CPU_Get()*/ );

  _ISR_Nest_level--;

#if(CPU_HAS_SOFTWARE_INTERRUPT_STACK == TRUE)

  if ( _ISR_Nest_level == 0 )
    /* restore old stack pointer */
    stack_ptr = _old_stack_ptr;
#endif

  _ISR_Enable( level );

  if ( _ISR_Nest_level )
    return;

  if ( !_Thread_Dispatch_is_enabled() ) {
    return;
  }

  if ( _Thread_Dispatch_necessary ) {
    _Thread_Dispatch();
  }
}
