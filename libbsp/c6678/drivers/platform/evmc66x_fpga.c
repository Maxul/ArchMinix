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
 * File Name:  	evmc66x_fpga.c
 *
 * Description:	This file contains the function definitions for accessing
 * 				fpga device registers 
 * 
 ******************************************************************************/
 
/************************
 * Include Files
 ************************/
#include "platform_internal.h"


/******************************************************************************
 * 
 * Function:	getBoardVersion  
 *
 * Description:	Gets the board version
 *
 * Parameters:	void
 *
 * Return Value: Board version
 ******************************************************************************/
uint32_t getBoardVersion(void)
{
    uint8_t uchValue_lo = 0;
    uint8_t uchValue_hi = 0;
    uint32_t ret;

    /* Read the lo REV ID */
    ret = fpgaReadConfigurationRegister(FPGA_REV_ID_LO_OFFSET, &uchValue_lo);

    IFPRINT(platform_write ("FPGA_REV_ID_LO_OFFSET = %d\n", uchValue_lo & 0xff));

    if (ret != SUCCESS) {        
        IFPRINT(platform_write ( "getBoardVersion: Error in reading the fpga config reg lo\n"));
    }
    
    /* Read the hi REV ID */
    ret = fpgaReadConfigurationRegister(FPGA_REV_ID_HI_OFFSET, &uchValue_hi);

    IFPRINT (platform_write("FPGA_REV_ID_HI_OFFSET = %d\n", uchValue_hi & 0xff));

    if (ret != SUCCESS) {        
        IFPRINT( platform_write ( "getBoardVersion: Error in reading the fpga config reg hi\n"));
    }

    return (uchValue_hi << 8 | uchValue_lo);
}

/******************************************************************************
 * 
 * Function:	getFpgaDevID  
 *
 * Description:	Gets the fpga device ID
 *
 * Parameters:	void
 *
 * Return Value: fpga device ID
 ******************************************************************************/
uint32_t getFpgaDevID(void)
{
    uint8_t uchValue_lo = 0;
    uint8_t uchValue_hi = 0;
    uint32_t ret;

    /* Read the lo REV ID */
    ret = fpgaReadConfigurationRegister(FPGA_DEV_ID_LO_OFFSET, &uchValue_lo);

    IFPRINT (platform_write("FPGA_DEV_ID_LO_OFFSET = %d\n", uchValue_lo & 0xff));

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "getBoardVersion: Error in reading the fpga config reg lo\n"));
    }
    

    /* Read the hi REV ID */
    ret = fpgaReadConfigurationRegister(FPGA_DEV_ID_HI_OFFSET, &uchValue_hi);

    IFPRINT (platform_write("FPGA_DEV_ID_HI_OFFSET = %d\n", uchValue_hi & 0xff));

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "getBoardVersion: Error in reading the fpga config reg hi\n"));
    }

    return (uchValue_hi << 8 | uchValue_lo);
}

/******************************************************************************
 * 
 * Function:	fpgaControlUserLEDs  
 *
 * Description:	This function enable/disables user LEDs 
 *
 * Parameters:	uchUserLEDNum - User LED number (0, 1, 2, 3)
 * 				uchStatus - User LED status	
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaControlUserLEDs(FPGA_UserLed uchUserLEDNum, 
                         FPGA_LedStatus uchStatus)
{
    uint8_t uchValue = 0; /* Default value */
    uint8_t temp;
    uint32_t ret;

    if ( (FPGA_USER_LED0 > uchUserLEDNum) ||
         (FPGA_USER_LED3 < uchUserLEDNum)) {         
         /* No action for the invalid LED numbers */
         return;
    }         

    /* Read the current LED status */
    ret = fpgaReadConfigurationRegister(FPGA_DBG_LED_OFFSET, &uchValue);

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaControlUserLEDs: Error in reading the fpga config reg\n"));
    }

    /* Turn on/off the corresponding LED bit number */     
    switch (uchStatus) {
        case FPGA_LED_OFF:
            temp      = (FPGA_LED_ON << uchUserLEDNum);
            uchValue |= temp;
            break;
        case FPGA_LED_ON:
            temp      = ~(FPGA_LED_ON << uchUserLEDNum);
            uchValue &= temp;            
            break;
        default: /* No action */
            break;
    }

    /* Send command to turn on/off the Corresponding LED */
    ret = fpgaWriteConfigurationRegister(FPGA_DBG_LED_OFFSET, uchValue);

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaControlUserLEDs: Error in writing the fpga config reg\n"));
    }

    return;
}

/******************************************************************************
 * 
 * Function:	fpgaDisableNandWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaDisableNandWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaDisableNandWriteProtect: Error in Reading the NAND WP\n"));
    }
    
    uchValue    &= ~(1 << 2); 
    
    /* Send command to disable the NandWrite Protect */
    ret = fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaDisableNandWriteProtect: Error in writing the NAND WP\n"));
    }
    
    return;

}

/******************************************************************************
 * 
 * Function:	fpgaEnableNandWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaEnableNandWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaEnableNandWriteProtect: Error in Reading the NAND WP\n"));
    }
    
    uchValue    |= (1 << 2); 
    
    /* Send command to enable the NandWrite Protect */
    fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaEnableNandWriteProtect: Error in Writing the NAND WP\n"));
    }
    
    return;
}

/******************************************************************************
 * 
 * Function:	fpgaDisableNorWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaDisableNorWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaDisableNorWriteProtect: Error in Reading the NOR WP\n"));
    }
    
    uchValue    |= (1 << 4); 
    
    /* Send command to disable the Nor Write Protect */
    ret = fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);

    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaDisableNorWriteProtect: Error in writing the NOR WP\n"));
    }
    
    return;
}

