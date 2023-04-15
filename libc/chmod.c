/**
 *  @file
 *
 *  @brief Change File Modes
 *  @ingroup libcsupport
 */

/*
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

#include <sys/stat.h>

#include <rtems/libio_.h>

/**
 *  POSIX 1003.1b 5.6.4 - Change File Modes
 */
int chmod( const char *path, mode_t mode )
{
  int rv = 0;
  rtems_filesystem_eval_path_context_t ctx;
  int eval_flags = RTEMS_FS_FOLLOW_LINK;
  const rtems_filesystem_location_info_t *currentloc =
    rtems_filesystem_eval_path_start( &ctx, path, eval_flags );

  rv = rtems_filesystem_chmod( currentloc, mode );

  rtems_filesystem_eval_path_cleanup( &ctx );

  return rv;
}
