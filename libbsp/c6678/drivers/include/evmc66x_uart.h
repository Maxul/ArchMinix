/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/
 
/******************************************************************************	
 *
 * File	Name:	evmc66x_i2c_uart.h
 *
 * Description:	This contains UART specific structure, typedefs, function
 *		prototypes.
 *
 ******************************************************************************/
#ifndef	_EVM66X_I2C_UART_H_
#define	_EVM66X_I2C_UART_H_

/************************
 * Defines and Macros
 ************************/
// Mask	to enable DLL and DLM
#define	DLAB		(0x80)			// Way to swap mem banks

// for 19200 baudrate for crystal clock	14.7456	MHz
#define	DLL_VAL		(0x30)
#define	DLM_VAL		(0x00)

// Macros to be	used for setting baudrate
#define	BAUD_RATE_9600		(0x0060)
#define	BAUD_RATE_19200		(0x0030)
#define	BAUD_RATE_56000		(0x0010)
#define	BAUD_RATE_115200	(0x0008)

#define hUartRegs    ((CSL_UartRegs*) CSL_UART_REGS)


/************************
 * Function declarations
************************/
void UartInit(void);
void UartSetBaudRate(uint16_t uiBaudRate);
uint16_t UartReadBaudRate(void);
uint8_t UartReadData(void);
void UartWriteData(uint8_t uchByte);
Bool UartIsDataReady(void);
#endif // _EVM66X_I2C_UART_H_

