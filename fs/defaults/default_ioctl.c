/**
 * @file
 *
 * @brief RTEMS Default File System controls a STREAMS device
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

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/libio_.h>

int rtems_filesystem_default_ioctl(
  rtems_libio_t   *iop,
  ioctl_command_t  request,
  void            *buffer
)
{
  rtems_set_errno_and_return_minus_one( ENOTTY );
}
