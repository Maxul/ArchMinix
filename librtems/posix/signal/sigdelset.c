/**
 * @file
 *
 * @brief Deleted Signal from Set
 * @ingroup POSIXAPI
 */

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
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

#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/sigset.h>
#include <rtems/seterr.h>

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigdelset(
  sigset_t   *set,
  int         signo
)
{
  if ( !set )
    rtems_set_errno_and_return_minus_one( EINVAL );

  if ( !signo )
    return 0;

  if ( !is_valid_signo(signo) )
    rtems_set_errno_and_return_minus_one( EINVAL );

  *set &= ~signo_to_mask(signo);
  return 0;
}
