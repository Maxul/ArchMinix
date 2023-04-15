/*
 *  main
 *
 *  _c_int00 --> _args_main --> main
 *
 *  System start after the initial assembly code
 *  that sets the first stack.
 *  The system build processes are called in sequence
 *  and the KERNEL is started.
 *  The Stack is only used before the KERNEL is started and
 *  is never again used.
 *  The boot_card never returns here.
 */

#include <bsp.h>

#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_cpIntcAux.h>
#include <ti/csl/src/intc/csl_intc.h>

#include "Keystone_common.h"

#define CPU_CLK_KHZ (1250000)

unsigned int Normally_Enabled_Interrupts = 0xFFF0;

extern char _bss_start, _bss_end;
extern char _blob_start, _blob_end;

extern volatile unsigned int cregister DNUM;
extern volatile unsigned int cregister TSCL;
extern volatile unsigned int cregister TSCH;

extern void TSC_init();
extern void RegInit(void);
extern void Timer_Init(void);
extern int boot_card(int argc, char** argv, char** envp);

/*
 *  main
 *
 *  System start after the initial assembly code
 *  that sets the first stack.
 *  The system build processes are called in sequence
 *  and the KERNEL is started.
 *  The Stack is only used before the KERNEL is started and
 *  is never again used.
 *  The boot_card never returns here.
 */
void Interrupts_Init()
{
    /*clear exception flag*/
    ECR = EFR;
    IERR = 0;

    /*Eanble external exception, global exception enable*/
    TSR = TSR | (1 << CSL_CHIP_TSR_XEN_SHIFT) | (1 << CSL_CHIP_TSR_GEE_SHIFT) | (1 << CSL_CHIP_TSR_GIE_SHIFT);

    /*enable NMI exception*/
    IER = IER | (1 << CSL_CHIP_IER_NMI_SHIFT);

    /* Enable All Interrupts */
    IER |= Normally_Enabled_Interrupts;
}

void main()
{
    int i;

    /* clear the bss segment , due to --dynamic=exe , it may not */
    memset(&_bss_start, 0, (&_bss_end) - (&_bss_start));

    /* clear the blob segment */
    // memset(&_blob_start, 0x00000000, (&_blob_end) - (&_blob_start));

    /* Disable all interrupts */
    asm("\tDINT");
    /* Disable GIE */
    ICR = 0xFFFFFFFF;
    IER = 0;
    RegInit();

    CSL_BootCfgUnlockKicker();

    Interrupts_Init();

    /* Let core0 do the bussiness of initializing PCI/SPI/SRIO/CPII/PA etc. */
    if (DNUM == 0) {
    }

    /* Enable Interrupts */
    IER |= Normally_Enabled_Interrupts;

    /* After INT being enabled, TSCL will start to count */
    TSC_init();
    Timer_Init();

    /* C entry point for ALL RTEMS BSPs */
    boot_card(0, 0, 0);
}
