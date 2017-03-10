/**
 * @file
 *
 * @brief Unblocks a Thread from the Queue
 *
 * @ingroup ScoreScheduler
 */

/*
 *  Copyright (C) 2011 Petr Benes.
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulercbsimpl.h>
#include <rtems/score/scheduleredfimpl.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/watchdogimpl.h>

Scheduler_Void_or_thread _Scheduler_CBS_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  Scheduler_CBS_Node   *node = _Scheduler_CBS_Thread_get_node( the_thread );
  Scheduler_CBS_Server *serv_info = node->cbs_server;
  Priority_Control      new_priority;

  _Scheduler_EDF_Enqueue( scheduler, the_thread );
  /* TODO: flash critical section? */

  /*
   * Late unblock rule for deadline-driven tasks. The remaining time to
   * deadline must be sufficient to serve the remaining computation time
   * without increased utilization of this task. It might cause a deadline
   * miss of another task.
   */
  if (serv_info) {
    time_t deadline = serv_info->parameters.deadline;
    time_t budget = serv_info->parameters.budget;
    time_t deadline_left = the_thread->cpu_time_budget;
    time_t budget_left = the_thread->real_priority -
                           _Watchdog_Ticks_since_boot;

    if ( deadline*budget_left > budget*deadline_left ) {
      /* Put late unblocked task to background until the end of period. */
      new_priority = the_thread->Start.initial_priority;
      the_thread->real_priority = new_priority;
      if ( the_thread->current_priority != new_priority ) {
        the_thread->current_priority = new_priority;
        _Scheduler_Change_priority(the_thread, new_priority, true);
      }
    }
  }

  /*
   *  If the thread that was unblocked is more important than the heir,
   *  then we have a new heir.  This may or may not result in a
   *  context switch.
   *
   *  Normal case:
   *    If the current thread is preemptible, then we need to do
   *    a context switch.
   *  Pseudo-ISR case:
   *    Even if the thread isn't preemptible, if the new heir is
   *    a pseudo-ISR system task, we need to do a context switch.
   */
  if (
    _Scheduler_Is_priority_higher_than(
       scheduler,
       the_thread->current_priority,
       _Thread_Heir->current_priority
    )
  ) {
    _Scheduler_Update_heir(
      the_thread,
      the_thread->current_priority == PRIORITY_PSEUDO_ISR
    );
  }

  SCHEDULER_RETURN_VOID_OR_NULL;
}
