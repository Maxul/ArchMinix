/****************************************************************************
 * Copyright (c) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
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

/* Errno value */
uint32_t platform_errno = 0;
uint32_t platform_init_return_code = 0;


/* Platform Library Version*/
#if (PLATFORM_VERSTRING_IN)
#pragma DATA_SECTION(platform_library_version,"platform_lib");
static char platform_library_version[16] = PLATFORM_LIB_VERSION;
#endif

/* Information we need to keep around for access */
#pragma DATA_SECTION(platform_mcb,"platform_lib");
static struct platform_mcb_t {
    uint32_t	frequency;
    int32_t  	board_version;
    int32_t		mastercore;
} platform_mcb = {0, 0, 0};

#if (PLATFORM_WRITE_IN)
#pragma DATA_SECTION(write_type,"platform_lib");
static WRITE_info	write_type;
#pragma DATA_SECTION(write_buffer,"platform_lib");
static char			write_buffer[MAX_WRITE_LEN];
#endif


/* This structure holds information about the devices on the platform */
#if (PLATFORM_NAND_IN)
#pragma DATA_SECTION(gDeviceNandBBlist,"platform_lib");
uint8_t gDeviceNandBBlist[BLOCKS_PER_DEVICE];

#pragma DATA_SECTION(gDeviceNand,"platform_lib");
PLATFORM_DEVICE_info gDeviceNand = {0x20,	0x36,PLATFORM_DEVICE_NAND, 8, BLOCKS_PER_DEVICE, PAGES_PER_BLOCK, BYTES_PER_PAGE, SPARE_BYTES_PER_PAGE, PLATFORM_DEVID_NAND512R3A2D, 5, 0x200, 0, NULL, NULL};
#endif

#if (PLATFORM_NOR_IN)
#pragma DATA_SECTION(gDeviceNor,"platform_lib");
PLATFORM_DEVICE_info gDeviceNor  = {0, 0, PLATFORM_DEVICE_NOR, 8, SPI_NOR_SECTOR_COUNT, (SPI_NOR_PAGE_COUNT / SPI_NOR_SECTOR_COUNT), SPI_NOR_PAGE_SIZE, 0, PLATFORM_DEVID_NORN25Q128, 0, 0, 0, NULL, NULL};
#endif

#if (PLATFORM_EEPROM_IN)
#pragma DATA_SECTION(gDeviceEeprom0,"platform_lib");
PLATFORM_DEVICE_info gDeviceEeprom0 = {PLATFORM_EEPROM_MANUFACTURER_ID,PLATFORM_EEPROM_DEVICE_ID_1,PLATFORM_DEVICE_EEPROM,	8, 1, 1, 65536, 0, PLATFORM_DEVID_EEPROM50, 0, 0, 0, NULL, NULL};
#pragma DATA_SECTION(gDeviceEeprom1,"platform_lib");
PLATFORM_DEVICE_info gDeviceEeprom1 = {PLATFORM_EEPROM_MANUFACTURER_ID,PLATFORM_EEPROM_DEVICE_ID_2,PLATFORM_DEVICE_EEPROM,	8, 1, 1, 65536, 0, PLATFORM_DEVID_EEPROM51, 0, 0, 0, NULL, NULL};
#endif

/* This structure holds information about the EMAC port on the platform */
#pragma DATA_SECTION(emac_port_mode,"platform_lib");
PLATFORM_EMAC_PORT_MODE emac_port_mode[PLATFORM_MAX_EMAC_PORT_NUM] =
{
    PLATFORM_EMAC_PORT_MODE_AMC,
    PLATFORM_EMAC_PORT_MODE_PHY
};

#if (PLATFORM_EXTMEMTEST_IN || PLATFORM_INTMEMTEST_IN)
static inline int32_t platform_memory_test (uint32_t start_address, uint32_t end_address);
#endif

/******************************************************************************
 * platform_get_frequency
 *
 * Internal function to read frequency from PLL.
 *
 ******************************************************************************/
static inline uint32_t platform_get_frequency(void)
{
    CSL_Status      status;
    PllcHwSetup     hwSetupRead;
    uint32_t        dsp_freq;
    
    status = CorePllcGetHwSetup (&hwSetupRead);

    if (status != CSL_SOK) {
        IFPRINT(platform_write("platform_get_info: Hardware setup parameters reading... Failed.\n"));
        IFPRINT(platform_write("\tReason: Error setting in hardware validation."\
                " [status = 0x%x].\n", status));
        platform_errno = PLATFORM_ERRNO_GENERIC;
        return (uint32_t)-1;
    } else {
        /* Compute the real dsp freq (*100) */
        dsp_freq = (hwSetupRead.pllM + 1)>> 1;
        dsp_freq = (dsp_freq * PLATFORM_BASE_CLK_RATE_MHZ)/(hwSetupRead.preDiv + 1);        
    }

    return (dsp_freq);
    
}

