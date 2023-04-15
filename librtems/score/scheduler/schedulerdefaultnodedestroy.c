/**
 * @file
 *
 * @brief Scheduler Default Node Destruction Operation
 *
 * @ingroup ScoreScheduler
 */

/*
 *  COPYRIGHT (c) 2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/scheduler.h>

void _Scheduler_default_Node_destroy(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  (void) scheduler;
  (void) the_thread;
}
