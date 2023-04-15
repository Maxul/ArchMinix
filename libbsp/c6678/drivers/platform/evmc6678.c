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
 * File	Name:       evmc6678.c
 *
 * Description: This contains   TMS320C6678 specific functions.
 * 
 ******************************************************************************/
 
/************************
 * Include Files
 ************************/
#include "platform_internal.h"

static void pll_delay(uint32_t ix)
{
    while (ix--) {
        asm("   NOP");
    }
}

int prog_pll1_values(PllcHwSetup *hwSetup)
{
    uint32_t temp;

    /* Check the Range for Multiplier and Divider (error > 63) here */
    if (hwSetup->pllM > 4095)
    {
        return -1;
    }

    if (hwSetup->preDiv > 63)
    {
        return -1;
    }

    /* Set the PLL Multiplier, Divider, BWADJ                                    *
     * The PLLM[5:0] bits of the multiplier are controlled by the PLLM Register  *
     * inside the PLL Controller and the PLLM[12:6] bits are controlled by the   *
     * chip-level MAINPLLCTL0 Register.                                          *
     * PLL Control Register (PLLM)  Layout                                       *
     * |31...6   |5...0        |                                                 *
     * |Reserved |PLLM         |                                                 *
     *                                                                           *
     * Main PLL Control Register (MAINPLLCTL0)                                   *
     * |31...24   |23...19   |18...12    | 11...6   |5...0 |                     *
     * |BWADJ[7:0]| Reserved |PLLM[12:6] | Reserved | PLLD |                     */

    /* Set pll multipler (13 bit field) */
    PLLM_REG     = (hwSetup->pllM & 0x0000003F); /* bits[5:0]  */
    temp          = (hwSetup->pllM & 0x1FC0) >> 6;       /* bits[12:6] */
    MAINPLLCTL0_REG  &=~(0x0007F000);                /*Clear PLLM field */
    MAINPLLCTL0_REG  |=((temp << 12) & 0x0007F000);

    /* Set the BWADJ     (12 bit field)                                          *
     * BWADJ[11:8] and BWADJ[7:0] are located in MAINPLLCTL0 and MAINPLLCTL1     *
     * registers. BWADJ[11:0] should be programmed to a value equal to half of   *
     * PLLM[12:0] value (round down if PLLM has an odd value)                    *
     * Example: If PLLM = 15, then BWADJ = 7                                     */
    temp = ((hwSetup->pllM + 1)>> 1) - 1; /* Divide the pllm by 2 */
    MAINPLLCTL0_REG &=~(0xFF000000);  /* Clear the BWADJ Field */
    MAINPLLCTL0_REG |=  ((temp << 24) & 0xFF000000);
    MAINPLLCTL1_REG &=~(0x0000000F);   /* Clear the BWADJ field */
    MAINPLLCTL1_REG |= ((temp >> 8) & 0x0000000F);

    /* Set the pll divider (6 bit field)                                         *
     * PLLD[5:0] is located in MAINPLLCTL0                                       */
    MAINPLLCTL0_REG   &= ~(0x0000003F);    /* Clear the Field */
    MAINPLLCTL0_REG   |= (hwSetup->preDiv & 0x0000003F);

    /* Set the OUTPUT DIVIDE (4 bit field) in SECCTL */
    SECCTL_REG    &= ~(0x00780000);     /* Clear the field       */
    SECCTL_REG   |= ((1 << 19) & 0x00780000) ;

    return(0);
}