Bool
serial_num_isvalid
(
    char    c
)
{
    if (
        ((c >= '0') && (c <= '9'))    ||
        ((c >= 'a') && (c <= 'z'))    ||
        ((c >= 'A') && (c <= 'Z'))
       )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#define MAX_SN_SIZE             10      /* Maximum number of the chars of Serial Number for the EVM */
#define MAX_SN_STORE_SIZE       128     /* Maximum size in bytes to store the serial number info */   
static void
getSerialNumber(char *buf)
{
#if (PLATFORM_EEPROM_IN)
	PLATFORM_DEVICE_info    *p_device;
    uint32_t                i;

    buf[0] = 0;

    p_device = platform_device_open(PLATFORM_DEVID_EEPROM50, 0);
    if (p_device != NULL) 
    {
        /* Serial number stored in the last 128 bytes of the EEPROM 0x50 */
        if (platform_device_read(p_device->handle, gDeviceEeprom0.page_size-MAX_SN_STORE_SIZE, (uint8_t *)buf, 16) == Platform_EOK)
        {
            for (i = 0; i < MAX_SN_SIZE; i++)
            {
                if(!serial_num_isvalid(buf[i]))
                {
                    break;
                }
            }
            buf[i] = 0;
        }
        else
        {
            IFPRINT(platform_write("Unable to read board serial number."));
        }
    }

    platform_device_close(p_device->handle);
#endif

    return;
}

/******************************************************************************
 * platform_get_info
 ******************************************************************************/
#if (PLATFORM_GETINFO_IN)
void platform_get_info(platform_info * p_info)
{
    uint32_t 			csr 		= CSL_chipReadCSR();
    volatile uint32_t 	*megm_rev   = (uint32_t *) (MEGM_REV_ID_REGISTER);
    uint32_t 			i;
    
    if (p_info == 0) {
        IFPRINT(platform_write("p_info argument is NULL\n"));
        platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
        return;
    }

    memset(p_info, 0, sizeof(platform_info));

    strncpy(p_info->version, platform_library_version, 16);

    p_info->cpu.core_count 	= PLATFORM_CORE_COUNT;
    p_info->cpu.id 			= (Uint16) CSL_FEXT(csr, CHIP_CSR_CPU_ID);
    p_info->cpu.revision_id = (Uint16) CSL_FEXT(csr, CHIP_CSR_REV_ID);
    strncpy(p_info->cpu.name, PLATFORM_INFO_CPU_NAME, 32);
    
    p_info->cpu.megamodule_revision_major = 
        (uint16_t)(((*megm_rev) & MEGM_REV_ID_MAJ_MASK) >> MEGM_REV_ID_MAJ_SHIFT);
    p_info->cpu.megamodule_revision_minor = 
        (uint16_t)(((*megm_rev) & MEGM_REV_ID_MIN_MASK) >> MEGM_REV_ID_MIN_SHIFT);

    strncpy(p_info->board_name, PLATFORM_INFO_BOARD_NAME, 32);


    if ( TRUE == CSL_BootCfgIsLittleEndian() ) {
        p_info->cpu.endian = PLATFORM_LE;
    } 
    else {
        p_info->cpu.endian = PLATFORM_BE;
    }

    p_info->emac.port_count = PLATFORM_MAX_EMAC_PORT_NUM;

    p_info->frequency 			= platform_get_frequency();
    platform_mcb.frequency 		= p_info->frequency;
    p_info->board_rev 			= getBoardVersion();  
    platform_mcb.board_version 	= p_info->board_rev;
    getSerialNumber(p_info->serial_nbr);

    platform_get_macaddr(PLATFORM_MAC_TYPE_EFUSE, &(p_info->emac.efuse_mac_address[0]));
    platform_get_macaddr(PLATFORM_MAC_TYPE_EEPROM, &(p_info->emac.eeprom_mac_address[0]));

    for (i = PLATFORM_USER_LED_CLASS; i < PLATFORM_END_LED_CLASS; i++ ) {        
        switch (i) {
            case PLATFORM_USER_LED_CLASS:
                p_info->led[i].count     = PLATFORM_FPGA_LED_COUNT;
                break;
            case PLATFORM_SYSTEM_LED_CLASS:
                p_info->led[i].count     = PLATFORM_I2C_LED_COUNT;
                break;
            default:
                IFPRINT(platform_write("Can't read LED Class information\n"));
                platform_errno = PLATFORM_ERRNO_LED;
                break;
        }        
    }
}
#endif

/******************************************************************************
 * platform_init
 ******************************************************************************/
#if (PLATFORM_INIT_IN)

/* Registers to enable or disable memory ECC for L1, L2 and MSMC memories */
#define L1PEDSTAT	0x01846404
#define L1PEDCMD	0x01846408
#define L1PEDADDR	0x0184640C
#define L2EDSTAT	0x01846004
#define L2EDCMD		0x01846008
#define L2EDADDR	0x0184600C
#define L2EDCPEC	0x01846018
#define L2EDCNEC	0x0184601C
#define L2EDCEN		0x01846030
#define SMCERRAR	0x0BC00008
#define	SMCERRXR	0x0BC0000C
#define SMEDCC		0x0BC00010
#define SMCEA		0x0BC00014
#define SMSECC		0x0BC00018

/*Enable EDC on MSMC*/
/* Note: Once MSMC EDC is enabled, error correction stays enabled until
 * the MSMC is reset
 */

static int MSMC_enableEDC ()
{
	unsigned int status = 0;

	*(unsigned int *)(SMEDCC) &= 0x7FFFFFFF;	//Clear SEN(bit31)=0
	*(unsigned int *)(SMEDCC) |= 0x40000000;	//Set ECM(bit30)=1

	/* Check the status */
	status = *(unsigned int *)(SMEDCC);


	if ((status>>30)==0x1)
		/* Enabled */
		return 1;

	/* Failed */
	return 0;
}


/*Enable EDC on L1P*/
static int enableL1PEDC ()
{
	unsigned int status = 0;

	*(unsigned int *)(L1PEDCMD) = 0x1;	//Set EN(bit0)=1

	/* Check the status */
	status = *(unsigned int *)(L1PEDSTAT);

	if ((status<<28) == 0x10000000)
		/* Enabled */
		return 1;

	/* Failed */
	return 0;
}


/*Enable EDC on L2*/
static int enableL2EDC ()
{
	unsigned int status = 0;

	*(unsigned int *)(L2EDCMD) = 0x1;

	/* Check the status */
	status = *(unsigned int *)(L2EDSTAT);

	if ((status<<28) == 0x10000000)
		/* Enabled */
		return 1;

	/* Failed */
	return 0;
}

/*Enable all bits in L2EDCEN*/
static int enableEDCL2EDCEN ()
{
	/* Set DL2CEN(bit0),PL2CEN(bit1),DL2SEN(bit2),PL2SEN(bit3),SDMAEN(bit4)=1 */
	*(unsigned int *)(L2EDCEN) |= 0x1F;
	return 1;
}

Platform_STATUS platform_init(platform_init_flags  * p_flags,
        					  platform_init_config * p_config)
{
    CSL_Status              status;
    PllcHwSetup             pllc_hwSetup;
    PllcHwSetup             pllc_hwSetupRead;

#ifdef PLATFORM_PLL_REINIT
    int loop_count;
#endif

    /*************************************************************************
     * This routine may be called before BIOS or the application has loaded.
     * Do not try and write debug statements from here.
     ***********************************************************************/

    if ((p_flags == 0) || (p_config == 0)){
        platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
        return ( (Platform_STATUS) Platform_EFAIL);
    }

    /* Start TCSL so its free running */
    CSL_chipWriteTSCL(0);

#ifdef PLATFORM_PLL_REINIT
    for (loop_count = 0; loop_count < 10; loop_count++) {
        platform_errno = 0;
#endif

        /* PLLC module handle structure */
        if (p_flags->pll) {

            /* Set the Core PLL */
            /* Clear local data structures */
            memset(&pllc_hwSetup, 0, sizeof(PllcHwSetup));

            /* Setup PLLC hardware parameters */
            pllc_hwSetup.divEnable  = (CSL_BitMask32) (PLLC_DIVEN_PLLDIV2 |
                                                       PLLC_DIVEN_PLLDIV5 |
                                                       PLLC_DIVEN_PLLDIV8) ;

            /* Setup PLLC hardware parameters */        
            pllc_hwSetup.pllM       = 
                    (((p_config->pllm) ? p_config->pllm : PLATFORM_PLL1_PLLM_val) - 1);
            pllc_hwSetup.preDiv   = PLATFORM_PLL_PREDIV_val - 1;
            pllc_hwSetup.pllDiv2  = PLATFORM_PLLDIV2_val - 1;
            pllc_hwSetup.pllDiv5  = PLATFORM_PLLDIV5_val - 1;
            pllc_hwSetup.pllDiv8  = PLATFORM_PLLDIV8_val - 1;
            pllc_hwSetup.postDiv  = PLATFORM_PLL_POSTDIV_val -1;

            /* set Pll */
            status = CorePllcHwSetup (&pllc_hwSetup);

            if (status != CSL_SOK) {
                 platform_errno = PLATFORM_ERRNO_PLL_SETUP;
                return ( (Platform_STATUS) Platform_EFAIL);
            }

            /* Read back */
            status = CorePllcGetHwSetup (&pllc_hwSetupRead);

            if (status != CSL_SOK) {
                platform_errno = PLATFORM_ERRNO_PLL_SETUP;
                return ( (Platform_STATUS) Platform_EFAIL);
            }

            /* Set the PA_SS PLL */
            status = SetPaPllConfig();
            if (status != CSL_SOK) {
                platform_errno = PLATFORM_ERRNO_PLL_SETUP;
                return ( (Platform_STATUS) Platform_EFAIL);
            }

			/* Set the DDR3 PLL if DDR flag is set */
            if (p_flags->ddr) {
                status = SetDDR3PllConfig();
                if (status != CSL_SOK) {
                    platform_errno = PLATFORM_ERRNO_PLL_SETUP;
                    return ( (Platform_STATUS) Platform_EFAIL);
                }
            }
        }
		
        /* Initialize DDR3 */
        if (p_flags->ddr) {
            xmc_setup();

            status = DDR3Init();

            if (status != CSL_SOK) {
                platform_errno = PLATFORM_ERRNO_GENERIC;
                return ( (Platform_STATUS) Platform_EFAIL);
            }
        }

#ifdef PLATFORM_PLL_REINIT
        if (!p_flags->pll || !p_flags->ddr) {
            break;
        }

        /* Run DDR3 test */
        if (platform_memory_test(PLL_REINIT_DDR3_TEST_START_ADDR, 
                PLL_REINIT_DDR3_TEST_END_ADDR) == Platform_EOK) {
            break;
        }
    }
    platform_init_return_code = loop_count;

    if (loop_count == 10) {
        platform_errno = PLATFORM_ERRNO_GENERIC;
        return ( (Platform_STATUS) Platform_EFAIL);
    }
#endif

    /* Save frequency, its needed by  platform_delay */
    if(!platform_mcb.frequency) {
      platform_mcb.frequency = platform_get_frequency();
    }
    PowerUpDomains();

    /* Enable Error Correction for memory */
    if (p_flags->ecc) {
    	   enableL1PEDC();
    	   enableEDCL2EDCEN();
    	   enableL2EDC();
    	   MSMC_enableEDC();
    }

    if (p_flags->phy) {
        configSerdes();
        // Init_SGMII(0);
        Init_SGMII(1);
    }


    return Platform_EOK;
}
#endif

/******************************************************************************
 * platform_get_coreid
 ******************************************************************************/
#if (PLATFORM_GETCOREID_IN)
uint32_t platform_get_coreid(void)
{
    return (CSL_chipReadDNUM());
}
#endif

/******************************************************************************
 * platform_getmacaddr
 ******************************************************************************/
#if (PLATFORM_GETMACADDR_IN)
Platform_STATUS platform_get_emac_info(uint32_t port_num, PLATFORM_EMAC_EXT_info * emac_info)
{
    uint32_t mac_addr2, mac_addr1;

	IFPRINT(platform_write("platform_get_emac_info called \n"));

    emac_info->port_num       = port_num;
    emac_info->mode           = emac_port_mode[port_num];

    CSL_BootCfgGetMacIdentifier(&mac_addr1, &mac_addr2);
    emac_info->mac_address[0] = ((mac_addr2 & 0x0000ff00) >> 8);
    emac_info->mac_address[1] =  (mac_addr2 & 0x000000ff);

    emac_info->mac_address[2] = ((mac_addr1 & 0xff000000) >> 24);
    emac_info->mac_address[3] = ((mac_addr1 & 0x00ff0000) >> 16);
    emac_info->mac_address[4] = ((mac_addr1 & 0x0000ff00) >> 8);
    emac_info->mac_address[5] =  (mac_addr1 & 0x000000ff);

    return Platform_EOK;
}

/*
 * August 15, 2011 - platform_get_macaddr() is deprecated, application needs to call
 * the new API platform_get_emac_info() to get the MAC address of the port
 */
Platform_STATUS platform_get_macaddr(PLATFORM_MAC_TYPE type, uint8_t * p_mac_address)
{
	IFPRINT(platform_write("platform_get_macaddr called \n"));

    switch (type) {

        case PLATFORM_MAC_TYPE_EFUSE:
        {
            uint32_t mac_addr2, mac_addr1;

            CSL_BootCfgGetMacIdentifier(&mac_addr1, &mac_addr2);
            p_mac_address[0] = ((mac_addr2 & 0x0000ff00) >> 8);
            p_mac_address[1] =  (mac_addr2 & 0x000000ff);

            p_mac_address[2] = ((mac_addr1 & 0xff000000) >> 24);
            p_mac_address[3] = ((mac_addr1 & 0x00ff0000) >> 16);
            p_mac_address[4] = ((mac_addr1 & 0x0000ff00) >> 8);
            p_mac_address[5] =  (mac_addr1 & 0x000000ff);

            return Platform_EOK;
        }
        
        default:
        case PLATFORM_MAC_TYPE_EEPROM:
        {
            memset(p_mac_address, 0, 6);
            return ((Platform_STATUS) Platform_EUNSUPPORTED);
        }
    }
}
#endif

/******************************************************************************
 * platform_get_phy_addr
 ******************************************************************************/
#if (PLATFORM_GETPHYADDR_IN)
int32_t platform_get_phy_addr(uint32_t port_num)
{
	IFPRINT(platform_write("platform_get_phy_addr called \n"));

	return port_num;
}
#endif

/******************************************************************************
 * platform_phy_link_status
 ******************************************************************************/
#if (PLATFORM_PHYLINKSTATUS_IN)
Platform_STATUS platform_phy_link_status(uint32_t port_num)
{
    uint32_t phy_addr;
    CSL_MDIO_USERACCESS    user_access_reg;

	IFPRINT(platform_write("platform_get_phy_link_status (portnum = %d) called \n", port_num));

    phy_addr = platform_get_phy_addr( port_num);

    CSL_MDIO_getUserAccessRegister(port_num, &user_access_reg);

    user_access_reg.phyAddr = phy_addr;
    user_access_reg.regAddr = 24;      //The LED Control Reg address 
    user_access_reg.data    &= 0xFFBE; // Need to check this in ?
    user_access_reg.data    |= 1;

    CSL_MDIO_setUserAccessRegister(port_num, &user_access_reg);

    return Platform_EOK;
}
#endif

/******************************************************************************
 * platform_get_switch_state
 ******************************************************************************/
#if (PLATFORM_GETSWITCHSTATE_IN)
uint32_t platform_get_switch_state(uint32_t id)
{
	IFPRINT(platform_write("platform_get_switch_state(id=%d) called \n", id));

   return (fpgaGetUserSwitch(id));
}
#endif


/******************************************************************************
 * platform_uart_read
 ******************************************************************************/
#if (PLATFORM_UART_IN)
Platform_STATUS platform_uart_read(uint8_t *buf, uint32_t delay)
{
    uint32_t delayCount = delay;

    if (buf == NULL){
        return ((Platform_STATUS) Platform_EINVALID);
    }

    while( (UartIsDataReady()) != 1)
    {
        if (delayCount--)
        {
            platform_delay(1);
        }
        else
        {
            IFPRINT(platform_write("platform_uart_read: Read timeout\n"));
            platform_errno = PLATFORM_ERRNO_READTO;
            *buf = UartReadData();
            return ( (Platform_STATUS) Platform_EFAIL);
        }
    }

    *buf = UartReadData ();

    return Platform_EOK;
}

/******************************************************************************
 * platform_uart_write
 ******************************************************************************/
Platform_STATUS platform_uart_write(uint8_t buf)
{
    UartWriteData(buf);

    return Platform_EOK;
}

/******************************************************************************
 * platform_uart_set_baudrate
 ******************************************************************************/
Platform_STATUS platform_uart_set_baudrate(uint32_t baudrate) {

	uint16_t brate;

	IFPRINT(platform_write("platform_uart_set_baudrate(baudrate=%d) called \n", baudrate));

    brate = ((Uint16) (PLATFORM_UART_INPUT_CLOCK_RATE/(baudrate * 16)));

	UartSetBaudRate(brate);

    return Platform_EOK;
}

/******************************************************************************
 * platform_uart_init
 ******************************************************************************/
Platform_STATUS platform_uart_init(void) {

	IFPRINT(platform_write("platform_uart_init called \n"));

	UartInit();

	platform_uart_set_baudrate(115200);

    return Platform_EOK;
}

#endif


/******************************************************************************
 * platform_led
 ******************************************************************************/
#if (PLATFORM_LED_IN)
Platform_STATUS platform_led(uint32_t led_id, PLATFORM_LED_OP operation, LED_CLASS_E led_class)
{


	IFPRINT(platform_write("platform_led(ledid=%d,operation=%d,class=%d) called \n", led_id, operation, led_class));

    switch (led_class) {
        case PLATFORM_USER_LED_CLASS:
            fpgaControlUserLEDs((FPGA_UserLed)(led_id),(FPGA_LedStatus) operation);
            break;
        case PLATFORM_SYSTEM_LED_CLASS:
        	return ( (Platform_STATUS) Platform_EUNSUPPORTED);
        default:
            IFPRINT(platform_write("platform_led: Invalid led_id %d\n", led_id));
            platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
            return ( (Platform_STATUS) Platform_EUNSUPPORTED);
    }
    return Platform_EOK;
}
#endif

/******************************************************************************
 * platform_delay
 ******************************************************************************/
#if (PLATFORM_DELAY_IN)
Platform_STATUS platform_delay(uint32_t usecs)
{
    int32_t delayCount = (int32_t) usecs * platform_mcb.frequency;
    int32_t start_val  = (int32_t) CSL_chipReadTSCL();

    while (((int32_t)CSL_chipReadTSCL() - start_val) < delayCount);

    return Platform_EOK;
}

void platform_delaycycles(uint32_t cycles)
{
	uint32_t start_val  = CSL_chipReadTSCL();

	while ((CSL_chipReadTSCL() - start_val) < cycles);

    return;
}
#endif

/******************************************************************************
 * platform_memory_test
 ******************************************************************************/
#if (PLATFORM_EXTMEMTEST_IN || PLATFORM_INTMEMTEST_IN)
static inline int32_t platform_memory_test (uint32_t start_address, uint32_t end_address)
{
    uint32_t index, value;

    /* Write a pattern */
    for (index = start_address; index < end_address; index += 4) {
        *(volatile uint32_t *) index = (uint32_t)index;
    }

    /* Read and check the pattern */
    for (index = start_address; index < end_address; index += 4) {
        	
    	value = *(uint32_t *) index;
    	
        if (value  != index) {
            IFPRINT(platform_write("platform_memory_test: Failed at address index = 0x%x value = 0x%x *(index) = 0x%x\n",
                index, value, *(volatile uint32_t *) index));
            platform_errno = index;
            return (Platform_EFAIL);
        }
    }

    /* Write a pattern for complementary values */
    for (index = start_address; index < end_address; index += 4) {
        *(volatile uint32_t *) index = (uint32_t)~index;
    }

    /* Read and check the pattern */
    for (index = start_address; index < end_address; index += 4) {

    	value = *(uint32_t *) index;

        if (value  != ~index) {
            IFPRINT(platform_write("platform_memory_test: Failed at address index = 0x%x value = 0x%x *(index) = 0x%x\n",
                index, value, *(volatile uint32_t *) index));
            platform_errno = index;
            return (Platform_EFAIL);
        }
    }

    return Platform_EOK;
}
#endif

/******************************************************************************
 * platform_external_memory_test
 ******************************************************************************/
#if (PLATFORM_EXTMEMTEST_IN)
Platform_STATUS platform_external_memory_test(uint32_t start_address, uint32_t end_address)
{
	IFPRINT(platform_write("platform_external_memory_test(start=0x%x,end=0x%x) called \n", start_address, end_address));

    if((start_address == 0) && (end_address == 0)) {
        start_address = PLATFORM_DDR3_SDRAM_START;
        end_address   = PLATFORM_DDR3_SDRAM_END - 1;
    }

    if ((start_address < PLATFORM_DDR3_SDRAM_START) ||
        (end_address >  PLATFORM_DDR3_SDRAM_END) ||
        (start_address >= end_address)){
        IFPRINT(platform_write("platform_external_memory_test: Start address (0x%08x) or end address (0x%08x)\n",
            start_address, end_address));
        platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
        return ((Platform_STATUS) Platform_EINVALID);
    }

    return platform_memory_test(start_address, end_address);
}
#endif

/******************************************************************************
 * platform_internal_memory_test
 ******************************************************************************/
#if (PLATFORM_INTMEMTEST_IN)
Platform_STATUS platform_internal_memory_test(uint32_t id)
{
    uint32_t start_address, end_address;
    CSL_PSC_MODSTATE state_prev, state_val;
    CSL_PSC_MDLRST   loc_rst_prev, loc_rst_val;
    uint32_t rstiso_prev_flag;    
    uint32_t module_number = 15;
    int32_t status = Platform_EOK;

	IFPRINT(platform_write("platform_internal_memory_test(core=%d) called \n", id));

    if (platform_get_coreid() == id) {
        IFPRINT(platform_write("platform_internal_memory_test: Can't run the test own core(%d)\n",
            id));
        platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
        return ((Platform_STATUS) Platform_EUNSUPPORTED);
    }

    if (id >= PLATFORM_CORE_COUNT) {
        IFPRINT(platform_write("platform_internal_memory_test: core id should be in range [0..%d]\n",
            PLATFORM_CORE_COUNT - 1));
        platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
        return ((Platform_STATUS) Platform_EINVALID);
    }

    /* For the 6678L */
    switch (id) {
        case 0:
            module_number = 15; //(C66x Core 0 and Timer 0)
            break;
        case 1:
            module_number = 16; //(C66x Core 1 and Timer 1)
            break;
        case 2:
            module_number = 17; //(C66x Core 2 and Timer 2)
            break;
        case 3:
            module_number = 18; //(C66x Core 3 and Timer 3)
            break;
        case 4:
            module_number = 19; //(C66x Core 4 and Timer 4)
            break;
        case 5:
            module_number = 20; //(C66x Core 5 and Timer 5)
            break;
        case 6:
            module_number = 21; //(C66x Core 6 and Timer 6)
            break;
        case 7:
            module_number = 22; //(C66x Core 7 and Timer 7)
            break;
    }
    
    /* Preserve the state, local reset and resetiso values */
    state_prev          = CSL_PSC_getModuleNextState(module_number);
    loc_rst_prev        = CSL_PSC_getModuleLocalReset(module_number);
    rstiso_prev_flag    = CSL_PSC_isModuleResetIsolationEnabled(module_number);

    /* Clear the state, local reset and resetiso values */
    state_val           = PSC_MODSTATE_SWRSTDISABLE;
    loc_rst_val         = PSC_MDLRST_ASSERTED;

    CSL_PSC_setModuleNextState(module_number, state_val);
    CSL_PSC_setModuleLocalReset(module_number, loc_rst_val);
    CSL_PSC_disableModuleResetIsolation(module_number);

    /* Test L1P memory region*/
    start_address = PLATFORM_L1P_BASE_ADDRESS | (id << 24);
    end_address   = start_address + PLATFORM_L1P_SIZE;
    if (platform_memory_test(start_address, end_address) != Platform_EOK) {
        status = Platform_EFAIL;
    }

    /* Test L1D memory region*/
    start_address = PLATFORM_L1D_BASE_ADDRESS | (id << 24);
    end_address   = start_address + PLATFORM_L1D_SIZE;
    if (platform_memory_test(start_address, end_address) != Platform_EOK) {
        status = Platform_EFAIL;
    }

    /* Restore the previous state, local reset and resetiso values */
    CSL_PSC_setModuleNextState(module_number, state_prev);
    CSL_PSC_setModuleLocalReset(module_number, loc_rst_prev);
    if (rstiso_prev_flag) {
         CSL_PSC_enableModuleResetIsolation(module_number);
    } else {
         CSL_PSC_disableModuleResetIsolation(module_number);
    }
    
    return status;
}
#endif

/******************************************************************************
 * platform_write  - Printf or echo to UART or both
 ******************************************************************************/
#if (PLATFORM_WRITE_IN)
WRITE_info platform_write_configure (WRITE_info	wtype) {
	WRITE_info original;
	IFPRINT(platform_write("platform_write_configure(write_type=%d) called \n", wtype));
	original = write_type;
	write_type = wtype;
	return original;
}

void platform_write(const char *fmt, ... )
{
    va_list 	arg_ptr;
	uint32_t	i, length;
	
    /* Initial platform_write to temporary buffer.. at least try some sort of sanity check so we don't write all over
     * memory if the print is too large.
     */
    if (strlen(fmt) > MAX_WRITE_LEN) {platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT; return;}
 
    va_start( arg_ptr, fmt );
    length = vsprintf( (char *)write_buffer, fmt, arg_ptr );
    va_end( arg_ptr );

	if ((write_type == PLATFORM_WRITE_PRINTF) || (write_type == PLATFORM_WRITE_ALL)) {
		printf( "%s", write_buffer );
	}

	if ((write_type == PLATFORM_WRITE_UART) || (write_type == PLATFORM_WRITE_ALL)) {
		/* Log to console port (routine only sends a byte at a time) */
		for (i=0; i < length; i++) {
			if (write_buffer[i] == '\n') {
				(void) platform_uart_write((uint8_t)0x0D);
				(void) platform_uart_write((uint8_t)0x0A);
			}
			else {
				(void) platform_uart_write((uint8_t)write_buffer[i]);
			}
		}
	}

	return;
}
#endif


/******************************************************************************
 * platform_flash_open
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN)  || (PLATFORM_EEPROM_IN)
PLATFORM_DEVICE_info *platform_device_open(uint32_t deviceid, uint32_t flags ) {
 
	PLATFORM_DEVICE_info *p_info;

	IFPRINT(platform_write("platform_device_open(deviceid=0x%x,flags=0x%x) called \n", deviceid, flags));

#if (PLATFORM_NAND_IN)

	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		/* Disable Write protect in NAND */
        if (nandInit() != SUCCESS) {
            IFPRINT(platform_write("platform_dveice_open: Initialization failed.\n"));
            return NULL;
        }

        /* Store the open flags */
        gDeviceNand.flags	= flags;

        /* Set the device to point to its bad block list */
        gDeviceNand.bblist = (uint8_t *)&gDeviceNandBBlist;

        p_info = &gDeviceNand;

   		 if (NandGetDetails(p_info) != SUCCESS) {
   		    IFPRINT(platform_write("platform_device_open: Unable to read device information.\n"));
            platform_errno = PLATFORM_ERRNO_BADFLASHDEV;
   		    return NULL;
   		 }

		p_info->handle	= deviceid;

       return p_info;
	}

