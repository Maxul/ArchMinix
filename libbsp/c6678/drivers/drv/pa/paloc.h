#ifndef _PALOC_H
#define _PALOC_H

/******************************************************************************
 * FILE PURPOSE: Local Defines for the PA LLD 
 ******************************************************************************
 * FILE NAME:   paloc.h
 *
 * DESCRIPTION: Internal module data structures and definitions  
 *
 *
 * REVISION HISTORY:
 *
 *  Copyright (c) Texas Instruments Incorporated 2009-2010
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

#ifdef __cplusplus
extern "C" {
#endif

/* System level header files */
#include "ti/drv/pa/pa.h"

/* PA memory buffer requirements for an instance of PA */
#define PA_BUFFNUM_INSTANCE                 0
#define PA_BUFFNUM_L2_TABLE                 1
#define PA_BUFFNUM_L3_TABLE                 2
#define PA_BUFFNUM_USR_STATS_LNK_TABLE      3
 
/* 
 * Table Entry status
 */
enum  {

  PA_TBL_STAT_INACTIVE,
  PA_TBL_STAT_PENDING_SUBS_REPLY,
  PA_TBL_STAT_ACTIVE
};

/*
 * L2 input MAC configuration 
 *    Broken out from paL2Entry for easy comparison
 */
typedef struct paL2MacCfg_s  {

  paMacAddr_t  dstMac;   /* Destination Mac Address */
  paMacAddr_t  srcMac;   /* Source Mac Address */
  
  uint16_t  vlan;        /* VLAN */
  uint16_t  ethertype;   /* Ethernet Type */
  
  uint32_t  mplsTag;     /* MPLS tag */
  uint16_t  inport;      /* Input EMAC port */
  
}  paL2MacCfg_t;

/*
 * L2 input SRIO configuration 
 *    Broken out from paL2Entry for easy comparison
 */

typedef struct paL2SrioCfg_s  {

  uint16_t    validBitMap; /* valid parameters bit map: refer tp pa.h */ 
  uint16_t    srcId;       /* Source Device ID */
  uint16_t    destId;      /* Destination device ID */
  uint8_t     tt;          /* Transport type: 8-bit or 16-bit device */
  uint8_t     cc;          /* Completion code */
  uint8_t     pri;         /*3-bit priority */
  uint8_t     msgType;     /* Message Type: Type 9 or 11 */
  uint16_t    typeParam1;  /* Type9: streamId; Type11: mbox */
  uint8_t     typeParam2;  /* Type9: cos; Type11: letter */
  
}  paL2SrioCfg_t;

/*
 * L2 input configuration 
 */
typedef union paL2InCfg_s  {
    paL2MacCfg_t    mac;    /* MAC specific parameters */
    paL2SrioCfg_t   srio;   /* SRIO specific parameters */
}  paL2InCfg_t;

/*
 * Common L2/L3 Entry header
 */
typedef struct paL2L3Header_t  {

  int8_t  type;           /* L2 or L3 entry             */
  int8_t  status;         /* PA_TBL_STAT_...            */
  int8_t  tableIdx;       /* Entry number in this table */
  
  int8_t  pdspNum;        /* PDSP identifier            */
  int8_t  lutIdx;         /* PDSP LUT entry             */
  int8_t  subType;        /* MAC, SRIO, IP , Custom     */
  int16_t lnkCnt;         /* Number of channels linking to this entry */
  
} paL2L3Header_t;

/*
 *  An L2 table element
 */
typedef struct paL2Entry_s  {

  paL2L3Header_t hdr;  /* L2 header, Must be first entry */
  paL2InCfg_t  cfg;    /* L2 configuration parameters as defined above */

} paL2Entry_t;


/*
 * L3 custom entry
 */
typedef struct paL3CustomCfg_s
{
    uint8_t customIdx;                        /* Index of Custom LUT1 type */
    uint8_t match[pa_NUM_BYTES_CUSTOM_LUT1];  /* Custom LUT1 byte masks */
} paL3CustomCfg_t;
  
/*
 * An L3 table element
 */
typedef struct paL3Entry_s  {

  paL2L3Header_t  hdr;         /* L3 header, Must be first entry */
  paHandleL2L3_t  pHandle;     /* Previous L2/L3 handle, NULL for no link */
  
  union  {
    paIpInfo_t      ipInfo;      /* IP specific parameters */
    paL3CustomCfg_t customInfo;  /* Custom LUT1 specific parameters */
  } u;
    
} paL3Entry_t;

/* PA Entry Types */
#define  PA_TABLE_ENTRY_TYPE_L2    2
#define  PA_TABLE_ENTRY_TYPE_L3    3 
#define  PA_TABLE_ENTRY_TYPE_L4    4
#define  PA_TABLE_ENTRY_TYPE_NONE  0xFF /* The L4 entry does not connect to a upper layer */

