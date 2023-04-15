/* ========================================================================== */
/**
 *  @file   platform_internal.h
 *
 *  @brief  Private definitions for the Platform Library.
 *
 *  ============================================================================
 */
/* --COPYRIGHT--,BSD
 * Copyright (c) 2011, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/

#ifndef PLATFORM_INTERNAL_H_
#define PLATFORM_INTERNAL_H_

/********************************************************************************************
 * 					BUILD OPTIONS FOR THE LIBRARY											*
 *******************************************************************************************/

/***
 * @brief  The following flags are used for controlling the build of Platform Library
 */

/*  brief Platform Library version number */
#define PLATFORM_LIB_VERSION "2.00.00.14"

/* Turn on and off debug statements (may not be safe in certain contexts) */
#define	PLATFORM_DEBUG	0

/* These flags compile in and out functionality offered by the library allowing you to control
 * the size of the code that is included.
 */
#ifdef _PLATFORM_LITE_
	/*
	 * Build a version of Platform Library suitable for use in an EEPROM or other small application.
	 * (likely needs tailoring to the specific application if size matters)
	 */
	#define	PLATFORM_VERSTRING_IN		1
	#define	PLATFORM_INIT_IN			1
	#define	PLATFORM_GETINFO_IN			1
	#define PLATFORM_GETCOREID_IN		1
	#define PLATFORM_GETSWITCHSTATE_IN	0
	#define PLATFORM_GETMACADDR_IN		1
	#define PLATFORM_GETPHYADDR_IN		0
	#define PLATFORM_PHYLINKSTATUS_IN	0
	#define PLATFORM_DELAY_IN			1
	#define PLATFORM_LED_IN				1
	#define PLATFORM_WRITE_IN			0
	#define	PLATFORM_EXTMEMTEST_IN		1
	#define PLATFORM_INTMEMTEST_IN		0
	#define	PLATFORM_EEPROM_IN			1
	#define	PLATFORM_EEPROM_WRITE_IN	1
	#define	PLATFORM_UART_IN			1
	#define	PLATFORM_NOR_IN				1
	#define	PLATFORM_NOR_READ_IN		1
	#define	PLATFORM_NOR_WRITE_IN		0
	#define PLATFORM_NAND_IN			1
	#define PLATFORM_NAND_READ_IN		1
	#define PLATFORM_NAND_WRITE_IN		0
	#define PLATFORM_SEMLOCK_IN			0
	#define PLATFORM_CACHE_IN			0
#else
	/*
	 * Build the FULL version of Platform Library
	 */
	#define	PLATFORM_VERSTRING_IN		1
	#define	PLATFORM_INIT_IN			1
	#define	PLATFORM_GETINFO_IN			1
	#define PLATFORM_GETCOREID_IN		1
	#define PLATFORM_GETSWITCHSTATE_IN	1
	#define PLATFORM_GETMACADDR_IN		1
	#define PLATFORM_GETPHYADDR_IN		1
	#define PLATFORM_PHYLINKSTATUS_IN	1
	#define PLATFORM_DELAY_IN			1
	#define PLATFORM_LED_IN				1
	#define PLATFORM_WRITE_IN			1
	#define	PLATFORM_EXTMEMTEST_IN		1
	#define PLATFORM_INTMEMTEST_IN		1
	#define	PLATFORM_EEPROM_WRITE_IN	1
	#define	PLATFORM_EEPROM_IN			1
	#define	PLATFORM_UART_IN			1
	#define	PLATFORM_NOR_IN				1
	#define	PLATFORM_NOR_READ_IN		1
	#define	PLATFORM_NOR_WRITE_IN		1
	#define PLATFORM_NAND_IN			1
	#define PLATFORM_NAND_READ_IN		1
	#define PLATFORM_NAND_WRITE_IN		1
	#define PLATFORM_SEMLOCK_IN			0
	#define PLATFORM_CACHE_IN			1
#endif

#if (PLATFORM_DEBUG && !PLATFORM_WRITE_IN)
#error	You must enable PLATFORM_WRITE to turn on DEBUG
#endif

#if (PLATFORM_DEBUG)
#define IFPRINT(x)   (x)
#else
#define IFPRINT(x)
#endif

/* This flag implements a workaround to re-initialize PLL and DDR 
   if DDR test after DDR initialization */
