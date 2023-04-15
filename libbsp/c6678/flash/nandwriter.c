/*
 * Copyright (c) 2011-2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**************************************************************************************
 * FILE PURPOSE: NAND writer utility
 **************************************************************************************
 * FILE NAME: nandwriter.c
 *
 * DESCRIPTION: A simple nand writer using platform lib APIs to program the NAND flash
 *              with an image that the ibl can read.
 *
 ***************************************************************************************/
#include "platform.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NAND writer utility version */
char version[] = "01.00.00.05";

/* The input file name is hard coded */
char* input_file = "nand_writer_input.txt";

uint32_t swap_byte = 0;
volatile uint32_t nand_erase_flag = 0;

/* Parameters defined in the input_file */
#define FILE_NAME "file_name"
#define START_ADDR "start_addr"
#define RBL_ECC "rbl_ecc"
#define SKIP_BAD "skip_bad"

/* Memory address to store the write data */
#define WRITE_DATA_ADDRESS 0x80000000

/* NAND device specific definitions */
#define BAD_BLOCK_MARKER_VALUE 0xA5

/******************************************************************************
 * Structure:   NAND_WRITER_INFO_T
 *
 *              NAND writer control data. This structure should be filled in
 *              by the user before running
 ******************************************************************************/
#define MAX_LINE_LENGTH 40
typedef struct NAND_WRITER_INFO_tag {
    char file_name[MAX_LINE_LENGTH]; /* CCS format data file name */
    uint32_t blockSizeBytes; /* The size of each sector */
    uint32_t deviceTotalBytes; /* Total number of bytes available in the device */
    uint32_t startAddr; /* Start address to write */
    uint32_t writeBytes; /* Number of bytes to be written into the device */
    uint8_t flags; /* Flag used for EVMK2H for multiple ECC formats */
    uint8_t skip_bad_blks; /* Flag used to skip bad blocks */
    uint8_t* writeData; /* Address to store the write data */
    uint8_t* readData; /* Address to store the read data */

} NAND_WRITER_INFO_T;

NAND_WRITER_INFO_T nandWriterInfo;

/* OSAL functions for Platform Library */
uint8_t* Osal_platformMalloc(uint32_t num_bytes, uint32_t alignment)
{
    return malloc(num_bytes);
}

void Osal_platformFree(uint8_t* dataPtr, uint32_t num_bytes)
{
    /* Free up the memory */
    if (dataPtr) {
        free(dataPtr);
    }
}

void Osal_platformSpiCsEnter(void)
{
    return;
}

void Osal_platformSpiCsExit(void)
{
    return;
}

/******************************************************************************
 * Function:    print_platform_errno
 ******************************************************************************/
void print_platform_errno(
    void)
{
    printf("Returned platform error number is %d\n", platform_errno);
}

/******************************************************************************
 * Function:    checkBadBlockMark
 *
 *              Checks for the bad block mark. Returns TRUE if a block is marked bad.
 ******************************************************************************/
