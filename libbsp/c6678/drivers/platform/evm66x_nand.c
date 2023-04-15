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
 * File	Name:       evm66x_nand.c
 *
 * Description: This contains   TMS320C6678 specific nand functions.
 * 
 * 
 ******************************************************************************/
#include "platform_internal.h"

#if (PLATFORM_NAND_IN)

/************************
 * Include Files
 ************************/

/************************
 * Globals
 ************************/

/************************
 * Defines and Macros
 ************************/

#pragma DATA_SECTION(nandPageBuf,"platform_lib");
static uint8_t nandPageBuf[BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE];

/************************
 * Globals
 ************************/

/******************************************************************************
 *
 * Function:    NandCmdSet
 *
 * Description: This function is used to indicates command cycle occurring,
 *              and to send command to NAND device
 *
 * Parameters:  uint32_t cmd - Command to NAND
 *
 * Return Value: None
 *
 *****************************************************************************/
static void NandCmdSet(uint32_t cmd) {
	/* 8-bit NAND */
	uint8_t *cle_addr = (uint8_t *) NAND_CMD_ADDR;

	*cle_addr = cmd;
}

/******************************************************************************
 *
 * Function:    NandAleSet
 *
 * Description: This function is used to indicates Address cycle occurring,
 *              and to send address value to NAND device
 *
 * Parameters:  uint32_t addr - Address to NAND
 *
 * Return Value: None
 *
 *****************************************************************************/
static void NandAleSet(uint32_t addr) {
	/* 8-bit NAND */
	uint8_t *ale_addr = (uint8_t *) NAND_ALE_ADDR;

	*ale_addr = addr;

	return;
}

/******************************************************************************
 *
 * Function:    NandWaitRdy
 *
 * Description: This function waits for the NAND status to be ready
 *
 * Parameters:  uint32_t in_timeout - time out value in micro seconds
 *
 * Return Value: Failure if Ready Pin is not high for prescribed time
 *
 *****************************************************************************/
static uint32_t NandWaitRdy(uint32_t in_timeout) {
	uint32_t count = 0;

	do {
		platform_delay(1);

		if ((CSL_FEXT(hEmif16Cfg->NANDFSR, EMIF16_NANDFSR_WAITSTAT) & 1) == 1) {
			break;
		}
		count++;
	} while (count < in_timeout);

	if (count >= in_timeout)
		return FAIL;
	else
		return SUCCESS;
}

#if (PLATFORM_NAND_READ_IN | PLATFORM_NAND_WRITE_IN)

/* Read raw ECC code after writing to NAND. */
static void NandRead4bitECC(uint32_t *code) {
	uint32_t mask = 0x03ff03ff;

	code[0] = hEmif16Cfg->NANDF4BECC1R & mask;
	code[1] = hEmif16Cfg->NANDF4BECC2R & mask;
	code[2] = hEmif16Cfg->NANDF4BECC3R & mask;
	code[3] = hEmif16Cfg->NANDF4BECC4R & mask;
}

static void NandCalculate4bitECC(uint32_t read, uint8_t *ecc_code) {
	uint32_t i, raw_ecc[4], *p;

	/* After a read, terminate ECC calculation by a dummy read
	 * of some 4-bit ECC register.  ECC covers everything that
	 * was read; correct() just uses the hardware state, so
	 * ecc_code is not needed.
	 */
	if (read) {
		i = hEmif16Cfg->NANDF4BECC1R;
		return;
	}

	/* Pack eight raw 10-bit ecc values into ten bytes, making
	 * two passes which each convert four values (in upper and
	 * lower halves of two 32-bit words) into five bytes.  The
	 * ROM boot loader uses this same packing scheme.
	 */
	NandRead4bitECC(raw_ecc);
	for (i = 0, p = raw_ecc; i < 2; i++, p += 2) {
		*ecc_code++ = p[0] & 0xff;
		*ecc_code++ = ((p[0] >> 8) & 0x03) | ((p[0] >> 14) & 0xfc);
		*ecc_code++ = ((p[0] >> 22) & 0x0f) | ((p[1] << 4) & 0xf0);
		*ecc_code++ = ((p[1] >> 4) & 0x3f) | ((p[1] >> 10) & 0xc0);
		*ecc_code++ = (p[1] >> 18) & 0xff;
	}

	return;
}

