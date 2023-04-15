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
 * File Name:   evmc66x_spi.c
 *
 * Description: This file implements SPI driver for the NOR flash
 *  
 *****************************************************************************/
 
/************************
 * Include Files
 ************************/
#include "platform_internal.h"


static uint32_t data1_reg_val;

static void 
spi_delay
( 
    uint32_t delay 
)
{
    volatile uint32_t i;

    for ( i = 0 ; i < delay ; i++ ){ };
}


/******************************************************************************
 * 
 * Function:    spi_claim  
 *
 * Description: This function claims the SPI bus in the SPI controller 
 *
 * Parameters:  Uint32 cs       - Chip Select number for the slave SPI device
 *              Uint32 freq     - SPI clock frequency  
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_claim
(
    uint32_t      cs,
    uint32_t      freq
)
{
    uint32_t scalar;

    PLIBSPILOCK()

    /* Enable the SPI hardware */
    SPI_SPIGCR0 = CSL_SPI_SPIGCR0_RESET_IN_RESET;
    spi_delay (2000);
    SPI_SPIGCR0 = CSL_SPI_SPIGCR0_RESET_OUT_OF_RESET;

    /* Set master mode, powered up and not activated */
    SPI_SPIGCR1 =   (CSL_SPI_SPIGCR1_MASTER_MASTER << CSL_SPI_SPIGCR1_MASTER_SHIFT)   |
                    (CSL_SPI_SPIGCR1_CLKMOD_INTERNAL << CSL_SPI_SPIGCR1_CLKMOD_SHIFT);
    
    
    /* CS0, CS1, CLK, Slave in and Slave out are functional pins */
    if (cs == 0) {
        SPI_SPIPC0 =    (CSL_SPI_SPIPC0_SCS0FUN0_SPI << CSL_SPI_SPIPC0_SCS0FUN0_SHIFT) |
                        (CSL_SPI_SPIPC0_CLKFUN_SPI << CSL_SPI_SPIPC0_CLKFUN_SHIFT)     |
                        (CSL_SPI_SPIPC0_SIMOFUN_SPI << CSL_SPI_SPIPC0_SIMOFUN_SHIFT)   |
                        (CSL_SPI_SPIPC0_SOMIFUN_SPI << CSL_SPI_SPIPC0_SOMIFUN_SHIFT);
    } else if (cs == 1) {
        SPI_SPIPC0 =    ((CSL_SPI_SPIPC0_SCS0FUN1_SPI << CSL_SPI_SPIPC0_SCS0FUN1_SHIFT) |
                        (CSL_SPI_SPIPC0_CLKFUN_SPI << CSL_SPI_SPIPC0_CLKFUN_SHIFT)     |
                        (CSL_SPI_SPIPC0_SIMOFUN_SPI << CSL_SPI_SPIPC0_SIMOFUN_SHIFT)   |
                        (CSL_SPI_SPIPC0_SOMIFUN_SPI << CSL_SPI_SPIPC0_SOMIFUN_SHIFT)) & 0xFFFF;
    }
    
    /* setup format */
    scalar = ((SPI_MODULE_CLK / freq) - 1 ) & 0xFF;

    if ( cs == 0) {
        SPI_SPIFMT0 =   (8 << CSL_SPI_SPIFMT_CHARLEN_SHIFT)               |
                        (scalar << CSL_SPI_SPIFMT_PRESCALE_SHIFT)                      |
                        (CSL_SPI_SPIFMT_PHASE_DELAY << CSL_SPI_SPIFMT_PHASE_SHIFT)     |
                        (CSL_SPI_SPIFMT_POLARITY_LOW << CSL_SPI_SPIFMT_POLARITY_SHIFT) |
                        (CSL_SPI_SPIFMT_SHIFTDIR_MSB << CSL_SPI_SPIFMT_SHIFTDIR_SHIFT);
    }else if ( cs == 1) {
        SPI_SPIFMT0 =   (16 << CSL_SPI_SPIFMT_CHARLEN_SHIFT)               |
                        (scalar << CSL_SPI_SPIFMT_PRESCALE_SHIFT)                      |
                        (CSL_SPI_SPIFMT_PHASE_NO_DELAY << CSL_SPI_SPIFMT_PHASE_SHIFT)     |
                        (CSL_SPI_SPIFMT_POLARITY_LOW << CSL_SPI_SPIFMT_POLARITY_SHIFT) |
                        (CSL_SPI_SPIFMT_SHIFTDIR_MSB << CSL_SPI_SPIFMT_SHIFTDIR_SHIFT);
    }
    
    /* hold cs active at end of transfer until explicitly de-asserted */
    data1_reg_val = (CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT) |
                    (0x02 << CSL_SPI_SPIDAT1_CSNR_SHIFT);
     if (cs == 0) {
         SPI_SPIDAT1 =   (CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT) |
                         (0x02 << CSL_SPI_SPIDAT1_CSNR_SHIFT);
     } 

    /* including a minor delay. No science here. Should be good even with
    * no delay
    */
    if (cs == 0) {
        SPI_SPIDELAY =  (8 << CSL_SPI_SPIDELAY_C2TDELAY_SHIFT) |
                        (8 << CSL_SPI_SPIDELAY_T2CDELAY_SHIFT);
        /* default chip select register */
        SPI_SPIDEF  = CSL_SPI_SPIDEF_RESETVAL;        
    } else if (cs == 1) {    
        SPI_SPIDELAY =  (6 << CSL_SPI_SPIDELAY_C2TDELAY_SHIFT) |
                        (3 << CSL_SPI_SPIDELAY_T2CDELAY_SHIFT);
    }
    
    /* no interrupts */
    SPI_SPIINT0 = CSL_SPI_SPIINT0_RESETVAL;
    SPI_SPILVL  = CSL_SPI_SPILVL_RESETVAL;

    /* enable SPI */
    SPI_SPIGCR1 |= ( CSL_SPI_SPIGCR1_ENABLE_ENABLE << CSL_SPI_SPIGCR1_ENABLE_SHIFT );

    if (cs == 1) {
        SPI_SPIDAT0 = 1 << 15; 
        spi_delay (10000);        
        /* Read SPIFLG, wait untill the RX full interrupt */
        if ( (SPI_SPIFLG & (CSL_SPI_SPIFLG_RXINTFLG_FULL<<CSL_SPI_SPIFLG_RXINTFLG_SHIFT)) ) {                
            /* Read one byte data */
            scalar = SPI_SPIBUF & 0xFF;
            /* Clear the Data */
            SPI_SPIBUF = 0;
        }
        else {
            /* Read one byte data */
            scalar = SPI_SPIBUF & 0xFF;
            return SPI_EFAIL;
        }
    }
    return SPI_EOK;
}

