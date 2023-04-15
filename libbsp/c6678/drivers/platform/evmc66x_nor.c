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

#include "platform_internal.h"

#if (PLATFORM_NOR_IN)

/******************************************************************************
 *
 * File Name:   evmc66x_nor.c
 *
 * Description: This file implements NOR flash driver for Nymonyx N25Q128 NOR flash
 *  
 *****************************************************************************/
 
/************************
 * Include Files
 ************************/

#if (PLATFORM_NOR_WRITE_IN)
static NOR_STATUS 
nor_wait_ready
(
    uint32_t  timeout
)
{
    NOR_STATUS  ret;
    uint8_t       status;
    uint8_t       cmd = SPI_NOR_CMD_RDSR;
    
    do 
    {

        /* Send Read Status command */
        ret = spi_xfer(1, &cmd, NULL, FALSE);
        if (ret)
        {
            return ret;
        }
        
        /* Read status value */
        ret = spi_xfer(1, NULL, &status, TRUE);
        if (ret)
        {
            return ret;
        }
        
        if ((status & SPI_NOR_SR_WIP) == 0)
        {
            break;
        }
        
        timeout--;
        if (!timeout)
        {
            break;
        }
        
    } while (TRUE);
    
    if ((status & SPI_NOR_SR_WIP) == 0)
        return NOR_EOK;
    
    /* Timed out */
    return NOR_EFAIL;
}
#endif

/******************************************************************************
 * 
 * Function:    nor_get_details  
 *
 * Description: This function the details of NOR flash device. 
 *
 * Parameters:  NOR_INFO nor_info - structure for information
 *
 * Return Value: status
 ******************************************************************************/
uint32_t
nor_get_details
(
		PLATFORM_DEVICE_info*   nor_info
)
{
    uint32_t uiStatus = SUCCESS, ret;
    uint8_t               idcode[3];     /* Initialize the SPI interface */
    
   /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);
        
    /* Read the ID codes */
    ret = spi_cmd(SPI_NOR_CMD_RDID, idcode, sizeof(idcode));
    if (ret) {
        spi_release();
        platform_errno = PLATFORM_ERRNO_NOR;
        return FAIL;
    }
      
    /* Get the actuals */
    nor_info->manufacturer_id   = idcode[0];
    nor_info->device_id         = (idcode[1] << SPI_NOR_CHAR_LENTH) | (idcode[2]);

    /* No blocks are bad for NOR.. for now */
    nor_info->bblist = NULL;

    spi_release();

    return uiStatus; 
}

/******************************************************************************
 * 
 * Function:    nor_init  
 *
 * Description: This function configures the SPI controller communication
 *              between the DSP and the NOR flash 
 *
 * Parameters:  None
 *
 * Return Value: error status
 * 
 ******************************************************************************/
NOR_STATUS 
nor_init
(   
    void
)
{
    NOR_STATUS          ret;
    uint8_t               idcode[3];     /* Initialize the SPI interface */

    /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);
    
    /* Read the ID codes */
    ret = spi_cmd(SPI_NOR_CMD_RDID, idcode, sizeof(idcode));
    if (ret) 
    {
        IFPRINT (platform_write( "nor_init: Error in reading the idcode\n"));
        spi_release();
        platform_errno = PLATFORM_ERRNO_NOR;
        return NOR_EFAIL;
    }
     
    IFPRINT (platform_write("SF: Got idcode %02x %02x %02x\n", idcode[0], idcode[1], idcode[2]));

    if (idcode[0] != SPI_NOR_MANUFACTURE_ID) {
        /* Expected Manufacturer ID does not match */
        spi_release();
        platform_errno = PLATFORM_ERRNO_BADFLASHDEV;
        return NOR_EFAIL;
    }
       
    spi_release();
    return NOR_EOK; 
}