/* Correct up to 4 bits in data we just read, using state left in the
 * hardware plus the ecc_code computed when it was first written.
 */
static int32_t NandCorrect4bitECC(uint8_t *data, uint8_t *ecc_code) {
	int32_t i;
	uint16_t ecc10[8];
	uint16_t *ecc16;
	uint32_t syndrome[4];
	uint32_t num_errors, corrected;

	/* All bytes 0xff?  It's an erased page; ignore its ECC. */
	for (i = 0; i < 10; i++) {
		if (ecc_code[i] != 0xff)
			goto compare;
	}
	return 0;

	compare:
	/* Unpack ten bytes into eight 10 bit values.  We know we're
	 * little-endian, and use type punning for less shifting/masking.
	 */
	ecc16 = (uint16_t *) ecc_code;

	ecc10[0] = (ecc16[0] >> 0) & 0x3ff;
	ecc10[1] = ((ecc16[0] >> 10) & 0x3f) | ((ecc16[1] << 6) & 0x3c0);
	ecc10[2] = (ecc16[1] >> 4) & 0x3ff;
	ecc10[3] = ((ecc16[1] >> 14) & 0x3) | ((ecc16[2] << 2) & 0x3fc);
	ecc10[4] = (ecc16[2] >> 8) | ((ecc16[3] << 8) & 0x300);
	ecc10[5] = (ecc16[3] >> 2) & 0x3ff;
	ecc10[6] = ((ecc16[3] >> 12) & 0xf) | ((ecc16[4] << 4) & 0x3f0);
	ecc10[7] = (ecc16[4] >> 6) & 0x3ff;

	/* Tell ECC controller about the expected ECC codes. */
	for (i = 7; i >= 0; i--) {
		hEmif16Cfg->NANDF4BECCLR = ecc10[i];
	}

	/* Allow time for syndrome calculation ... then read it.
	 * A syndrome of all zeroes 0 means no detected errors.
	 */
	i = hEmif16Cfg->NANDFSR;
	NandRead4bitECC(syndrome);
	if (!(syndrome[0] | syndrome[1] | syndrome[2] | syndrome[3]))
		return 0;

	/*
	 * Clear any previous address calculation by doing a dummy read of an
	 * error address register.
	 */
	i = hEmif16Cfg->NANDFEA1R;

	/* Start address calculation, and wait for it to complete.
	 * We _could_ start reading more data while this is working,
	 * to speed up the overall page read.
	 */
	CSL_FINS(hEmif16Cfg->NANDFCTL, EMIF16_NANDFCTL_ADDR_CALC_ST, 1);
	for (;;) {
		uint32_t fsr = hEmif16Cfg->NANDFSR;

		switch ((fsr >> 8) & 0x0f) {
		case 0: /* no error, should not happen */
			i = hEmif16Cfg->NANDFEV1R;
			return 0;
		case 1: /* five or more errors detected */
			i = hEmif16Cfg->NANDFEV1R;
			return -1;
		case 2: /* error addresses computed */
		case 3:
			num_errors = 1 + ((fsr >> 16) & 0x03);
			goto correct;
		default: /* still working on it */
			platform_delay(5);
			continue;
		}
	}

	correct:
	/* correct each error */
	for (i = 0, corrected = 0; i < num_errors; i++) {
		int error_address, error_value;

		if (i > 1) {
			error_address = hEmif16Cfg->NANDFEA2R;
			error_value = hEmif16Cfg->NANDFEV2R;
			;
		} else {
			error_address = hEmif16Cfg->NANDFEA1R;
			error_value = hEmif16Cfg->NANDFEV1R;
		}

		if (i & 1) {
			error_address >>= 16;
			error_value >>= 16;
		}
		error_address &= 0x3ff;
		error_address = (512 + 7) - error_address;

		if (error_address < 512) {
			data[error_address] ^= error_value;
			corrected++;
		}
	}

	return corrected;
}

