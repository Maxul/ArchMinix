/******************************************************************************
 * Copyright (c) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
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
 * File Name:  	evmc66x_i2c_eeprom.c
 *
 * Description:	Provides a polling i2c driver. 
 * 
 ******************************************************************************/

/************************
 * Include Files
 ************************/
#include "platform_internal.h"

#if (PLATFORM_EEPROM_IN)

/************************
 * Defines and Macros
 ************************/
#define DELAY_CONST 			2
#define MACADDRESS_SIZE			12
#define MAC_OFFSET				0xF400

/************************
 * Globals
 ************************/
uint32_t gI2cBlock[17];

/******************************************************************************
 * 
 * Function:	i2cDelay
 *
 * Description:	Creates a delay.
 *
 * Parameters:	UINT32 uiCount - counter for the loop
 *
 * Return Value: void
 *
 ******************************************************************************/
static void i2cDelay (uint32_t uiCount)
{
	uint32_t i;

  	for (i = 0; i < uiCount; i++) platform_delaycycles(50000);
}

/******************************************************************************
 * 
 * Function:	i2cEepromWriteBlock  
 *
 * Description:	Enters master transmitter mode, writes a specified number
 *              of bytes.
 *
 * Parameters:	uint8_t uchEepromI2cAddress - i2c address of EEPROM
 * 				uint32_t *puiData - pointer to the buffer base address
 * 				uint32_t uiNumBytes - number of bytes of buffer
 * 				uint32_t uiEndBusState - The state on which bus should be left
 *
 * Return Value: I2C_RET - status
 * 
 ******************************************************************************/  