/* PA Enter sub-types */
#define  PA_TABLE_ENTRY_SUBTYPE_MAC     0
#define  PA_TABLE_ENTRY_SUBYTPE_SRIO    1
#define  PA_TABLE_ENTRY_SUBYTPE_IP      2 
#define  PA_TABLE_ENTRY_SUBYTPE_CUSTOM  3
#define  PA_TABLE_ENTRY_SUBYTPE_PORT16  4
#define  PA_TABLE_ENTRY_SUBYTPE_PORT32  5


/*
 * An L4 16-bit port entry internal representation 
 */
typedef struct paL4Port_s  {

  uint8_t  lnkP;           /* Linking PDSP number */
  uint8_t  lnkId;          /* Linking LUT1 entry index */
  uint16_t destPort;       /* 16-bit destination port */
  
} paL4Port_t;


typedef struct  paL4Entry_s {

  uint8_t  lnkType;      /* Identifies the handle type which linking to the L4 entry */
                         /* PA_TABLE_ENTRY_TYPE_NONE if there is no link */
  uint8_t  lnkTableIdx;  /* Entry number in this table of the upper link*/
  uint8_t  customIndex;  /* custom Index, valid if subType =  PA_TABLE_ENTRY_SUBYTPE_CUSTOM */
  uint8_t  subType;      /* PA entry sub-type: PA_TABLE_ENTRY_SUBYTPE_CUSTOM, PA_TABLE_ENTRY_SUBYTPE_PORT16 or
                                               PA_TABLE_ENTRY_SUBYTPE_PORT32 */
  
  union  {
    paL4Port_t portInfo;         /* 16-bit port with link */
    uint32_t   port;             /* 32-bit port */
    uint8_t    customInfo[4];    /* custom port */
  } u;
  
} paL4Entry_t;

#define PA_L4_CUSTOM_NO_LINK  0xff

typedef struct  paUsrStatsLnkEntry_s
{
    uint16_t    lnkIndex;    /* next layer counter index */
} paUsrStatsLnkEntry_t;

#define PA_USR_STATS_LNK_END     0x8000

typedef struct paMemBuf_s {
    int     size;
    void*   base;
} paMemBuf_t;

typedef struct {
  paProtocolLimit_t         protoLimit;           /* the protocol limit configuration structure */
  paIpReassmConfig_t        outIpReassmConfig;    /* the outer IP Reassembly  configuration structure */
  paIpReassmConfig_t        inIpReassmConfig;     /* the inner IP Reassembly  configuration structure */
  paCmdSetConfig_t          cmdSetConfig;         /* the command set configuration structure */
  paUsrStatsConfig_t        usrStatsConfig;       /* the user-defined statistics configuration structure */
  paQueueDivertConfig_t     queueDivertConfig;    /* the queue-diversion configuration structure */
} paSysInfo_t;
  
/*
 * One instance of the driver
 */
typedef struct paInst_s  {

  paMemBuf_t paBufs[pa_N_BUFS];

  int  l2n;             /* Number of entries in L2 table */
  int  l3n;             /* Number of entries in L3 table */
  int  nUsrStats;       /* Number of user-defined statistics in the user-statistics linking table */
  
  uint32_t baseAddr;    /* PA sub-system base address */
  uint16_t ready;       /* TRUE: PA sub-system is active */
  paSysInfo_t cfg;      /* record the global system configurations */
} paInst_t;

/*
 * PDSPs 
 */
enum  {

  PASS_PDSP0 = 0,
  PASS_PDSP1,
  PASS_PDSP2,
  PASS_PDSP3,
  PASS_PDSP4,
  PASS_PDSP5

};

/*
 * PASS command ID formatting
 *  Bit 15 is used to distinguish the L2 table from the L3 table in the command comId field
 */
#define PA_COMID_L2        (0 << 15)
#define PA_COMID_L3        (1 << 15)
#define PA_COMID_L_MASK    (1 << 15)
#define PA_COMID_IDX_MASK  (~(1 << 15))

/*
 *  MPLS labels are restriced to 20 bits
 */
#define PA_MPLS_LABEL_MASK  0x000fffff

/*
 * Flow labels are restricted to 20 bits
 */
#define PA_IP_FLOW_MASK 0x000fffff

/*
 * Maximum loop count for restart ack from PDSP.
 */
#define PA_MAX_PDSP_ENABLE_LOOP_COUNT   50000

/*
 * Define TRUE and FALSE
 */
#undef TRUE
#undef FALSE
#define TRUE     (uint16_t)1
#define FALSE    (uint16_t)0

/* PA Local Object */
typedef struct
{
    /* PA start config */
    paStartCfg_t cfg;
}Pa_LocalObj;

extern Pa_LocalObj    paLObj;

#ifdef __cplusplus
}
#endif

#endif  /* _PALOC_H */
