/**
 * @file
 *
 * @brief Sends a signal Asynchronously directed to a thread
 * @ingroup POSIXAPI
 */

/*
 *  3.3.10 Send a Signal to a Thread, P1003.1c/D10, p. 43
 *
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

#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include <rtems/posix/pthreadimpl.h>
#include <rtems/posix/psignalimpl.h>
#include <rtems/score/isr.h>
#include <rtems/score/threadimpl.h>

int pthread_kill(
  pthread_t   thread,
  int         sig
)
{
  POSIX_API_Control  *api;
  Thread_Control     *the_thread;
  Objects_Locations  location;

  if ( !is_valid_signo( sig ) ) {
    return EINVAL;
  }

  the_thread = _Thread_Get( thread, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      /*
       *  If sig == 0 then just validate arguments
       */

      api = the_thread->API_Extensions[ THREAD_API_POSIX ];

      if ( _POSIX_signals_Vectors[ sig ].sa_handler == SIG_IGN ) {
        _Objects_Put( &the_thread->Object );
        return 0;
      }

      /* XXX critical section */

      api->signals_pending |= signo_to_mask( sig );

      (void) _POSIX_signals_Unblock_thread( the_thread, sig, NULL );
      _Objects_Put( &the_thread->Object );
      return 0;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
#endif
    case OBJECTS_ERROR:
      break;
  }

  return ESRCH;
}
