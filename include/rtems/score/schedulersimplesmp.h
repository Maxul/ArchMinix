/**
 * @file
 *
 * @brief Simple SMP Scheduler API
 *
 * @ingroup ScoreSchedulerSMPSimple
 */

/*
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2013 embedded brains GmbH.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_SCHEDULERSIMPLE_SMP_H
#define _RTEMS_SCORE_SCHEDULERSIMPLE_SMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/scheduler.h>
#include <rtems/score/schedulerpriority.h>
#include <rtems/score/schedulersmp.h>

/**
 * @defgroup ScoreSchedulerSMPSimple Simple Priority SMP Scheduler
 *
 * @ingroup ScoreSchedulerSMP
 *
 * The Simple Priority SMP Scheduler allocates a processor for the processor
 * count highest priority ready threads.  The thread priority and position in
 * the ready chain are the only information to determine the scheduling
 * decision.  Threads with an allocated processor are in the scheduled chain.
 * After initialization the scheduled chain has exactly processor count nodes.
 * Each processor has exactly one allocated thread after initialization.  All
 * enqueue and extract operations may exchange threads with the scheduled
 * chain.  One thread will be added and another will be removed.  The scheduled
 * and ready chain is ordered according to the thread priority order.  The
 * chain insert operations are O(count of ready threads), thus this scheduler
 * is unsuitable for most real-time applications.
 *
 * The thread preempt mode will be ignored.
 *
 * @{
 */

typedef struct {
  Scheduler_SMP_Context Base;
  Chain_Control         Ready;
} Scheduler_simple_SMP_Context;

/**
 * @brief Entry points for the Simple SMP Scheduler.
 */
#define SCHEDULER_SIMPLE_SMP_ENTRY_POINTS \
  { \
    _Scheduler_simple_SMP_Initialize, \
    _Scheduler_default_Schedule, \
    _Scheduler_simple_SMP_Yield, \
    _Scheduler_simple_SMP_Block, \
    _Scheduler_simple_SMP_Unblock, \
    _Scheduler_simple_SMP_Change_priority, \
    _Scheduler_simple_SMP_Ask_for_help, \
    _Scheduler_simple_SMP_Node_initialize, \
    _Scheduler_default_Node_destroy, \
    _Scheduler_simple_SMP_Update_priority, \
    _Scheduler_priority_Priority_compare, \
    _Scheduler_default_Release_job, \
    _Scheduler_default_Tick, \
    _Scheduler_SMP_Start_idle \
    SCHEDULER_OPERATION_DEFAULT_GET_SET_AFFINITY \
  }

void _Scheduler_simple_SMP_Initialize( const Scheduler_Control *scheduler );

void _Scheduler_simple_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

void _Scheduler_simple_SMP_Block(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
);

Thread_Control *_Scheduler_simple_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
);

Thread_Control *_Scheduler_simple_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Priority_Control         new_priority,
  bool                     prepend_it
);

Thread_Control *_Scheduler_simple_SMP_Ask_for_help(
  const Scheduler_Control *scheduler,
  Thread_Control          *offers_help,
  Thread_Control          *needs_help
);

void _Scheduler_simple_SMP_Update_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Priority_Control         new_priority
);

Thread_Control *_Scheduler_simple_SMP_Yield(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
