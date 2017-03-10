/**
 * @file
 *
 * @brief Initialises a RWLock Attributes object attr with the default value
 * @ingroup POSIXAPI
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

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>

/*
 *  RWLock Attributes Initialization
 */

int pthread_rwlockattr_init(
  pthread_rwlockattr_t *attr
)
{
  if ( !attr )
    return EINVAL;

  attr->is_initialized = true;
  attr->process_shared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
