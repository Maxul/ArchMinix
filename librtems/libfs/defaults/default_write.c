/**
 * @file
 *
 * @brief RTEMS Default File System sends a message to another user
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

ssize_t rtems_filesystem_default_write(
  rtems_libio_t *iop,
  const void    *buffer,
  size_t         count
)
{
  rtems_set_errno_and_return_minus_one( ENOTSUP );
}
