/**
 * @file
 *
 * @brief Kill No POSIX
 * @ingroup libcsupport
 *
 *  Marginal implementations of some POSIX API routines
 *  to be used when POSIX is disabled.
 *
 *    + kill
 *    + _kill_r
 *    + sleep
 */

/*
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include <unistd.h>

/*
 *  These are directly supported (and completely correct) in the posix api.
 */

#if !defined(RTEMS_POSIX_API)
int kill( pid_t pid, int sig )
{
  return 0;
}

#if defined(RTEMS_NEWLIB)
#include <reent.h>

int _kill_r( struct _reent *ptr, pid_t pid, int sig )
{
  return 0;
}
#endif
#endif
