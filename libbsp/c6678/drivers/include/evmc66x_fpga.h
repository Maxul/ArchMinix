/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
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
 * File	Name:	evmc66x_fpga.h
 *
 * Description:	This is	the header file	for FPGA interface
 *
 ******************************************************************************/

#ifndef	_FPGA_H_
#define	_FPGA_H_

/************************
 * Defines and Macros
 ************************/
#define SPI_FPGA_MAX_FREQ          (25000000) /* SPI FPGA Max frequency in Hz */

/* ------------------------------------------------------------------------ *
 *  FPGA Error Status                                                       *
 * ------------------------------------------------------------------------ */
#define FPGA_STATUS              Uint32           /* NOR status error code */
#define FPGA_EFAIL               (FPGA_STATUS)-1  /* General failure code */
#define FPGA_EOK                 0                /* General success code */

#define FPGA_RD_CMD             (1 << 7)
#define FPGA_WR_CMD             (0 << 7)


/* FPGA Configuration Register offsets */
#define FPGA_DEV_ID_LO_OFFSET   (0x00)
#define FPGA_DEV_ID_HI_OFFSET   (0x01)
#define FPGA_REV_ID_LO_OFFSET   (0x02)
#define FPGA_REV_ID_HI_OFFSET   (0x03)
#define FPGA_BM_GPI_LO_OFFSET   (0x04)
#define FPGA_BM_GPI_HI_OFFSET   (0x05)
#define FPGA_DBG_LED_OFFSET     (0x08)
#define FPGA_MMC_CTL_OFFSET     (0x09)
#define FPGA_RSTBST_REG_OFFSET  (0x0B)
#define FPGA_MISC_REG_OFFSET    (0x0C)
#define FPGA_MAX_ADDR_OFFSET    (0x3F)

// Macros for errors
#define	INVALID_FPGA_CMD		(5)
#define	INVALID_FPGA_ADDR		(6)

/************************
 * Structures and Enums
 ************************/
typedef	enum _FPGA_LedStatus
{
    FPGA_LED_OFF = 0,
    FPGA_LED_ON	= 1
}FPGA_LedStatus;

typedef	enum _FPGA_UserLed
{
    FPGA_USER_LED0 = 0,
    FPGA_USER_LED1 = 1,
    FPGA_USER_LED2 = 2,
    FPGA_USER_LED3 = 3    
}FPGA_UserLed;

/************************
 * Function declarations
 ************************/
uint32_t getBoardVersion(void);
uint32_t getFpgaDevID(void);
uint32_t fpgaGetAMCDetect(void);
void fpgaEnableNandWriteProtect(void);
void fpgaDisableNandWriteProtect(void);
void fpgaEnableNorWriteProtect(void);
void fpgaDisableNorWriteProtect(void);
void fpgaEnableEepromWriteProtect(void);
void fpgaDisableEepromWriteProtect(void);
void fpgaControlUserLEDs(FPGA_UserLed uchUserLEDNum, 
						 FPGA_LedStatus uchStatus);
uint32_t fpgaGetUserSwitch(uint32_t	uiSwitchNum);
uint32_t fpgaWriteConfigurationRegister(uint8_t uchAddress,
										uint8_t uchValue);
uint32_t fpgaReadConfigurationRegister(uint8_t uchAddress,
										uint8_t *puchValue);
#endif // _FPGA_H_





