/**
 *  @file
 *
 *  @brief Execute a Program
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

#ifndef HAVE_EXECVE

#include <errno.h>
#include <rtems/seterr.h>

/**
 *  POSIX 1003.1b 3.1.2
 */
int execve(
  const char *path,
  char *const argv[],
  char *const envp[]
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}

#endif
