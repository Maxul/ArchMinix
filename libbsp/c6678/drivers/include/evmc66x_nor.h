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
 * File Name:   evmc66x_nor.h
 *
 * Description: This is the header file for NOR flash API's.
 *
 ******************************************************************************/
#ifndef NOR_H_
#define NOR_H_

/* ------------------------------------------------------------------------ *
 *  SPI NOR Definitions                                                     *
 * ------------------------------------------------------------------------ */
#define SPI_NOR_MANUFACTURE_ID     0x20     /* Numonyx N25Q128 Manufacture ID assigned by JEDEC */
#define SPI_NOR_SECTOR_COUNT       256      /* Total number of data sectors on the NOR */
#define SPI_NOR_SECTOR_SIZE        65536    /* Number of bytes in a data sector */
#define SPI_NOR_BOOT_SECTOR_COUNT  8        /* Total number of boot data sectors on the NOR */
#define SPI_NOR_PAGE_COUNT         65536    /* Total number of data pages on the NOR */
#define SPI_NOR_PAGE_SIZE          256      /* Number of data pages in a data sector */
#define SPI_NOR_OTP_BYTES          64       /* Number of OTP data bytes */
#define SPI_NOR_MAX_FLASH_SIZE     (SPI_NOR_SECTOR_COUNT*SPI_NOR_SECTOR_SIZE) /* Total device size in Bytes 16Mbytes */

/* ------------------------------------------------------------------------ *
 *  SPI NOR Commands                                                        *
 * ------------------------------------------------------------------------ */
#define SPI_NOR_CMD_RDID           0x9f     /* Read manufacture/device ID */
#define SPI_NOR_CMD_WREN           0x06     /* Write enable */
#define SPI_NOR_CMD_WRDI           0x04     /* Write Disable */
#define SPI_NOR_CMD_RDSR           0x05     /* Read Status Register */
#define SPI_NOR_CMD_WRSR           0x01     /* Write Status Register */
#define SPI_NOR_CMD_READ           0x03     /* Read data */
#define SPI_NOR_CMD_FAST_READ      0x0B     /* Read data bytes at higher speed */
#define SPI_NOR_CMD_PP             0x02     /* Page Program */
#define SPI_NOR_CMD_SSE            0x20     /* Sub Sector Erase */
#define SPI_NOR_CMD_SE             0xd8     /* Sector Erase */
#define SPI_NOR_CMD_BE             0xc7     /* Bulk Erase */

#define SPI_NOR_SR_WIP             (1 << 0)   /* Status Register, Write-in-Progress bit */
#define SPI_NOR_BE_SECTOR_NUM      (Uint32)-1 /* Sector number set for bulk erase */

/* Read status Write In Progress timeout */
#define SPI_NOR_PROG_TIMEOUT          100000
#define SPI_NOR_PAGE_ERASE_TIMEOUT    500000
#define SPI_NOR_SECTOR_ERASE_TIMEOUT  100000000

/* ------------------------------------------------------------------------ *
 *  NOR Error Status                                                        *
 * ------------------------------------------------------------------------ */
#define NOR_STATUS              Uint32          /* NOR status error code */
#define NOR_EFAIL               (NOR_STATUS)-1  /* General failure code */
#define NOR_EOK                 0               /* General success code */

/* ------------------------------------------------------------------------ *
 *  Function declarations                                                   *
 * ------------------------------------------------------------------------ */
NOR_STATUS 
nor_init
(   
    void
);

NOR_STATUS
nor_read
(
	PLATFORM_DEVICE_info*   p_device,
    Uint32      addr, 
    Uint32      len, 
    Uint8*      buf
);

NOR_STATUS 
nor_write
(
	PLATFORM_DEVICE_info*   p_device,
    Uint32      addr, 
    Uint32      len, 
    Uint8*      buf
);

NOR_STATUS 
nor_erase
(
	PLATFORM_DEVICE_info*   p_device,
    Uint32  sector_number
);

uint32_t
nor_get_details
(
		PLATFORM_DEVICE_info* nor_info
);

#endif /* NOR_H_ */
