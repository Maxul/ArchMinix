/**
 * @file
 *
 * @brief Scheduler CBS Release Job
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
#include <rtems/score/threadimpl.h>
#include <rtems/score/watchdogimpl.h>

void _Scheduler_CBS_Release_job(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  uint32_t                 deadline
)
{
  Scheduler_CBS_Node   *node = _Scheduler_CBS_Thread_get_node( the_thread );
  Scheduler_CBS_Server *serv_info = node->cbs_server;
  Priority_Control      new_priority;
  Priority_Control      unused;

  if (deadline) {
    /* Initializing or shifting deadline. */
    if (serv_info)
      new_priority = (_Watchdog_Ticks_since_boot + serv_info->parameters.deadline)
        & ~SCHEDULER_EDF_PRIO_MSB;
    else
      new_priority = (_Watchdog_Ticks_since_boot + deadline)
        & ~SCHEDULER_EDF_PRIO_MSB;
  }
  else {
    /* Switch back to background priority. */
    new_priority = the_thread->Start.initial_priority;
  }

  /* Budget replenishment for the next job. */
  if (serv_info)
    the_thread->cpu_time_budget = serv_info->parameters.budget;

  _Thread_Set_priority( the_thread, new_priority, &unused, true );
}