CSL_Status CorePllcHwSetup (
        PllcHwSetup          *hwSetup
        )
{
    CSL_Status       status = CSL_SOK;
    volatile uint32_t i, loopCount;
    uint32_t temp;

    /* Unlock the Boot Config */
    CSL_BootCfgUnlockKicker();


    /* 1. Wait for Stabilization time (min 100 us)                             *
     * The below loop is good enough for the Gel file to get minimum of        *
     * 100 micro seconds, this should be appropriately modified for port       *
     * to a C function                                                         *
     * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
     * more than required, but should be Okay                                  */
    pll_delay(140056);

    /* 2. Check the status of BYPASS bit in SECCTL register,                   *
     *    execute the following steps if                                       *
     *    BYPASS == 1 (if bypass enabled), if BYPASS==0 then Jump to Step 3    */
    temp = SECCTL_REG & 0x00800000; /* Check the Bit 23 value */
     
    if (temp != 0) /* PLL BYPASS is enabled, we assume if not in Bypass ENSAT = 1 */
    {
        /* 2a. Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
         * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.          *
         * The PLL initialization sequence in the boot ROM sets this bit to 0 and        *
         * could lead to non-optimal PLL operation. Software can set the bit to the      *
         * optimal value of 1 after boot                                                 *
         * Ref: http://www.ti.com/lit/er/sprz334b/sprz334b.pdf                           *
         * |31...7   |6     |5 4       |3...0      |                                     *
         * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                     */
         
        MAINPLLCTL1_REG = MAINPLLCTL1_REG | 0x00000040;
         
        /* 2b. Clear PLLEN bit (bypass enabled in PLL controller mux) */
        PLLCTL_REG &= ~(1 << 0);
         
        /* 2c. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
        PLLCTL_REG &= ~(1 << 5);
         
        /* 2d. Wait for 4 RefClks(to make sure the PLL controller *
         * mux switches properly to the bypass)                   *
         * Assuming slowest Ref clock of 25MHz, min: 160 ns delay */
        pll_delay(225);
        
        /* 2e. Bypass needed to perform PWRDN cycle for C6670 and C6657              *
         * Needed on all devices when in NOBOOT, I2C or SPI boot modes               *
         * Ref: Figure 4-2 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf         *
         * PLL Secondary Control Register (SECCTL)  Layout                           *
         * |31...24  |23     |22...19       |18...0   |                              *
         * |Reserved |BYPASS |OUTPUT DIVIDE |Reserved |                              */
        SECCTL_REG |= 0x00800000; /* Set the Bit 23 */
        
        /* 2f. Advisory 8: Multiple PLLs May Not Lock After Power-on Reset Issue     *
         * In order to ensure proper PLL startup, the PLL power_down pin needs to be *
         * toggled. This is accomplished by toggling the PLLPWRDN bit in the PLLCTL  *
         * register. This needs to be done before the main PLL initialization        *
         * sequence                                                                  *
         * Ref: Figure 4-1 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf         *
         * PLL Control Register (PLLCTL)  Layout                                     *
         * |31...4   |3      |2        |1        |0        |                         *
         * |Reserved |PLLRST |Reserved |PLLPWRDN |Reserved |                         */
        
        PLLCTL_REG |= 0x00000002; /* Power Down the PLL */
        
        /* 2g. Stay in a loop such that the bit is set for 5 µs (minimum) and        *
         * then clear the bit.                                                       */
        
        pll_delay(14005);
        
        /* 2h. Power up the PLL */
        PLLCTL_REG &= ~(0x00000002);
    }
    else
    {
        /* 3. Enable BYPASS in the PLL contoller */
        
        /* 3a. Clear PLLEN bit (bypass enabled in PLL controller mux) */
        PLLCTL_REG &= ~(1 << 0);
        
        /* 3b. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
        PLLCTL_REG &= ~(1 << 5);
        
        /* 3c. Wait for 4 RefClks (to make sure the PLL controller *
         * mux switches properly to bypass)                        *
         * Assuming slowest Ref clock of 25MHz, min: 160 ns delay  */
         pll_delay(225);
    }
    
    /* 4, 5, 6 and 7 are done here:                                             *
     * Program the necessary multipliers/dividers and BW adjustments            */
    prog_pll1_values(hwSetup);
	
	/* 8. Set PLL dividers if needed */
    
	/* part of 8, go stat bit needs to be zero here                         *
     * Read the GOSTAT bit in PLLSTAT to make sure the bit reurns to 0 to   *
     * indicate that the GO operation has completed                         *
     * wait for the GOSTAT, but don't trap if lock is never read            */
    for (i = 0; i < 100; i++)
    {
        pll_delay(300);
        if ( (PLL1_STAT & 0x00000001) == 0 ) {
            break;
        }
    }
    if ( i == 100 ) {
        return CSL_ESYS_FAIL;
    }
	
     /* part of 8, Set PLL dividers if needed */
    PLL1_DIV2 = (0x8000) | (hwSetup->pllDiv2);
    PLL1_DIV5 = (0x8000) | (hwSetup->pllDiv5);
    PLL1_DIV8 = (0x8000) | (hwSetup->pllDiv8);
    
    /* part of 8, Program ALNCTLn     *
     * Set bit 1, 4 and 7             */
    PLLALNCTL_REG |= ((1 << 1) | (1 << 4) | (1 << 7));
    
    /* part of 8, Set GOSET bit in PLLCMD to initiate the GO operation to change the divide *
     * values and align the SYSCLKSs as programmed                                          */
    PLLCMD_REG |= 0x00000001;
    
    /* part of 8, go stat bit needs to be zero here                         *
     * Read the GOSTAT bit in PLLSTAT to make sure the bit reurns to 0 to   *
     * indicate that the GO operation has completed                         *
     * wait for the GOSTAT, but don't trap if lock is never read            */
    for (i = 0; i < 100; i++)
    {
        pll_delay(300);
        if ( (PLL1_STAT & 0x00000001) == 0 ) {
            break;
        }
    }
    if ( i == 100 ) {
        return CSL_ESYS_FAIL;
    }
    
    /* 9. Place PLL in Reset, In PLLCTL, write PLLRST = 1 (PLL is reset)        */
    PLLCTL_REG |= 0x00000008;
    
    /* 10. Wait for PLL Reset assertion Time (min: 7 us)                        */
    pll_delay(14006);
    
    /* 11. In PLLCTL, write PLLRST = 0 (PLL reset is de-asserted) */
    PLLCTL_REG &= ~(0x00000008);
    
    /* 12. PLL Lock Delay needs to be 500 RefClk periods * (PLLD + 1)           *
     * i.e., Wait for at least 500 * CLKIN cycles * (PLLD + 1) (PLL lock timer) *
     * Using 2000 25ns RefClk periods per DM                                    *
     * Wait for PLL to lock min 50 us                                           */
    pll_delay(140056 >> 1);
    
    /* 13. In SECCTL, write BYPASS = 0 (enable PLL mux to switch to PLL mode) */
    SECCTL_REG &= ~(0x00800000); /* Release Bypass */
    
    /* 14. In PLLCTL, write PLLEN = 1 (enable PLL controller mux to switch to PLL mode) */
    PLLCTL_REG |= (1 << 0);
    
    /* 15. The PLL and PLL Controller are now initialized in PLL mode - Completed. */
    
    return status;
}

