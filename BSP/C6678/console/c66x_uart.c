/**
 * @file
 *
 *  Beijing University of Posts & Communications , CHINA
 *  Copyright (c) 2016 Maxul Lee <lmy2010lmy@gmail.com>
 *
 * @brief Driver for serial ports on the TMS320C6678.
 *
 *  COPYRIGHT (c) 1989-2016.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include "c66x_uart.h"

#include <bsp.h>                /* Must be before libio.h */
#include <bsp/uart-output-char.h>

#include <rtems/libio.h>
#include <rtems/console.h>
#include <rtems/bspIo.h>

#include <termios.h>

#include <libchip/serial.h>

//#include "platform_internal.h"

#define UART_REGS_BASE            (0x02540000)

unsigned long Console_Configuration_Count = 1;

static int     uart_first_open(int major, int minor, void *arg);
static int     uart_last_close(int major, int minor, void *arg);
static int     uart_read(int minor);
static ssize_t uart_write(int minor, const char *buf, size_t len);
static void    uart_init(int minor);
static void    uart_write_polled(int minor, char c);
static int     uart_set_attributes(int minor, const struct termios *t);

static bool libchip_serial_default_probe(int minor);

const console_fns uart_fns = {
    libchip_serial_default_probe,
    uart_first_open,
    uart_last_close,
    uart_read,
    uart_write,
    uart_init,
    uart_write_polled,
    uart_set_attributes,
    FALSE
};

console_tbl Console_Configuration_Ports [] = {
    {
      .sDeviceName = "/dev/com0",
      .deviceType = SERIAL_CUSTOM,
      .pDeviceFns = &uart_fns,
      .deviceProbe = NULL,
      .pDeviceFlow = NULL,
      .ulCtrlPort1 = UART_REGS_BASE,
      .ulCtrlPort2 = 0,
      .ulClock = BAUD_RATE_115200,
      //.ulIntVector = 12,
    },
};


static void uart_delay_cycles(uint32_t cycles)
{
    while (cycles--) {
        asm ("\tNOP");
    }
}

static int
uart_is_ready(void)
{
	int ret = FALSE;

	if (CSL_UART_LSR_DR_READY == (CSL_FEXT(uart_registers->LSR, UART_LSR_DR))) {
		ret  = TRUE;
    }

	return (ret);
}


static bool libchip_serial_default_probe(int minor)
{
  /*
   * If the configuration dependent probe has located the device then
   * assume it is there
   */

  return true;
}

static int uart_first_open(int major, int minor, void *arg)
{
	return 0;
}

static int uart_last_close(int major, int minor, void *arg)
{
	return 0;
}

static int uart_read(int minor)
{
    return -1;
}

static ssize_t uart_write(int minor, const char *buf, size_t len)
{
    size_t i = len;

    for (i = 0; i < len; i++) {
        if (buf[i]) {
        	uart_write_polled(minor, buf[i]);
        }
    }

    return i;
}

static int uart_set_attributes(int minor, const struct termios *t)
{
    return 0;
}

static void uart_init(int minor)
{
    uint8_t DLL_val = 0;
    uint8_t DLH_val = 0;

    /* Setting baud rate to 115200 */
    DLL_val = (uint8_t )(0x00FF & BAUD_RATE_115200);
    DLH_val = (uint8_t )(0x00FF & (BAUD_RATE_115200  >> 8));

    /* Allows access to the divisor latches of the baud generator during a
       read or write operation (DLL and DLH) */
    uart_registers->LCR = 0x80;

    /* Set the baudrate,for accessing LCR[7] should be enable */
    uart_registers->DLL  = DLL_val;
    uart_registers->DLH  = DLH_val;

    /* Allows access to the receiver buffer register (RBR),
       the transmitter holding register (THR), and the
       interrupt enable register (IER) selected. */
    uart_registers->LCR = 0x18;

    /* Disable THR, RHR, Receiver line status interrupts */
    uart_registers->IER = 0;

    /* If autoflow control is desired,
     * write appropriate values to the modem
     * control register (MCR). Note that all UARTs
     * do not support autoflow control, see
     * the device-specific data manual for supported features.
     *
     * MCR
     * ====================================================
     * Bit  Field   Value   Description
     * 5    AFE     0       Autoflow control is disabled
     * 4    LOOP    0       Loop back mode is disabled.
     * 1    RTS     0       RTS control (UARTn_RTS is disabled,
     *                      UARTn_CTS is only enabled.)
     * =====================================================
     *
     *
     */
    uart_registers->MCR = 0;

    /* Choose the desired response to
     * emulation suspend events by configuring
     * the FREE bit and enable the UART by setting
     * the UTRST and URRST bits in the power and
     * emulation management register (PWREMU_MGMT).
     *
     *
     * PWREMU_MGMT
     * =================================================
     * Bit  Field   Value   Description
     * 14   UTRST   1       Transmitter is enabled
     * 13   URRST   1       Receiver is enabled
     * 0    FREE    1       Free-running mode is enabled
     * ===================================================
     *
     */
    uart_registers->PWREMU_MGMT = 0x6001;

    /* Cleanup previous data (rx trigger is also set to 0)*/
    uart_registers->FCR = 0xC1;

    /* Setting the baud rate */
    uart_registers->LCR = 0x80;

    /* Set the baudrate,for accessing LCR[7] should be enable */
    uart_registers->DLL  = DLL_val;
    uart_registers->DLH  = DLH_val;
    uart_registers->LCR  = 0x03;
}

static void uart_write_polled(int minor, char c)
{
    while (!(uart_registers->LSR & UART_LSR_THRE_MASK)) {
        uart_delay_cycles(10000);
    }
    uart_registers->THR = (UART_THR_DATA_MASK & (uint8_t)c);


}

static void _BSP_put_char( char c )
{
	/*
    while (!(uart_registers->LSR & UART_LSR_THRE_MASK)) {
        uart_delay_cycles(10000);
    }
    uart_registers->THR = (UART_THR_DATA_MASK & c);
    */
    while (!(uart_registers->LSR & UART_LSR_THRE_MASK)) {
        uart_delay_cycles(10000);
    }
    CSL_FINS(uart_registers->THR, UART_THR_DATA, c);



}

#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_uart.h>
static int _BSP_poll_char(void)
{
	uint8_t uRcvChar = 0;

    while( (uart_is_ready()) != 1)
    {
    	;//loop
    }

    uRcvChar = CSL_FEXT(uart_registers->RBR, UART_RBR_DATA);

	return uRcvChar;
}

BSP_output_char_function_type BSP_output_char = _BSP_put_char;
BSP_polling_getchar_function_type BSP_poll_char = _BSP_poll_char;

