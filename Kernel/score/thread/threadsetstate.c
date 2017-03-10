/**
 * @file
 *
 * @brief Sets States for a Thread
 *
 * @ingroup ScoreThread
 */

/*
 *  Thread Handler / Thread Set State
 *
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/threadimpl.h>
#include <rtems/score/assert.h>
#include <rtems/score/schedulerimpl.h>

States_Control _Thread_Set_state(
  Thread_Control *the_thread,
  States_Control  state
)
{
  ISR_lock_Context lock_context;
  States_Control   previous_state;
  States_Control   next_state;

  _Assert( state != 0 );

  _Scheduler_Acquire( the_thread, &lock_context );

  previous_state = the_thread->current_state;
  next_state = _States_Set( state, previous_state);
  the_thread->current_state = next_state;

  if ( _States_Is_ready( previous_state ) ) {
    _Scheduler_Block( the_thread );
  }

  _Scheduler_Release( the_thread, &lock_context );

  return previous_state;
}