#define PLATFORM_PLL_REINIT
#define PLL_REINIT_DDR3_TEST_START_ADDR (0x80000000)
#define PLL_REINIT_DDR3_TEST_END_ADDR   (PLL_REINIT_DDR3_TEST_START_ADDR + (128 *1024))

/********************************************************************************************
 * 					Includes for the Library Routines										*
 *******************************************************************************************/

#include "types.h"
#include <ti/csl/csl_types.h>

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ti/csl/csl_chip.h"
#include "ti/csl/csl_chipAux.h"
#include "ti/csl/csl_semAux.h"
#include "ti/csl/cslr_device.h"
#include "ti/csl/cslr_psc.h"
#include "ti/csl/csl_psc.h"
#include "ti/csl/cslr_emif16.h"
#include "ti/csl/csl_emif4f.h"
#include "ti/csl/csl_emif4fAux.h"

#include "evmc66x_fpga.h"

#include "evmc66x_i2c.h"
#include "ti/csl/cslr_i2c.h"

#include "ti/csl/csl_cpsw.h"
#include "ti/csl/csl_cpsgmii.h"
#include "ti/csl/csl_cpsgmiiAux.h"
#include "ti/csl/csl_mdio.h"
#include "ti/csl/csl_mdioAux.h"

#include "evmc66x_uart.h"
#include "ti/csl/cslr_uart.h"

#include "evmc66x_gpio.h"
#include "ti/csl/csl_gpioAux.h"

#include "evmc66x_i2c_eeprom.h"

#include "evmc66x_nand.h"
#include "ti/csl/csl_pscAux.h"
#include "ti/csl/csl_bootcfg.h"
#include "ti/csl/csl_bootcfgAux.h"

#include "evmc66x_nor.h"
#include "evmc66x_spi.h"
#include "ti/csl/cslr_spi.h"

#include "ti/csl/csl_pllcAux.h"
#include "ti/csl/csl_xmcAux.h"
#include "evmc66x_pllc.h"

/********************************************************************************************
 * 					Platform Specific Declarations											*
 *******************************************************************************************/

#define PLATFORM_INFO_CPU_NAME 		"TMS320C6678"
#define PLATFORM_INFO_BOARD_NAME 	"TMDXEVM6678L"

#define MEGM_REV_ID_REGISTER   (0x01812000)
#define MEGM_REV_ID_MAJ_MASK   (0xFFFF0000)
#define MEGM_REV_ID_MAJ_SHIFT  (16)
#define MEGM_REV_ID_MIN_MASK   (0x0000FFFF)
#define MEGM_REV_ID_MIN_SHIFT  (0)

/* Clock rate */
#define PLATFORM_BASE_CLK_RATE_MHZ (100)

/* PREDIV */
#define PLATFORM_PLL_PREDIV_val (1)

/* POSTDIV */
#define PLATFORM_PLL_POSTDIV_val (2)

/* Default PLL PLLM value (100/1*(20/2)) = 1.0GHz) */
#define  PLATFORM_PLL1_PLLM_val (20)

/* Default PLL PLLD value for 1.0GHz) */
#define  PLATFORM_PLL1_PLLD_val (1)

/* Default UART baudrate value */
#define  PLATFORM_UART_BAUDRATE_val (19200)

/* Input crystal frequency 100 MHz */
#define PLATFORM_UART_INPUT_CLOCK_RATE ((PLATFORM_BASE_CLK_RATE_MHZ * PLATFORM_PLL1_PLLM_val * 1000000)/(PLATFORM_PLL_PREDIV_val * 12 * PLATFORM_PLL1_PLLD_val)) /* SYSCLK7 = CPU_CLK/7 in Hz */

/* 1/x-rate clock for CorePac (emulation) and the ADTF module */
#define  PLATFORM_PLLDIV2_val  (3)
/* 1/y-rate clock for system trace module only */
#define  PLATFORM_PLLDIV5_val (5)
/* 1/z-rate clock is used as slow_sysclck in the system */
#define  PLATFORM_PLLDIV8_val (64)


/* LED Definitions */
#define PLATFORM_FPGA_LED_COUNT  (4)
#define PLATFORM_I2C_LED_COUNT   (0)
#define PLATFORM_TOTAL_LED_COUNT (PLATFORM_FPGA_LED_COUNT + PLATFORM_I2C_LED_COUNT)

/* Number of cores on the platform */
#define PLATFORM_CORE_COUNT 8