I2C_RET i2cEepromWriteBlock( uint8_t uchEepromI2cAddress, uint32_t *puiData,
							 uint32_t uiNumBytes, uint32_t uiEndBusState)
{
  	uint32_t uiTimeoutCounter;
  	uint32_t uiPollingStatus;
  	uint32_t uiValue;
  	uint32_t uiStatusReg;
  	uint32_t uiCount;
  	uint32_t uiByteType;

   
  	/* Check for the bus busy signal */
  	uiTimeoutCounter = 0;
  	do  
  	{ 
    	uiPollingStatus = I2C_REG_STR_FIELD_BB(I2CR->ICSTR);
    
    	if (uiPollingStatus)  
    	{
      		i2cDelay (I2C_MASTER_TRANSMITTER_BUS_ACCESS_DELAY_US);
      
      		uiTimeoutCounter += 1;
      		if (uiTimeoutCounter >= I2C_MAX_MASTER_TRANSMITTER_BUS_ACCESS_TIMEOUT)  
      		{
      			/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
        		return (I2C_RET_IDLE_TIMEOUT);
      		}
    	} 
    	else  
    	{ 
    		/* The bus is free */
      		uiTimeoutCounter = 0;
    	}
  	}while (uiTimeoutCounter != 0);
      
  	/* Enter master transmitter mode, set the slave address register */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMT;
  	I2CR->ICSAR = (uint8_t)uchEepromI2cAddress;
  	i2cDelay (DELAY_CONST);
  
  	/* Put the first byte into the transmit register, set the start bit */
  	uiValue = (*puiData >> 8) & 0x00ff;
  	I2CR->ICDXR =  uiValue;
  	uiByteType = I2C_BYTE_LSB;
  
  	/* Set the start bit */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMTSTRT;
  
  	for (uiCount = 1; uiCount < uiNumBytes; uiCount++)  
  	{
    	uiTimeoutCounter = 0;
  
    	do  
    	{
	      	/* Read status */
      		uiStatusReg = I2CR->ICSTR;
    
      		/* On Nack return failure */
      		if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
      		{
        		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_NAK;
				return (I2C_RET_NO_ACK);
      		}
        
      		/* Check for transmit ready */
      		if (I2C_REG_STR_FIELD_XRDY(uiStatusReg))  
      		{
        		uiTimeoutCounter = 0;
        
        		if (uiByteType == I2C_BYTE_MSB)  
        		{
          			uiValue = (*puiData >> 24) & 0x00ff;
          			uiByteType = I2C_BYTE_SMSB;
        		} 
        		else if (uiByteType == I2C_BYTE_SMSB)  
        		{
          			uiValue =  (*puiData >> 16) & 0x00ff;
          			uiByteType = I2C_BYTE_SLSB;
        		} 
        		else if (uiByteType == I2C_BYTE_SLSB)  
        		{
          			uiValue = (*puiData >> 8) & 0x00ff;
          			uiByteType = I2C_BYTE_LSB;
        		}  
        		else  
        		{
          			uiValue = (*puiData) & 0x00ff;
          			uiByteType = I2C_BYTE_MSB;
          			puiData += 1;
        		}
        
        	/*Write data Transmit Data Register */
    		I2CR->ICDXR = uiValue;
        
      		}  
      		else  
      		{   
      			/* XRDY bit not set */
        		i2cDelay (DELAY_CONST);
        		uiTimeoutCounter += 1;
        		
        		if (uiTimeoutCounter >= I2C_MAX_MASTER_TRANSMITTER_TIMEOUT)  
        		{
          			/* Return to slave receiver, clear nack and bus busy */
        			I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
					I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                    platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
          			return (I2C_RET_IDLE_TIMEOUT);
        		}
      		}
          
    	}while (uiTimeoutCounter != 0);

  	} /* end for loop */
  
  	/* If releasing the bus, send a stop bit */
  	if (uiEndBusState == I2C_RELEASE_BUS)  
  	{
	    /* Wait for the ardy bit to go high */
	    uiTimeoutCounter = 0;
	    do  
	    {
	    	uiStatusReg = I2CR->ICSTR;
	      	if (I2C_REG_STR_FIELD_ARDY(uiStatusReg))  
	      	{
	        	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMTSTOP;
	        	I2CR->ICSTR = I2C_VAL_REG_STR_CLR_BUSY;
	        	i2cDelay (DELAY_CONST);
	        	uiTimeoutCounter = 0;        
	      	} 
	      	else  
	      	{ 
	      		/* Registers not ready for access */
	        	uiTimeoutCounter += 1;
	        
	        	if (uiTimeoutCounter >= I2C_MAX_MASTER_TRANSMITTER_ARDY_TIMEOUT)  
	        	{
	           		/* On timeout put the peripheral into reset, wait, then
	            	* take it out of reset */
	           		I2CR->ICMDR = I2C_VAL_REG_MDR_RESET;
	           		i2cDelay (DELAY_CONST);
	           		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
         			platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
	           		return (I2C_RET_IDLE_TIMEOUT);
	         	}
	        	
	        	i2cDelay (DELAY_CONST); 
	      	}
	    }while (uiTimeoutCounter != 0);  
	} /* end bus release */
  
  	return (I2C_RET_OK);
  
} // i2cEepromWriteBlock 
   
/******************************************************************************
 * 
 * Function:	i2cEepromRead  
 *
 * Description:	Reads a fixed number of bytes from an I2C prom. The read
 *              consists of a master write of 2 bytes (forming a 16 bit address,
 *              msb transmitted first), followed by a master read of the
 *              input number of bytes.The bytes that are read are placed
 *              in puiData in big endian format
 *
 * Parameters:	uint8_t uchEepromI2cAddress - i2c address of EEPROM
 * 				uint32_t *puiData - pointer to the buffer base address
 * 				uint32_t uiNumBytes - number of bytes of buffer
 * 				uint32_t uiEndBusState - The state on which bus should be left
 *
 * Return Value: I2C_RET - status
 * 
 ******************************************************************************/ 
