/**
 * @file
 *
 * @brief Inoperable implementation of execv for POSIX threads
 * @ingroup POSIXAPI
 */

/*
 *  execv() - POSIX 1003.1b 3.1.2
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

#ifndef HAVE_EXECV

#include <errno.h>
#include <rtems/seterr.h>

int execv(
  const char *file,
  char *const argv[]
)
{
  rtems_set_errno_and_return_minus_one( ENOSYS );
}

#endif
