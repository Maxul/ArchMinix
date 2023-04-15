/*===============================================================*\
| Project: RTEMS TMS320C6678 BSP                                  |
+-----------------------------------------------------------------+
| Partially based on the code references which are named below.   |
| Adaptions, modifications, enhancements and any recent parts of  |
| the code are:                                                   |
|                 Copyright (c) 2016                              |
|                 Maxul Lee <lmy2010lmy@gamil.com>                |
|                                                                 |
+-----------------------------------------------------------------+
| The license and distribution terms for this file may be         |
| found in the file LICENSE in this distribution or at            |
|                                                                 |
| http://www.rtems.org/license/LICENSE.                           |
|                                                                 |
+-----------------------------------------------------------------+
| this file contains the NOR flash driver                         |
\*===============================================================*/

#include <rtems/flashdisk.h>

#define NOR_FLASHDISK_CONFIG_COUNT 1
#define NOR_FLASHDISK_DEVICE_COUNT 1
#define NOR_FLASHDISK_SEGMENT_COUNT 256U
#define NOR_FLASHDISK_SEGMENT_SIZE 65536U
#define NOR_FLASHDISK_BLOCK_SIZE 512U

#define FLASHDISK_BLOCKS_PER_SEGMENT \
    (NOR_FLASHDISK_SEGMENT_SIZE / NOR_FLASHDISK_BLOCK_SIZE)

#define FLASHDISK_SIZE \
    (NOR_FLASHDISK_SEGMENT_COUNT * NOR_FLASHDISK_SEGMENT_SIZE)

#include "platform_internal.h"

extern PLATFORM_DEVICE_info gDeviceNor;

static void rtems_c6x_nor_erase_device(void)
{
    uint32_t block;
    uint32_t max_blks = NOR_FLASHDISK_SEGMENT_COUNT;

    for (block = 0; block < max_blks; block++) {
        printf("Formatting... %d%%\n", (block + 1) * 100 / max_blks);
        if (nor_erase(&gDeviceNor, block) != 0) {
            printf("Formatted failed on block %d\n", block);
            return;
        }
    }

    printf("\nFormatted %d nor blocks \n", block);
}

static rtems_device_driver rtems_c6x_nor_flashdisk_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void* arg)
{
    //  erase_device();

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

static int rtems_c6x_nor_flashdisk_read(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    void* buffer,
    uint32_t size)
{
    int eno = 0;

    uint32_t addr = get_linear_offset(sd, segment, offset);
    nor_read(&gDeviceNor, addr, size, buffer);
    //  printf("read: offset %p, size %d\n", data-flashdisk_data, size);

    return eno;
}

static int rtems_c6x_nor_flashdisk_write(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment,
    uint32_t offset,
    const void* buffer,
    uint32_t size)
{
    int eno = 0;

    uint32_t addr = get_linear_offset(sd, segment, offset);
    printf("flashdisk_write %d\n", offset);
    nor_write(&gDeviceNor, addr, size, (Uint8*)buffer);
    //  printf("write: offset %p, size %d\n", data-flashdisk_data, size);

    return eno;
}

static int rtems_c6x_nor_flashdisk_blank(
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

    nor_read(&gDeviceNor, current, size, tmp);
    while (eno == 0 && current != end) {
        if (*(tmp + current) != 0xff) {
            eno = EIO;
            break;
        }
        ++current;
    }

    free(tmp);
    return eno;
}

static int rtems_c6x_nor_flashdisk_verify(
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
    nor_read(&gDeviceNor, addr, size, tmp);
    if (memcmp(tmp, buffer, size) != 0)
        eno = EIO;

    free(tmp);
    return eno;
}

static int rtems_c6x_nor_flashdisk_erase(
    const rtems_fdisk_segment_desc* sd,
    uint32_t device,
    uint32_t segment)
{
    int eno = 0;

    uint32_t addr = get_linear_offset(sd, segment, 0);
    nor_erase(&gDeviceNor, addr / sd->size);

    return eno;
}

static int rtems_c6x_nor_flashdisk_erase_device(
    const rtems_fdisk_device_desc* sd,
    uint32_t device)
{
    int eno = 0;

    rtems_c6x_nor_erase_device();

    return eno;
}

static const rtems_fdisk_segment_desc flashdisk_segment_desc = {
    .count = NOR_FLASHDISK_SEGMENT_COUNT,
    .segment = 0,
    .offset = 0,
    .size = NOR_FLASHDISK_SEGMENT_SIZE
};

static const rtems_fdisk_driver_handlers flashdisk_ops = {
    .read = rtems_c6x_nor_flashdisk_read,
    .write = rtems_c6x_nor_flashdisk_write,
    .blank = rtems_c6x_nor_flashdisk_blank,
    .verify = rtems_c6x_nor_flashdisk_verify,
    .erase = rtems_c6x_nor_flashdisk_erase,
    .erase_device = rtems_c6x_nor_flashdisk_erase_device
};

static const rtems_fdisk_device_desc flashdisk_device = {
    .segment_count = 1,
    .segments = &flashdisk_segment_desc,
    .flash_ops = &flashdisk_ops
};

const rtems_flashdisk_config
    rtems_flashdisk_configuration[NOR_FLASHDISK_CONFIG_COUNT]
    = {
          { .block_size = NOR_FLASHDISK_BLOCK_SIZE,
              .device_count = NOR_FLASHDISK_DEVICE_COUNT,
              .devices = &flashdisk_device,
              .flags = 0 /*RTEMS_FDISK_CHECK_PAGES
                 | RTEMS_FDISK_BLANK_CHECK_BEFORE_WRITE*/
              ,
              .unavail_blocks = 1 /*FLASHDISK_BLOCKS_PER_SEGMENT*/,
              .compact_segs = 2,
              .avail_compact_segs = 1,
              .info_level = 0 }
      };

uint32_t rtems_flashdisk_configuration_size = NOR_FLASHDISK_CONFIG_COUNT;

#define FLASHDISK_DRIVER                                            \
    {                                                               \
        .initialization_entry = rtems_c6x_nor_flashdisk_initialize, \
        .open_entry = rtems_blkdev_generic_open,                    \
        .close_entry = rtems_blkdev_generic_close,                  \
        .read_entry = rtems_blkdev_generic_read,                    \
        .write_entry = rtems_blkdev_generic_write,                  \
        .control_entry = rtems_blkdev_generic_ioctl                 \
    }