/******************************************************************************
 * 
 * Function:    spi_release  
 *
 * Description: This function releases the bus in SPI controller 
 *
 * Parameters:  None
 *
 * Return Value: None
 * 
 ******************************************************************************/
void 
spi_release
(
    void
)
{
    /* Disable the SPI hardware */
    SPI_SPIGCR1 = CSL_SPI_SPIGCR1_RESETVAL;

    PLIBSPIRELEASE()
}

/******************************************************************************
 * 
 * Function:    spi_xfer  
 *
 * Description: This function sends and receives 8-bit data serially
 *
 * Parameters:  uint32_t nbytes   - Number of bytes of the TX data
 *              uint8_t* data_out - Pointer to the TX data
 *              uint8_t* data_in  - Pointer to the RX data
 *              Bool terminate  - TRUE: terminate the transfer, release the CS
 *                                FALSE: hold the CS
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_xfer
(
    uint32_t              nbytes,
    uint8_t*              data_out,
    uint8_t*              data_in,
    Bool                terminate
)
{
    uint32_t          i, buf_reg;
    uint8_t*          tx_ptr = data_out;
    uint8_t*          rx_ptr = data_in;
    
    
    /* Clear out any pending read data */
    SPI_SPIBUF;
    
    for (i = 0; i < nbytes; i++) 
    {
        /* Wait untill TX buffer is not full */
        while( SPI_SPIBUF & CSL_SPI_SPIBUF_TXFULL_MASK );
        
        /* Set the TX data to SPIDAT1 */
        data1_reg_val &= ~0xFFFF;
        if(tx_ptr) 
        {
            data1_reg_val |= *tx_ptr & 0xFF;
            tx_ptr++;
        }
        
        /* Write to SPIDAT1 */
        if((i == (nbytes -1)) && (terminate)) 
        {
            /* Release the CS at the end of the transfer when terminate flag is TRUE */
            SPI_SPIDAT1 = data1_reg_val & ~(CSL_SPI_SPIDAT1_CSHOLD_ENABLE << CSL_SPI_SPIDAT1_CSHOLD_SHIFT);
        } else 
        {
            SPI_SPIDAT1 = data1_reg_val;
        }
        
        
        /* Read SPIBUF, wait untill the RX buffer is not empty */
        while ( SPI_SPIBUF & ( CSL_SPI_SPIBUF_RXEMPTY_MASK ) );
        
        /* Read one byte data */
        buf_reg = SPI_SPIBUF;
        if(rx_ptr) 
        {
            *rx_ptr = buf_reg & 0xFF;
            rx_ptr++;
        }
    }
    
    return SPI_EOK;
    
}