#endif

#if (PLATFORM_NOR_IN)
	    
	NOR_STATUS      nor_status;
    
	if (deviceid == PLATFORM_DEVID_NORN25Q128) {

		/* Disable Write protect in NOR */
       //fpgaDisableNorWriteProtect();
       nor_status = nor_init();
       if (nor_status != NOR_EOK) {
            IFPRINT(platform_write("platform_device_open: Initialization failed.\n"));
            return NULL;
        } 

       /* Store the open flags */
       gDeviceNor.flags	= flags;

       p_info = &gDeviceNor;

	    if (nor_get_details(p_info) != SUCCESS) {
            IFPRINT(platform_write("platform_device_open: Unable to read device information.\n"));
            platform_errno = PLATFORM_ERRNO_BADFLASHDEV;
            return NULL;
	    }
 
   		p_info->handle	= deviceid;
      
    	return p_info;
	}
#endif

#if (PLATFORM_EEPROM_IN)

	if (deviceid == PLATFORM_DEVID_EEPROM50) {
	    /* Store the open flags */
	    gDeviceEeprom0.flags	= flags;
        evmI2CInit();
		p_info = &gDeviceEeprom0;
   		p_info->handle	= deviceid;
    	return p_info;
	}

	if (deviceid == PLATFORM_DEVID_EEPROM51) {
	    /* Store the open flags */
	    gDeviceEeprom1.flags	= flags;
        evmI2CInit();
		p_info = &gDeviceEeprom1;
   		p_info->handle	= deviceid;
    	return p_info;
	}