I2C_RET i2cEepromRead ( uint32_t byte_addr, uint32_t uiNumBytes,
                        uint8_t *puiData, uint8_t uchEepromI2cAddress)
{

  	uint32_t  uiStatusReg;
  	uint32_t  uiTimeoutCounter;
  	uint32_t  iCount;
  	I2C_RET uiReturnValue;
  	uint16_t  ushValue;

    
  	/* Write the byte address to the eeprom. Do not send a stop */
  	uiReturnValue = i2cEepromWriteBlock ( uchEepromI2cAddress, &byte_addr, 
  										  2, I2C_DO_NOT_RELEASE_BUS);
  	if (uiReturnValue != I2C_RET_OK)
    	return (uiReturnValue);
    
  	/* Give the I2C prom 10ms to process the read command */
  	i2cDelay (DELAY_CONST);
  
  	/* Set the start bit, begin the master read */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTRCV;

 	for (iCount = 0; iCount < uiNumBytes; iCount++)  
 	{
    	uiTimeoutCounter = 0;
  
    	do  
    	{
      		/* Read status */
      		uiStatusReg = I2CR->ICSTR;
    
      		/* On Nack return failure */
      		if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
      		{
        		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_NAK;
        		return (I2C_RET_NO_ACK);
      		}
        
      		/* Check for receive byte ready */
      		if (I2C_REG_STR_FIELD_RRDY(uiStatusReg))  
      		{
        		ushValue = I2CR->ICDRR & 0x00ff;
        		uiTimeoutCounter = 0;
        		*puiData = ushValue;
        		puiData++;
      		}  	
      		else  
      		{   /* RRDY bit not set */
        		i2cDelay (DELAY_CONST);
        		uiTimeoutCounter += 1;

        		if (uiTimeoutCounter >= I2C_MAX_MASTER_RECEIVE_TIMEOUT)  {
          		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
          		return (I2C_RET_IDLE_TIMEOUT);
        	}
      	}          
    } while (uiTimeoutCounter != 0);

  	} /* end for loop */
        
  	/* The data block has been read. Send the stop bit */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTRCVSTOP;
	  
  	/* Wait for the rrdy and read the dummy byte */
  	uiTimeoutCounter = 0;
  	do  
  	{
    	uiStatusReg = I2CR->ICSTR;

    	/* Check for receive byte ready */
    	if (I2C_REG_STR_FIELD_RRDY(uiStatusReg))  
    	{
      		ushValue = I2CR->ICDRR & 0x00ff;
      		uiTimeoutCounter = 0;
    	}  
    	else  
    	{  /* rrdy not set */
      		i2cDelay (DELAY_CONST);
      		uiTimeoutCounter += 1;
      		
      		if (uiTimeoutCounter >= I2C_MAX_MASTER_RECEIVE_TIMEOUT)  
      		{
        		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;                
        		return (I2C_RET_IDLE_TIMEOUT);
      		}
    	}

  	}while (uiTimeoutCounter != 0);
  
  	return (I2C_RET_OK);
  
} // i2cEepromRead
        
/******************************************************************************
 * 
 * Function:	i2cEepromWrite  
 *
 * Description:	Writes specified number of bytes from the I2C EEPROM 
 *
 * Parameters:	uint8_t uchEepromI2cAddress - i2c address of EEPROM
 * 				uint32_t *puiData - pointer to the buffer base address
 * 				uint32_t uiNumBytes - number of bytes of buffer
 * 				uint32_t uiEndBusState - The state on which bus should be left
 *
 * Return Value: I2C_RET - status
 * 
 ******************************************************************************/ 
I2C_RET i2cEepromWrite( uint16_t uchEepromI2cAddress, uint32_t *puiData,
						uint32_t uiNumBytes, uint32_t uiEndBusState)
{

  	uint32_t uiBytesWritten, uiBlockCount, uiByteNum, uiCount, uiSizeBytes, uiAddress;
  	I2C_RET iret;

  	uiSizeBytes = uiNumBytes;
  	uiAddress = *puiData;
  	uiByteNum = 1;
  	uiBytesWritten = 0;
	
  	/* Block the data into 64 byte blocks, perform a write */
  	do{

    	/* Get the number of bytes in the current block */
    	uiBlockCount = uiSizeBytes - uiBytesWritten;
    	if (uiBlockCount > 64)
      		uiBlockCount = 64;

    	if (uiBlockCount == 0)
      		break;

    	for (uiCount = 0; uiCount < (uiBlockCount+3)>>2; uiCount++)
      		gI2cBlock[uiCount+1] = puiData[uiByteNum++];

    	gI2cBlock[0] = uiAddress;
    
    	iret = i2cEepromWriteBlock(uchEepromI2cAddress, gI2cBlock, uiBlockCount+2, uiEndBusState);
    	if (iret != I2C_RET_OK)
      		return (iret);

		i2cDelay(DELAY_CONST);

    	uiBytesWritten = uiBytesWritten + uiBlockCount;
    	uiAddress = uiAddress + uiBlockCount;

  	}while (uiBlockCount);

  	return (I2C_RET_OK);
}