/******************************************************************************
 * 
 * Function:    spi_cmd  
 *
 * Description: This function sends a single byte command and receives response data
 *
 * Parameters:  uint8_t  cmd      - Command sent to the NOR flash
 *              uint8_t* response - Pointer to the RX response data
 *              uint32_t len      - Lenght of the response in bytes
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_cmd
(
    uint8_t               cmd,
    uint8_t*              response,
    uint32_t              len
)
{
    Bool        flags = FALSE;
    uint32_t      ret;
    
    if (len == 0)
    {
        flags = TRUE;
    }
    
    /* Send the command byte */
    ret = spi_xfer(1, &cmd, NULL, flags);
    if (ret) 
    {
    	IFPRINT (platform_write("SF: Failed to send command %02x: %d\n", cmd, ret));
        return ret;
    }
    
    /* Receive the response */
    if (len) 
    {
        ret = spi_xfer(len, NULL, response, TRUE);
        if (ret)
        {
        	IFPRINT (platform_write("SF: Failed to read response (%zu bytes): %d\n",  len, ret));
        }
    }
    
    return ret;
}

/******************************************************************************
 * 
 * Function:    spi_cmd_read  
 *
 * Description: This function sends a read command and reads data from the flash
 *
 * Parameters:  uint8_t  cmd      - Command sent to the NOR flash
 *              uint32_t cmd_len  - Length of the command in bytes
 *              uint8_t* dat      - Pointer to the data read
 *              uint32_t data_len - Lenght of the data read in bytes
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_cmd_read
(
    uint8_t*              cmd,
    uint32_t              cmd_len,
    uint8_t*              data,
    uint32_t              data_len
)
{
    Bool        flags = FALSE;
    uint32_t      ret;
    
    if (data_len == 0)
    {
        flags = TRUE;
    }

    /* Send read command */
    ret = spi_xfer(cmd_len, cmd, NULL, flags);
    if (ret) 
    {
    	IFPRINT (platform_write("SF: Failed to send read command (%zu bytes): %d\n",
               cmd_len, ret));
    } 
    else if (data_len != 0) 
    {
        /* Read data */
        ret = spi_xfer(data_len, NULL, data, TRUE);
        if (ret)
        {
        	IFPRINT (platform_write("SF: Failed to read %zu bytes of data: %d\n",
                   data_len, ret));
        }
    }
    
    return ret;
}

/******************************************************************************
 * 
 * Function:    spi_cmd_write  
 *
 * Description: This function sends a write command and writes data to the flash
 *
 * Parameters:  uint8_t  cmd      - Command sent to the NOR flash
 *              uint32_t cmd_len  - Length of the command in bytes
 *              uint8_t* dat      - Pointer to the data to be written
 *              uint32_t data_len - Lenght of the data in bytes
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_cmd_write
(
    uint8_t*        cmd,
    uint32_t        cmd_len,
    uint8_t*        data,
    uint32_t        data_len
)
{
    Bool           flags = FALSE;
    uint32_t         ret;
    
    if (data_len == 0)
    {
        flags = TRUE;
    }
    
    /* Send write command */
    ret = spi_xfer(cmd_len, cmd, NULL, flags);
    if (ret) 
    {
    	IFPRINT (platform_write("SF: Failed to send write command (%zu bytes): %d\n",
            cmd_len, ret));
    } 
    else if (data_len != 0) 
    {
        /* Write data */
        ret = spi_xfer(data_len, data, NULL, TRUE);
        if (ret)
        {
        	IFPRINT (platform_write("SF: Failed to write %zu bytes of data: %d\n",
            data_len, ret));
        }
    }
    
    return ret;
}