#endif

	platform_errno = PLATFORM_ERRNO_UNSUPPORTED;

	return NULL;
}
#endif

/******************************************************************************
 * platform_flash_close
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN)  || (PLATFORM_EEPROM_IN)
Platform_STATUS platform_device_close(PLATFORM_DEVHANDLE deviceid) {
 
	IFPRINT(platform_write("platform_device_close(handle=0x%x) called \n", deviceid));

#if (PLATFORM_NAND_IN)
	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		gDeviceNand.handle = 0;
		return Platform_EOK;
	}
#endif

#if (PLATFORM_NOR_IN)
	if (deviceid == PLATFORM_DEVID_NORN25Q128) {
		gDeviceNor.handle = 0;
     	return Platform_EOK;   
	}
#endif

#if (PLATFORM_EEPROM_IN)

	if (deviceid == PLATFORM_DEVID_EEPROM50) {
		gDeviceEeprom0.handle = 0;
		return Platform_EOK;
	}

	if (deviceid == PLATFORM_DEVID_EEPROM51) {
		gDeviceEeprom1.handle = 0;
		return Platform_EOK;
	}

#endif

	return Platform_EUNSUPPORTED;
}
#endif

/******************************************************************************
 * platform_flash_read_spare_data
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN) || (PLATFORM_EEPROM_IN)
Platform_STATUS platform_device_read_spare_data(PLATFORM_DEVHANDLE deviceid,
												uint32_t block_number,
												uint32_t page_number,
												uint8_t *buf) {

	IFPRINT(platform_write("platform_device_read_spare_data(handle=0x%x, block=%d, page=%d,buf=0x%x) called \n",
			deviceid, block_number, page_number, buf));

	if (buf == NULL) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		return Platform_EINVALID;
	}

#if (PLATFORM_NAND_IN)
	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		if ((block_number >= gDeviceNand.block_count) || (page_number >= gDeviceNand.page_count))
        {
			platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
			return Platform_EINVALID;
		}

	    if (NandReadSpareArea (&gDeviceNand, block_number, page_number, buf) != SUCCESS) {
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
       return Platform_EOK;
	}
#endif

	return Platform_EUNSUPPORTED;
													
}
#endif

/******************************************************************************
 * platform_device_mark_block_bad
 ******************************************************************************/
