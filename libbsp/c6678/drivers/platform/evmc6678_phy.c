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
 * File Name:  	evm66x_phy.c
 *
 * Description:	This file contains the phy related APIs
 *
 ******************************************************************************/
/* Chip Level definitions include */
#include "platform_internal.h"


/**
 *  @brief
 *      Configure the sgmii Serdes on devices using the initialization sequence
 */
void configSerdes()
{
    CSL_SGMII_STATUS sgmii_status;
    
    /* Unlock the chip configuration registers to allow SGMII SERDES registers to
    * be written */
    CSL_BootCfgUnlockKicker();
    
    /* Configure the SERDES */
    /* Multiply to be 8 with Quarter Rate in the Rx registers */
    CSL_BootCfgSetSGMIIConfigPLL (0x00000041);
	
	platform_delaycycles(100);

    //31:25    Reserved    0000000
    //23:24    LOOPBACK    00
    //   22    ENOC        1
    //21:18    EQ          0001
    //17:15    CDR         001 -- first order threshold of 17
    //14:12    LOS         000 -- tie off
    //11:10    ALIGN       01  -- Comma Aligned
    //09:07    TERM        100 -- tie off (100)
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off (10)  //00 = Full Rate, 01 = Half Rate (*0.5), 10 = Quarter Rate (*0.25)
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENRX        1
    // 0000 0000 0100 0100 0000 0010 0001 0001 = 0x0044_0211 -- My estimated value
    // 0000 0000 0100 0100 0000 0100 0001 0001 = 0x0044_0411 -- New DV value
    // 0000 0000 0000 1000 0000 1000 0100 0001 = 0x0008_0841 -- Original DV value
    
    CSL_BootCfgSetSGMIIRxConfig (0, 0x00700621);
    CSL_BootCfgSetSGMIIRxConfig (1, 0x00700621);

    //31:22    Reserved    0
    //21:20    LOOPBACK    00
    //19:18    RDTCT       00  -- tie off
    //   17    ENIDL       0   -- tie off
    //   16    MYSNC       1   -- tie off
    //15:12    DEMPHASIS   ???? - 0001 Lets give some de-emphasis
    //11:08    SWING       ????
    //   07    CM          1   -- tie off
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENTX        1
    // 0000 0000 0011 0001 ???? ???? 1001 0001 = 0x0031_1E91 -- My estimated value
    // 0000 0000 0000 0001 0000 1111 0001 0001 = 0x0001_0F11 -- New DV value
    // 0000 0000 0100 0000 0001 1110 0100 0001 = 0x0040_1e41 -- Original DV value
    CSL_BootCfgSetSGMIITxConfig (0, 0x000108A1);    
    CSL_BootCfgSetSGMIITxConfig (1, 0x000108A1);
    
    /* Poll the SGMII0 lock bit to confirm that the sgmii module has recognized
    that the SERDES PLL has locked */   
    do
    {
        CSL_SGMII_getStatus(0, &sgmii_status);
    } while (sgmii_status.bIsLocked != 1);
    
    /* Poll the SGMII1 lock bit to confirm that the sgmii module has recognized
    that the SERDES PLL has locked */   
    do
    {
        CSL_SGMII_getStatus(1, &sgmii_status);
    } while (sgmii_status.bIsLocked != 1);
    
    /* All done with configuration. Return Now. */
    return;
}

/** ============================================================================
 *   @n@b Init_SGMII
 *
 *   @b Description
 *   @n SGMII peripheral initialization code.
 *
 *   @param[in]
 *   @n macPortNum      MAC port number for which the SGMII port setup must
 *                      be performed.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
#ifdef SIMULATOR_SUPPORT
void Init_SGMII (uint32_t macPortNum)
{
    CSL_SGMII_ADVABILITY    sgmiiCfg;
	CSL_SGMII_STATUS        sgmiiStatus;

    /* Reset the port before configuring it */
    CSL_SGMII_doSoftReset (macPortNum);
    while (CSL_SGMII_getSoftResetStatus (macPortNum) != 0);

    /* Hold the port in soft reset and set up
     * the SGMII control register:
     *      (1) Enable Master Mode
     *      (2) Enable Auto-negotiation
     */
    CSL_SGMII_startRxTxSoftReset (macPortNum);
    CSL_SGMII_enableMasterMode (macPortNum);
    CSL_SGMII_enableAutoNegotiation (macPortNum);
    CSL_SGMII_endRxTxSoftReset (macPortNum);

    /* Setup the Advertised Ability register for this port:
     *      (1) Enable Full duplex mode
     *      (2) Enable Auto Negotiation
     *      (3) Enable the Link
     */
    sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
    sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
    CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);

    /* All done with configuration. Return Now. */
    return;
}
#else
void Init_SGMII (uint32_t macPortNum)
{
    CSL_SGMII_ADVABILITY    sgmiiCfg;
    CSL_SGMII_STATUS        sgmiiStatus;

    /* Reset the port before configuring it */
    CSL_SGMII_doSoftReset (macPortNum);
    while (CSL_SGMII_getSoftResetStatus (macPortNum) != 0);

     if (macPortNum == 1) {
        /* Hold the port in soft reset and set up
         * the SGMII control register:
         *      (1) Disable Master Mode
         *      (2) Enable Auto-negotiation
         */
        CSL_SGMII_startRxTxSoftReset (macPortNum);
        CSL_SGMII_disableMasterMode (macPortNum);
        CSL_SGMII_enableAutoNegotiation (macPortNum);
        CSL_SGMII_endRxTxSoftReset (macPortNum);

        /* Setup the Advertised Ability register for this port:
         *      (1) Enable Full duplex mode
         *      (2) Enable Auto Negotiation
         *      (3) Enable the Link
         */
        sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
        sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
        CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);

        do
        {
            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
        } while (sgmiiStatus.bIsLinkUp != 1);

        /* Wait for SGMII Autonegotiation to complete without error */
        do
        {
            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
            if (sgmiiStatus.bIsAutoNegError != 0)
                return; /* This is an error condition */
        } while (sgmiiStatus.bIsAutoNegComplete != 1);
    }

    if (macPortNum == 0) {
        /* Hold the port in soft reset and set up
         * the SGMII control register:
         *      (1) Disable Master Mode
         *      (2) Enable Auto-negotiation
         */
        CSL_SGMII_startRxTxSoftReset (macPortNum);
        CSL_SGMII_disableMasterMode (macPortNum);
        CSL_SGMII_enableAutoNegotiation (macPortNum);
        CSL_SGMII_endRxTxSoftReset (macPortNum);

        /* Setup the Advertised Ability register for this port:
         *      (1) Enable Full duplex mode
         *      (2) Enable Auto Negotiation
         *      (3) Enable the Link
         */
        sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
        sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
        sgmiiCfg.bLinkUp        =   1;
        CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);

        do
        {
            CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
        } while (sgmiiStatus.bIsLinkUp != 1);
     }

    /* All done with configuration. Return Now. */
    return;
}
#endif


