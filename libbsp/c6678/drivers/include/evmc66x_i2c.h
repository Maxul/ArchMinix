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
 * File	Name:	evmc66x_i2c.h
 *
 * Description:	This contains i2c specific structure, typedefs,	function
 *				prototypes.
 *
 *
 ******************************************************************************/
#ifndef	_EVM66X_I2C_H_
#define	_EVM66X_I2C_H_

/************************
 * Include Files
 ************************/
#include "ti/csl/cslr_i2c.h"
#include "ti/csl/cslr_device.h"

/************************
 * Defines and Macros
 ************************/
#define	EEPROM_I2C_SLAVE_ADDR	 	 (0x50)
#define I2C_OWN_ADDR                 (0x10)
    
    // Interrupt
#define I2C_DISABLE_INTR    		 (0x00)

#ifdef _I2C_400KHZ_
// I2C prescalar set to 16 to get module freq (1000MHz/6)/147 = 11.9 MHz
#define I2C_PRESCALER 				 (13)
// Setting the divider to divide module freq 11.9MHz/((9+6)+(9+6)) = 396.67 KHz
#define I2C_CLK_LOW					 (9)
#define I2C_CLK_HIGH				 (9)
#else
// I2C prescalar set to 16 to get module freq (1000MHz/6)/17 = 9.8 MHz
#define I2C_PRESCALER 				 (16)
// Setting the divider to divide module freq 9.8MHz/((36+6)+(36+6)) = 116.67 KHz
#define I2C_CLK_LOW					 (36)
#define I2C_CLK_HIGH				 (36)
#endif

/************************
 * Globals
 ************************/
#define I2CR    ((CSL_I2cRegs*) CSL_I2C_DATA_CONTROL_REGS)

/************************
 * Function declarations
 ************************/
void evmI2CInit(void);
#endif // _EVM64X_I2C_H_