CSL_Status CorePllcGetHwSetup (
        PllcHwSetup             *hwSetup
        )
{
    CSL_Status       status   = CSL_SOK;
    volatile uint32_t i, loopCount;

    /* Unlock the Boot Config */
    CSL_BootCfgUnlockKicker();

    hwSetup->divEnable = 0;

    hwSetup->pllM       = (PLLM_REG & 0x3F);
    hwSetup->preDiv     = PREDIV_REG;
    hwSetup->pllDiv2    = PLLDIV2_REG ;
    hwSetup->pllDiv5    = PLLDIV5_REG;
    hwSetup->pllDiv8    = PLLDIV8_REG;

    /* wait for the GOSTAT, but don't trap if lock is never read */
    for (i = 0; i < 100; i++) {
        pll_delay(300);
        if ( (PLL1_STAT & 0x00000001) == 0 ) {
            break;
        }
    }
    if (i == 100) {
        return CSL_ESYS_FAIL;
    }

    return status;
}

CSL_Status 
SetPaPllConfig
(
 void
 ) 
{
    uint32_t passclksel = (DEVSTAT_REG & PASSCLKSEL_MASK);
    uint32_t papllctl0val = PAPLLCTL0_REG;
    uint32_t obsclkval = OBSCLCTL_REG;
    uint32_t pa_pllm = PLLM_PASS;
    uint32_t pa_plld = PLLD_PASS;
    uint32_t temp;
    int pass_freq;
    int pass_freM,pass_freD;

    if (passclksel == 0) 
    {
        IFPRINT(platform_write("SYSCLK/ALTCORECLK is the input to the PA PLL...\n"));
    }

    /* Unlock the Boot Config */
    CSL_BootCfgUnlockKicker();

    if (DNUM == 0) 
    {
        /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
         * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.      *
         * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
         * could lead to non-optimal PLL operation. Software can set the bit to the  *
         * optimal value of 1 after boot                                             *
         * PAPLLCTL1_REG Bit map                                                     *
         * |31...7   |6     |5 4       |3...0      |                                 *
         * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */

        PAPLLCTL1_REG |= 0x00000040;

        /* Wait for the PLL Reset time (min: 1000 ns)                                */
        /*pll_delay(1400);*/

        /* Put the PLL in Bypass Mode                                                *
         * PAPLLCTL0_REG Bit map                                                     *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */

        PAPLLCTL0_REG |= 0x00800000; /* Set the Bit 23 */

        /*Wait 4 cycles for the slowest of PLLOUT or reference clock source (CLKIN)*/
        /*pll_delay(4);*/

        /* Wait for the PLL Reset time (min: 1000 ns)                                */
        /*pll_delay(1400);*/

        /* Program the necessary multipliers/dividers and BW adjustments             */
        /* Set the divider values */
        PAPLLCTL0_REG &= ~(0x0000003F);
        PAPLLCTL0_REG |= (pa_plld & 0x0000003F);

        /* Set the Multipler values */
        PAPLLCTL0_REG &= ~(0x0007FFC0);
        PAPLLCTL0_REG |= ((pa_pllm << 6) & 0x0007FFC0 );

        /* Set the BWADJ */
        temp = ((pa_pllm + 1) >> 1) - 1;
        PAPLLCTL0_REG &= ~(0xFF000000); 
        PAPLLCTL0_REG |= ((temp << 24) & 0xFF000000);
        PAPLLCTL1_REG &= ~(0x0000000F);
        PAPLLCTL1_REG |= ((temp >> 8) & 0x0000000F);
        
        /* In PLL Controller, reset the PLL (bit 14) in PAPLLCTL1_REG register       */
        PAPLLCTL1_REG |= 0x00004000;

        /*Wait for PLL to lock min 5 micro seconds*/
        pll_delay(7000);

        /*In PAPLLCTL1_REG, write PLLSELECT = 1 (for selecting the output of PASS PLL as the input to PASS) */
        PAPLLCTL1_REG |= 0x00002000;
        
        /*In PAPLLCTL1_REG, write PLLRST = 0 to bring PLL out of reset */
        PAPLLCTL1_REG &= ~(0x00004000);

        /*Wait for PLL to lock min 50 micro seconds*/
        pll_delay(70000);

        /* Put the PLL in PLL Mode                                                   *
         * PAPLLCTL0_REG Bit map                                                     *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        PAPLLCTL0_REG &= ~(0x00800000); /* ReSet the Bit 23 */

        papllctl0val = PAPLLCTL0_REG;

        /* Tells the multiplier value for the PA PLL */
        pa_pllm = (((papllctl0val & PA_PLL_CLKF_MASK) >> 6) + 1);
        IFPRINT(platform_write("PA PLL programmable multiplier = %d\n", pa_pllm));

        /* Tells the divider value for the PA PLL */
        pa_plld = (((papllctl0val & PA_PLL_CLKR_MASK) >> 0) +1);
        IFPRINT(platform_write("PA PLL programmable divider = %d\n", pa_plld));

        // Compute the real pass clk freq (*100)
        pass_freq = (12288 * (pa_pllm) / (pa_plld) / (1));

        // Displayed frequency in MHz
        pass_freM = pass_freq / 100;

        // passclk freq first decimal if freq expressed in MHz
        pass_freD = ((pass_freq - pass_freM * 100) + 5) / 10;

        // Add roundup unit to MHz displayed and reajust decimal value if necessary...
        if (pass_freD > 9)
        {
            pass_freD = pass_freD - 10;
            pass_freM = pass_freM + 1;
        }

        IFPRINT(platform_write("PLL3 Setup for PASSCLK @ %d.%d MHz... \n", pass_freM, pass_freD ));
        IFPRINT(platform_write("PLL3 Setup... Done.\n" ));

        return CSL_SOK;

    }
    else
    {
        IFPRINT(platform_write("DSP core #%d cannot set PA PLL \n",,2,,,DNUM));
        return CSL_ESYS_FAIL;
    }

}

