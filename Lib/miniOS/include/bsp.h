/*  bsp.h
 *
 *  This include file contains all board IO definitions.
 *
 *  XXX : put yours in here
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_NO_CPU_NO_BSP_BSP_H
#define LIBBSP_NO_CPU_NO_BSP_BSP_H

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_CONSOLE_MINOR 0
#define BSP_CONSOLE_UART_BASE 0x02540000

#define BSP_GPIO_PIN_COUNT 32
#define BSP_GPIO_PINS_PER_BANK 32
/* functions */

rtems_isr_entry set_vector(                     /* returns old vector */
  rtems_isr_entry     handler,                  /* isr routine        */
  rtems_vector_number vector,                   /* vector number      */
  int                 type                      /* RTEMS or RAW intr  */
);

#ifdef __cplusplus
}
#endif

#endif
