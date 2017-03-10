#ifndef    _EVM66X_I2C_UART_H_
#define    _EVM66X_I2C_UART_H_

#include <stdint.h>
#include <cslr_uart.h>

#define UART_REGS_BASE            (0x02540000)

/**************************************************************************
 * Register Overlay Structure
 **************************************************************************/
typedef struct  {
    volatile uint32_t RBR;
    volatile uint32_t IER;
    volatile uint32_t IIR;
    volatile uint32_t LCR;
    volatile uint32_t MCR;
    volatile uint32_t LSR;
    volatile uint32_t MSR;
    volatile uint32_t SCR;
    volatile uint32_t DLL;
    volatile uint32_t DLH;
    volatile uint32_t REVID1;
    volatile uint32_t REVID2;
    volatile uint32_t PWREMU_MGMT;
    volatile uint32_t MDR;
} uart_registers_t;

/* Mask    to enable DLL and DLM */
#define    DLAB        (0x80)            /* Way to swap mem banks */

/* Baudrate table for crystal clock 14.7456 MHz*/
#define    BAUD_RATE_9600      (0x0060)
#define    BAUD_RATE_19200     (0x0030)
#define    BAUD_RATE_115200    (0x0058)

#define uart_registers    ((uart_registers_t*) UART_REGS_BASE)

/* Following 2 lines are added due to CSL3.x tools limitations */
#define THR RBR   /* RBR & THR have same offset */
#define FCR IIR   /* IIR & FCR have same offset */

#define UART_LSR_THRE_MASK (0x00000020u)
#define UART_THR_DATA_MASK (0x000000FFu)

#endif /* _EVM66X_I2C_UART_H_ */
