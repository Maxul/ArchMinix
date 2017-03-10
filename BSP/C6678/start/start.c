#include <bsp.h>

#include <c6x.h>
#include <ti/csl/cslr_chip.h>
#include <ti/csl/csl_bootcfgAux.h>

#define CPU_CLK_KHZ (1250000)

unsigned int Normally_Enabled_Interrupts = 0xFFF0;

void RegInit(void);

void KeyStone_Exception_cfg(Bool);

extern int boot_card(int argc, char **argv, char **envp);

void Interrupts_Init()
{
	/*clear exception flag*/
	ECR = EFR;
	IERR = 0;

	/*Eanble external exception, global exception enable*/
	TSR = TSR|
		(1<<CSL_CHIP_TSR_XEN_SHIFT)|
		(1<<CSL_CHIP_TSR_GEE_SHIFT)|
		(1<<CSL_CHIP_TSR_GIE_SHIFT);

	/*enable NMI exception*/
	IER = IER|(1<<CSL_CHIP_IER_NMI_SHIFT);

	/* Enable All Interrupts */
	IER |= Normally_Enabled_Interrupts;
}

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
int main(int argc, char **argv)
{
	/* Control Registers Initialization */
	RegInit();

	KeyStone_Exception_cfg(TRUE);
	CSL_BootCfgUnlockKicker();

	Interrupts_Init();

	Timer_Init();

	/* C entry point for ALL RTEMS BSPs */
	boot_card(0, NULL, NULL);

	return 0;
}