/******************************************************************************
 * 
 * Function:    nor_read  
 *
 * Description: This function reads data from the NOR flash 
 *
 * Parameters:  uint32_t addr     - Byte address of the NOR flash
 *              uint32_t len      - Lenth in bytes to read
 *              uint8_t* buf      - Pointer of the buffer to store the bytes read
 *
 * Return Value: error status
 * 
 ******************************************************************************/
#if (PLATFORM_NOR_READ_IN)
NOR_STATUS
nor_read
(
	PLATFORM_DEVICE_info*   p_device,
    uint32_t      addr,
    uint32_t      len,
    uint8_t*      buf
)
{
    uint8_t       cmd[4];
    NOR_STATUS  ret_val;

    /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);

    /* Validate address input */
    if(addr + len > SPI_NOR_MAX_FLASH_SIZE) 
    {
    	platform_errno = PLATFORM_ERRNO_FLASHADDR;
        spi_release();
        return NOR_EFAIL;
    }

    /* Initialize the command to be sent serially */
    cmd[0]              = SPI_NOR_CMD_READ;
    cmd[1]              = (uint8_t)(addr>>16);
    cmd[2]              = (uint8_t)(addr>>8);
    cmd[3]              = (uint8_t)addr;

    ret_val = (spi_cmd_read(cmd, 4, buf, len)); 

    spi_release();
    return (ret_val);
}
#endif

/******************************************************************************
 * 
 * Function:    nor_write  
 *
 * Description: This function writes data to the NOR flash 
 *
 * Parameters:  uint32_t addr     - Byte address of the NOR flash
 *              uint32_t len      - Lenth in bytes to write
 *              uint8_t* buf      - Pointer of the buffer to store the write data
 *
 * Return Value: error status
 * 
 ******************************************************************************/
#if (PLATFORM_NOR_WRITE_IN)
NOR_STATUS 
nor_write
(
	PLATFORM_DEVICE_info*   p_device,
    uint32_t      addr,
    uint32_t      len,
    uint8_t*      buf
)
{
    uint32_t      page_addr;
    uint32_t      byte_addr;
    uint32_t      page_size;
    uint32_t      loopCount;

    uint32_t      chunk_len;
    uint32_t      actual;
    uint32_t      ret;
    uint8_t       cmd[4];

    /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);
    
    /* Validate address input */
    if(addr + len > SPI_NOR_MAX_FLASH_SIZE) 
    {
    	platform_errno = PLATFORM_ERRNO_NOFREEBLOCKS;
        spi_release();
        return NOR_EFAIL;
    }

    page_size = SPI_NOR_PAGE_SIZE;
    page_addr = addr / page_size;
    byte_addr = addr & (SPI_NOR_PAGE_SIZE - 1); /* % page_size; */
    
    ret = NOR_EOK;
    for (actual = 0; actual < len; actual += chunk_len) 
    {
        /* Send Write Enable command */
        ret = spi_cmd(SPI_NOR_CMD_WREN, NULL, 0);
        if (ret) 
        {
        	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
            spi_release();
            return NOR_EFAIL;
        }

        /* Send Page Program command */
        chunk_len = ((len - actual) < (page_size - byte_addr) ? 
            (len - actual) : (page_size - byte_addr));
        
        cmd[0]  = SPI_NOR_CMD_PP;
        cmd[1]  = (uint8_t)(addr>>16);
        cmd[2]  = (uint8_t)(addr>>8);
        cmd[3]  = (uint8_t)addr;
        
        ret = spi_cmd_write(cmd, 4, buf + actual, chunk_len);
        if (ret) 
        {
        	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
            spi_release();            
            return NOR_EFAIL;
        }
        
        ret = nor_wait_ready(SPI_NOR_PROG_TIMEOUT);
        if (ret) 
        {
        	platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
            spi_release();
            return NOR_EFAIL;
        }
        
        page_addr++;
        addr += chunk_len;
        byte_addr = 0;

        loopCount = 4000;
        while (loopCount--) {
            asm("   NOP");
        }

    }
    
    spi_release();    
    return ((NOR_STATUS) ret);
}
#endif

