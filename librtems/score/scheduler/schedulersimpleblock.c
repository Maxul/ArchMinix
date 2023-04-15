/**
 * @file
 *
 * @brief Simple Scheduler Block a Thread
 *
 * @ingroup ScoreScheduler
 */

/*
 *  COPYRIGHT (c) 2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulersimpleimpl.h>

void _Scheduler_simple_Block(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  _Scheduler_Generic_block(
    scheduler,
    the_thread,
    _Scheduler_simple_Extract,
    _Scheduler_simple_Schedule_body
  );
}
