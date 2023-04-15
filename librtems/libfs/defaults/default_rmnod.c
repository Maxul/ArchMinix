/**
 * @file
 *
 * @brief RTEMS Default Filesystem - Default Remove Node
 * @ingroup libfs
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

int rtems_filesystem_default_rmnod(
  const rtems_filesystem_location_info_t *parentloc,
  const rtems_filesystem_location_info_t *loc
)
{
  rtems_set_errno_and_return_minus_one( ENOTSUP );
}