/******************************************************************************
 * 
 * Function:    nor_erase  
 *
 * Description: This function writes data to the NOR flash 
 *
 * Parameters:  uint32_t  sector_number - Sector number to erase,
 *                                      if sector_number = -1, do bulk erase
 *
 * Return Value: error status
 * 
 ******************************************************************************/
#if (PLATFORM_NOR_WRITE_IN)
NOR_STATUS 
nor_erase
(
	PLATFORM_DEVICE_info*   p_device,
    uint32_t  sector_number
)
{
    NOR_STATUS  ret;
    uint8_t       cmd[4];
    uint32_t      cmd_len;
    uint32_t      address;

    /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);
    
    /*
    * This function currently uses sector erase only.
    * probably speed things up by using bulk erase
    * when possible.
    */
    
    if (sector_number == SPI_NOR_BE_SECTOR_NUM)
    {
        cmd[0]  = SPI_NOR_CMD_BE;
        cmd_len = 1;

    }
    else if (sector_number >= SPI_NOR_SECTOR_COUNT)
    {
    	platform_errno = PLATFORM_ERRNO_NORADDR;
        return NOR_EFAIL;
    }
    else
    {
        address = sector_number * SPI_NOR_SECTOR_SIZE;
        cmd[0]  = SPI_NOR_CMD_SE;
        cmd[1] = (address >> 16) & 0xff;
        cmd[2] = (address >>  8) & 0xff;
        cmd[3] = (address >>  0) & 0xff;

        cmd_len = 4;
    }

    /* Send Write Enable command */
    ret = spi_cmd(SPI_NOR_CMD_WREN, NULL, 0);
    if (ret) 
    {
    	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
        spi_release();
        return NOR_EFAIL;
    }
    
    ret = spi_cmd_write(cmd, cmd_len, NULL, 0);
    if (ret) 
    {
    	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
        spi_release();
        return NOR_EFAIL;
    }
    
    ret = nor_wait_ready(SPI_NOR_SECTOR_ERASE_TIMEOUT);
    if (ret) 
    {
    	platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
        spi_release();
        return NOR_EFAIL;
    }
    spi_release();
    return ret;
}

NOR_STATUS
nor_erase_subsector
(
	PLATFORM_DEVICE_info*   p_device,
	uint32_t      addr
)
{
    NOR_STATUS  ret;
    uint8_t       cmd[4];
    uint32_t      cmd_len;
    uint32_t      address;

    /* Claim the SPI controller */
    spi_claim(SPI_NOR_CS, SPI_MAX_FREQ);

    /*
    * This function currently uses sector erase only.
    * probably speed things up by using bulk erase
    * when possible.
    */

    if (0 <= addr)
    {
        address = addr;
        cmd[0]  = SPI_NOR_CMD_SSE;
        cmd[1] = (address >> 16) & 0xff;
        cmd[2] = (address >>  8) & 0xff;
        cmd[3] = (address >>  0) & 0xff;

        cmd_len = 4;
    }

    /* Send Write Enable command */
    ret = spi_cmd(SPI_NOR_CMD_WREN, NULL, 0);
    if (ret)
    {
    	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
        spi_release();
        return NOR_EFAIL;
    }

    ret = spi_cmd_write(cmd, cmd_len, NULL, 0);
    if (ret)
    {
    	platform_errno = PLATFORM_ERRNO_DEV_FAIL;
        spi_release();
        return NOR_EFAIL;
    }

    ret = nor_wait_ready(SPI_NOR_SECTOR_ERASE_TIMEOUT);
    if (ret)
    {
    	platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
        spi_release();
        return NOR_EFAIL;
    }
    spi_release();
    return ret;
}
#endif

#endif /*(PLATFORM_NOR_IN)*/