#if PLATFORM_NAND_WRITE_IN
Platform_STATUS platform_device_mark_block_bad(PLATFORM_DEVHANDLE deviceid,
												uint32_t block_number) {

    uint8_t *buf = NULL;
    PLATFORM_DEVICE_info *p_device;

    if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		p_device = &gDeviceNand;
    }
	else {
		p_device = NULL;
	}

	if (p_device == NULL) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		return Platform_EINVALID;
	}
    
    buf =  (uint8_t *) Osal_platformMalloc(platform_roundup(p_device->spare_size, PLATFORM_CACHE_LINE_SIZE),
    										PLATFORM_CACHE_LINE_SIZE);

    platform_device_read_spare_data(deviceid, block_number, 0, buf);

    buf[p_device->bboffset] = 0xA5; /* BI indicator byte*/
    
	if ( block_number >= gDeviceNand.block_count ) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		Osal_platformFree(buf, platform_roundup(p_device->spare_size, PLATFORM_CACHE_LINE_SIZE));
		return Platform_EINVALID;
	}

	if (NandWriteSpareArea (p_device, block_number, 0, buf) != SUCCESS) {
		Osal_platformFree(buf, platform_roundup(p_device->spare_size, PLATFORM_CACHE_LINE_SIZE));
	    return ( (Platform_STATUS) Platform_EFAIL);
	 }

	Osal_platformFree(buf, platform_roundup(p_device->spare_size, PLATFORM_CACHE_LINE_SIZE));

    return Platform_EOK;

}
#endif