/******************************************************************************
 * 
 * Function:    spi_read_word  
 *
 * Description: This function sends a read command and reads data in 16-bit data format  
 *
 * Parameters:  uint16_t* cmd_buf  - Pointer to the command sent
 *              uint32_t cmd_len   - Length of the command in words
 *              uint16_t* data_buf - Pointer to the data read
 *              uint32_t data_len  - Lenght of the data read in words
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_read_word
(
    uint16_t*             cmd_buf,
    uint32_t              cmd_len,
    uint16_t*             data_buf,
    uint32_t              data_len
)
{
    uint32_t          data1_reg;
    uint16_t*         tx_ptr = cmd_buf;
    uint16_t*         rx_ptr = data_buf;
    
	/* disable the SPI communication by setting 
	 * the SPIGCR1.ENABLE to 0 
	 * 
	 * 
	 * SPIGCR1
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 24 	ENABLE 	0			SPI disable
	 * ============================================
	 */
	data1_reg = 0x1 << 24;
	data1_reg = ~data1_reg;
    SPI_SPIGCR1 &= data1_reg;

	/*
	 * clean TX data into SPIDAT0
	 * 
	 * SPIDAT0
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 15-0 TXDATA 	0-FFFFh 	SPI transmit data
	 * ============================================
	 * 
	 */

    SPI_SPIDAT0 = 0;

	/* 8.
	 * Enable the SPI communication by setting 
	 * the SPIGCR1.ENABLE to 1 
	 * 
	 * 
	 * SPIGCR1
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 24 	ENABLE 	1			SPI enable
	 * ============================================
	 */

	data1_reg = 0x1 << 24;
    SPI_SPIGCR1 = (SPI_SPIGCR1 | data1_reg);    
    
    {
        {
            SPI_SPIDAT0 = *tx_ptr;
            spi_delay(10000);
        }

        /* Read SPIFLG, wait untill the RX full interrupt */
        if ( (SPI_SPIFLG & (CSL_SPI_SPIFLG_RXINTFLG_FULL<<CSL_SPI_SPIFLG_RXINTFLG_SHIFT)) ) {                
            /* Read one byte data */
            *rx_ptr = SPI_SPIBUF & 0xFF;
            SPI_SPIBUF = 0;
        }
        else {
            return SPI_EFAIL;
        }
        
    }
    
    return SPI_EOK;

}

/******************************************************************************
 * 
 * Function:    spi_write_word  
 *
 * Description: This function sends a write command and writes data in 16-bit data format 
 *
 * Parameters:  uint16_t* cmd_buf  - Pointer to the command sent
 *              uint32_t cmd_len   - Length of the command in bytes
 *              uint16_t* data_buf - Pointer to the data read
 *              uint32_t data_len  - Lenght of the data read in bytes
 *
 * Return Value: error status
 * 
 ******************************************************************************/
SPI_STATUS 
spi_write_word
(
    uint16_t*             cmd_buf,
    uint32_t              cmd_len,
    uint16_t*             data_buf,
    uint32_t              data_len
)
{
    uint32_t          data1_reg;
    uint16_t*         tx_ptr = cmd_buf;

	/* disable the SPI communication by setting 
	 * the SPIGCR1.ENABLE to 0 
	 * 
	 * 
	 * SPIGCR1
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 24 	ENABLE 	0			SPI disable
	 * ============================================
	 */
	data1_reg = 0x1 << 24;
	data1_reg = ~data1_reg;
    SPI_SPIGCR1 &= data1_reg;

	/*
	 * clean TX data into SPIDAT0
	 * 
	 * SPIDAT0
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 15-0 TXDATA 	0-FFFFh 	SPI transmit data
	 * ============================================
	 * 
	 */

    SPI_SPIDAT0 = 0;

	/* 8.
	 * Enable the SPI communication by setting 
	 * the SPIGCR1.ENABLE to 1 
	 * 
	 * 
	 * SPIGCR1
	 * ============================================
	 * Bit 	Field 	Value 		Description
	 * 24 	ENABLE 	1			SPI enable
	 * ============================================
	 */

	data1_reg = 0x1 << 24;
    SPI_SPIGCR1 = (SPI_SPIGCR1 | data1_reg);    
    
    {
        {
            SPI_SPIDAT0 = *tx_ptr;
            spi_delay(10000);
        }

        /* Read SPIFLG, wait untill the RX full interrupt */
        if ( (SPI_SPIFLG & (CSL_SPI_SPIFLG_TXINTFLG_EMPTY<<CSL_SPI_SPIFLG_TXINTFLG_SHIFT)) ) {                
            /* Clear the SPIBUF */
            SPI_SPIBUF = 0;
            return SPI_EOK;
        }
        else {
            return SPI_EFAIL;
        }
        
    }
}


