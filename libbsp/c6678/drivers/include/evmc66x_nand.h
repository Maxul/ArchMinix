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
 * File	Name:	evmc66x_nand.h
 *
 * Description:	This is	the header file	for Nand flash API's.
 ******************************************************************************/
#ifndef	NAND_H_	
#define	NAND_H_	

/*********************************
 * Defines and Macros and globals
 *********************************/
/* NAND FLASH ADDRESS */
#define NAND_DATA_ADDR  ((volatile uint8_t*)0x70000000)  /*emif16_cs2_base */
#define NAND_ALE_ADDR   ((volatile uint8_t*)0x70002000)
#define NAND_CMD_ADDR   ((volatile uint8_t*)0x70004000)

#define NAND_TYPE_MASK_0X00000020 		(0x00000020)  

/* NAND FLASH COMMANDS */
#define NAND_LO_PAGE                    (0)     /* Read the lo page CMD */
#define NAND_HI_PAGE                    (1)     /* Read the hi page CMD */
#define NAND_SPARE_AREA_READ            (0x50)  /* Read the Spare area CMD */
					
#define NAND_ADD_00H                    (0x00)  
#define NAND_ADD_08H                    (0x08)
#define NAND_CMD_05H                    (0x05)  /* Random Data Read Command */
#define NAND_CMD_10H                    (0x10)  /* Program Confirm Command */
#define NAND_CMD_30H                    (0x30)
#define NAND_CMD_E0H                    (0xE0)
#define NAND_BLOCK_ERASE                (0x60)  /* Block Erase Command */
#define NAND_ERASE_CONFIRM              (0xD0)  /* Erase Confirm Command */
#define NAND_GET_FEATURES               (0xEE)
#define NAND_OTP_DATA_PROG              (0xA0)
#define NAND_OTP_DATA_PROT              (0xA5)
#define NAND_OTP_DATA_READ              (0xAF)
#define NAND_PAGE_READ                  (0x00)  /* Page Read Command */
#define NAND_PAGE_READ_LAST             (0x3F)  /* Page Read Cache Mode Start Last*/
#define NAND_PAGE_READ_RANDOM           (0x00)
#define NAND_PAGE_READ_SEQUENTIAL       (0x31)  /* page Read Cache mode start */
#define NAND_INT_DATA_MOVE_PROG         (0x85)  /* Program for Internal Data Move */
#define NAND_PROG_PAGE                  (0x80)  /* Program Page Command */
#define NAND_PROG_PAGE_CACHE            (0x80)  /* Program Page command */
#define NAND_RANDOM_DATA_IN             (0x85)  /* Program for internal Data Move */
#define NAND_RANDOM_DATA_READ           (0x00)
#define NAND_INT_DATA_MOVE_READ         (0xA5)
#define NAND_RDID                       (0x90)  /* Read NAND ID Command */
#define NAND_READ_PARAM_PAGE            (0xEC)
#define NAND_STATUS                     (0x70)  /* Read Status command */
#define NAND_READ_UNIQUE_ID             (0xED)
#define NAND_RST                        (0xFF)  /* Reset Command */
#define NAND_RDY                        (0x40)
#define NAND_RDIDADD                    (0x20)

/* Maximum number of ECC bytes per page */
#define NAND_MAX_NUM_ECC_BYTES      10

 /**
  *  Handle to access BOOTCFG registers.
  */
#define hEmif16Cfg     ((CSL_Emif16Regs*)CSL_EMIF16_REGS)
 
 /**
 @}
 */


/****************
 * Defines
 ****************/
#define	PACK_ADDR(col, page, block) \
		((col &	0x000000ff) | ((page & 0x0000001f)<<9)	| ((block & 0x00000fff)	<< 14 ))

// Macros for delay in micro Sec
#define	STD_DELAY					(25)
#define EMIF16_NAND_PROG_TIMEOUT           (100000)
#define EMIF16_NAND_RESET_TIMEOUT          (100000)
#define EMIF16_NAND_BLOCK_ERASE_TIMEOUT    (2000000)
#define EMIF16_WAIT_PIN_POLL_ST_DLY        (10)


