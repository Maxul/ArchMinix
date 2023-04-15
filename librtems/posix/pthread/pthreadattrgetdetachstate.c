/**
 * @file
 *
 * @brief Function gets the detachstate Attribute in the attr Object
 * @ingroup POSIXAPI
 */

/*
 *  16.1.1 Thread Creation Attributes, P1003.1c/Draft 10, p, 140
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

int pthread_attr_getdetachstate(
  const pthread_attr_t  *attr,
  int                   *detachstate
)
{
  if ( !attr || !attr->is_initialized || !detachstate )
    return EINVAL;

  *detachstate = attr->detachstate;
  return 0;
}
