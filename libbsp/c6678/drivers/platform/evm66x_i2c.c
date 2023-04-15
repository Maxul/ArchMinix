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
 * File	Name:       evm66x_i2c.c
 *
 * Description: This contains   TMS320C6678 specific i2c functions.
 * 
 ******************************************************************************/

 /* I2C modules */
#include "platform_internal.h"

#if (PLATFORM_EEPROM_IN)

/******************************************************************************
 * 
 * Function:    evmI2cInit  
 *
 * Description: This function initializes I2C controller. 
 *
 * Parameters:  void
 *
 * Return Value: void
 ******************************************************************************/
void evmI2CInit(void)
{
    // Set I2C in reset    
    I2CR->ICMDR &= (~CSL_I2C_ICMDR_IRS_MASK);

    platform_delaycycles(100);
    
    // Set Own Address
    I2CR->ICOAR = I2C_OWN_ADDR;
 
    // Set Default I2C High and Low Clock Hold
    I2CR->ICPSC  = I2C_PRESCALER;
    I2CR->ICCLKL = I2C_CLK_LOW;
    I2CR->ICCLKH = I2C_CLK_HIGH;
 
    // Enable the Xmt, Master Mode
    I2CR->ICMDR = ( CSL_I2C_ICMDR_MST_MASK |
    CSL_I2C_ICMDR_TRX_MASK |
    CSL_I2C_ICMDR_FREE_MASK );

                    
    // Take I2C Out of Reset
    I2CR->ICMDR |= CSL_I2C_ICMDR_IRS_MASK;

    platform_delaycycles(100);
}	

#endif

/* Nothing past this point */

