/**
 *  @file
 *
 *  @brief Execute a File
 *  @ingroup POSIXAPI
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

#ifndef HAVE_EXECLE

#include <errno.h>
#include <rtems/seterr.h>

int execle(
  const char *path,
  char const *arg,
  ...
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}

#endif
