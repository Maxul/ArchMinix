/**
 * @file
 *
 * @brief Call to function by Thread will call init_routine with no Arguments
 * @ingroup POSIXAPI
 */

/*
 *  16.1.8 Dynamic Package Initialization, P1003.1c/Draft 10, p. 154
 *
 *  COPYRIGHT (c) 1989-1999.
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
#include <errno.h>

#include <rtems/score/onceimpl.h>

int pthread_once(
  pthread_once_t  *once_control,
  void           (*init_routine)(void)
)
{
  if ( !once_control || !init_routine )
    return EINVAL;

  if ( once_control->is_initialized != 1 )
    return EINVAL;

  return _Once( &once_control->init_executed, init_routine );
}
