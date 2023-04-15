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
 * File Name:  	evmc66x_gpio.c
 *
 * Description:	This file contains the lower level function to access GPIO 
 * 				
 * 
 *****************************************************************************/

/************************
 * Include Files
 ************************/
#include "platform_internal.h"


#define GPIOBANKNUM     (0)


/******************************************************************************
 * 
 * Function:		gpioInit  
 *
 * Description:		Initializes the GPIO peripheral
 *
 * Parameters:		void
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioInit(void)
{			
    int32_t pinNum;
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    
    
	// Disable the GPIO global interrupts
	CSL_GPIO_bankInterruptDisable(hGpio, GPIOBANKNUM);
	
	// Clear all falling edge trigger and rising edge trigger
	for (pinNum = GPIO_0; pinNum <= GPIO_15; pinNum++) {
        CSL_GPIO_clearFallingEdgeDetect(hGpio, pinNum);
        CSL_GPIO_clearRisingEdgeDetect (hGpio, pinNum);
    }
	
	// Set all GPIO as input
    //	GPIOREGS->DIR = GPIOREGS->DIR & 0xffffffff;
		
	// Configure the GPIOs for NAND flash controller communication
	// Configure data bus as output
	gpioSetDataBusDirection(GPIO_OUT);
	
	// Configure the signal pins direction
	gpioSetDirection(DSP_FPGA_CMD0, GPIO_OUT );
	gpioSetDirection(DSP_FPGA_CMD1, GPIO_OUT );
	
	gpioSetDirection(DSP_FPGA_STROBE, GPIO_OUT );
	gpioSetDirection(FPGA_DSP_READY, GPIO_IN );	
}

/******************************************************************************
 * 
 * Function:		gpioSetDirection  
 *
 * Description:		This function configures the specified GPIO's direction  
 *
 * Parameters:		uiNumber - 	GPIO number to configure
 * 					direction - GPIO_OUT or GPIO_IN
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioSetDirection( uint32_t uiNumber, GpioDirection direction )
{   
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    if ( direction == GPIO_OUT ) {
        CSL_GPIO_setPinDirOutput(hGpio, uiNumber); // Set to OUTPUT
    }
    else {
        CSL_GPIO_setPinDirInput (hGpio, uiNumber); // Set to INPUT

    }
}

/******************************************************************************
 * 
 * Function:		gpioSetDataBusDirection  
 *
 * Description:		This function configures the GPIO[7:0]'s direction  
 *
 * Parameters:		direction - GPIO_OUT or GPIO_IN
 * 					
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioSetDataBusDirection( GpioDirection direction )
{
    int32_t pinNum;
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    if ( direction == GPIO_OUT ) {
        for (pinNum=GPIO_0; pinNum <=GPIO_7; pinNum++) {
            CSL_GPIO_setPinDirOutput(hGpio, pinNum); // Set to OUTPUT
        }
    }
    else {
        for (pinNum=GPIO_0; pinNum <=GPIO_7; pinNum++) {
            CSL_GPIO_setPinDirInput(hGpio, pinNum); // Set to INPUT
        }
    }
}

/******************************************************************************
 * 
 * Function:		gpioSetOutput  
 *
 * Description:		This function sets the specified GPIO's pin state to 1
 *
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 * Pre-Condition:	The specified GPIO should be configured as output 
 * 
 *****************************************************************************/
void gpioSetOutput( uint32_t uiNumber)
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    CSL_GPIO_setOutputData(hGpio, uiNumber);
}

/******************************************************************************
 * 
 * Function:		gpioClearOutput  
 *
 * Description:		This function Clears the specified GPIO's pin state to 0  
 *
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 * Pre-Condition:	The specified GPIO should be configured as output 
 * 
 *****************************************************************************/
void gpioClearOutput( uint32_t uiNumber)
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    CSL_GPIO_clearOutputData(hGpio, uiNumber);
}

/******************************************************************************
 * 
 * Function:		gpioReadInput  
 *
 * Description:		This function gets the specified GPIO's pin state  
 *
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	uint32_t - Input state of GPIO if success
 * 							  	- else GPIO status
 * 
 * Pre-Condition:	The specified GPIO should be configured as input
 * 
 *****************************************************************************/
uint32_t gpioReadInput( uint32_t uiNumber )
{   
    uint8_t inData = 0;
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    
    
    if(uiNumber > GPIO_MAX_NUMBER)
    	return INVALID_GPIO_NUMBER;
    
    CSL_GPIO_getInputData(hGpio, uiNumber, &inData);

    if( ( inData & GPIO_HIGH ) == GPIO_HIGH )
    	return GPIO_HIGH;
    else
    	return GPIO_LOW;
}

/******************************************************************************
 * 
 * Function:		gpioWriteDataBus  
 *
 * Description:		This function sets the GPIO[7:0] pins state  
 *
 * Parameters:		uchValue - 	Value to set as output
 *
 * Return Value: 	void
 * 
 * Pre-Condition:	The GPIO[7:0] should be configured as output
 * 
 *****************************************************************************/
