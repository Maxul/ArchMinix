/**
 * @file
 *
 * @brief RTEMS Default File System Link
 */

/*
 *  COPYRIGHT (c) 2010.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/libio_.h>
#include <rtems/seterr.h>

int rtems_filesystem_default_link(
  const rtems_filesystem_location_info_t *parentloc,
  const rtems_filesystem_location_info_t *targetloc,
  const char *name,
  size_t namelen
)
{
  rtems_set_errno_and_return_minus_one( ENOTSUP );
}
