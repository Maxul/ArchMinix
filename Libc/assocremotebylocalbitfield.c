/**
 * @file
 *
 * @brief RTEMS Assoc Routines
 * @ingroup Associativity Routines
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */


uint32_t   rtems_assoc_remote_by_local_bitfield(
    const rtems_assoc_t *ap,
    uint32_t             local_value
)
{
  uint32_t   b;
  uint32_t   remote_value = 0;

  for (b = 1; b; b <<= 1)
    if (b & local_value)
      remote_value |= rtems_assoc_remote_by_local(ap, b);

  return remote_value;
}