/******************************************************************************
 *
 * Function:    NandReadDataByte
 *
 * Description: This function is used to read Nand data byte
 *
 * Parameters:  uint8_t* puchValue - Pointer to data buffer
 *
 * Return Value: None
 *
 *****************************************************************************/
static void NandReadDataByte(uint8_t* puchValue) {
	/*8-bit NAND*/
	uint8_t *data_addr = (uint8_t *) NAND_DATA_ADDR;

	*puchValue = *data_addr;
}

/******************************************************************************
 *
 * Function:    NandReadDataBytes
 *
 * Description: This function is used to read data bytes from the NAND device
 *
 * Parameters:  uiNumBytes - Number of bytes to be read
 *              pBuffer - Data buffer
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/
static uint32_t NandReadDataBytes(uint32_t uiNumBytes, uint8_t *pBuffer) {
	uint32_t i;

	/* 8-bit NAND */
	for (i = 0; i < uiNumBytes; i++) {
		/* NANDRead done directly without checking for nand width */
		NandReadDataByte((uint8_t *) pBuffer);
		pBuffer++;
	}
	return SUCCESS;
}

/******************************************************************************
 *
 * Function:    NandReadSpareArea
 *
 * Description: Function to read Nand spare area
 *
 * Parameters:  uiBlkAddr - Block Address
 *              uiPage - Page Number
 *              pBuffer - Data Buffer
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/
uint32_t NandReadSpareArea(PLATFORM_DEVICE_info *p_device, uint32_t uiBlkAddr,
		uint32_t uiPage, uint8_t *pBuffer) {
	uint32_t uiAddr, ret_val = SUCCESS;
	uint32_t uiColumn;
	uint8_t status;

	/* Read the data to the destination buffer and detect error */
	uiColumn = p_device->column;

	/* Send 0x50h command to read the spare area */
	NandCmdSet(NAND_SPARE_AREA_READ); // First cycle send 0
	platform_delay(10);

	/*
	 * Send address of the block + page to be read
	 * Address cycles = 4, Block shift = 22, Page Shift = 16, Bigblock = 0
	 */
	uiAddr = PACK_ADDR(uiColumn, uiPage, uiBlkAddr);
	NandAleSet((uiAddr >> 0u) & 0xFF);   // A0-A7  1st Cycle;  column addr
	NandAleSet((uiAddr >> 9u) & 0xFF);   // A9-A16 2nd Cycle;  page   addr
	NandAleSet((uiAddr >> 17u) & 0xFF);   // A17-A24 3rd Cycle; Block addr
	NandAleSet((uiAddr >> 25u) & 0x1);    // A25-A26  4th Cycle;  Plane addr
	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	// Wait for Ready Busy Pin to go HIGH
	ret_val = NandWaitRdy(EMIF16_NAND_PROG_TIMEOUT);

	if (ret_val != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_BUSY;
		IFPRINT(platform_write("NandReadSpareArea: Device timeout.\n"));
		return FAIL;
	}

	/* Read the data to the destination buffer and detect error */
	NandReadDataBytes(p_device->spare_size, pBuffer);

	NandCmdSet(NAND_STATUS);
	platform_delay(10);
	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		IFPRINT(platform_write("NandReadSpareArea: Status (0x%x) has error bit set.\n", status));
		return FAIL;
	}

	return SUCCESS;
}
#endif

/******************************************************************************
 *
 * Function:    readNandPage
 *
 * Description: This function reads a page from NAND flash and detects and
 *              corrects the bit errors if ECC is enabled
 *
 * Parameters:  NAND_ADDR address - Block Address/Page address of NAND flash
 *              uint8_t* puchBuffer - Pointer to buffer
 *
 * Return Value: status
 *
 * Assumptions: puchBuffer points to a 2KB buffer
 *
 ******************************************************************************/
