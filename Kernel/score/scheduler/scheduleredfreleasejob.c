/**
 * @file
 *
 * @brief Scheduler EDF Release Job
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

#include <rtems/score/scheduleredf.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/watchdogimpl.h>

void _Scheduler_EDF_Release_job(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  uint32_t                 deadline
)
{
  Priority_Control new_priority;
  Priority_Control unused;

  (void) scheduler;

  if (deadline) {
    /* Initializing or shifting deadline. */
    new_priority = (_Watchdog_Ticks_since_boot + deadline)
                   & ~SCHEDULER_EDF_PRIO_MSB;
  }
  else {
    /* Switch back to background priority. */
    new_priority = the_thread->Start.initial_priority;
  }

  _Thread_Set_priority( the_thread, new_priority, &unused, true );
}