/******************************************************************************
 * 
 * Function:	fpgaEnableNorWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaEnableNorWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaEnableNorWriteProtect: Error in Reading the NOR WP\n"));
    }
    
    uchValue    |= (1 << 4); 
    
    /* Send command to enable the Nor Write Protect */
    ret = fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaEnableNorWriteProtect: Error in Writing the NOR WP\n"));
    }
    
    return;
}
/******************************************************************************
 * 
 * Function:	fpgaDisableEepromWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaDisableEepromWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaDisableEepromWriteProtect: Error in Reading the EEPROM WP\n"));
    }    
    uchValue    |= (1 << 5); 
    
    /* Send command to disable the EEPROMWrite Protect */
    ret = fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaDisableEepromWriteProtect: Error in Writing the EEPROM WP\n"));
    }    
    
    return;
}
/******************************************************************************
 * 
 * Function:	fpgaEnableEepromWriteProtect  
 *
 * Description:	This function enables the write protect in NAND 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void fpgaEnableEepromWriteProtect(void)
{
    uint8_t uchValue = 0x14; /* default misc reg value */
    uint32_t ret;

    /* Read the current Misc Register status */
    ret = fpgaReadConfigurationRegister(FPGA_MISC_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaEnableEepromWriteProtect: Error in Reading the EEPROM WP\n"));
    }    
    
    uchValue    &= ~(1 << 5); 
    
    /* Send command to enable the EEPROM Write Protect */
    ret = fpgaWriteConfigurationRegister(FPGA_MISC_REG_OFFSET, uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaEnableEepromWriteProtect: Error in Writing the EEPROM WP\n"));
    }    
    
    return;
}
/******************************************************************************
 * 
 * Function:	fpgaGetUserSwitch  
 *
 * Description:	This function gets the status of user switch 
 *
 * Parameters:	uint8_t* - address of byte location
 *
 * Return Value: status 0 = switch off, 1 = switch on
 * 
 ******************************************************************************/
uint32_t fpgaGetUserSwitch(uint32_t uiSwitchNum)
{
    uint8_t uchValue = 0;
    uint32_t ret;

    /* Read the current Reset Button Status Register */
    ret = fpgaReadConfigurationRegister(FPGA_RSTBST_REG_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write("fpgaGetUserSwitch: Error in Reading the USER SWITCH\n"));
    }    
    
    /* Check if BIT 7 is high or low for the user switch */
    uchValue    &= (0x80); 

    if (uchValue == 0)
        return 1;
    else
        return 0;
}

/******************************************************************************
 * 
 * Function:	fpgaGetAMCDetect  
 *
 * Description:	This function gets the status, whether AMC is detected or not   
 *
 * Parameters:	uint8_t* - address of byte location
 *
 * Return Value: status 0 = AMC not detected 
 * 						1 = AMC Detected
 * 
 ******************************************************************************/
uint32_t fpgaGetAMCDetect(void)
{
    uint8_t uchValue = 0;
    uint32_t ret;

    /* Read the current BM_GPIO Switch */
    ret = fpgaReadConfigurationRegister(FPGA_MMC_CTL_OFFSET, &uchValue);
    if (ret != SUCCESS) {        
    	IFPRINT (platform_write( "fpgaGetAMCDetect: Error in Reading the AMC switch\n"));
    }    
    
    uchValue    &= (1); 

    if (uchValue == 0)
        return 1;
    else
        return 0;
}
/******************************************************************************
 * 
 * Function:	fpgaWriteConfigurationRegister  
 *
 * Description:	This function writes to spicified FPGA configuration reg. 
 *
 * Parameters:	uchAddress - Address of configuration reg
 * 				uchValue   - Value to be written	 
 *
 * Return Value: status
 ******************************************************************************/	
uint32_t fpgaWriteConfigurationRegister(uint8_t uchAddress, uint8_t uchValue)
{
    uint16_t cmd;
    uint32_t ret;
    uint16_t value = (uint16_t) uchValue;

    value = value & 0x00FF;

    if(uchAddress > FPGA_MAX_ADDR_OFFSET )
        return INVALID_FPGA_ADDR;

    /* Open the SPI controller for FPGA */
    spi_claim(SPI_FPGA_CS, SPI_FPGA_MAX_FREQ);

    /* Get the current state of the debug LEDs */
    cmd      = (FPGA_WR_CMD | uchAddress)<< 8;
    cmd     |= uchValue;
    ret      = spi_write_word(&cmd, (Uint32 )1, &value, (Uint32)1);
    if (ret) 
    {
        spi_release();        
        return FAIL;
    }    

    spi_release();        
    return SUCCESS;
}

/******************************************************************************
 * 
 * Function:	fpgaReadConfigurationRegister  
 *
 * Description:	This function reads spicified FPGA configuration reg. 
 *
 * Parameters:	uchAddress - Address of configuration reg
 * 				puchValue   - pointer to store the read value
 *
 * Return Value: status
 ******************************************************************************/	
uint32_t fpgaReadConfigurationRegister(uint8_t uchAddress, uint8_t* puchValue)
{
    uint16_t cmd;
    uint32_t ret;
    uint16_t value;
    
    if(uchAddress > FPGA_MAX_ADDR_OFFSET )
        return INVALID_FPGA_ADDR;

    /* Open the SPI controller for FPGA */
    spi_claim(SPI_FPGA_CS, SPI_FPGA_MAX_FREQ);

    /* Get the current state of the debug LEDs */
    cmd      = (FPGA_RD_CMD | uchAddress) << 8;
    ret      = spi_read_word(&cmd, 1, &value, 1);
    
    *puchValue = value & 0xFF; 
    
    if (ret) 
    {
        spi_release();        
        return FAIL;
    }    

    spi_release();        
    return SUCCESS;
}
