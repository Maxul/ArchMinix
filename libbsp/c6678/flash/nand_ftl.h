/*===============================================================*\
| Project: RTEMS TMS320C6678 BSP                                  |
+-----------------------------------------------------------------+
| Partially based on the code references which are named below.   |
| Adaptions, modifications, enhancements and any recent parts of  |
| the code are:                                                   |
|                 Copyright (c) 2017                              |
|                 yvonneason <1433902804@qq.com>                  |
|                                                                 |
+-----------------------------------------------------------------+
| The license and distribution terms for this file may be         |
| found in the file LICENSE in this distribution or at            |
|                                                                 |
| http://www.rtems.org/license/LICENSE.                           |
|                                                                 |
+-----------------------------------------------------------------+
| this file contains the NAND flash driver                        |
\*===============================================================*/

#include <rtems/flashdisk.h>

#define FLASHDISK_CONFIG_COUNT 1
#define FLASHDISK_DEVICE_COUNT 1
#define FLASHDISK_SEGMENT_COUNT 4
#define FLASHDISK_SEGMENT_SIZE (512 * 32)
#define FLASHDISK_BLOCK_SIZE 512U

#define FLASHDISK_BLOCKS_PER_SEGMENT \
    (FLASHDISK_SEGMENT_SIZE / FLASHDISK_BLOCK_SIZE)

#define FLASHDISK_SIZE \
    (FLASHDISK_SEGMENT_COUNT * FLASHDISK_SEGMENT_SIZE)

static uint32_t nand_hdl;

#include "platform_internal.h"

static void erase_device(void)
{
    uint32_t block;
    uint32_t max_blks = FLASHDISK_SEGMENT_COUNT;

    for (block = 0; block < max_blks; block++) {
        printf("Formatting... %d%%\n", (block + 1) * 100 / max_blks);
        if (platform_device_erase_block(nand_hdl, block) != Platform_EOK) {
            printf("Formatted failed on block %d\n", block);
            return;
        }
    }

    printf("\nFormatted %d nor blocks \n", block);
}

static rtems_device_driver flashdisk_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void* arg)
{
    PLATFORM_DEVICE_info* p_device;

    p_device = platform_device_open(PLATFORM_DEVID_NAND512R3A2D, 0);
    if (p_device != NULL) {
        nand_hdl = p_device->handle;
    } else {
        puts("Unable to open Nand Flash!!!");
    }
    erase_device();

    return rtems_fdisk_initialize(major, minor, arg);
}

static uint32_t get_linear_offset(
    const rtems_fdisk_segment_desc* sd,
    uint32_t segment,
    uint32_t offset)
{
    offset += sd->offset + (segment - sd->segment) * sd->size;

    return offset;
}

static int flashdisk_read(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    void* buffer,
    uint32_t size)
{
    int eno = 0;

    uint32_t addr = get_linear_offset(sd, segment, offset);

    /* Read a block of nand_buf */
    if (platform_device_read(nand_hdl, addr, buffer, size) != Platform_EOK) {
        printf("Failure in reading offset %d\n", addr);
        eno = EIO;
    }
    return eno;
}

static int flashdisk_write(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    const void* buffer,
    uint32_t size)
{
    int eno = 0;

    uint32_t addr = get_linear_offset(sd, segment, offset);
    printf("write addr%d\n", addr);
    /* Write a block of nand_buf */
    if (platform_device_write(nand_hdl, addr, buffer, size) != Platform_EOK) {
        printf("Failure in writing offset %d\n", addr);
        eno = EIO;
    }
    return eno;
}

static int flashdisk_blank(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    uint32_t size)
{
    int eno = 0;

    unsigned char* tmp = malloc(size);
    if (tmp == NULL)
        return EIO;

    uint32_t current = get_linear_offset(sd, segment, offset);
    uint32_t end = current + size;

    if (platform_device_read(nand_hdl, current, tmp, size) != Platform_EOK) {
        eno = EIO;
        goto out;
    }

    while (eno == 0 && current != end) {
        if (*(tmp + current) != 0xff) {
            eno = EIO;
            break;
        }
        ++current;
    }
out:
    free(tmp);
    return eno;
}

static int flashdisk_verify(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    const void* buffer,
    uint32_t size)
{
    int eno = 0;

    unsigned char* tmp = malloc(size);
    if (tmp == NULL)
        return EIO;

    uint32_t addr = get_linear_offset(sd, segment, offset);
    if (platform_device_read(nand_hdl, addr, tmp, size) != Platform_EOK) {
        eno = EIO;
        goto out;
    }

    if (memcmp(tmp, buffer, size) != 0)
        eno = EIO;

out:
    free(tmp);
    return eno;
}

static int flashdisk_erase(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment)
{
    int eno = 0;

    if (platform_device_erase_block(nand_hdl, segment) != Platform_EOK)
        return EIO;
    return eno;
}

static int flashdisk_erase_device(
    const rtems_fdisk_device_desc* sd,
    uint32_t device)
{
    int eno = 0;

    erase_device();

    return eno;
}

static const rtems_fdisk_segment_desc flashdisk_segment_desc = {
    .count = FLASHDISK_SEGMENT_COUNT,
    .segment = 0,
    .offset = 0,
    .size = FLASHDISK_SEGMENT_SIZE
};

static const rtems_fdisk_driver_handlers flashdisk_ops = {
    .read = flashdisk_read,
    .write = flashdisk_write,
    .blank = flashdisk_blank,
    .verify = flashdisk_verify,
    .erase = flashdisk_erase,
    .erase_device = flashdisk_erase_device
};

static const rtems_fdisk_device_desc flashdisk_device = {
    .segment_count = 1,
    .segments = &flashdisk_segment_desc,
    .flash_ops = &flashdisk_ops
};

const rtems_flashdisk_config
    rtems_flashdisk_configuration[FLASHDISK_CONFIG_COUNT]
    = {
          { .block_size = FLASHDISK_BLOCK_SIZE,
              .device_count = FLASHDISK_DEVICE_COUNT,
              .devices = &flashdisk_device,
              .flags = 0 /*RTEMS_FDISK_CHECK_PAGES
                 | RTEMS_FDISK_BLANK_CHECK_BEFORE_WRITE*/
              ,
              .unavail_blocks = 1 /*FLASHDISK_BLOCKS_PER_SEGMENT*/,
              .compact_segs = 2,
              .avail_compact_segs = 1,
              .info_level = 0 }
      };

uint32_t rtems_flashdisk_configuration_size = FLASHDISK_CONFIG_COUNT;

#define FLASHDISK_DRIVER                              \
    {                                                 \
        .initialization_entry = flashdisk_initialize, \
        .open_entry = rtems_blkdev_generic_open,      \
        .close_entry = rtems_blkdev_generic_close,    \
        .read_entry = rtems_blkdev_generic_read,      \
        .write_entry = rtems_blkdev_generic_write,    \
        .control_entry = rtems_blkdev_generic_ioctl   \
    }