#if (PLATFORM_NAND_READ_IN)
uint32_t readNandPage(PLATFORM_DEVICE_info *p_device, NAND_ADDR address,
		uint8_t* puchBuffer) {
	int32_t i = 0;
	uint8_t *puchSpareAreaBuf;
	uint8_t *pBuffer_loc;
	uint32_t uiAddr;
	uint32_t ret_val = SUCCESS;
	uint8_t status;
	/* ECC locations for the Numonyx nand device */
	uint8_t eccLoc[NAND_MAX_NUM_ECC_BYTES] = { 0, 1, 2, 3, 4, 6, 7, 13, 14, 15 };
	uint8_t eccCode[NAND_MAX_NUM_ECC_BYTES];
	uint8_t eccCalc[NAND_MAX_NUM_ECC_BYTES];

	if (puchBuffer == NULL)
		return NULL_POINTER_ERROR;

	pBuffer_loc = nandPageBuf;
	puchSpareAreaBuf = nandPageBuf + BYTES_PER_PAGE;

	/*
	 * Send address of the block + page to be read
	 * Address cycles = 4, Block shift = 14, Page Shift = 9, Bigblock = 0
	 */
	uiAddr = PACK_ADDR(0x0, address.uiPageAddr, address.uiBlockAddr);

	/***********************READ A Command***************************************/
	NandCmdSet(NAND_LO_PAGE); // First cycle send 0
	platform_delay(10);

	NandAleSet((uiAddr >> 0u) & 0xFF);   // A0-A7  1st Cycle;  column addr
	NandAleSet((uiAddr >> 9u) & 0xFF);   // A9-A16 2nd Cycle;  page   addr & blk
	NandAleSet((uiAddr >> 17u) & 0xFF);   // A17-A24 3rd Cycle; Block addr
	NandAleSet((uiAddr >> 25u) & 0x1);    // A25-A26  4th Cycle;  Plane addr

	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	/* Wait for Ready Busy Pin to go HIGH  */
	ret_val = NandWaitRdy(EMIF16_NAND_PROG_TIMEOUT);

	if (ret_val != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
		return FAIL;
	}

	/* Start 4-bit ECC HW calculation for read */
	CSL_FINS(hEmif16Cfg->NANDFCTL, EMIF16_NANDFCTL_4BIT_ECC_ST, 1);

	/* Read the data to the destination buffer and detect error */
	NandReadDataBytes(gDeviceNand.page_size, pBuffer_loc);
	platform_delay(10);

	/* Calculate the 4-bit ECC bytes for read */
	NandCalculate4bitECC(TRUE, eccCalc);

	NandReadDataBytes(16, puchSpareAreaBuf);
	platform_delay(10);

	/* Get the ECC bytes from the spare area data */
	for (i = 0; i < NAND_MAX_NUM_ECC_BYTES; i++) {
		eccCode[i] = puchSpareAreaBuf[eccLoc[i]];
	}

	if (NandCorrect4bitECC(nandPageBuf, eccCode) < 0) {
		return FAIL;
	}

	NandCmdSet(NAND_STATUS);
	platform_delay(10);

	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		return FAIL;
	}

	memcpy(puchBuffer, nandPageBuf, gDeviceNand.page_size);

	return SUCCESS;
}
#endif

/******************************************************************************
 *
 * Function:    NandWriteDataBytes
 *
 * Description: This function is used to write data bytes to the NAND device
 *
 * Parameters:  uiNumBytes - Number of bytes to be written
 *              pBuffer - Data buffer
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/

#if (PLATFORM_NAND_WRITE_IN)
static void NandWriteDataByte(uint8_t uchData) {
	volatile uint8_t* dest;

	/* Data is written to the data register on the rising edge of WE# when
	 � CE#, CLE, and ALE are LOW, and
	 � the device is not busy. */
	dest = (volatile uint8_t *) (NAND_DATA_ADDR);
	*dest = uchData;
}

