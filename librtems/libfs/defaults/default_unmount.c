/**
 * @file
 *
 * @brief RTEMS Default File System Unmounts
 * @ingroup LibIOFSOps File System Operations 
 */

/*
 *  COPYRIGHT (c) 2010.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/libio.h>
#include <rtems/libio_.h>
#include <rtems/seterr.h>

int rtems_filesystem_default_unmount(
  rtems_filesystem_mount_table_entry_t *mt_entry     /* IN */
)
{
  rtems_set_errno_and_return_minus_one( ENOTSUP );
}