/******************************************************************************
 * 
 * Function:	i2cEepromWriteByteAddr
 *(Same as above function except it takes byte address as a parameter)
 *
 * Description:	Writes specified number of bytes from the I2C EEPROM
 *
 * Parameters:	uint8_t uchEepromI2cAddress - i2c address of EEPROM
 * 				uint32_t *puiData - pointer to the buffer base address
 * 				uint32_t uiNumBytes - number of bytes of buffer
 * 				uint32_t uiEndBusState - The state on which bus should be left
 *
 * Return Value: I2C_RET - status
 *
 ******************************************************************************/
I2C_RET i2cEepromWriteByteAddr( uint32_t byte_addr, uint16_t uchEepromI2cAddress, uint8_t *puiData,
						uint32_t uiNumBytes, uint32_t uiEndBusState)
{

  	uint32_t uiBytesWritten, uiBlockCount, uiByteNum, uiCount, uiSizeBytes, uiAddress;
  	I2C_RET iret;

  	uiSizeBytes = uiNumBytes;
  	uiAddress = byte_addr;
  	uiByteNum = 0;
  	uiBytesWritten = 0;

  	/* Block the data into 64 byte blocks, perform a write */
  	do{

    	/* Get the number of bytes in the current block */
    	uiBlockCount = uiSizeBytes - uiBytesWritten;
    	if (uiBlockCount > 64)
      		uiBlockCount = 64;

    	if (uiBlockCount == 0)
      		break;

    	for (uiCount = 0; uiCount < (uiBlockCount+3)>>2; uiCount++)
    	{
      		gI2cBlock[uiCount+1] = (puiData[uiByteNum] << 24) | (puiData[uiByteNum+1] << 16) | (puiData[uiByteNum+2] << 8) | (puiData[uiByteNum+3]);
      		uiByteNum += 4;
    	}

    	gI2cBlock[0] = uiAddress;

    	iret = i2cEepromWriteBlock(uchEepromI2cAddress, gI2cBlock, uiBlockCount+2, uiEndBusState);
    	if (iret != I2C_RET_OK)
      		return (iret);

		i2cDelay(0x100);

    	uiBytesWritten = uiBytesWritten + uiBlockCount;
    	uiAddress = uiAddress + uiBlockCount;

  	}while (uiBlockCount);


  	return (I2C_RET_OK);
}


/******************************************************************************
 *
 * Function:  	i2cEepromMacWrite  
 *
 * Description:	Writing MAC addresses to EEPROM.
 *
 * Parameters:	uint8_t *puchSrc - pointer to mac address
 *
 * Return Value: I2C_RET - status
 * 
 ******************************************************************************/ 