uint32_t NandWriteDataBytes(PLATFORM_DEVICE_info *p_device, uint32_t uiNumBytes,
		uint8_t *pBuffer) {
	uint32_t i;

	/* Starting the ECC in the NANDFCR register for CS2(bit no.8)
	 Read ECC to Clear Before Starting */
	if (p_device->width == 8) {
		/* 8-bit NAND */
		for (i = 0; i < uiNumBytes; i++) {
			/* NAND Write done directly without checking for nand width */
			NandWriteDataByte((uint8_t) *pBuffer);
			pBuffer++;
		}
	}
	return SUCCESS;
}
#endif

/******************************************************************************
 *
 * Function:    writeNandPage
 *
 * Description: This function a page to NAND flash. It computes ECC and
 *              and writes it to spare area if ECC is enabled
 *
 * Parameters:  NAND_ADDR address - Block Address/Page address of NAND flash
 *              uint8_t* puchBuffer - Pointer to buffer
 *
 * Return Value: status
 *
 * Assumptions: puchBuffer points to a 2KB buffer
 *
 ******************************************************************************/
#if (PLATFORM_NAND_WRITE_IN)
uint32_t writeNandPage(PLATFORM_DEVICE_info *p_device, NAND_ADDR address,
		uint8_t* puchBuffer) {
	int32_t iErrors = SUCCESS;
	int32_t i = 0;
	uint8_t puchSpareAreaBuf[SPARE_BYTES_PER_PAGE];
	uint8_t *pBuffer_loc;
	uint32_t addr;
	uint32_t ret_val = SUCCESS;
	uint8_t status;

	/* ECC locations for the Numonyx nand device */
	uint8_t eccLoc[NAND_MAX_NUM_ECC_BYTES] = { 0, 1, 2, 3, 4, 6, 7, 13, 14, 15 };
	uint8_t eccCalc[NAND_MAX_NUM_ECC_BYTES];

	/* Init the buffer by reading the existing values in the spare area */
	iErrors = NandReadSpareArea(p_device, address.uiBlockAddr,
			address.uiPageAddr, puchSpareAreaBuf);
	if (iErrors != SUCCESS) {
		return iErrors;
	}

	/* 1st half page*/
	NandCmdSet(NAND_LO_PAGE);
	platform_delay(20);
	NandCmdSet(NAND_PROG_PAGE);
	platform_delay(10);

	/*
	 * Send address of the block + page to be read
	 * Address cycles = 4, Block shift = 22, Page Shift = 16, Bigblock = 0
	 */
	addr = PACK_ADDR(0x0, address.uiPageAddr, address.uiBlockAddr);

	NandAleSet((addr >> 0u) & 0xFF);   // A0-A7  1st Cycle;  column addr
	NandAleSet((addr >> 9u) & 0xFF);   // A9-A16 2nd Cycle;  page   addr & blk
	NandAleSet((addr >> 17u) & 0xFF);   // A17-A24 3rd Cycle; Block addr
	NandAleSet((addr >> 25u) & 0x1);    // A25-A26  4th Cycle;  Plane addr

	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	/* Start 4-bit ECC HW calculation for write */
	CSL_FINS(hEmif16Cfg->NANDFCTL, EMIF16_NANDFCTL_4BIT_ECC_ST, 1);
	platform_delay(10);

	/* Write the data */
	pBuffer_loc = puchBuffer;
	NandWriteDataBytes(p_device, p_device->page_size, pBuffer_loc);

	platform_delay(10);

	/* Calculate the 4-bit ECC bytes for write */
	NandCalculate4bitECC(FALSE, eccCalc);

	/* Update the calculated ECC bytes to spare area data */
	for (i = 0; i < NAND_MAX_NUM_ECC_BYTES; i++) {
		puchSpareAreaBuf[eccLoc[i]] = eccCalc[i];
	}

	NandWriteDataBytes(p_device, p_device->spare_size, puchSpareAreaBuf);
	platform_delay(5);

	/* Wait for Ready Busy Pin to go HIGH  */
	NandCmdSet(NAND_CMD_10H);

	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	ret_val = NandWaitRdy(EMIF16_NAND_PROG_TIMEOUT * 50);

	if (ret_val != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
		return FAIL;
	}

	NandCmdSet(NAND_STATUS);
	platform_delay(10);

	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		return FAIL;
	}

	return SUCCESS;
}
#endif