/***************************************************************************************
 * FUNCTION PURPOSE: Power up all the power domains
 ***************************************************************************************
 * DESCRIPTION: this function powers up the PA subsystem domains
 ***************************************************************************************/
void PowerUpDomains (void)
{
    /* PASS power domain is turned OFF by default. It needs to be turned on before doing any 
     * PASS device register access. This not required for the simulator. */

    /* Set PASS Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_ALWAYSON);

    /* Enable the clocks for PASS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_EMIF4F, PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_EMIF25_SPI,  PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_ALWAYSON);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_ALWAYSON));


    /* PASS power domain is turned OFF by default. It needs to be turned on before doing any 
     * PASS device register access. This not required for the simulator. */

    /* Set PASS Power domain to ON */        
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);

    /* Enable the clocks for PASS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto,  PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
}

CSL_Status SetDDR3PllConfig(void) 
{

    uint32_t ddr3pllctl0val = DDR3PLLCTL0_REG;
    uint32_t obsclkval = OBSCLCTL_REG;
    uint32_t ddr3_pllm = PLLM_DDR3;
    uint32_t ddr3_plld = PLLD_DDR3;
    uint32_t temp;

    /* Unlock the Boot Config */
    CSL_BootCfgUnlockKicker();

    if (DNUM == 0) 
    {
        /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
         * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.      *
         * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
         * could lead to non-optimal PLL operation. Software can set the bit to the  *
         * optimal value of 1 after boot                                             *
         * DDR3PLLCTL1_REG Bit map                                                     *
         * |31...7   |6     |5 4       |3...0      |                                 *
         * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */

        DDR3PLLCTL1_REG |= 0x00000040;

        /* Wait for the PLL Reset time (min: 1000 ns)                                */
        /*pll_delay(1400);*/

        /* Put the PLL in Bypass Mode                                                *
         * DDR3PLLCTL0_REG Bit map                                                     *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */

        DDR3PLLCTL0_REG |= 0x00800000; /* Set the Bit 23 */

        /*Wait 4 cycles for the slowest of PLLOUT or reference clock source (CLKIN)*/
        /*pll_delay(4);*/

        /* Wait for the PLL Reset time (min: 1000 ns)                                */
        /*pll_delay(1400);*/

        /* Program the necessary multipliers/dividers and BW adjustments             */
        /* Set the divider values */
        DDR3PLLCTL0_REG &= ~(0x0000003F);
        DDR3PLLCTL0_REG |= (ddr3_plld & 0x0000003F);

        /* Set the Multipler values */
        DDR3PLLCTL0_REG &= ~(0x0007FFC0);
        DDR3PLLCTL0_REG |= ((ddr3_pllm << 6) & 0x0007FFC0 );

        /* Set the BWADJ */
        temp = ((ddr3_pllm + 1) >> 1) - 1;
        DDR3PLLCTL0_REG &= ~(0xFF000000); 
        DDR3PLLCTL0_REG |= ((temp << 24) & 0xFF000000);
        DDR3PLLCTL1_REG &= ~(0x0000000F);
        DDR3PLLCTL1_REG |= ((temp >> 8) & 0x0000000F);
        
        /* In PLL Controller, reset the PLL (bit 13) in DDR3PLLCTL1_REG register       */
        DDR3PLLCTL1_REG |= 0x00002000;

        /*Wait for PLL to lock min 5 micro seconds*/
        pll_delay(7000);

        /*In DDR3PLLCTL1_REG, write PLLRST = 0 to bring PLL out of reset */
        DDR3PLLCTL1_REG &= ~(0x00002000);

        /*Wait for PLL to lock min 50 micro seconds*/
        pll_delay(70000);

        /* Put the PLL in PLL Mode                                                   *
         * DDR3PLLCTL0_REG Bit map                                                   *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        DDR3PLLCTL0_REG &= ~(0x00800000); /* ReSet the Bit 23 */

        ddr3pllctl0val = DDR3PLLCTL0_REG;

        /* Tells the multiplier value for the DDR3 PLL */
        ddr3_pllm = (((ddr3pllctl0val & 0x0007FFC0) >> 6) + 1);
        IFPRINT(platform_write("DDR3 PLL programmable multiplier = %d\n", ddr3_pllm));

        /* Tells the divider value for the DDR3 PLL */
        ddr3_plld = (((ddr3pllctl0val & 0x0000003F) >> 0) +1);
        IFPRINT(platform_write("DDR3 PLL programmable divider = %d\n", ddr3_plld));

        IFPRINT(platform_write("PLL2 Setup... Done.\n" ));

        return CSL_SOK;

    }
    else
    {
        IFPRINT(platform_write("DSP core #%d cannot set DDR3 PLL \n",,2,,,DNUM));
        return CSL_ESYS_FAIL;
    }
}