I2C_RET i2cEepromMacWrite( uint8_t *puchSrc)
{
    uint32_t uiIndex;
    uint32_t uiStatusReg;
    uint32_t uiPollingStatus;
    uint32_t uiTimeoutCounter;
		
	/* Check for the bus busy signal */
  	uiTimeoutCounter = 0;
  	
  	do  
  	{ 
    	uiPollingStatus = I2C_REG_STR_FIELD_BB(I2CR->ICSTR);
    
    	if (uiPollingStatus)  
    	{
      		i2cDelay (I2C_MASTER_TRANSMITTER_BUS_ACCESS_DELAY_US);
      
      		uiTimeoutCounter += 1;
      		if (uiTimeoutCounter >= I2C_MAX_MASTER_TRANSMITTER_BUS_ACCESS_TIMEOUT)  
      		{
      			/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
        		return (I2C_RET_IDLE_TIMEOUT);
      		}
    	} 
    	else  
    	{ 
    		/* The bus is free */
      		uiTimeoutCounter = 0;
    	}
  	}while (uiTimeoutCounter != 0);
      
  	/* Enter master transmitter mode, set the slave address register */
	I2CR->ICCNT = MACADDRESS_SIZE + 2; 
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMT;
  	I2CR->ICSAR = EEPROM_SLAVE_ADDR_51;
	/* Set the start bit */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMTSTRT;
  	i2cDelay (DELAY_CONST);
	
	// setting the first byte of offset
    I2CR->ICDXR = ((MAC_OFFSET & 0xFF00)>>8);          
	i2cDelay (DELAY_CONST);
	/* Read status */
    uiStatusReg = I2CR->ICSTR;

  	/* On Nack return failure */
  	if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
  	{
    	/* Return to slave receiver, clear nack and bus busy */
    	I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
		I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
        platform_errno = PLATFORM_ERRNO_DEV_NAK;
    	return (I2C_RET_NO_ACK);
  	}
     		      		 
    /* Check for transmit ready */
	i2cDelay (DELAY_CONST);
    while(0 == I2C_REG_STR_FIELD_XRDY(uiStatusReg));	
    
    // setting the second byte of offset
    I2CR->ICDXR = (MAC_OFFSET & 0xFF);            
	i2cDelay (DELAY_CONST);
	/* Read status */
    uiStatusReg = I2CR->ICSTR;

  	/* On Nack return failure */
  	if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
  	{
    	/* Return to slave receiver, clear nack and bus busy */
    	I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
		I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
        platform_errno = PLATFORM_ERRNO_DEV_NAK;
    	return (I2C_RET_NO_ACK);
  	}
     		      		 
    /* Check for transmit ready */
	i2cDelay (DELAY_CONST);
    while(0 == I2C_REG_STR_FIELD_XRDY(uiStatusReg));
    // EEPROM offset written
	
	for ( uiIndex = 0 ; uiIndex < MACADDRESS_SIZE ; uiIndex++ )
    {
	   I2CR->ICDXR = puchSrc[uiIndex];            // Write  
       
	   i2cDelay(DELAY_CONST);
		/* Read status */
      	uiStatusReg = I2CR->ICSTR; 

			/* On Nack return failure */
      	if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
      	{
        		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_NAK;
				return (I2C_RET_NO_ACK);
      	}

	       /* Check for transmit ready */
      	while(0 == I2C_REG_STR_FIELD_XRDY(uiStatusReg));  
	}	

	/* Set the stop bit */
    I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMTSTOP;
	I2CR->ICSTR = I2C_VAL_REG_STR_CLR_BUSY;
    i2cDelay (DELAY_CONST);
	

    return I2C_RET_OK;
}

/******************************************************************************
 * 
 * Function:  	i2cEepromMacRead 
 *
 * Description:	API to read MAC addresses from I2C EEPROM
 *
 * Parameters:  uint8_t *puchDst - pointer to buffer for reading mac address
 *
 * Return Value:   I2C_RET - status
 * 
 ******************************************************************************/ 