/******************************************************************************
 *
 * Function:    NandWriteSpareArea
 *
 * Description: Function to write a spare area of the NAND
 *
 * Parameters:  uiBlkAddr - Block Address
 *              uiPage - Page Number
 *              pBuffer - Data Buffer
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/
#if (PLATFORM_NAND_WRITE_IN)
uint32_t NandWriteSpareArea(PLATFORM_DEVICE_info *p_device, uint32_t uiBlkAddr,
		uint32_t uiPage, uint8_t *pBuffer) {
	uint32_t addr;
	uint32_t ret_val = SUCCESS;
	uint8_t status;
	uint8_t *pBuffer_loc;
	uint32_t uiColumn;

	/* Read the data to the destination buffer and detect error */
	uiColumn = p_device->column;

	/* Spare Area*/
	NandCmdSet(NAND_SPARE_AREA_READ);
	platform_delay(20);
	NandCmdSet(NAND_PROG_PAGE);
	platform_delay(10);

	/*
	 * Send address of the block + page to be read
	 * Address cycles = 4, Block shift = 22, Page Shift = 16, Bigblock = 0
	 */
	addr = PACK_ADDR(uiColumn, uiPage, uiBlkAddr);

	NandAleSet((addr >> 0u) & 0xFF);   // A0-A7  1st Cycle;  column addr
	NandAleSet((addr >> 9u) & 0xFF);   // A9-A16 2nd Cycle;  page   addr & blk
	NandAleSet((addr >> 17u) & 0xFF);   // A17-A24 3rd Cycle; Block addr
	NandAleSet((addr >> 25u) & 0x1);    // A25-A26  4th Cycle;  Plane addr

	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	/* Write the data */
	pBuffer_loc = pBuffer;
	NandWriteDataBytes(p_device, p_device->spare_size, (uint8_t *) pBuffer_loc);

	/* Wait for Ready Busy Pin to go HIGH  */
	NandCmdSet(NAND_CMD_10H);

	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	ret_val = NandWaitRdy(EMIF16_NAND_PROG_TIMEOUT * 50);

	if (ret_val != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
		return FAIL;
	}

	NandCmdSet(NAND_STATUS);
	platform_delay(10);

	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		return FAIL;
	}

	return SUCCESS;
}
#endif

/******************************************************************************
 *
 * Function:    nandFlashBlockErase
 *
 * Description: This function erases the specified block of NAND flash
 *
 * Parameters:  NAND_ADDR address - Block Address of NAND flash
 *
 * Return Value: status
 *
 *****************************************************************************/
#if (PLATFORM_NAND_WRITE_IN)
uint32_t nandFlashBlockErase(PLATFORM_DEVICE_info *p_device,
		uint32_t uiBlockNumber) {
	uint32_t addr = 0, ret_val = SUCCESS;
	uint8_t status;

	NandCmdSet(NAND_BLOCK_ERASE); // Block erase command
	platform_delay(25);

	/*
	 * Send address of the block + page to be read
	 * Address cycles = 2, Block shift = 22, Page shiht = 16
	 */
	addr = PACK_ADDR(0x0, 0x0, uiBlockNumber);

	/* Properly adjust the shifts to match to the data sheet */
	NandAleSet((addr >> 9u) & 0xFF);   // A9-A16 2nd Cycle;  page   addr & blk
	platform_delay(25);
	NandAleSet((addr >> 17u) & 0xFF);   // A17-A24 3rd Cycle; Block addr
	platform_delay(25);
	NandAleSet((addr >> 25u) & 0x3);    // A25-A26  4th Cycle;  Plane addr
	platform_delay(1000);

	NandCmdSet(NAND_ERASE_CONFIRM); // Erase confirm
	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	/* Wait for erase operation to finish: 2msec  */
	ret_val = NandWaitRdy(EMIF16_NAND_BLOCK_ERASE_TIMEOUT);

	if (ret_val != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
		return FAIL;
	}

	NandCmdSet(NAND_STATUS);
	platform_delay(10);

	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		return FAIL;
	}

	return SUCCESS;

}
#endif

