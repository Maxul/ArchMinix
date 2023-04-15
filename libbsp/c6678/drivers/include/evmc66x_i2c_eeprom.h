/******************************************************************************	
 * Copyright (c) 2011 Texas Instruments	Incorporated - http://www.ti.com
 *
 *  Redistribution and use in source and binary	forms, with or without
 *  modification, are permitted	provided that the following conditions
 *  are	met:
 *
 *    Redistributions of source	code must retain the above copyright
 *    notice, this list	of conditions and the following	disclaimer.
 *
 *    Redistributions in binary	form must reproduce the	above copyright	
 *    notice, this list	of conditions and the following	disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas	Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this	software without specific prior	written	permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	
 *  "AS	IS" AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO,	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR	ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY,	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO,	PROCUREMENT OF SUBSTITUTE GOODS	OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION)	HOWEVER	CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,	OR TORT	
 *  (INCLUDING NEGLIGENCE OR OTHERWISE)	ARISING	IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/	

/******************************************************************************	
 *
 * File	Name:	evmc64x_i2c_eeprom.h
 *
 * Description:	Provides defintions and	prototypes local to the	i2c module.
 *
 ******************************************************************************/

#ifndef	_EVM66X_I2C_EEPROM_H
#define	_EVM66X_I2C_EEPROM_H

/************************
 * Defines and Macros
 ************************/
// Register values for specific	purpose	
#define	I2C_VAL_REG_MDR_RESET		(0x4000)
#define	I2C_VAL_REG_MDR_SLVRCV		(0x40A0)
#define	I2C_VAL_REG_MDR_MSTRCV		(0x64A0)
#define	I2C_VAL_REG_MDR_MSTRCVSTOP	(0x4CA0)
#define	I2C_VAL_REG_MDR_MSTXMT		(0x46A0)
#define	I2C_VAL_REG_MDR_MSTXMTSTRT	(0x66A0)
#define	I2C_VAL_REG_MDR_MSTXMTSTOP	(0x4CA0)

#define	I2C_VAL_REG_STR_RESET		(0x0410)
#define	I2C_VAL_REG_STR_ON_FAIL		(0x1002)  // Clear bus busy, clear nack	
#define	I2C_VAL_REG_STR_CLR_BUSY	(0x1000)  // Clear busy	

// Bit field definitions
#define	BOOTBITMASK(x,y)			((( (1 << ((x)-(y)+1) )	- 1 )) << (y))
#define	BOOT_READ_BITFIELD(z,x,y)	((z) & BOOTBITMASK(x,y)) >> (y)	
#define	I2C_REG_STR_FIELD_BB(x)		BOOT_READ_BITFIELD((x),	12, 12)	
#define	I2C_REG_STR_FIELD_NACK(x)	BOOT_READ_BITFIELD((x),	 1,  1)	
#define	I2C_REG_STR_FIELD_ARDY(x)	BOOT_READ_BITFIELD((x),	 2,  2)	
#define	I2C_REG_STR_FIELD_XRDY(x)	BOOT_READ_BITFIELD((x),	 4,  4)	
#define	I2C_REG_STR_FIELD_RRDY(x)	BOOT_READ_BITFIELD((x),	 3,  3)	

// Return values
#define	I2C_RET_OK					(0)
#define	I2C_RET_LOST_ARB			(1)
#define	I2C_RET_NO_ACK				(2)
#define	I2C_RET_IDLE_TIMEOUT		(3)
#define	I2C_RET_BAD_REQUEST			(4)
#define	I2C_RET_CLOCK_STUCK_LOW		(5)
#define	I2C_RET_NULL_PTR_ERROR		(6)
#define	I2C_RET_GEN_ERROR			(99)

// Desired frequency for module operation
#define	I2C_TARGET_MODULE_FREQ_MHZ	(8)

// Timeout limit for master	receiver. The units are	
// in number of bits, so provide some overhead
#define	I2C_MAX_MASTER_RECEIVE_TIMEOUT	 (24)	/* 3 bytes */

// Timeout limit for master	transmitter. The units are
// in number of bits, so provide some overhead
#define	I2C_MAX_MASTER_TRANSMITTER_TIMEOUT  (24)   /* 3	bytes */

// Timeout limit for the master transmitter	to get access
// to the bus. In 10ms units.
#define	I2C_MAX_MASTER_TRANSMITTER_BUS_ACCESS_TIMEOUT  (5)
#define	I2C_MASTER_TRANSMITTER_BUS_ACCESS_DELAY_US     (25)

/* Timeout limit after a master transmitter	operation is
 * complete, and waiting for access	to the MMRs. This should be on 
 * the order of two bytes, for the last two that are being sent 
 * (one in the shift register, one in the dxr. The units are in bits. */
#define	I2C_MAX_MASTER_TRANSMITTER_ARDY_TIMEOUT	  (32)	/* 4 bytes */

/* Timeout limit in	slave receiver mode. The unit is in
 * expected	bit periods, but is long since the master
 * may have	a long delay before beginning transmission. */
#define	I2C_MAX_SLAVE_RECEIVE_TIMEOUT	   (5000000)

#define EEPROM_SLAVE_ADDR_50   	(0x50)
#define EEPROM_SLAVE_ADDR_51   	(0x51)

/************************
 * Structures and Enums
 ************************/
 	
// Bus release
enum {
  I2C_RELEASE_BUS,
  I2C_DO_NOT_RELEASE_BUS
};

typedef	uint16_t I2C_RET;

// Byte	ordering
enum {
  I2C_BYTE_LSB,	
  I2C_BYTE_SLSB,
  I2C_BYTE_SMSB,
  I2C_BYTE_MSB
};

/************************
 * Function declarations
 ************************/
I2C_RET	i2cEepromWriteBlock( uint8_t uchEepromI2cAddress, uint32_t *puiData,
							 uint32_t uiNumBytes, uint32_t uiEndBusState);
I2C_RET	i2cEepromRead ( uint32_t byte_addr, uint32_t	uiNumBytes,
						uint8_t *puiData, uint8_t uchEepromI2cAddress);
I2C_RET	i2cEepromWrite( uint16_t uchEepromI2cAddress, uint32_t *puiData,
						uint32_t uiNumBytes, uint32_t uiEndBusState);
I2C_RET i2cEepromWriteByteAddr( uint32_t byte_addr, uint16_t uchEepromI2cAddress,
						uint8_t *puiData, uint32_t uiNumBytes, uint32_t uiEndBusState);
I2C_RET	i2cEepromMacWrite(uint8_t	*puchSrc);
I2C_RET	i2cEepromMacRead(uint8_t *puchDst);

#endif // _EVM66X_I2C_EEPROM_H


