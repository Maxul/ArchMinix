/*
 * nimu_internal.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 */

/**
 *   @file  nimu_internal.h
 *
 *   @brief
 *      Network Interface Module Ethernet driver adaptation layer. These are internal
 *      definitions for the driver.
 *
 */
#ifndef _NIMU_INTERNAL_H_
#define _NIMU_INTERNAL_H_

#include <ti/csl/tistdtypes.h>

#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_tmr.h>
#include <ti/csl/cslr_cgem.h>
#include <ti/csl/cslr_chip.h>
#include <ti/csl/cslr_cpsgmii.h>
#include <ti/csl/cslr_xmc.h>

/* CSL EMAC include */
#include <ti/csl/csl_cpsgmii.h>
#include <ti/csl/csl_cpsgmiiAux.h>
#include <ti/csl/csl_cpsw.h>
#include <ti/csl/csl_mdio.h>
#include <ti/csl/csl_mdioAux.h>

/* BootCfg module include */
#include <ti/csl/csl_bootcfg.h>
#include <ti/csl/csl_bootcfgAux.h>

/* CSL CHIP, SEM Functional layer includes */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_semAux.h>

/* CSL Cache module includes */
#include <ti/csl/csl_cacheAux.h>

/* CPPI LLD include */
#include <ti/drv/cppi/cppi_desc.h>
#include <ti/drv/cppi/cppi_drv.h>

/* NetCP includes */
#include <ti/drv/pa/pa.h>
#include <ti/drv/qmss/qmss_drv.h>

/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"
#include <ti/platform/platform.h>

/* Standard C-native includes  */
#include <stdlib.h>
#include <string.h>

#include "nimu_eth.h"

#define NETDBG 1

/* Number of cores on c6678 */
#define NUM_CORES 8

extern NETIF_DEVICE* GLOBAL_PTR_NET_DEVICE;

extern int32_t Init_Qmss(void);
extern int32_t Init_PASS(void);
extern int32_t Init_Cpsw(uint32_t mtu, uint8_t* srcmacaddress);
extern int32_t Init_Cppi(void);

extern int32_t Verify_Init(void);
extern void Init_MDIO(void);
extern void Init_SGMII(uint32_t macPortNum);
extern void Init_Switch(uint32_t mtu);
extern int32_t Switch_update_addr(uint32_t portNum, uint8_t macAddress[6], uint16_t add);
extern void Init_MAC(uint32_t, uint8_t*, uint32_t);

extern int32_t Setup_Tx(void);
extern int32_t Setup_Rx(void);
extern int32_t Setup_PASS(void);

extern int32_t Add_MACAddress(paEthInfo_t* ethInfo, paRouteInfo_t* routeInfo);
extern uint32_t Convert_CoreLocal2GlobalAddr(uint32_t addr);
extern void CycleDelay(int32_t count);

extern int32_t Cpsw_SwitchOpen(void);
extern int32_t Mdio_Open(void);
extern int32_t Sgmii_Open(void);
extern int32_t Download_PAFirmware(void);

/**
 *  @brief  Nimu_CppiDescCfg
 *
 *          Structure to specify the CPPI descriptor configuration
 *          for a NIMU free queue used to hold pre-allocated
 *          buffers.
 */
typedef struct _Nimu_CppiDescCfg {
    /** CPPI Memory region to be used for this set of free descriptors. */
    uint32_t descMemRegion;

    /** Number of CPPI free descriptors to allocate */
    uint32_t numDesc;

    /** Size of CPPI free descriptors to allocate */
    uint32_t descSize;

    /** CPPI Descriptor Type.
     *
     *  Valid values are:
     *      Cppi_DescType_HOST,
     *      Cppi_DescType_MONOLITHIC
     */
    Cppi_DescType descType;
} Nimu_CppiDescCfg;

#endif /* _NIMU_ETH_H_ */