/******************************************************************************
 *
 * Function:    NandConfig
 *
 * Description: This function is used to congigure the NAND Device
 *
 * Parameters:  None
 *
 * Return Value: Err Status
 *
 *****************************************************************************/
static uint32_t NandConfig(void) {
	uint32_t power_domain_num = 0;
	uint32_t mdctl_emif16_module_num = 3;
	uint32_t mdstat_emif16_module_num = 3;
	CSL_PSC_MODSTATE mdstat;

	/* Wake up EMIF16 module:
	 mdstat = CSL_PSC_getModuleState(mdstat_emif16_module_num); */
	{
		uint32_t loop_cnt = 0;

		/* program pdctl and mdctl to enable the module. */
		CSL_PSC_enablePowerDomain(power_domain_num);
		CSL_PSC_setModuleNextState(mdctl_emif16_module_num,
				PSC_MODSTATE_ENABLE);

		// start the process and wait. but timeout in 1000 loops.
		CSL_PSC_startStateTransition(power_domain_num);
		while (((CSL_PSC_isStateTransitionDone(power_domain_num)) != 0)
				&& (loop_cnt < 1000)) {
			loop_cnt++;
		}

		mdstat = CSL_PSC_getModuleState(mdstat_emif16_module_num);
		/* report result. */
		if (mdstat != PSC_MODSTATE_ENABLE) {
			platform_errno = PLATFORM_ERRNO_PSCMOD_ENABLE;
			return FAIL; /* Could not enable the PSC Module */
		}
	}

	/* Config nand FCR reg. 8 bit NAND, 4-bit HW ECC */
	hEmif16Cfg->A0CR = (0 | (0 << 31) /* selectStrobe */
	| (0 << 30) /* extWait (never with NAND) */
	| (0xf << 26) /* writeSetup  10 ns */
	| (0x3f << 20) /* writeStrobe 40 ns */
	| (7 << 17) /* writeHold   10 ns */
	| (0xf << 13) /* readSetup   10 ns */
	| (0x3f << 7) /* readStrobe  60 ns */
	| (7 << 4) /* readHold    10 ns */
	| (3 << 2) /* turnAround  40 ns */
	| (0 << 0)); /* asyncSize   8-bit bus */

	CSL_FINS(hEmif16Cfg->NANDFCTL, EMIF16_NANDFCTL_CE0NAND,
			CSL_EMIF16_NANDFCTL_CE0NAND_ENABLE);
	CSL_FINS(hEmif16Cfg->NANDFCTL, EMIF16_NANDFCTL_4BIT_ECC_SEL,
			CSL_EMIF16_NANDFCTL_4BIT_ECC_SEL_RESETVAL);

	/* Set the wait polarity */
	hEmif16Cfg->AWCCR = (0x80 /* max extended wait cycle */
	| (0 << 16) /* CS2 uses WAIT0 */
	| (0 << 28)); /* WAIT0 polarity low */

	/*
	 Wait Rise.
	 Set to 1 by hardware to indicate rising edge on the
	 corresponding WAIT pin has been detected.
	 The WP0-3 bits in the Async Wait Cycle Config register have
	 no effect on these bits.
	 */

	/*
	 Asynchronous Timeout.
	 Set to 1 by hardware to indicate that during an extended
	 asynchronous memory access cycle, the WAIT signal did not
	 go inactive within the number of cycles defined by the
	 MAX_EXT_WAIT field in Async Wait Cycle Config register.
	 */

	hEmif16Cfg->IRR = (1 /* clear async timeout */
	| (1 << 2)); /* clear wait rise */

	return SUCCESS;
}

