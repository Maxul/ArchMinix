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

#include "platform_internal.h"

#if (PLATFORM_UART_IN)

/******************************************************************************
 *
 * File Name:  	evmc66x_uart.c
 *
 * Description:	This file contains APIs for  UART. 
 * 
 * 
 ******************************************************************************/

/************************
 * Include Files
 ************************/

/******************************************************************************
 * 
 * Function:	UartInit  
 *
 * Description:	This function initializes the UART. 
 *
 * Parameters:	void
 *
 * Return Value: void
 * 
 ******************************************************************************/
void UartInit(void)
{
    //      Allows access to the divisor latches of the baud generator during a 
    // read or write operation (DLL and DLH)
    CSL_FINS (hUartRegs->LCR, UART_LCR_DLAB, CSL_UART_LCR_DLAB_ENABLE);
    //      Break condition is disabled.
    CSL_FINS (hUartRegs->LCR, UART_LCR_BC,   CSL_UART_LCR_BC_DISABLE);
    //      Stick parity is disabled.
    CSL_FINS (hUartRegs->LCR, UART_LCR_SP,   CSL_UART_LCR_SP_DISABLE);
    //      Odd parity is selected
    CSL_FINS (hUartRegs->LCR, UART_LCR_EPS,  CSL_UART_LCR_EPS_ODD);
    //      No PARITY bit is transmitted or checked
    CSL_FINS (hUartRegs->LCR, UART_LCR_PEN,  CSL_UART_LCR_PEN_DISABLE);    

    // Set the baudrate,for accessing LCR[7] should be enable 
    hUartRegs->DLL  = DLL_VAL;
    hUartRegs->DLH  = DLM_VAL;

    // Allows access to the receiver buffer register (RBR), 
    // the transmitter holding register (THR), and the
    // interrupt enable register (IER) selected.
    CSL_FINS (hUartRegs->LCR, UART_LCR_DLAB, CSL_UART_LCR_DLAB_DISABLE);
    // Even Parity is selected
    CSL_FINS (hUartRegs->LCR, UART_LCR_EPS, CSL_UART_LCR_EPS_EVEN);
    // Parity Enable 
    CSL_FINS (hUartRegs->LCR, UART_LCR_PEN, CSL_UART_LCR_PEN_ENABLE);
 
    // Disable THR, RHR, Receiver line status interrupts
    CSL_FINS (hUartRegs->IER, UART_IER_ERBI,  CSL_UART_IER_ERBI_DISABLE);
    CSL_FINS (hUartRegs->IER, UART_IER_ETBEI, CSL_UART_IER_ETBEI_DISABLE);
    CSL_FINS (hUartRegs->IER, UART_IER_ELSI,  CSL_UART_IER_ELSI_DISABLE);
    CSL_FINS (hUartRegs->IER, UART_IER_EDSSI, CSL_UART_IER_EDSSI_DISABLE);  

    /* If autoflow control is desired, 
    * write appropriate values to the modem 
    * control register (MCR). Note that all UARTs 
    * do not support autoflow control, see 
    * the device-specific data manual for supported features.
    * 
    * MCR
    * ====================================================
    * Bit  Field   Value   Description
    * 5    AFE     0       Autoflow control is disabled
    * 4    LOOP    0       Loop back mode is disabled.
    * 1    RTS     0       RTS control (UARTn_RTS is disabled, 
    *                      UARTn_CTS is only enabled.)
    * =====================================================
    * 
    * 
    */

    hUartRegs->MCR = 0;

    /* Choose the desired response to 
    * emulation suspend events by configuring 
    * the FREE bit and enable the UART by setting 
    * the UTRST and URRST bits in the power and 
    * emulation management register (PWREMU_MGMT).
    * 
    * 
    * PWREMU_MGMT
    * =================================================
    * Bit  Field   Value   Description
    * 14   UTRST   1       Transmitter is enabled
    * 13   URRST   1       Receiver is enabled
    * 0    FREE    1       Free-running mode is enabled
    * ===================================================
    * 
    */
    hUartRegs->PWREMU_MGMT = 0x6001;

    /* Cleanup previous data (rx trigger is also set to 0)*/
    /* Set FCR = 0x07;        */
    CSL_FINS (hUartRegs->FCR, UART_FCR_FIFOEN,   CSL_UART_FCR_FIFOEN_ENABLE);
    CSL_FINS (hUartRegs->FCR, UART_FCR_TXCLR,    CSL_UART_FCR_TXCLR_CLR);
    CSL_FINS (hUartRegs->FCR, UART_FCR_RXCLR,    CSL_UART_FCR_RXCLR_CLR);
    CSL_FINS (hUartRegs->FCR, UART_FCR_DMAMODE1, CSL_UART_FCR_DMAMODE1_DISABLE);
    CSL_FINS (hUartRegs->FCR, UART_FCR_RXFIFTL,  CSL_UART_FCR_RXFIFTL_CHAR1);

    return;
}

