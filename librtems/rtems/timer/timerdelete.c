/**
 *  @file
 *
 *  @brief RTEMS Delete Timer
 *  @ingroup ClassicTimer
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/timerimpl.h>
#include <rtems/score/watchdogimpl.h>

rtems_status_code rtems_timer_delete(
  rtems_id id
)
{
  Timer_Control     *the_timer;
  Objects_Locations  location;

  _Objects_Allocator_lock();
  the_timer = _Timer_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      _Objects_Close( &_Timer_Information, &the_timer->Object );
      _Timer_Cancel( the_timer );
      _Objects_Put( &the_timer->Object );
      _Timer_Free( the_timer );
      _Objects_Allocator_unlock();
      return RTEMS_SUCCESSFUL;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:            /* should never return this */
#endif
    case OBJECTS_ERROR:
      break;
  }

  _Objects_Allocator_unlock();

  return RTEMS_INVALID_ID;
}