/* Mmeory Sections */
#define PLATFORM_L1P_BASE_ADDRESS 0x10E00000
#define PLATFORM_L1P_SIZE         0x8000    /* 32K bytes */
#define PLATFORM_L1D_BASE_ADDRESS 0x10F00000
#define PLATFORM_L1D_SIZE         0x8000    /* 32K bytes */
#define PLATFORM_DDR3_SDRAM_START 0x80000000
#define PLATFORM_DDR3_SDRAM_END   0x9FFFFFFF /* 512 MB */

/* 24AA1025 EEPROM */
#define PLATFORM_EEPROM_MANUFACTURER_ID (0x01)
#define PLATFORM_EEPROM_DEVICE_ID_1     (0x50)
#define PLATFORM_EEPROM_DEVICE_ID_2     (0x51)

/********************************************************************************************
 * 					General Declarations													*
 *******************************************************************************************/

/* Size of a string we can output with platform_write */
#define	MAX_WRITE_LEN	80

/* Device Tables */
#if (PLATFORM_NAND_IN)
extern PLATFORM_DEVICE_info gDeviceNand;
#endif

#if (PLATFORM_NOR_IN)
extern  PLATFORM_DEVICE_info gDeviceNor;
#endif

#if (PLATFORM_EEPROM_IN)
extern PLATFORM_DEVICE_info gDeviceEeprom0;
extern PLATFORM_DEVICE_info gDeviceEeprom1;
#endif


/********************************************************************************************
 * 					Function Prototypes														*
 *******************************************************************************************/
#if (PLATFORM_SEMLOCK_IN)
#define PLIBSPILOCK() Osal_platformSpiCsEnter();
#define PLIBSPIRELEASE() Osal_platformSpiCsExit ();
#else
#define PLIBSPILOCK()
#define PLIBSPIRELEASE()
#endif

/* Function prototypes that don't live anywhere else */
extern void configSerdes();
extern void Init_SGMII(uint32_t macport);
extern void PowerUpDomains (void);
extern void xmc_setup();

/********************************************************************************************
 *                     PLL control local Declarations                                          *
 *******************************************************************************************/

/**
 *  Handle to access BOOTCFG registers.
 */
#define hPscCfg     ((CSL_PscRegs*)CSL_PSC_REGS)

/**
  @}
  */
/* Boot Cfg Registers */
#define   DEVSTAT_REG           (*((volatile uint32_t *) 0x02620020))
#define   MAINPLLCTL0_REG       (*((volatile uint32_t *) 0x02620328))
#define   MAINPLLCTL1_REG       (*((volatile uint32_t *) 0x0262032C))
#define   DDR3PLLCTL0_REG       (*((volatile uint32_t *) 0x02620330))
#define   DDR3PLLCTL1_REG       (*((volatile uint32_t *) 0x02620334))

/* PA PLL Registers */
#define   PAPLLCTL0_REG         (*((volatile uint32_t *) 0x02620338))
#define   PAPLLCTL1_REG         (*((volatile uint32_t *) 0x0262033C))

#define   OBSCLCTL_REG          (*((volatile uint32_t *) 0x026203AC))

/*PLL controller registers*/
#define   PLLCTL_REG            (*((volatile uint32_t *) 0x02310100))
#define   SECCTL_REG            (*((volatile uint32_t *) 0x02310108))
#define   PLLM_REG              (*((volatile uint32_t *) 0x02310110))
#define   PLLCMD_REG            (*((volatile uint32_t *) 0x02310138))
#define   PLLSTAT_REG           (*((volatile uint32_t *) 0x0231013C))
#define   PLLALNCTL_REG         (*((volatile uint32_t *) 0x02310140))
#define   PLLDIV2_REG           (*((volatile uint32_t *) 0x0231011C))
#define   PLLDIV5_REG           (*((volatile uint32_t *) 0x02310164))
#define   PLLDIV8_REG           (*((volatile uint32_t *) 0x02310170))
#define   PREDIV_REG            (*((volatile uint32_t *) 0x02310114))

#define PA_PLL_OBS_CLK_SEL_MASK (1 << 4) /* OBSCLKCTL Register Bit 4 - set to 0 to see PA PLL reference (input) clock, set to 1 to see PA PLL output*/
#define PA_PLL_OBS_CLK_EN_MASK  (1 << 5) /* OBSCLKCTL Register Bit 5 - set to 1 to enable power to PA PLL observation clock*/

