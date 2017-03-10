/**
 * @file
 *
 * @brief System Generates Signal for process after realtime seconds elapsed
 * @ingroup POSIXAPI
 */

/*
 *  3.4.1 Schedule Alarm, P1003.1b-1993, p. 79
 */

/*  COPYRIGHT (c) 1989-2013.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <rtems/posix/pthreadimpl.h>
#include <rtems/posix/psignalimpl.h>
#include <rtems/score/threaddispatch.h>
#include <rtems/score/todimpl.h>
#include <rtems/score/watchdogimpl.h>

/*
 *  _POSIX_signals_Alarm_TSR
 */
static void _POSIX_signals_Alarm_TSR(
  Objects_Id      id __attribute__((unused)),
  void           *argument __attribute__((unused))
)
{
  #if defined(RTEMS_DEBUG)
    int status;
    #define KILL_STATUS status =
  #else
    #define KILL_STATUS (void)
  #endif

  KILL_STATUS kill( getpid(), SIGALRM );

  #if defined(RTEMS_DEBUG)
    /*
     *  There is no reason to think this might fail but we should be
     *  cautious.
     */
    _Assert(status == 0);
  #endif
}

static Watchdog_Control _POSIX_signals_Alarm_timer = WATCHDOG_INITIALIZER(
  _POSIX_signals_Alarm_TSR,
  0,
  NULL
);

unsigned int alarm(
  unsigned int seconds
)
{
  unsigned int      remaining = 0;
  Watchdog_Control *the_timer;
  Watchdog_States   state;

  the_timer = &_POSIX_signals_Alarm_timer;

  _Thread_Disable_dispatch();

  state = _Watchdog_Remove_seconds( the_timer );
  if ( state == WATCHDOG_ACTIVE ) {
    /*
     *  The stop_time and start_time fields are snapshots of ticks since
     *  boot.  Since alarm() is dealing in seconds, we must account for
     *  this.
     */

    remaining = the_timer->initial -
      ((the_timer->stop_time - the_timer->start_time) / TOD_TICKS_PER_SECOND);
  }

  if ( seconds )
    _Watchdog_Insert_seconds( the_timer, seconds );

  _Thread_Enable_dispatch();

  return remaining;
}