/******************************************************************************
 * platform_flash_write_spare_data
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN) || (PLATFORM_EEPROM_IN)
Platform_STATUS platform_device_write_spare_data(PLATFORM_DEVHANDLE deviceid,
												uint32_t block_number,
												uint32_t page_number,
												uint8_t *buf) {

	IFPRINT(platform_write("platform_device_write_spare_data(handle=0x%x, block=%d, page=%d,buf=0x%x) called \n",
			deviceid, block_number, page_number, buf));

	if (buf == NULL) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		return Platform_EINVALID;
	}

#if (PLATFORM_NAND_WRITE_IN)
	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		if ((block_number >= gDeviceNand.block_count) || (page_number >= gDeviceNand.page_count))
        {
			platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
			return Platform_EINVALID;
		}

	    if (NandWriteSpareArea (&gDeviceNand, block_number, page_number, buf) != SUCCESS) {
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
       return Platform_EOK;
	}
#endif

	return Platform_EUNSUPPORTED;
													
}
#endif

/******************************************************************************
 * platform_flash_read
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN) || (PLATFORM_EEPROM_IN)
Platform_STATUS platform_device_read(PLATFORM_DEVHANDLE 	deviceid,
									 uint32_t 	offset,
                                     uint8_t 	*buf,
                                     uint32_t	len) {

	IFPRINT(platform_write("platform_device_read(handle=0x%x, offset=%d, len=%d, buf=0x%x) called \n",
			deviceid, offset, len, buf));

	if (buf == NULL || len == 0) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		return Platform_EINVALID;
	}

#if (PLATFORM_NAND_IN)
	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
    	NAND_ADDR addr;
        uint32_t  read_len = 0;

	    addr.uiColumnAddr= 0;

        for (read_len = 0; read_len < len; read_len += gDeviceNand.page_size)
        {
            if (platform_offset_to_blocknpage(deviceid, offset+read_len, &addr.uiBlockAddr, &addr.uiPageAddr) != Platform_EOK) {
                return Platform_EUNSUPPORTED;
            }

            if (readNandPage (&gDeviceNand, addr, &buf[read_len]) != SUCCESS) {
                return ( (Platform_STATUS) Platform_EFAIL);
            }
        }
		    
       	return Platform_EOK;
	}
#endif

#if (PLATFORM_NOR_IN)
	if (deviceid == PLATFORM_DEVID_NORN25Q128) {
	    if (nor_read (&gDeviceNor, offset, len, buf) != SUCCESS) {
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
     	return Platform_EOK;   
	}
#endif

#if (PLATFORM_EEPROM_IN)

	if (deviceid == PLATFORM_DEVID_EEPROM50) {
	    if (i2cEepromRead (offset, len, buf, 0x50) != I2C_RET_OK) {
	        IFPRINT(platform_write("platform_eeprom_read: EEPROM read failed\n"));
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
    	return Platform_EOK;
	}

	if (deviceid == PLATFORM_DEVID_EEPROM51) {
	    if (i2cEepromRead (offset, len, buf, 0x51) != I2C_RET_OK) {
	        IFPRINT(platform_write("platform_eeprom_read: EEPROM read failed\n"));
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
    	return Platform_EOK;
	}

#endif

	return Platform_EUNSUPPORTED;

}
#endif

/******************************************************************************
 * Computes a block and page based on an offset
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN)

Platform_STATUS platform_offset_to_blocknpage(PLATFORM_DEVHANDLE 	deviceid,
									 uint32_t 	offset,
                                     uint32_t 	*block,
                                     uint32_t	*page) {
	uint32_t	leftover;
	uint32_t	block_size;
	uint32_t	block_count, page_size, page_count;

	IFPRINT(platform_write("platform_offset_to_blocknpage(handle=0x%x, offset=%d) called \n",
			deviceid, offset));

	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		block_count = gDeviceNand.block_count;
		page_size	= gDeviceNand.page_size;
		page_count 	= gDeviceNand.page_count;
	}
	else if (deviceid == PLATFORM_DEVID_NORN25Q128) {
		block_count = gDeviceNor.block_count;
		page_size	= gDeviceNor.page_size;
		page_count 	= gDeviceNor.page_count;
	}
	else {
		return Platform_EUNSUPPORTED;
	}

	block_size = (page_count * page_size);

	*block 	  = offset / block_size;
	leftover  = offset % block_size;
	*page 	  = leftover / page_size;
	if (leftover % page_size) {
		/* All writes must be page aligned for now */
		return Platform_EUNSUPPORTED;
	}
	if (*block > block_count) {
		return Platform_EINVALID;
	}
	if (*page > page_count) {
		return Platform_EINVALID;
	}

	IFPRINT(platform_write("platform_offset_to_blocknpage: offset = %d block = %d page = %d \n", offset, *block, *page));

	return Platform_EOK;

}