// Macros for errors
#define	INVALID_NAND_ADDR			(8)
#define	NAND_PAGE_WRITE_FAIL		(9)
#define	NAND_SPARE_AREA_WRITE_FAIL	(10)
#define	NAND_PAGE_READ_FAIL			(11)
#define	NAND_SPARE_AREA_READ_FAIL	(12)
#define	NAND_BLOCK_ERASE_FAIL		(13)
#define	NAND_ID_READ_ERROR			(14)
#define	NAND_TWO_BIT_ERROR			(15)
#define	INVALID_NAND_DEVICE			(16)

#define	NAND_MAN_ID_NUMONYX			(0x20)
#define	NAND_DEVICE_ID				(0x36)

#define	BYTES_PER_PAGE				(512)
#define	SPARE_BYTES_PER_PAGE		(16)
#define	PAGES_PER_BLOCK				(32)
#define	TOTAL_BYTES_PER_PAGE		(BYTES_PER_PAGE	+ SPARE_BYTES_PER_PAGE)	
#define	BLOCKS_PER_DEVICE			(4096)

// ECC related macros
#define	ECC_BLOCK_SIZE				(256)	// in Bytes
#define	ECC_SPARE_OFFSET			(SPARE_BYTES_PER_PAGE-3*(BYTES_PER_PAGE/ECC_BLOCK_SIZE))

typedef	struct _NAND_ADDR
{
    uint32_t uiColumnAddr;
    uint32_t uiPageAddr;
    uint32_t uiBlockAddr;
} NAND_ADDR;

typedef	struct	{
    uint32_t uiDeviceID;		// DeviceID
    uint32_t uiPageCount;		// Page	count per block
    uint32_t uiPageSize;		// Byte	count per page(include redundant parts)
    uint32_t blk_shift;		// Number of bits by which block address is to be shifted
    uint32_t addr_cycles;		// Number of address cycles
} StdNANDDeviceInfoObj;	

//  NAND_info structure	
typedef	struct	{
    uint32_t uiNandWidth;		// NAND	width 1-&gt;16 bits 0-&gt;8 bits
    uint32_t dev_code;		// NAND_DevTable index
    uint32_t uiPageCount;		// page	count per block
    uint32_t addr_cycles;		// Number of address cycles
    uint32_t big_block;		// 1 - Big block device, 0 - small block device
    uint32_t uiPageSize;		// Number of bytes in a	page including extra data 264/528
    uint32_t blk_shift;		// Number of bits by which block address is to be shifted
    uint32_t page_shift;		// Number of bits by which	page address is	to be shifted
    uint32_t numPage;			// Number of pages where boot loader is	stored
    uint32_t block;			// starting block number where User boot loader	is stored
    uint32_t page;			// starting page number	where boot-loader is stored
} StdNANDBootInfoObj;

/************************
 * Function declarations
 ************************/
uint32_t nandInit(void);
uint32_t readNandPage(PLATFORM_DEVICE_info* pNandInfo, NAND_ADDR address, uint8_t* puchBuffer);
uint32_t writeNandPage(PLATFORM_DEVICE_info* pNandInfo, NAND_ADDR address,	uint8_t* puchBuffer);
uint32_t enableNandECC(uint32_t uiUseSoftEcc);
uint32_t nandFlashBlockErase(PLATFORM_DEVICE_info* pNandInfo, uint32_t uiBlockNumber);
uint32_t NandGetDetails(PLATFORM_DEVICE_info* pNandInfo);
uint32_t NandReadSpareArea(PLATFORM_DEVICE_info* pNandInfo, uint32_t uiBlkAddr, uint32_t uiPage, uint8_t *pBuffer);
uint32_t NandWriteSpareArea (PLATFORM_DEVICE_info* pNandInfo, uint32_t uiBlkAddr, uint32_t uiPage, uint8_t *pBuffer);

/************************
 * Function declarations
 ************************/

#endif // NAND_H_