/* PA PLL Registers */
#define BYPASS_BIT_SHIFT 23
#define CLKF_BIT_SHIFT   6
#define PASSCLKSEL_MASK    (1 << 17)    /* Tells the configuration of the PASSCLKSEL pin */
#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */
#define PLL1_BASE           0x02310000
#define PLL1_PLLCTL              (*(unsigned int*)(PLL1_BASE + 0x100))   // PLL1 Control
#define PLL1_SECCTL              (*(unsigned int*)(PLL1_BASE + 0x108))   // PLL1 Sec Control
#define PLL1_PLLM                (*(unsigned int*)(PLL1_BASE + 0x110))   // PLL1 Multiplier
#define PLL1_DIV1                (*(unsigned int*)(PLL1_BASE + 0x118))   // DIV1 divider
#define PLL1_DIV2                (*(unsigned int*)(PLL1_BASE + 0x11C))   // DIV2 divider
#define PLL1_DIV3                (*(unsigned int*)(PLL1_BASE + 0x120))   // DIV3 divider
#define PLL1_CMD                 (*(unsigned int*)(PLL1_BASE + 0x138))   // CMD control
#define PLL1_STAT                (*(unsigned int*)(PLL1_BASE + 0x13C))   // STAT control
#define PLL1_ALNCTL              (*(unsigned int*)(PLL1_BASE + 0x140))   // ALNCTL control
#define PLL1_DCHANGE             (*(unsigned int*)(PLL1_BASE + 0x144))   // DCHANGE status
#define PLL1_CKEN                (*(unsigned int*)(PLL1_BASE + 0x148))   // CKEN control
#define PLL1_CKSTAT              (*(unsigned int*)(PLL1_BASE + 0x14C))   // CKSTAT status
#define PLL1_SYSTAT              (*(unsigned int*)(PLL1_BASE + 0x150))   // SYSTAT status
#define PLL1_DIV4                (*(unsigned int*)(PLL1_BASE + 0x160))   // DIV4 divider
#define PLL1_DIV5                (*(unsigned int*)(PLL1_BASE + 0x164))   // DIV5 divider
#define PLL1_DIV6                (*(unsigned int*)(PLL1_BASE + 0x168))   // DIV6 divider
#define PLL1_DIV7                (*(unsigned int*)(PLL1_BASE + 0x16C))   // DIV7 divider
#define PLL1_DIV8                (*(unsigned int*)(PLL1_BASE + 0x170))   // DIV8 divider
#define PLL1_DIV9                (*(unsigned int*)(PLL1_BASE + 0x174))   // DIV9 divider
#define PLL1_DIV10               (*(unsigned int*)(PLL1_BASE + 0x178))   // DIV10 divider
#define PLL1_DIV11               (*(unsigned int*)(PLL1_BASE + 0x17C))   // DIV11 divider
#define PLL1_DIV12               (*(unsigned int*)(PLL1_BASE + 0x180))   // DIV12 divider
#define PLL1_DIV13               (*(unsigned int*)(PLL1_BASE + 0x184))   // DIV13 divider
#define PLL1_DIV14               (*(unsigned int*)(PLL1_BASE + 0x188))   // DIV14 divider
#define PLL1_DIV15               (*(unsigned int*)(PLL1_BASE + 0x18C))   // DIV15 divider
#define PLL1_DIV16               (*(unsigned int*)(PLL1_BASE + 0x190))   // DIV16 divider

#define BWADJ_BIT_SHIFT     24
#define BYPASS_BIT_SHIFT    23
#define CLKF_BIT_SHIFT      6
#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
#define PA_PLL_BWADJ_MASK (1 << BWADJ_BIT_SHIFT) /* Tells the bandwidth adjust value for the PA PLL */
#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */

#define ENSAT_BIT_SHIFT 6
#define RESET_BIT_SHIFT 14
#define PLLSEL_BIT_SHIFT 13
#define PA_PLL_ENSAT_MASK (1 << ENSAT_BIT_SHIFT) /* Tells the configuration of the ENSAT bit */
#define PA_PLL_RESET_MASK (1 << RESET_BIT_SHIFT) /* Tells the configuration of the RESET bit */
#define PA_PLL_PLLSEL_MASK (1 << PLLSEL_BIT_SHIFT) /* Tells the configuration of the pllsel bit */

/* PASS PLL settings for 1044 MHz */
#define PLLM_PASS 20
#define PLLD_PASS 0

/* DDR3 PLL settings for 1333 MHz */
#define PLLM_DDR3 19
#define PLLD_DDR3 0

#endif