/******************************************************************************
 * 
 * Function:	UartSetBaudRate  
 *
 * Description:	This function sets the UART baudrate. 
 *
 * Parameters:	UINT16 uiBaudRate - baudrate to set
 *
 * Return Value: void
 * 
 ******************************************************************************/
void UartSetBaudRate(uint16_t uiBaudRate)
{
    uint8_t uiDLLVal = 0;
    uint8_t uiDLHVal = 0;

    hUartRegs->LCR = 0x80;
    uiDLLVal = (uint8_t )(0x00FF & uiBaudRate);
    uiDLHVal = (uint8_t )(0x00FF & (uiBaudRate  >> 8));

    // Set the baudrate,for accessing LCR[7] should be enable 
    hUartRegs->DLL  = uiDLLVal;
    hUartRegs->DLH  = uiDLHVal;
    hUartRegs->LCR = 0x03;
}

/******************************************************************************
 * 
 * Function:	UartReadBaudRate  
 *
 * Description:	This function reads the UART baudrate. 
 *
 * Parameters:	UART_device eUartDevice - Uart Device
 *
 * Return Value: UINT16 - 16 bit Baudrate read from UART
 * 
 ******************************************************************************/
uint16_t UartReadBaudRate(void)
{
    uint16_t ushBaudrate = 0;
    uint16_t ushTemp = 0;

    hUartRegs->LCR = 0x80;
    // Read the baudrate 
    ushBaudrate = hUartRegs->DLL;
    ushTemp     = hUartRegs->DLH;
    ushBaudrate = (ushBaudrate & 0xFF) | ((ushTemp & 0xFF) << 8); 
    hUartRegs->LCR = 0x03;
    return ushBaudrate;
}

/******************************************************************************
 * 
 * Function:	UartReadData  
 *
 * Description: This function reads a byte of data from I2C UART device  
 * 
 * Return Value: uint8_t - 8-bit value read from the RBR register
 ******************************************************************************/
uint8_t UartReadData(void)
{
	uint8_t uRcvChar = 0;
	
	uRcvChar = CSL_FEXT(hUartRegs->RBR, UART_RBR_DATA);
	
	return uRcvChar;
}

/******************************************************************************
 * 
 * Function:	UartWriteData
 *
 * Description:	This function writes a byte of data to UART device  
 * 
 * Parameters:	uint8_t uchAddress - Address of 8-bit register
 * 				uint8_t uchByte	-  8-bit data to write to THR
 *
 * Return Value: none
 ******************************************************************************/
void UartWriteData(uint8_t uchByte)

{
    while (!(CSL_FEXT(hUartRegs->LSR, UART_LSR_THRE))) {
		platform_delaycycles(10000);
    }
    CSL_FINS(hUartRegs->THR, UART_THR_DATA, uchByte);
    return;
}



/******************************************************************************
 * 
 * Function:	UartIsDataReady
 *
 * Description:	This function gets the status of DR bit  
 * 
 * Parameters:	none
 * 
 * Return Value: Status of DR bit
 * 				
 ******************************************************************************/
Bool UartIsDataReady(void)
{
	Bool DR_val = FALSE;
	
	if (CSL_UART_LSR_DR_READY == (CSL_FEXT(hUartRegs->LSR, UART_LSR_DR))) {
        DR_val  = TRUE;
    }
	
	return (DR_val);
}
/* Nothing past this point */

#endif
