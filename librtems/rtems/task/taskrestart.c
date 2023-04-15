/**
 *  @file
 *
 *  @brief RTEMS Task Restart
 *  @ingroup ClassicTasks
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/rtems/tasks.h>
#include <rtems/score/threadimpl.h>

rtems_status_code rtems_task_restart(
  rtems_id  id,
  uint32_t  argument
)
{
  Thread_Control          *the_thread;
  Objects_Locations        location;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( _Thread_Restart( the_thread, _Thread_Executing, NULL, argument ) ) {
        _Objects_Put( &the_thread->Object );
        return RTEMS_SUCCESSFUL;
      }
      _Objects_Put( &the_thread->Object );
      return RTEMS_INCORRECT_STATE;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
      _Thread_Dispatch();
      return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}
