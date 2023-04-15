/**
 * @file
 *
 * @brief Returns Scheduling Parameter Attributes of Thread Attributes Object
 * @ingroup POSIXAPI
 */

/*
 *  13.5.1 Thread Creation Scheduling Attributes, P1003.1c/Draft 10, p. 120
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

int pthread_attr_getschedparam(
  const pthread_attr_t   *attr,
  struct sched_param     *param
)
{
  if ( !attr || !attr->is_initialized || !param )
    return EINVAL;

  *param = attr->schedparam;
  return 0;
}
