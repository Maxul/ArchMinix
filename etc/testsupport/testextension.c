/*
 * Copyright (c) 2014 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/test.h>
#include <rtems/profiling.h>

#if defined(RTEMS_PROFILING)
static bool report_done;

RTEMS_INTERRUPT_LOCK_DEFINE( static, report_lock, "test report" )
#endif

void rtems_test_fatal_extension(
  rtems_fatal_source source,
  bool is_internal,
  rtems_fatal_code code
)
{
#if defined(RTEMS_PROFILING)
  rtems_interrupt_lock_context lock_context;

  /*
   * Ensures to report only once on SMP machines and ensures that the report is
   * output completely.
   */
  rtems_interrupt_lock_acquire( &report_lock, &lock_context );

  if ( !report_done ) {
    report_done = true;

    printk(
      "\n*** PROFILING REPORT BEGIN %s ***\n",
      rtems_test_name
    );

    rtems_profiling_report_xml(
      rtems_test_name,
      printk_plugin,
      NULL,
      1,
      "  "
    );

    printk(
      "*** PROFILING REPORT END %s ***\n",
      rtems_test_name
    );
  }

  rtems_interrupt_lock_release( &report_lock, &lock_context );
#endif

  (void) source;
  (void) is_internal;
  (void) code;
}
