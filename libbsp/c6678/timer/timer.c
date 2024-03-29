/**
 * @file
 *
 *  Copyright (c) 2016 Maxul Lee <lmy2010lmy@gmail.com>
 *
 * @brief Driver for timers on the TMS320C6678.
 *
 *  COPYRIGHT (c) 1989-2016.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

/****************************************************************************\
 *           Copyright (C) 2012 Texas Instruments Incorporated.             *
 *                           All Rights Reserved                            *
 *                                                                          *
 * GENERAL DISCLAIMER                                                       *
 * ------------------                                                       *
 * All software and related documentation is provided "AS IS" and without   *
 * warranty or support of any kind and Texas Instruments expressly disclaims*
 * all other warranties, express or implied, including, but not limited to, *
 * the implied warranties of merchantability and fitness for a particular   *
 * purpose.  Under no circumstances shall Texas Instruments be liable for   *
 * any incidental, special or consequential damages that result from the    *
 * use or inability to use the software or related documentation, even if   *
 * Texas Instruments has been advised of the liability.                     *
 ****************************************************************************
This example shows three main features of Timers on KeyStone DSP:
1. generate one-shot pulse and interrupt
2. generate continual clock and interrupts
3. generate continual square waves and interrupts
4. watch-dog
 ****************************************************************************
 * Created by :                                                             *
 *            Brighton Feng                                                 *
 *            Texas Instruments                                             *
 *            March 5, 2012                                                 *
 * Updated by Brighton Feng on Jun 25, 2014, add test case to generate
                                        square waves with timer + EDMA      *
***************************************************************************/
#include "KeyStone_common.h"
#include <c6x.h>
#include <csl_bootcfgAux.h>
#include <csl_edma3.h>
#include <stdio.h>

/*delay (in millisecond) before the timer generate one-shot pulse*/
#define PULSE_DELAY_MS 2000
/*period (in millisecond) of the clock generated by timer*/

///////////////////////////////////////
// set this to tune the speed
#define CLOCK_PERIOD_MS 1
///////////////////////////////////////

/*number of the clocks generated in this test*/
#define NUM_CLOCKS_GENERATED 500

/*period (in millisecond) of the waveform generated by timer*/
#define WAVE_PERIOD_MS 1
/*Duty cycle of the waveform (percentage of low period)*/
#define WAVE_LOW_PERCENT 66
/*number of the waves generated in this test*/
#define NUM_WAVES_GENERATED 500

/*period (in millisecond) of the watch-dog timer*/
#define WATCH_DOG_PERIOD_MS 3000

/*since the timer runs at (DSP core clock)/6
the conversion between millisecond and timer PRD value is:
PRD = (xxx_MS/1000)*DSP_CLK_HZ/6
    = xxx_MS*(DSP_CLK_HZ/1000)/6*/

volatile Uint32 uiTick = 0;

void Timer_Interrupts_Init(void)
{
    gpCGEM_regs->INTMUX1 = 0;
    gpCGEM_regs->INTMUX2 = 0;
    // map local timer interrupt to INT14
#ifdef CSL_GEM_TINT4L
    gpCGEM_regs->INTMUX3 = (CSL_GEM_TINTLN << CSL_CGEM_INTMUX3_INTSEL14_SHIFT)
        | (CSL_GEM_TINT4L << CSL_CGEM_INTMUX3_INTSEL15_SHIFT);
#else
    gpCGEM_regs->INTMUX3 = (CSL_GEM_TINTLN << CSL_CGEM_INTMUX3_INTSEL14_SHIFT)
        | (CSL_GEM_TINT8L << CSL_CGEM_INTMUX3_INTSEL15_SHIFT);
#endif

    // enable INT14
    CPU_interrupt_enable((1 << 14) | (1 << 15));
}

/*use local timer (==DNUM of a core) to generate a clock on TIMO0,
interrupts are generated as well*/
void generate_clocks_by_timer()
{
    Timer64_Config tmrCfg;

    // puts("clocks will be generated on TIMO0...");

    uiTick = 0;

    // select output on TIMO0 from local timer.
    gpBootCfgRegs->TOUTSEL = (DNUM * 2) << CSL_BOOTCFG_TOUTSEL_TOUTSEL0_SHIFT;

    /*configure the timer to generate clocks and interrupts*/
    tmrCfg.timer_num = DNUM;
    tmrCfg.timerMode = TIMER_PERIODIC_CLOCK;
    tmrCfg.period = (unsigned long long)CLOCK_PERIOD_MS * gDSP_Core_Speed_Hz / 6000;
    tmrCfg.reload_period = 0; // not used for this case
    Timer64_Init(&tmrCfg);
#if 0
	//wait until a number of interupts happen
	while(uiTick<NUM_CLOCKS_GENERATED*2)
		asm(" nop 5");
	Reset_Timer(DNUM); 	//stop the timer
#endif
    // puts("clocks were generated on TIMO0\n");
}

void Timer_Init()
{
#if 0
	/*enable TSC, memory protection interrupts, EDC for internal RAM;
    clear cache; protect L1 as cache*/
	KeyStone_common_CPU_init();
	/*print device information.
	Enable memory protection interrupts, EDC for MSMC RAM*/
	KeyStone_common_device_init();

	//enable exception handling
	KeyStone_Exception_cfg(TRUE);
#endif
    CACHE_setL1PSize(CACHE_L1_32KCACHE);
    CACHE_setL1DSize(CACHE_L1_32KCACHE);
    CACHE_setL2Size(CACHE_0KCACHE);
#if 0
	if(C6678_EVM==gDSP_board_type)
	{
		//DSP core speed: 100*10/1=1000MHz
		KeyStone_main_PLL_init(100, 10, 1);
	}
	else if(TCI6614_EVM==gDSP_board_type
		||DUAL_NYQUIST_EVM==gDSP_board_type
		||C6670_EVM==gDSP_board_type)
	{
		//DSP core speed: 122.88*236/29= 999.9889655MHz
		KeyStone_main_PLL_init(122.88, 236, 29);
	}
	else
	{
		puts("Unknown DSP board type!");
		return;
	}
#endif
    Timer_Interrupts_Init();

    generate_clocks_by_timer();
#if 0
	generate_pulse_by_timer();
	generate_clocks_by_timer();
	generate_waves_by_timer();
	watchdog_timer_test();

	TSC_delay_ms(2*WATCH_DOG_PERIOD_MS);
	while(1);
#endif
}