#endif

/******************************************************************************
 * Computes a block and page based on an offset
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN)

Platform_STATUS platform_blocknpage_to_offset(PLATFORM_DEVHANDLE 	deviceid,
									 uint32_t 	*offset,
                                     uint32_t 	block,
                                     uint32_t	page) {

	uint32_t	block_count, page_size, page_count;

	IFPRINT(platform_write("platform_blocknpage_to_offset(handle=0x%x, block=%d, page=%d) called \n",
			deviceid, block, page));

	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		block_count = gDeviceNand.block_count;
		page_size	= gDeviceNand.page_size;
		page_count 	= gDeviceNand.page_count;
	}
	else if (deviceid == PLATFORM_DEVID_NORN25Q128) {
		block_count = gDeviceNor.block_count;
		page_size	= gDeviceNor.page_size;
		page_count 	= gDeviceNor.page_count;
	}
	else {
		return Platform_EUNSUPPORTED;
	}

	if (block >block_count || page > page_count) {
			return Platform_EINVALID;
	}

	*offset = 	(block * (page_count * page_size)) + (page * page_size);

	IFPRINT(platform_write("platform_blocknpage_to_offset: offset = %d block = %d page = %d \n", *offset, block, page));

	return Platform_EOK;
}

#endif

/******************************************************************************
 * platform_flash_write
 ******************************************************************************/

#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN) || (PLATFORM_EEPROM_IN)

Platform_STATUS platform_device_write(PLATFORM_DEVHANDLE 	deviceid,
									 uint32_t 	offset,
                                     uint8_t 	*buf,
                                     uint32_t	len) {

	IFPRINT(platform_write("platform_device_write(handle=0x%x, offset=%d, len=%d, buf=0x%x) called \n",
			deviceid, offset, len, buf));

	/* Check general args */
	if (buf == NULL || len == 0) {
		platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
		return Platform_EINVALID;
	}

#if (PLATFORM_NAND_WRITE_IN || PLATFORM_NOR_WRITE_IN)
	if ((deviceid == PLATFORM_DEVID_NAND512R3A2D) || (deviceid == PLATFORM_DEVID_NORN25Q128))
	{
	    NAND_ADDR 			 addr;
	    NAND_ADDR 			 StartAddress;
	    Platform_STATUS		 retval = Platform_EOK;
	    PLATFORM_DEVICE_info *p_device;
		int32_t				block, page;
		int32_t				bufindex;
		int32_t				index;
		int32_t				useable_page_size;
		int32_t				block_size;
		uint32_t			noraddress;
		uint8_t				*pBlock	= NULL;
		uint8_t				*pPage	= NULL;

		if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
			p_device = &gDeviceNand;
		}
		else {
			p_device = &gDeviceNor;
		}

		/* Calculate the usable page area.. Its the size of the page  */
		useable_page_size = p_device->page_size;

		/* allocate a buffer big enough to hold one block (must include spare area) */
		block_size = p_device->page_size * p_device->page_count;

		pBlock = Osal_platformMalloc(platform_roundup(block_size, PLATFORM_CACHE_LINE_SIZE),
									 PLATFORM_CACHE_LINE_SIZE);

		if ( !pBlock ){
			platform_errno = PLATFORM_ERRNO_OOM;
			retval = Platform_EFAIL;
			goto FLASHWRITE_ERROR;
		}

		/* allocate a buffer big enough to hold one page of the flash */
		pPage =  Osal_platformMalloc(platform_roundup(p_device->page_size, PLATFORM_CACHE_LINE_SIZE),
				 	 	 	 	 	 PLATFORM_CACHE_LINE_SIZE);

		if ( !pPage ){
			platform_errno = PLATFORM_ERRNO_OOM;
			retval = Platform_EFAIL;
			goto FLASHWRITE_ERROR;
		}

		/* Get starting address */
		addr.uiColumnAddr= 0;
		if (platform_offset_to_blocknpage(deviceid, offset, &addr.uiBlockAddr, &addr.uiPageAddr) != Platform_EOK) {
			retval = Platform_EUNSUPPORTED;
			goto FLASHWRITE_ERROR;
		}

		/*
		 * We can now write the file to flash. The basic algorithm is this:
		 *  Read the bad block table
		 *  While we have data to write do
		 *     skip block if bad
		 *     read the block   (page level)
		 *     erase the block  (block level)
		 *     write the block	(page level)
		 */

		/* set index to start of the data to write */
		bufindex = 0;

		/* Initialize the starting block we will be using */
		block 	= addr.uiBlockAddr;
		StartAddress.uiBlockAddr  = addr.uiBlockAddr;
		StartAddress.uiPageAddr	  = addr.uiPageAddr;
		StartAddress.uiColumnAddr = 0;

		while (len) {

			/* Skip bad blocks */
			if (p_device->bblist) {
				if (p_device->bblist[block] == 0x00) {
					/* Block is bad, skip to next good one */
					IFPRINT(platform_write( "platform_device_write: Skipping bad block %d. \n", block));
					block++;
					while (p_device->bblist[block] == 0x00) {
						block++;
					}
				}
			}

			/* Make sure we didn't exceed number of blocks  */
			if (block > p_device->block_count) {
				IFPRINT(platform_write( "Due to bad blocks, you have run out of room in the Flash for this image. \n"));
				platform_errno = PLATFORM_ERRNO_NOFREEBLOCKS;
				retval = Platform_EFAIL;
				goto FLASHWRITE_ERROR;
			}

			IFPRINT(platform_write( "platform_device_write: Writing block %d \n", block));

			/*
			 * Read the block..
			 *    may need to preserve what we don't actually write out.
			 */
			for (page=0, index = 0; page < p_device->page_count;
				 page ++, index +=p_device->page_size) {
				addr.uiBlockAddr = block;
				addr.uiPageAddr	 = page;
				addr.uiColumnAddr= 0;
				if (p_device->handle == PLATFORM_DEVID_NAND512R3A2D) {
#if (PLATFORM_NAND_WRITE_IN)

					if (readNandPage (&gDeviceNand, addr, (uint8_t *)(pBlock + index)) != SUCCESS) {
						retval = Platform_EFAIL;
						IFPRINT(platform_write( "platform_device_write: Error reading block %d page %d. \n", block, page));
						goto FLASHWRITE_ERROR;
					}
#endif
				}
			    else {
#if (PLATFORM_NOR_WRITE_IN)
			    	(void) platform_blocknpage_to_offset(p_device->handle, &noraddress, addr.uiBlockAddr, addr.uiPageAddr);
			    	 if (nor_read (&gDeviceNor, noraddress, p_device->page_size, (uint8_t *)(pBlock + index)) != SUCCESS) {
						 retval = Platform_EFAIL;
						 platform_write( "platform_device_write: Error reading block %d page %d. \n", block, page);
						 goto FLASHWRITE_ERROR;
			    	}
#endif
			    }
			}

			/* erase the block */
			if (p_device->handle == PLATFORM_DEVID_NAND512R3A2D) {
#if (PLATFORM_NAND_WRITE_IN)
				if (nandFlashBlockErase (&gDeviceNand, block) != SUCCESS) {
					IFPRINT(platform_write( "platform_device_write: Could not erase block %d. \n", block));
					retval = Platform_EFAIL;
					goto FLASHWRITE_ERROR;
				}
#endif
			}
			else {
#if (PLATFORM_NOR_WRITE_IN)
			    if (nor_erase (&gDeviceNor, block) != SUCCESS) {
					IFPRINT(platform_write( "platform_device_write: Could not erase block %d. \n", block));
					retval = Platform_EFAIL;
					goto FLASHWRITE_ERROR;
			    }
#endif
			}

			/*
			 * Write the block...
			 * 	 - Get one page of data and write out or
			 *   - if we run out of data re-write what we previously read.
			 */
			for (page = 0, index = 0; page < p_device->page_count;
				 page ++, index +=p_device->page_size) {

				IFPRINT(platform_write( "platform_device_write: Writing to block %d page %d \n", block, page));

				if (block == StartAddress.uiBlockAddr && page < StartAddress.uiPageAddr) {
				   /* if we are within the first block we are writing, then we may have a start
					* page other than page 0, so re-write what was there.
					*/
					memcpy(pPage, (pBlock+bufindex), useable_page_size);
				}
				else
				if (len == 0) {
					/* we wrote all the data, so write back any pages we pre-read that are left */
					memcpy(pPage, (pBlock+bufindex), useable_page_size);
				}
				else
				if (len < useable_page_size) {
					/* we have less than a full page of data */
					memset(pPage, 0xFF, p_device->page_size);
					memcpy(pPage, (buf+bufindex), len);
					len = 0;
				}
				else {
					/* get a full page of data */
					memcpy(pPage, (buf+bufindex), useable_page_size);
					len -= useable_page_size;
					bufindex += useable_page_size;
				}
				/* write the page */
				addr.uiBlockAddr = block;
				addr.uiPageAddr	 = page;
				addr.uiColumnAddr= 0;
				if (p_device->handle == PLATFORM_DEVID_NAND512R3A2D) {
#if (PLATFORM_NAND_WRITE_IN)
					if (writeNandPage (&gDeviceNand, addr, pPage) != SUCCESS) {
						IFPRINT(platform_write( "platform_device_write: Error writing block %d page %d. \n", block, page));
						retval = Platform_EFAIL;
						goto FLASHWRITE_ERROR;
					}
#endif
				}
				else {
#if (PLATFORM_NOR_WRITE_IN)
			    	(void) platform_blocknpage_to_offset(p_device->handle, &noraddress, addr.uiBlockAddr, addr.uiPageAddr);
					if (nor_write(&gDeviceNor, noraddress,  p_device->page_size, pPage) != SUCCESS) {
						IFPRINT(platform_write( "platform_device_write: Error writing block %d page %d. \n", block, page));
						retval = Platform_EFAIL;
						goto FLASHWRITE_ERROR;
					}
#endif
				}
			}

			/* increment to the next block */
			block++;
		}
	
FLASHWRITE_ERROR:

		if (pBlock) {
			Osal_platformFree( pBlock, platform_roundup(block_size, PLATFORM_CACHE_LINE_SIZE) );
		}

		if (pPage) {
			Osal_platformFree( pPage, platform_roundup(p_device->page_size, PLATFORM_CACHE_LINE_SIZE));
		}

		return retval;
	}
