/**
 *  @file
 *
 *  @ingroup shared_coverhd
 *
 *  @brief This include file has defines to represent the overhead associated
 *  with calling a particular directive from C on this target.
 */

/* coverhd.h
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef __COVERHD_h
#define __COVERHD_h

#ifdef __cplusplus
extern "C" {
#endif

#define CALLING_OVERHEAD_INITIALIZE_EXECUTIVE      0
#define CALLING_OVERHEAD_SHUTDOWN_EXECUTIVE        0
#define CALLING_OVERHEAD_TASK_CREATE               0
#define CALLING_OVERHEAD_TASK_IDENT                0
#define CALLING_OVERHEAD_TASK_START                0
#define CALLING_OVERHEAD_TASK_RESTART              0
#define CALLING_OVERHEAD_TASK_DELETE               0
#define CALLING_OVERHEAD_TASK_SUSPEND              0
#define CALLING_OVERHEAD_TASK_RESUME               0
#define CALLING_OVERHEAD_TASK_SET_PRIORITY         0
#define CALLING_OVERHEAD_TASK_MODE                 0
#define CALLING_OVERHEAD_TASK_GET_NOTE             0
#define CALLING_OVERHEAD_TASK_SET_NOTE             0
#define CALLING_OVERHEAD_TASK_WAKE_WHEN            0
#define CALLING_OVERHEAD_TASK_WAKE_AFTER           0
#define CALLING_OVERHEAD_INTERRUPT_CATCH           0
#define CALLING_OVERHEAD_CLOCK_GET                 0
#define CALLING_OVERHEAD_CLOCK_SET                 0
#define CALLING_OVERHEAD_CLOCK_TICK                0

#define CALLING_OVERHEAD_TIMER_CREATE              0
#define CALLING_OVERHEAD_TIMER_IDENT               0
#define CALLING_OVERHEAD_TIMER_DELETE              0
#define CALLING_OVERHEAD_TIMER_FIRE_AFTER          0
#define CALLING_OVERHEAD_TIMER_FIRE_WHEN           0
#define CALLING_OVERHEAD_TIMER_RESET               0
#define CALLING_OVERHEAD_TIMER_CANCEL              0
#define CALLING_OVERHEAD_SEMAPHORE_CREATE          0
#define CALLING_OVERHEAD_SEMAPHORE_DELETE          0
#define CALLING_OVERHEAD_SEMAPHORE_IDENT           0
#define CALLING_OVERHEAD_SEMAPHORE_OBTAIN          0
#define CALLING_OVERHEAD_SEMAPHORE_RELEASE         0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_CREATE      0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_IDENT       0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_DELETE      0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_SEND        0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_URGENT      0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_BROADCAST   0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_RECEIVE     0
#define CALLING_OVERHEAD_MESSAGE_QUEUE_FLUSH       0

#define CALLING_OVERHEAD_EVENT_SEND                0
#define CALLING_OVERHEAD_EVENT_RECEIVE             0
#define CALLING_OVERHEAD_SIGNAL_CATCH              0
#define CALLING_OVERHEAD_SIGNAL_SEND               0
#define CALLING_OVERHEAD_PARTITION_CREATE          0
#define CALLING_OVERHEAD_PARTITION_IDENT           0
#define CALLING_OVERHEAD_PARTITION_DELETE          0
#define CALLING_OVERHEAD_PARTITION_GET_BUFFER      0
#define CALLING_OVERHEAD_PARTITION_RETURN_BUFFER   0
#define CALLING_OVERHEAD_REGION_CREATE             0
#define CALLING_OVERHEAD_REGION_IDENT              0
#define CALLING_OVERHEAD_REGION_DELETE             0
#define CALLING_OVERHEAD_REGION_GET_SEGMENT        0
#define CALLING_OVERHEAD_REGION_RETURN_SEGMENT     0
#define CALLING_OVERHEAD_PORT_CREATE               0
#define CALLING_OVERHEAD_PORT_IDENT                0
#define CALLING_OVERHEAD_PORT_DELETE               0
#define CALLING_OVERHEAD_PORT_EXTERNAL_TO_INTERNAL 0
#define CALLING_OVERHEAD_PORT_INTERNAL_TO_EXTERNAL 0

#define CALLING_OVERHEAD_IO_INITIALIZE             0
#define CALLING_OVERHEAD_IO_OPEN                   0
#define CALLING_OVERHEAD_IO_CLOSE                  0
#define CALLING_OVERHEAD_IO_READ                   0
#define CALLING_OVERHEAD_IO_WRITE                  0
#define CALLING_OVERHEAD_IO_CONTROL                0
#define CALLING_OVERHEAD_FATAL_ERROR_OCCURRED      0
#define CALLING_OVERHEAD_RATE_MONOTONIC_CREATE     0
#define CALLING_OVERHEAD_RATE_MONOTONIC_IDENT      0
#define CALLING_OVERHEAD_RATE_MONOTONIC_DELETE     0
#define CALLING_OVERHEAD_RATE_MONOTONIC_CANCEL     0
#define CALLING_OVERHEAD_RATE_MONOTONIC_PERIOD     0
#define CALLING_OVERHEAD_MULTIPROCESSING_ANNOUNCE  0

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