/******************************************************************************
 *
 * Function:    NandOpenDevice
 *
 * Description: This function is used to open the NAND device and configure it
 *
 * Parameters:  None
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/
static uint32_t NandOpenDevice(void) {
	uint8_t status;

	/* Initialize NAND interface */
	if (NandConfig() != SUCCESS) {
		IFPRINT(platform_write("NandOpenDevice ... could not initialize the Nand Interface. \n"));
		return FAIL;
	}

	/* Send reset command to NAND */
	NandCmdSet(NAND_RST);
	platform_delay(EMIF16_WAIT_PIN_POLL_ST_DLY);

	if (NandWaitRdy(EMIF16_NAND_RESET_TIMEOUT) != SUCCESS) {
		platform_errno = PLATFORM_ERRNO_DEV_TIMEOUT;
		IFPRINT(platform_write("NandOpenDevice ... Nand wait ready failed. \n"));
		return FAIL;
	}

	NandCmdSet(NAND_STATUS);
	platform_delay(10);

	NandReadDataByte(&status);

	if ((status & 0x01) == 1) {
		/* if SR0 bit is set to 1, there is Error - operation failed */
		platform_errno = PLATFORM_ERRNO_DEV_FAIL;
		IFPRINT(platform_write("NandOpenDevice ... Nand status error bit was set. \n"));
		return FAIL;
	}

	return SUCCESS;
}

/******************************************************************************
 *
 * Function:    NandGetDetails
 *
 * Description: Get details of the NAND flash used from the id and the
 *              table of NAND
 *
 * Parameters:  pNandInfo - Pointer to Nand Info structure
 *
 * Return Value: Error/Success codes
 *
 *****************************************************************************/
uint32_t NandGetDetails(PLATFORM_DEVICE_info *pNandInfo) {
	uint32_t i;
	uint32_t uiStatus;
	NAND_ADDR address;

	/* Clear the Information */
	pNandInfo->device_id = pNandInfo->manufacturer_id = 0x0;

	/* Read manufacturer ID and device ID */
	NandCmdSet(NAND_RDID);
	platform_delay(10);
	NandAleSet(NAND_ADD_00H);
	platform_delay(10);

	/* Always reading the ID alone in 8 bit mode */
	NandReadDataByte((uint8_t *) &pNandInfo->manufacturer_id);
	NandReadDataByte((uint8_t *) &pNandInfo->device_id);

	/* Get the bad block table */
	address.uiPageAddr = 0;
	address.uiColumnAddr = 0;

	for (i = 0; i < BLOCKS_PER_DEVICE; i++) {
		address.uiBlockAddr = i;

		/* Clear the Spare Area */
		memset(nandPageBuf, 0, SPARE_BYTES_PER_PAGE);

		// Read the spare area in to buffer
		uiStatus = NandReadSpareArea(pNandInfo, address.uiBlockAddr,
				address.uiPageAddr, nandPageBuf);
		if (uiStatus != SUCCESS) {
			return PLATFORM_ERRNO_NANDBBT;
		} else { // Success in reading the NAND spare area
			if (nandPageBuf[pNandInfo->bboffset] == 0xFF) {
				pNandInfo->bblist[i] = 0xFF;
			} else {
				pNandInfo->bblist[i] = 0x00;
			}
		}
	}
	return SUCCESS;
}

/******************************************************************************
 *
 * Function:    nandInit
 *
 * Description: This function configures the GPIO pins for communication
 *              between DSP and FPGA NAND flash controller.
 *
 * Parameters:  None
 *
 * Return Value: status
 *
 ******************************************************************************/
uint32_t nandInit(void) {
	/* Open the NAND Device */
	return NandOpenDevice();
}

#endif /*(PLATFORM_NAND_IN)*/