/*--------------------------------------------------------------*/
/* xmc_setup()                                                  */
/* XMC MPAX register setting to access DDR3 config space        */
/*--------------------------------------------------------------*/
void xmc_setup()
{  
    /* mapping for ddr emif registers XMPAX*2 */
    CSL_XMC_XMPAXL    mpaxl;
    CSL_XMC_XMPAXH    mpaxh;

    /* base addr + seg size (64KB)*/    //"1B"-->"B" by xj */
    mpaxh.bAddr     = (0x2100000B >> 12);
    mpaxh.segSize   = (0x2100000B & 0x0000001F);

    /* replacement addr + perm*/
    mpaxl.rAddr     = 0x100000;
    mpaxl.sr        = 1;
    mpaxl.sw        = 1;
    mpaxl.sx        = 1;
    mpaxl.ur        = 1;
    mpaxl.uw        = 1;
    mpaxl.ux        = 1;

    /* set the xmpax for index2 */
    CSL_XMC_setXMPAXH(2, &mpaxh);
    CSL_XMC_setXMPAXL(2, &mpaxl);    
}

/* Set the desired DDR3 configuration -- assumes 66.67 MHz DDR3 clock input */
CSL_Status DDR3Init() 
{

    CSL_Status status = CSL_SOK;    
    volatile unsigned int loopCount;
    uint32_t ddr3config, ddrPhyCtrl;
    uint8_t ddrPHYReadLatency;
    EMIF4F_TIMING1_CONFIG sdram_tim1;
    EMIF4F_TIMING2_CONFIG sdram_tim2;
    EMIF4F_TIMING3_CONFIG sdram_tim3;
    EMIF4F_OUTPUT_IMP_CONFIG    zqcfg;
    EMIF4F_PWR_MGMT_CONFIG      pwrmgmtcfg;
    EMIF4F_SDRAM_CONFIG         sdramcfg;

    CSL_BootCfgUnlockKicker();        

    /* Wait for PLL to lock = min 500 ref clock cycles. 
       With refclk = 100MHz, = 5000 ns = 5us */
    platform_delaycycles(50000);

    /**************** 3.3 Leveling Register Configuration ********************/
    CSL_BootCfgGetDDRConfig(0, &ddr3config);
    ddr3config &= ~(0x007FE000);  // clear ctrl_slave_ratio field
    CSL_BootCfgSetDDRConfig(0, ddr3config);

    CSL_BootCfgGetDDRConfig(0, &ddr3config);
    ddr3config |= 0x00200000;     // set ctrl_slave_ratio to 0x100
    CSL_BootCfgSetDDRConfig(0, ddr3config);

    CSL_BootCfgGetDDRConfig(12, &ddr3config);
    ddr3config |= 0x08000000;    // Set invert_clkout = 1
    CSL_BootCfgSetDDRConfig(12, ddr3config);

    CSL_BootCfgGetDDRConfig(0, &ddr3config);
    ddr3config |= 0xF;            // set dll_lock_diff to 15
    CSL_BootCfgSetDDRConfig(0, ddr3config);

    CSL_BootCfgGetDDRConfig(23, &ddr3config);
    ddr3config |= 0x00000200;    // See section 4.2.1, set for partial automatic levelling
    CSL_BootCfgSetDDRConfig(23, ddr3config);

    /**************** 3.3 Partial Automatic Leveling ********************/
    ddr3config = 0x0000005E    ; CSL_BootCfgSetDDRConfig(2,  ddr3config);
    ddr3config = 0x0000005E;  CSL_BootCfgSetDDRConfig(3,  ddr3config);
    ddr3config = 0x0000005E;  CSL_BootCfgSetDDRConfig(4,  ddr3config);
    ddr3config = 0x00000051;  CSL_BootCfgSetDDRConfig(5,  ddr3config);
    ddr3config = 0x00000038;  CSL_BootCfgSetDDRConfig(6,  ddr3config);
    ddr3config = 0x0000003A;  CSL_BootCfgSetDDRConfig(7,  ddr3config);
    ddr3config = 0x00000024;  CSL_BootCfgSetDDRConfig(8,  ddr3config);
    ddr3config = 0x00000020;  CSL_BootCfgSetDDRConfig(9,  ddr3config);
    ddr3config = 0x00000044;  CSL_BootCfgSetDDRConfig(10, ddr3config);

    ddr3config = 0x000000DD;  CSL_BootCfgSetDDRConfig(14,  ddr3config);
    ddr3config = 0x000000DD;  CSL_BootCfgSetDDRConfig(15,  ddr3config);
    ddr3config = 0x000000BE;  CSL_BootCfgSetDDRConfig(16,  ddr3config);
    ddr3config = 0x000000CA;  CSL_BootCfgSetDDRConfig(17,  ddr3config);
    ddr3config = 0x000000A9;  CSL_BootCfgSetDDRConfig(18,  ddr3config);
    ddr3config = 0x000000A7;  CSL_BootCfgSetDDRConfig(19,  ddr3config);
    ddr3config = 0x0000009E;  CSL_BootCfgSetDDRConfig(20,  ddr3config);
    ddr3config = 0x000000A1;  CSL_BootCfgSetDDRConfig(21,  ddr3config);
    ddr3config = 0x000000BA;  CSL_BootCfgSetDDRConfig(22,  ddr3config);

    /*Do a PHY reset. Toggle DDR_PHY_CTRL_1 bit 15 0->1->0 */
    CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
    ddrPhyCtrl &= ~(0x00008000);
    CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

    CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
    ddrPhyCtrl |= (0x00008000);
    CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

    CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
    ddrPhyCtrl &= ~(0x00008000);
    CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

    /***************** 3.4 Basic Controller and DRAM configuration ************/
    /* enable configuration */
    /*    hEmif->SDRAM_REF_CTRL    = 0x00005162; */
    CSL_EMIF4F_EnableInitRefresh();
    CSL_EMIF4F_SetRefreshRate(0x5162);

    /*    hEmif->SDRAM_TIM_1   = 0x1113783C; */

    sdram_tim1.t_wtr    = 4;
    sdram_tim1.t_rrd    = 7;
    sdram_tim1.t_rc     = 0x20;
    sdram_tim1.t_ras    = 0x17;
    sdram_tim1.t_wr     = 9;
    sdram_tim1.t_rcd    = 8;
    sdram_tim1.t_rp     = 8;
    CSL_EMIF4F_SetTiming1Config(&sdram_tim1);

    /*    hEmif->SDRAM_TIM_2   = 0x30717FE3; */
    sdram_tim2.t_cke    = 3;
    sdram_tim2.t_rtp    = 4;
    sdram_tim2.t_xsrd   = 0x1FF;
    sdram_tim2.t_xsnr   = 0x071;
    sdram_tim2.t_xp     = 3;
    sdram_tim2.t_odt    = 0;
    CSL_EMIF4F_SetTiming2Config (&sdram_tim2);

    /*    hEmif->SDRAM_TIM_3   = 0x559F86AF; */
    sdram_tim3.t_rasMax     = 0xF;
    sdram_tim3.t_rfc        = 0x06A;
    sdram_tim3.t_tdqsckmax  = 0;
    sdram_tim3.zq_zqcs      = 0x3F;
    sdram_tim3.t_ckesr      = 4;
    sdram_tim3.t_csta       = 0x5;
    sdram_tim3.t_pdll_ul    = 0x5;
    CSL_EMIF4F_SetTiming3Config (&sdram_tim3);    

    /*    hEmif->DDR_PHY_CTRL_1   = 0x0010010F; */
    ddrPHYReadLatency   = 0x0F;
    ddrPhyCtrl          = (0x08008);
    CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

    /*    hEmif->ZQ_CONFIG        = 0x70073214; */
    zqcfg.zqRefInterval     = 0x3214;
    zqcfg.zqZQCLMult        = 3;
    zqcfg.zqZQCLInterval    = 1;
    zqcfg.zqSFEXITEn        = 1;
    zqcfg.zqDualCSEn        = 1;
    zqcfg.zqCS0En           = 1;
    zqcfg.zqCS1En           = 0;
    CSL_EMIF4F_SetOutputImpedanceConfig(&zqcfg);

    /*    hEmif->PWR_MGMT_CTRL    = 0x0; */
    pwrmgmtcfg.csTime           = 0;
    pwrmgmtcfg.srTime           = 0;
    pwrmgmtcfg.lpMode           = 0;
    pwrmgmtcfg.dpdEnable        = 0;
    pwrmgmtcfg.pdTime           = 0;
    CSL_EMIF4F_SetPowerMgmtConfig  (&pwrmgmtcfg);

    /* New value with DYN_ODT disabled and SDRAM_DRIVE = RZQ/7 */ 
    /*    hEmif->SDRAM_CONFIG     = 0x63062A32; */
    CSL_EMIF4F_GetSDRAMConfig (&sdramcfg);
    sdramcfg.pageSize           = 2;
    sdramcfg.eBank              = 0;
    sdramcfg.iBank              = 3;
    sdramcfg.rowSize            = 4;
    sdramcfg.CASLatency         = 10;
    sdramcfg.narrowMode         = 0;
    sdramcfg.CASWriteLat        = 2;
    sdramcfg.SDRAMDrive         = 1;
    sdramcfg.disableDLL         = 0;
    sdramcfg.dynODT             = 0;
    sdramcfg.ddrDDQS            = 0;
    sdramcfg.ddrTerm            = 3;
    sdramcfg.iBankPos           = 0;
    sdramcfg.type               = 3;
    CSL_EMIF4F_SetSDRAMConfig (&sdramcfg);

    pll_delay(840336); /*Wait 600us for HW init to complete*/

    /* Refresh rate = (7.8*666MHz] */
    /*    hEmif->SDRAM_REF_CTRL   = 0x00001450;     */
    CSL_EMIF4F_SetRefreshRate(0x00001450);

    /***************** 4.2.1 Partial automatic leveling ************/
    /*    hEmif->RDWR_LVL_RMP_CTRL      =  0x80000000; */
    CSL_EMIF4F_SetLevelingRampControlInfo(1, 0, 0, 0, 0);

    /* Trigger full leveling - This ignores read DQS leveling result and uses ratio forced value */
    /*    hEmif->RDWR_LVL_CTRL          =  0x80000000; */
    CSL_EMIF4F_SetLevelingControlInfo(1, 0, 0, 0, 0);

    /************************************************************
      Wait for min 1048576 DDR clock cycles for leveling to complete 
      = 1048576 * 1.5ns = 1572864ns = 1.57ms.
      Actual time = ~10-15 ms 
     **************************************************************/
    pll_delay(4201680); //Wait 3ms for leveling to complete

    return (status);

}