void gpioWriteDataBus ( uint8_t uchValue )
{
    CSL_GpioHandle  hGpio;
    int32_t pinNum;
    uint8_t value;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    for (pinNum = GPIO_0; pinNum <= GPIO_7; pinNum++) {
        value = (uchValue >> pinNum) & 0x1;

        if (value == 1) {
            CSL_GPIO_setOutputData  (hGpio, pinNum);
        }
        else {
            CSL_GPIO_clearOutputData(hGpio, pinNum);
        }
    }
}

/******************************************************************************
 * 
 * Function:		gpioClearDataBus  
 *
 * Description:		This function sets the value to the Clear data register
 *
 * Parameters:		uchValue - 	Value to set as output
 *
 * Return Value: 	void
 * 
 * Pre-Condition:	None
 * 
 *****************************************************************************/
void gpioClearDataBus(uint8_t uchValue)
{
    CSL_GpioHandle  hGpio;
    int32_t pinNum;
    uint8_t value;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    for (pinNum = GPIO_0; pinNum <= GPIO_7; pinNum++) {
        value = (uchValue >> pinNum) & 0x1;

        if (value == 1) {
            CSL_GPIO_clearOutputData(hGpio, pinNum);
        }
    }

}

/******************************************************************************
 * 
 * Function:		gpioSetDataBus  
 *
 * Description:		This function sets the value to the Set data register
 *
 * Parameters:		uchValue - 	Value to set as output
 *
 * Return Value: 	void
 * 
 * Pre-Condition:	None
 * 
 *****************************************************************************/
void gpioSetDataBus(uint8_t uchValue)
{
    CSL_GpioHandle  hGpio;
    int32_t pinNum;
    uint8_t value;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);    

    for (pinNum = GPIO_0; pinNum <= GPIO_7; pinNum++) {
        value = (uchValue >> pinNum) & 0x1;

        if (value == 1) {
            CSL_GPIO_setOutputData(hGpio, pinNum);
        }
    }

}

/******************************************************************************
 * 
 * Function:		gpioReadDataBus  
 *
 * Description:		This function gets the GPIO[7:0] pins state
 * 
 * Parameters:		void
 *
 * Return Value: 	uint8_t - Input state of GPIO[7:0]
 * 
 * Pre-Condition:	The GPIO[7:0] should be configured as input
 * 
 *****************************************************************************/
uint8_t gpioReadDataBus( void )
{    
    CSL_GpioHandle  hGpio;
    int32_t pinNum;
    uint8_t value, bitval;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  
    value = 0;

    for (pinNum = GPIO_0; pinNum <= GPIO_7; pinNum++) {
        CSL_GPIO_getInputData(hGpio, pinNum, &bitval);
        value |= bitval << pinNum;
    }

    return (value) ; // GPIOREGS->IN_DATA;
}

/******************************************************************************
 * 
 * Function:		gpioEnableGlobalInterrupt  
 *
 * Description:		This function Enables GPIO interrupts to CPU
 * 
 * Parameters:		void
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioEnableGlobalInterrupt( void )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_bankInterruptEnable(hGpio, GPIOBANKNUM); //GPIOREGS->BINTEN |= 0x01;
}

/******************************************************************************
 * 
 * Function:		gpioDisableGlobalInterrupt 
 *
 * Description:		This function Disables GPIO interrupts to CPU
 * 
 * Parameters:		void
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioDisableGlobalInterrupt( void )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_bankInterruptDisable(hGpio, GPIOBANKNUM); //GPIOREGS->BINTEN = 0x00;
}

/******************************************************************************
 * 
 * Function:		gpioSetRisingEdgeInterrupt 
 *
 * Description:		This function sets specified GPIO's rising edge interrupt
 * 
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioSetRisingEdgeInterrupt( uint32_t uiNumber )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_setRisingEdgeDetect(hGpio, uiNumber); //GPIOREGS->SET_RIS_TRIG |= (1 << uiNumber);
}

/******************************************************************************
 * 
 * Function:		gpioClearRisingEdgeInterrupt 
 *
 * Description:		This function clears specified GPIO's rising edge interrupt
 * 
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioClearRisingEdgeInterrupt( uint32_t uiNumber )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_clearRisingEdgeDetect(hGpio, uiNumber); //GPIOREGS->CLR_RIS_TRIG |= (1 << uiNumber);
}

/******************************************************************************
 * 
 * Function:		gpioSetFallingEdgeInterrupt 
 *
 * Description:		This function sets specified GPIO's falling edge interrupt
 * 
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioSetFallingEdgeInterrupt( uint32_t uiNumber )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_setFallingEdgeDetect(hGpio, uiNumber); //GPIOREGS->SET_FAL_TRIG |= (1 << uiNumber);
}

/******************************************************************************
 * 
 * Function:		gpioClearFallingEdgeInterrupt 
 *
 * Description:		This function clears specified GPIO's falling edge interrupt
 * 
 * Parameters:		uiNumber - 	GPIO number to configure
 *
 * Return Value: 	void
 * 
 *****************************************************************************/
void gpioClearFallingEdgeInterrupt( uint32_t uiNumber )
{
    CSL_GpioHandle  hGpio;

    // Open the CSL GPIO Module 0
    hGpio = CSL_GPIO_open (0);  

    CSL_GPIO_clearFallingEdgeDetect(hGpio, uiNumber); //GPIOREGS->CLR_FAL_TRIG |= (1 << uiNumber);
}
/* Nothing past this point */

