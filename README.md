# ArchMinix

This is a repo that contains the major efforts I made to port the c6678 bsp into the master branch of [rtems 4.11](https://github.com/RTEMS/rtems).

## About ArchMinix

- libcpu: c6x dsp support
- libbsp: c6678 board support
- librtems: rtos support
- libc: c support

## About RTEMS

I've partitioned the source files into multiple sub-folders.

Hope it can serve as a starting point for you to port them to other RTOSes such as [RT-Thread](https://github.com/RT-Thread/rt-thread/tree/master/bsp/tms320c6678), FreeRTOS, Zephyr, and so on.

## About BSP

The repo contains:

- boot
- clock
- console
- timer
- network
- flash

## More than BSP

- dynamic loader

## About libc

RTEMS uses newlib, whereas TI CCS uses its own libc, called rts. I have consolidated them.

## How to build

You can download [TI CCS toolchain](https://www.ti.com.cn/tool/cn/CCSTUDIO) for C6x platforms.
