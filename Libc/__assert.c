/**
 *  @file
 *
 *  @brief Evaluate Assertion
 *  @ingroup libcsupport
 */

/*
 *  COPYRIGHT (c) 2007-2013.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/bspIo.h>
#include <rtems.h>

#if defined(RTEMS_NEWLIB) && !defined(HAVE___ASSERT_FUNC)
#include <assert.h>

/**
 * Newlib 1.16.0 added this method.  Together these provide an
 * RTEMS safe, low memory implementation.
 */
void __assert_func(
  const char *file,
  int         line,
  const char *func,
  const char *failedexpr
)
{
  rtems_assert_context assert_context = {
    .file = file,
    .line = line,
    .function = func,
    .failed_expression = failedexpr
  };

  printk("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
    failedexpr,
    file,
    line,
    (func) ? ", function: " : "",
    (func) ? func : ""
  );
  rtems_fatal( RTEMS_FATAL_SOURCE_ASSERT, (rtems_fatal_code) &assert_context );
}
#endif

#if defined(RTEMS_NEWLIB) && !defined(HAVE___ASSERT)

/**
 *  small RTEMS Specific Implementation
 */
void __assert(
  const char *file,
  int         line,
  const char *failedexpr
)
{
  __assert_func (file, line, NULL, failedexpr);
}
#endif
