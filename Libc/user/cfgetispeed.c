/**
 *  @file
 *
 *  @brief Baud Rate Functions
 *  @ingroup Termios
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

#include <rtems.h>
#if defined(RTEMS_NEWLIB)

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <termios.h>

speed_t cfgetispeed(
  const struct termios *tp
)
{
  return (tp->c_cflag / (CIBAUD / CBAUD)) &  CBAUD;
}
#endif