Bool checkBadBlockMark(
    PLATFORM_DEVICE_info* p_device,
    uint32_t block)
{
    if (p_device->bblist[block] != 0xff) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/******************************************************************************
 * Function:    markBlockBad
 *
 *              Mark a block as bad. Byte 5 of the spare area data is
 *              written as 0xA5.
 ******************************************************************************/
void markBlockBad(
    PLATFORM_DEVICE_info* p_device,
    uint32_t block)
{
    uint8_t* spare_data;

    spare_data = malloc(p_device->spare_size);
    if (spare_data == NULL) {
        printf("Can not allocate spare_data memory!\n");
        return;
    }

    platform_device_read_spare_data(p_device->handle, block, 0, spare_data);

    /* Set all data bytes to 0xff, only set the user defined bad block mark value */
    spare_data[p_device->bboffset] = BAD_BLOCK_MARKER_VALUE;

    /* Write the data to page */
    platform_device_write_spare_data(p_device->handle, block, 0, spare_data);

    /* Save the user defined bad block mark in bblist */
    p_device->bblist[block] = BAD_BLOCK_MARKER_VALUE;

    free(spare_data);
}

/******************************************************************************
 * Function:    formBlock
 *
 *      Form a block of data to write to the NAND. The block is
 *      created as a byte stream from the 4 byte stream in which
 *      the MSB is always sent first.
 ******************************************************************************/
void formBlock(
    uint32_t* data,
    uint32_t blockSize,
    uint8_t* scratch)
{
    uint32_t i, j;

    /* Convert the data to a byte stream */
    for (i = j = 0; j < blockSize; i++, j += 4) {
        scratch[j + 0] = (data[i] >> 24) & 0xff;
        scratch[j + 1] = (data[i] >> 16) & 0xff;
        scratch[j + 2] = (data[i] >> 8) & 0xff;
        scratch[j + 3] = (data[i] >> 0) & 0xff;
    }
}

/******************************************************************************
 * Function:    flash_nand
 *
 *              Write the image to flash.
 *              Returns TRUE if the image is written successfully
 *                      FALSE if the image write fails
 ******************************************************************************/
Bool flash_nand(
    PLATFORM_DEVICE_info* p_device)
{
    uint32_t wPos, wLen;
    uint32_t block, start_block;
    uint8_t* scrach_block;
    uint32_t num_blks;
    uint32_t bad_block_found;

    if (swap_byte) {
        scrach_block = malloc(nandWriterInfo.blockSizeBytes);
        if (scrach_block == NULL) {
            printf("Can not allocate scratch block memory!\n");
            return (FALSE);
        }
    }

    start_block = nandWriterInfo.startAddr / nandWriterInfo.blockSizeBytes;

    /* skip the bad blocks if the flag is set */
    if (nandWriterInfo.skip_bad_blks) {
        num_blks = nandWriterInfo.writeBytes / nandWriterInfo.blockSizeBytes;
        if ((nandWriterInfo.writeBytes - nandWriterInfo.blockSizeBytes * num_blks) > 0)
            num_blks++;

        while (TRUE) {
            bad_block_found = FALSE;
            for (block = start_block; block < start_block + num_blks; block++) {
                if (checkBadBlockMark(p_device, block)) {
                    start_block = block + 1;
                    bad_block_found = TRUE;
                    break;
                }
            }
            if (bad_block_found == FALSE)
                break;
        }
    }

    /* Program the NAND */
    for (block = start_block, wPos = 0; wPos < nandWriterInfo.writeBytes; block++, wPos += nandWriterInfo.blockSizeBytes) {
        while (checkBadBlockMark(p_device, block)) {
            printf("Bad block # %d detected, skipping block ... \n", block);
            if (++block == p_device->block_count) {
                printf("Flash failed: End of device reached\n");
                if (swap_byte)
                    free(scrach_block);
                return (FALSE);
            }
        }

        printf("Flashing block %d (%d bytes of %d)\n", block, wPos, nandWriterInfo.writeBytes);

        platform_device_erase_block(p_device->handle, block);

        wLen = nandWriterInfo.blockSizeBytes;
        if (nandWriterInfo.writeBytes - wPos < nandWriterInfo.blockSizeBytes) {
            wLen = nandWriterInfo.writeBytes - wPos;
        }

        if (swap_byte) {
            formBlock((uint32_t*)(&nandWriterInfo.writeData[wPos]), nandWriterInfo.blockSizeBytes, scrach_block);
        } else {
            scrach_block = &nandWriterInfo.writeData[wPos];
        }
        if (platform_device_write(p_device->handle,
                block * nandWriterInfo.blockSizeBytes,
                scrach_block,
                wLen)
            != Platform_EOK) {
            printf("platform_device_write block # %d failed!\n", block);
            print_platform_errno();
            if (swap_byte)
                free(scrach_block);
            return (FALSE);
        }
    }

    if (swap_byte)
        free(scrach_block);
    return (TRUE);
}

/******************************************************************************
 * Function:    flash_verify
 *
 *              Read back the data file that was just flashed. On errors mark the block as bad.
 *              Returns TRUE if the image verified correctly.
 *                      FALSE if the image verification failed
 ******************************************************************************/
Bool flash_verify(
    PLATFORM_DEVICE_info* p_device)
{
    uint32_t rPos, rLen;
    uint32_t i, j;
    uint32_t block, start_block;
    uint8_t* scrach_block;
    uint32_t* read_data_w;

    if (swap_byte) {
        scrach_block = malloc(nandWriterInfo.blockSizeBytes);
        if (scrach_block == NULL) {
            printf("Can not allocate scratch block memory!\n");
            return (FALSE);
        }
    }

    start_block = nandWriterInfo.startAddr / nandWriterInfo.blockSizeBytes;

    for (block = start_block, rPos = 0; rPos < nandWriterInfo.writeBytes; block++, rPos += nandWriterInfo.blockSizeBytes) {
        while (checkBadBlockMark(p_device, block)) {
            printf("Bad block # %d detected, skipping block ... \n", block);
            if (++block == p_device->block_count) {
                printf("Flash failed: End of device reached\n");
                if (swap_byte)
                    free(scrach_block);
                return (FALSE);
            }
        }

        printf("Reading and verifying block %d (%d bytes of %d)\n", block, rPos, nandWriterInfo.writeBytes);

        if (!swap_byte) {
            scrach_block = &nandWriterInfo.readData[rPos];
        }

        /* Read a block of data */
        if (platform_device_read(p_device->handle,
                block * nandWriterInfo.blockSizeBytes,
                scrach_block,
                nandWriterInfo.blockSizeBytes)
            != Platform_EOK) {
            printf("Failure in reading block %d\n", block);
            print_platform_errno();
            if (platform_errno == PLATFORM_ERRNO_ECC_FAIL) {
                printf("marking block %d as bad, re-flash attempted\n", block);
                markBlockBad(p_device, block);
            }
            if (swap_byte)
                free(scrach_block);
            return (FALSE);
        }

        /* Convert the packed data */
        if (swap_byte) {
            read_data_w = (uint32_t*)(&nandWriterInfo.readData[rPos]);
            for (i = 0, j = 0; i < nandWriterInfo.blockSizeBytes; i += 4)
                read_data_w[j++] = (scrach_block[i + 0] << 24) | (scrach_block[i + 1] << 16) | (scrach_block[i + 2] << 8) | scrach_block[i + 3];
        }

        rLen = nandWriterInfo.blockSizeBytes;
        if (nandWriterInfo.writeBytes - rPos < nandWriterInfo.blockSizeBytes) {
            rLen = nandWriterInfo.writeBytes - rPos;
        }

        /* Compare the data read with data programmed */
        for (i = rPos; i < rLen; i++) {
            if (nandWriterInfo.readData[i] != nandWriterInfo.writeData[i]) {
                printf("Failure in block %d, at byte %d, (at byte %d in the data file) expected 0x%08x, read 0x%08x\n",
                    block, i, rPos, nandWriterInfo.writeData[i], nandWriterInfo.readData[i]);
                if (swap_byte)
                    free(scrach_block);
                return (FALSE);
            }
        }
    }

    if (swap_byte)
        free(scrach_block);
    return (TRUE);
}

/******************************************************************************
 * Function:    parse_input_file
 ******************************************************************************/
static Bool
parse_input_file(
    FILE* fp)
{
    char line[MAX_LINE_LENGTH];
    char tokens[] = " :=;\n\r";
    char *key, *data;

    memset(line, 0, MAX_LINE_LENGTH);

    fgets(line, MAX_LINE_LENGTH, fp);
    key = (char*)strtok(line, tokens);
    data = (char*)strtok(NULL, tokens);

    if (strlen(data) == 0) {
        return FALSE;
    }

    if (strcmp(key, FILE_NAME) != 0) {
        return FALSE;
    }

    strcpy(nandWriterInfo.file_name, data);

    fgets(line, MAX_LINE_LENGTH, fp);
    key = (char*)strtok(line, tokens);
    data = (char*)strtok(NULL, tokens);

    if (strlen(data) == 0) {
        return FALSE;
    }

    if (strcmp(key, START_ADDR) != 0) {
        return FALSE;
    }

    nandWriterInfo.startAddr = (uint32_t)atoi(data);

    fgets(line, MAX_LINE_LENGTH, fp);
    key = (char*)strtok(line, tokens);
    data = (char*)strtok(NULL, tokens);

    if (strlen(data) != 0 && (strcmp(key, RBL_ECC) == 0)) {
        nandWriterInfo.flags = (uint8_t)atoi(data);
    } else {
        nandWriterInfo.flags = 0;
    }

    /* Scan skip bad block input parameter */
    fgets(line, MAX_LINE_LENGTH, fp);
    key = (char*)strtok(line, tokens);
    data = (char*)strtok(NULL, tokens);

    if (strlen(data) != 0 && (strcmp(key, SKIP_BAD) == 0)) {
        nandWriterInfo.skip_bad_blks = (uint8_t)atoi(data);
    } else {
        nandWriterInfo.skip_bad_blks = 0;
    }

    return TRUE;
}

/******************************************************************************
 * Function:    find_file_length
 ******************************************************************************/
static Bool
find_file_length(
    FILE* fp)
{
    char line[MAX_LINE_LENGTH];
    char* pEnd;
    char* ext;
    uint32_t data_len, write_addr;

    memset(line, 0, MAX_LINE_LENGTH);

    ext = strrchr(nandWriterInfo.file_name, '.');
    if (ext && (strcmp(ext, ".dat") == 0)) {
        fgets(line, MAX_LINE_LENGTH, fp);

        /* Read the write address from the CCS header */
        strtoul(line, &pEnd, 16);
        strtoul(pEnd, &pEnd, 16);
        write_addr = strtoul(pEnd, &pEnd, 16);
        strtoul(pEnd, &pEnd, 16);

        /* Read the data length */
        data_len = (strtoul(pEnd, NULL, 16)) * 4;
    } else {
        /* find the data length by seeking to the end and getting position */
        fseek(fp, 0, SEEK_END);
        data_len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    if (data_len > (nandWriterInfo.deviceTotalBytes - nandWriterInfo.startAddr)) {
        printf("The data file is too big to fit into the device.\n");
        return FALSE;
    }

    nandWriterInfo.writeBytes = data_len;
    if (write_addr != WRITE_DATA_ADDRESS)
        write_addr = WRITE_DATA_ADDRESS;
    nandWriterInfo.writeData = (uint8_t*)write_addr;
    nandWriterInfo.readData = (uint8_t*)(write_addr + nandWriterInfo.deviceTotalBytes);

    return TRUE;
}

static Bool
nand_erase_all_blks(PLATFORM_DEVICE_info* p_device)
{
    uint32_t block;
    uint32_t max_blks;

    max_blks = p_device->block_count;

    for (block = 0; block < max_blks; block++) {
        printf(".");
        if (platform_device_erase_block(p_device->handle, block) != Platform_EOK)
            return FALSE;
    }

    printf("\nFormatted %d nand blocks \n", block);
    return (TRUE);
}

/******************************************************************************
 * Function:    main
 ******************************************************************************/
void main()
{
    FILE* fp;
    platform_init_flags init_flags;
    platform_init_config init_config;
    PLATFORM_DEVICE_info* p_device;
    Bool ret;
    uint32_t rCount;
    printf("NAND Writer Utility Version %s\n\n", version);

    fp = fopen(input_file, "r");
    if (fp == NULL) {
        printf("Error in opening %s input file\n", input_file);
        return;
    }

    ret = parse_input_file(fp);
    fclose(fp);

    if (ret == FALSE) {
        printf("Error in parsing %s input file\n", input_file);
        return;
    }

    /* Initialize main Platform lib */
    memset(&init_config, 0, sizeof(platform_init_config));
    memset(&init_flags, 1, sizeof(platform_init_flags));
    init_flags.pll = 0;
    init_flags.ddr = 0;
    init_flags.phy = 0;
    if (platform_init(&init_flags, &init_config) != Platform_EOK) {
        printf("Platform init failed!\n");
        print_platform_errno();
        return;
    }
#if (defined(_EVMC6657L_) || defined(DEVICE_K2H))
    p_device = platform_device_open(PLATFORM_DEVID_MT29F1G08ABCHC, nandWriterInfo.flags);
#elif defined(DEVICE_K2E)
    p_device = platform_device_open(PLATFORM_DEVID_MT29F4G08ABADA, nandWriterInfo.flags);
#elif defined(DEVICE_K2L)
    p_device = platform_device_open(PLATFORM_DEVID_MT29F16G08ADBCAH4C, nandWriterInfo.flags);
#elif (defined(DEVICE_K2G) || defined(SOC_K2G))
    p_device = platform_device_open(PLATFORM_DEVID_MT29F2G16ABAFA, nandWriterInfo.flags);
#else
    p_device = platform_device_open(PLATFORM_DEVID_NAND512R3A2D, 0);
#endif
    if (p_device == NULL) {
        printf("NAND device open failed!\n");
        print_platform_errno();
        return;
    }
    nandWriterInfo.deviceTotalBytes = p_device->block_count * p_device->page_count * p_device->page_size;
    nandWriterInfo.blockSizeBytes = p_device->page_count * p_device->page_size;

    if ((nandWriterInfo.startAddr % nandWriterInfo.blockSizeBytes) != 0) {
        printf("The start programming address 0x%8x set in %s is not at the beginning of a block, block size = 0x%4x\n",
            nandWriterInfo.startAddr,
            nandWriterInfo.file_name,
            nandWriterInfo.blockSizeBytes);
        return;
    }

    /* Check if we need to erase the nand completely */
    if (nand_erase_flag == 0x12345678) {
        if (nand_erase_all_blks(p_device) == FALSE) {
            printf("Formatting all nand blocks - failed \n");
        } else {
            printf("Formatting all nand blocks - Successful\n");
        }
        platform_device_close(p_device->handle);
        /* Operation Complete */
        return;
    }

    /* Open and find the length of the data file */
    fp = fopen(nandWriterInfo.file_name, "rb");
    if (fp == NULL) {
        printf("Failed to open file %s\n", nandWriterInfo.file_name);
        platform_device_close(p_device->handle);
        return;
    }

    /* Parse the CCS format file */
    ret = find_file_length(fp);
    fclose(fp);
    if (ret == FALSE) {
        printf("Error in parsing CCS file %s\n", nandWriterInfo.file_name);
        platform_device_close(p_device->handle);
        return;
    }

    /* Write the flash, verify the results. On read back failure mark
     * the block as bad and try rewriting again */
    rCount = 0;

    do {
        if (flash_nand(p_device) == FALSE) {
            printf("NAND write giving up\n");
            return;
        }

        rCount += 1;

    } while ((flash_verify(p_device) == FALSE) && (rCount < 5));

    if (rCount >= 5) {
        printf("NAND write failed (maximum retries reached)\n");
    } else {
        printf("NAND programming completed successfully\n");
    }

    platform_device_close(p_device->handle);

    return;
}