#endif

#if (PLATFORM_EEPROM_IN) && (PLATFORM_EEPROM_WRITE_IN)

	if (deviceid == PLATFORM_DEVID_EEPROM50) {
	   if (i2cEepromWriteByteAddr(offset, 0x50, buf, len, I2C_RELEASE_BUS) != I2C_RET_OK) {
	       IFPRINT(platform_write("platform_device_write: EEPROM write for address 0x%x failed\n", 0x50));
	       platform_errno = PLATFORM_ERRNO_EEPROM;
	       return ( (Platform_STATUS) Platform_EFAIL);
	    }
    	return Platform_EOK;
	}

	if (deviceid == PLATFORM_DEVID_EEPROM51) {
	   if (i2cEepromWriteByteAddr(offset, 0x51, buf, len, I2C_RELEASE_BUS) != I2C_RET_OK) {
	       IFPRINT(platform_write("platform_device_write: EEPROM write for address 0x%x failed\n", 0x51));
	       platform_errno = PLATFORM_ERRNO_EEPROM;
	       return ( (Platform_STATUS) Platform_EFAIL);
	    }
    	return Platform_EOK;
	}

#endif

	return Platform_EUNSUPPORTED;

}
#endif
									
/******************************************************************************
 * platform_flash_erase_block
 ******************************************************************************/
#if (PLATFORM_NAND_IN) || (PLATFORM_NOR_IN) || (PLATFORM_EEPROM_IN)
Platform_STATUS platform_device_erase_block(PLATFORM_DEVHANDLE	deviceid,
                                           uint32_t block_number) {

	IFPRINT(platform_write("platform_device_erase_block(handle=0x%x, blockt=%d) called \n",
			deviceid, block_number));

#if (PLATFORM_NAND_IN) && (PLATFORM_NAND_WRITE_IN)
	if (deviceid == PLATFORM_DEVID_NAND512R3A2D) {
		if (block_number >= gDeviceNand.block_count) {
			platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
			return Platform_EINVALID;
		}
	    if (nandFlashBlockErase (&gDeviceNand, block_number) != SUCCESS) {
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }		    
       	return Platform_EOK;
	}
#endif

#if (PLATFORM_NOR_IN) && (PLATFORM_NOR_WRITE_IN)
	if (deviceid == PLATFORM_DEVID_NORN25Q128) {
		if (block_number > gDeviceNor.block_count) {
			platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
			return Platform_EINVALID;
		}
	    if (nor_erase (&gDeviceNor, block_number) != SUCCESS) {
	        return ( (Platform_STATUS) Platform_EFAIL);
	    }
     	return Platform_EOK;   
	}
#endif

	return Platform_EUNSUPPORTED;
}
#endif
																

#if PLATFORM_CACHE_IN
uint32_t Convert_CoreLocal2GlobalAddr (uint32_t  addr)
{
    uint32_t coreNum;

    /* Get the core number. */
#ifdef SIMULATOR_SUPPORT
    coreNum = CSL_chipReadDNUM();
#else
    coreNum = platform_get_coreid();
#endif
    /* Check if the address is a valid Local L2 address to convert */
    if ( (addr >= (uint32_t) 0x800000) &&
         (addr <  (uint32_t) 0x880000) ) {
         /* Compute the global address. */
         return ((1 << 28) | (coreNum << 24) | (addr & 0x00ffffff));
    }
    else {
        return (addr);
    }
}
#endif