I2C_RET i2cEepromMacRead( uint8_t *puchDst)
{
    uint32_t uiIndex;
    uint32_t uiStatusReg;
    uint32_t uiTimeoutCounter;
    uint32_t uiPollingStatus;
    uint8_t uchDummyByte;
		
	/* Check for the bus busy signal */
  	uiTimeoutCounter = 0;
  	do  
  	{ 
    	uiPollingStatus = I2C_REG_STR_FIELD_BB(I2CR->ICSTR);
    
    	if (uiPollingStatus)  
    	{	
      		i2cDelay (I2C_MASTER_TRANSMITTER_BUS_ACCESS_DELAY_US);
      
      		uiTimeoutCounter += 1;
      		if (uiTimeoutCounter >= I2C_MAX_MASTER_TRANSMITTER_BUS_ACCESS_TIMEOUT)  
      		{
      			/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
        		return (I2C_RET_IDLE_TIMEOUT);
      		}
    	} 
    	else  
    	{ 
    		/* The bus is free */
      		uiTimeoutCounter = 0;
    	}
  	}while (uiTimeoutCounter != 0);
      
  	/* Enter master transmitter mode, set the slave address register */
	I2CR->ICCNT = 2; 
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMT;
  	I2CR->ICSAR = EEPROM_SLAVE_ADDR_51;
	/* Set the start bit */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTXMTSTRT;
  	i2cDelay (DELAY_CONST);

    // setting the first byte of offset
    I2CR->ICDXR =  ((MAC_OFFSET & 0xFF00)>>8);         
	i2cDelay (DELAY_CONST);
	/* Read status */
    uiStatusReg = I2CR->ICSTR;

  	/* On Nack return failure */
  	if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
  	{
    	/* Return to slave receiver, clear nack and bus busy */
    	I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
		I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
        platform_errno = PLATFORM_ERRNO_DEV_NAK;
    	return (I2C_RET_NO_ACK);
  	}
     		      		 
    /* Check for transmit ready */
	i2cDelay (DELAY_CONST);
    while(0 == I2C_REG_STR_FIELD_XRDY(uiStatusReg));	
    
    // setting the second byte of offset
    I2CR->ICDXR = (MAC_OFFSET & 0xFF);              
	i2cDelay (DELAY_CONST);
	/* Read status */
    uiStatusReg = I2CR->ICSTR;

  	/* On Nack return failure */
  	if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
  	{
    	/* Return to slave receiver, clear nack and bus busy */
    	I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
		I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
        platform_errno = PLATFORM_ERRNO_DEV_NAK;
    	return (I2C_RET_NO_ACK);
  	}
     		      		 
    /* Check for transmit ready */
	i2cDelay (DELAY_CONST);
    while(0 == I2C_REG_STR_FIELD_XRDY(uiStatusReg));
    // EEPROM offset written
	
    i2cDelay (DELAY_CONST);	
	// Reading Data 
	I2CR->ICCNT = MACADDRESS_SIZE; 
  		
	/* Set the start bit, begin the master read */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTRCV;
  	i2cDelay (DELAY_CONST);

	for ( uiIndex = 0 ; uiIndex < MACADDRESS_SIZE; uiIndex++ )
    {        		
	   	puchDst[uiIndex] = I2CR->ICDRR ; //Read MAc data	   	
	   	i2cDelay (DELAY_CONST);	
		/* Read status */
      	uiStatusReg = I2CR->ICSTR;      	
      		/* On Nack return failure */
  		if (I2C_REG_STR_FIELD_NACK(uiStatusReg))  
  		{
    		/* Return to slave receiver, clear nack and bus busy */
    		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
			I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
            platform_errno = PLATFORM_ERRNO_DEV_NAK;
    		return (I2C_RET_NO_ACK);
  		}
    
  		/* Check for receive byte ready */ 
      	while(0 == I2C_REG_STR_FIELD_RRDY(uiStatusReg));      	 
	}

	/* The data block has been read. Send the stop bit */
  	I2CR->ICMDR = I2C_VAL_REG_MDR_MSTRCVSTOP;	
	i2cDelay (DELAY_CONST);
		    
    /* Wait for the rrdy and read the dummy byte */
  	uiTimeoutCounter = 0;
  	uchDummyByte = I2CR->ICDRR;
  	do  
  	{
    	uiStatusReg = I2CR->ICSTR;

    	/* Check for receive byte ready */
    	if (I2C_REG_STR_FIELD_RRDY(uiStatusReg))  
    	{	      		
      		i2cDelay (DELAY_CONST);      		
      		uchDummyByte = I2CR->ICDRR;
      		if(uchDummyByte)
      			i2cDelay (DELAY_CONST);
      		uiTimeoutCounter = 0;      		
    	}  
    	else  
    	{  /* rrdy not set */
      		i2cDelay (DELAY_CONST);
      		uiTimeoutCounter += 1;
      		
      		if (uiTimeoutCounter >= I2C_MAX_MASTER_RECEIVE_TIMEOUT)  
      		{
        		/* Return to slave receiver, clear nack and bus busy */
        		I2CR->ICMDR = I2C_VAL_REG_MDR_SLVRCV;
				I2CR->ICSTR = I2C_VAL_REG_STR_ON_FAIL;
                platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;                
        		return (I2C_RET_IDLE_TIMEOUT);
      		}
    	}
  	}while (uiTimeoutCounter != 0);
  	
    return I2C_RET_OK;
}

#endif
