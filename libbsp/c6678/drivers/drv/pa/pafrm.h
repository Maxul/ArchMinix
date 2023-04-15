#ifndef _PAFRM_H
#define _PAFRM_H

/******************************************************************************
 * FILE PURPOSE: PASS Firmware Definitions for the PA LLD 
 ******************************************************************************
 * FILE NAME:   pafrm.h
 *
 * DESCRIPTION: Define PASS firmware related constants and data structures  
 *              which are C-type equivalents corresponding to the PDSP-type 
 *              data strutcures defined at fw/pdsp_pa.h
 *
 * REVISION HISTORY:
 *
 *  Copyright (c) Texas Instruments Incorporated 2009-2012
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
#include <ti/drv/pa/pa.h>

/* System Resource Definitions */
#define PAFRM_NUM_PDSPS         6
#define PAFRM_NUM_CRC_ENGINES   PAFRM_NUM_PDSPS
#define PAFRM_NUM_TIMERS        PAFRM_NUM_PDSPS
#define PAFRM_SRAM_SIZE         0x2000
#define PAFRM_NUM_SRAMS         (0x8000/0x2000)


/* System Timestamp */
#define PAFRM_SYS_TIMESTAMP_ADDR           0x6460 
#define PAFRM_SYS_TIMESTAMP_SRAM_INDEX     (PAFRM_SYS_TIMESTAMP_ADDR/PAFRM_SRAM_SIZE) 
#define PAFRM_SYS_TIMESTAMP_OFFSET         (PAFRM_SYS_TIMESTAMP_ADDR%PAFRM_SRAM_SIZE)/sizeof(uint32_t)

/* User Statistics */
#define PAFRM_USR_STATS_ADDR               0x0800 
#define PAFRM_USR_STATS_SRAM_INDEX         (PAFRM_USR_STATS_ADDR/PAFRM_SRAM_SIZE) 
#define PAFRM_USR_STATS_OFFSET             (PAFRM_USR_STATS_ADDR%PAFRM_SRAM_SIZE)/sizeof(uint32_t)

/* PDSP Versions */
#define PAFRM_PDSP_VERSION_BASE            0x7F04
#define PAFRM_PDSP_VERSION_SRAM_INDEX      (PAFRM_PDSP_VERSION_BASE/PAFRM_SRAM_SIZE)
#define PAFRM_PDSP_VERSION_SIZE            0x20
#define PAFRM_PDSP_VERSION_OFFSET(core)    ((PAFRM_PDSP_VERSION_BASE + PAFRM_PDSP_VERSION_SIZE*(core))%PAFRM_SRAM_SIZE)/sizeof(uint32_t)  

/*
 * Routing information used to forward packets to the host (via PKTDMA) 
 */
typedef struct pafrmForwardHost_s  {

  uint32_t  context;            /* Context returned as swInfo0 for matched packet */
  uint8_t   multiRoute;         /* True if multiple destination enabled */
  uint8_t   multiIdx;           /* Index of the multiple destination set */
  uint8_t   paPdspRouter;       /* PA PDSP number used as multi-route router */
  uint8_t   psFlags;            /* use the bit 7:4 */ /* bit 7: Disable CRC, bit 6:4 port number (0/1/2) bit 3:0 errflags = 0*/
                                /* psFlags may be required when the packet is forwarded through QoS queue */
  uint8_t   cmd[4];             /* optional simple command: 0 means no command */
  
} pafrmForwardHost_t;   /* 12 bytes */

/*
 * Routing information used to forward packets to the SA (via PKTDMA) 
 */
typedef struct pafrmForwardSa_s   {

  uint32_t  swInfo0;            /* Packet descriptor swInfo0 required by SA operation */
  uint32_t  swInfo1;            /* Packet descriptor swInfo1 required by SA operation */
  uint8_t   cmd[4];             /* optional simple command: 0 means no command */

} pafrmForwardSa_t;

/*
 * Routing information used to forward packets to the SRIO (via PKTDMA) 
 */
typedef struct pafrmForwardSrio_s   {

  uint32_t  psInfo0;           /* 8-byte protocol-specific information required by SRIO  */
  uint32_t  psInfo1;           /* routing */
  uint8_t   pktType;           /* Packet type specified for SRIO operation */
  uint8_t   rsv4[3];

} pafrmForwardSrio_t;

/*
 * Routing information used to forward packets to the Ethernet port 
 */
typedef struct pafrmForwardEth_s   {
  uint8_t  psFlags;  /* use the bit 7:4 */ /* bit 7: Disable CRC, bit 6:4 port number (0/1/2) bit 3:0 errflags = 0*/
  uint8_t  rsvd1;
  uint16_t rsvd2;
  uint32_t rsvd3;
  uint8_t  cmd[4];  
} pafrmForwardEth_t;

#define PAFRM_ETH_PS_FLAGS_DISABLE_CRC          0x80
#define PAFRM_ETH_PS_FLAGS_PORT_MASK            0x70
#define PAFRM_ETH_PS_FLAGS_PORT_SHIFT              4


/* Routing information used to forward packets within PA */
typedef struct pafrmForwardPa_s  {

  uint8_t  paDest;      /* PDSP destination */
  uint8_t  customType;  /* None, LUT1, LUT2 */
  uint8_t  customIdx;   /* Index of the custom type if LUT1 or LUT2 custom */
  uint8_t  rsvd2;
  
  uint32_t rsvd3;
  uint8_t  cmd[4];
  
} pafrmForwardPa_t;

/* Routing information used to drop the packet */
typedef struct pafrmDiscard_s  {

  uint32_t rsvd2;
  
  uint32_t rsvd3;
  uint8_t  cmd[4];
  
} pafrmDiscard_t;


#define PAFRM_CUSTOM_TYPE_NONE pa_CUSTOM_TYPE_NONE    /* 0 */
#define PAFRM_CUSTOM_TYPE_LUT1 pa_CUSTOM_TYPE_LUT1    /* 1 */
#define PAFRM_CUSTOM_TYPE_LUT2 pa_CUSTOM_TYPE_LUT2    /* 2 */

/*
 * Routing information used to forward packets fromm PA sub-system to various destinations
 */
typedef struct pafrmForward_s  {

  uint8_t forwardType;          /* Forwarding type as defined below */
  uint8_t flowId;               /* PKTDMA flow Id, valid if forwarding via PKTDMA */
  uint16_t queue;               /* Destination queue number, valid if forwarding via PKTDMA */
  
  union  {
    pafrmForwardHost_t host;    /* Host specific routing information */
    pafrmForwardSa_t   sa;      /* SA specific routing information */
    pafrmForwardSrio_t srio;    /* SRIO specific routing information */
    pafrmForwardEth_t  eth;     /* Ethernet specific routing information */
    pafrmForwardPa_t   pa;      /* PA internal routing information */
    pafrmDiscard_t     discard; /* Discard specific routing information */
  } u;
  
} pafrmForward_t;

enum  {
 PAFRM_FORWARD_TYPE_HOST = 0,     /* use PAFRM_DEST_CDMA */       
 PAFRM_FORWARD_TYPE_SA,           /* use PAFRM_DEST_CDMA */ 
 PAFRM_FORWARD_TYPE_PA,           /* use pa.paDest */
 PAFRM_FORWARD_TYPE_ETH,          /* use PAFRM_DEST_ETH */
 PAFRM_FORWARD_TYPE_SRIO,         /* use PAFRM_DEST_CDMA */
 PAFRM_FORWARD_TYPE_DISCARD
};

/*
 * PA sub-system configuration data structure definitions
 */

/* PA System configurations: */
/* Max Count configuration */
typedef struct pafrmComMaxCount_s  {

  uint8_t  vlanMaxCount;    /* Maximum number of VLAN supported, default = 2, maximum = 3 */
  uint8_t  ipMaxCount;      /* Maximum number of IP layers supported, default = 2, maximum = 7 */
  uint8_t  greMaxCount;     /* Maximum number of GRE layers supported, default = 2, maximum = 7 */
  uint8_t  rsvd;            /* reserved for 32-bit alignment */
  
} pafrmComMaxCount_t;

/* IP Reassembly Configuration */
typedef struct pafrmIpReassmConfig_s {

  uint8_t numTrafficFlow; /* Maximum number of IP reassembly traffic flows supported, default = 0, maximum = 32 */
  uint8_t destFlowId;     /* CPPI flow which instructs how the link-buffer queues are used for forwarding packets */
  uint16_t destQueue;     /* Destination host queue where PASS will deliver the packets which require IP reassembly assistance */
} pafrmIpReassmConfig_t;

/* Command Set Configuration */
typedef struct pafrmCmdSetConfig_t {

  uint8_t  numCmdSets; /* Number of command sets supported (32, 64) 
                          If the number of command sets is set to 64, then each command entry will be limited to 64 bytes.
                          If the number of command sets is set to 32, then each command entry will be limited to 128 bytes */
  uint8_t  size;       /* maximum size of each command set  */
  uint16_t rsvd;       /* alignment */                  
} pafrmCmdSetConfig_t;

/* User-defined Statistics system configuration */
typedef struct pafrmUsrStatsConfig_s{

  uint16_t numCounters;     /* Number of user-defined counters, default = 0, maximum = 512 */
  uint16_t num64bCounters;  /* Number of 64-bit counters, default = 0, maximum = 256 */
   
} pafrmUsrStatsConfig_t;

/* Queue Diversion Configuration */
typedef struct pafrmQueueDivertConfig_s{

  uint16_t destQueue;     /* Destination queue where PASS will deliver the LUT2 response packet which contains the 
                             queue diversion information */
  uint8_t  destFlowId;    /* CPPI flow which instructs how the link-buffer queues are used for forwarding 
                             the LUT2 response packets */
  uint8_t  rsvd;          /* alignment */                            
} pafrmQueueDivertConfig_t;

/* Packet Control Configuration */
typedef struct pafrmPacketControlConfig_s{

  uint8_t   ctrlBitMap;   /* Packet control bits */
  uint8_t   rsvd;         /* alignment */ 
  uint16_t  rsvd2;        /* alignment */    
  uint16_t  rxPaddingErrCntIndex;    /* Specify the user statistics index of Rx MAC padding error counter */
  uint16_t  txPaddingCntIndex;       /* Specify the user statistics index of Tx MAC padding counter */               
} pafrmPacketControlConfig_t;

#define PAFRM_PACKET_VERIFY_PPPoE               (1 << 0)
#define PAFRM_PACKET_VERIFY_IP                  (1 << 1)
#define PAFRM_PACKET_CTRL_MAC_PADDING_CHK       (1 << 2)
#define PAFRM_PACKET_CTRL_IP_FRAGS_TO_EROUTE    (1 << 3)


/* Exception routing enumeration */
/* For reference only */
enum  {

  EROUTE_LUT1_FAIL = 0,  /* packet failed to match in LUT1 table */
  EROUTE_VLAN_MAX_DEPTH, /* packet exceeded maximum number of VLAN tags */
  EROUTE_IP_MAX_DEPTH,   /* packet exceeded maximum number of IP headers */
  EROUTE_MPLS_MAX_DEPTH, /* packet exceeded maximum number of MPLS headers */
  EROUTE_GRE_MAX_DEPTH,  /* packet exceeded maximum number of GRE headers */
  EROUTE_PARSE_FAIL,     /* packet failed to parse */
  EROUTE_LUT2_FAIL,      /* packet failed to match in LUT2 table */
  EROUTE_IP_FRAG,        /* IP fragmented packet found in classify2 lookup */
  EROUTE_IPV6_OPT_FAIL,  /* Packet failed due to unsupported IPV6 option header */
  EROUTE_UDP_LITE_FAIL,  /* Udp lite checksum coverage invalid */
  EROUTE_ROUTE_OPTION,   /* IPv4 strict source route or IPv6 routing extension header */
  EROUTE_SYSTEM_FAIL,    /* Unknown system failure - should never happen */
  EROUTE_MAC_BROADCAST,  /* MAC broadcast packet */
  EROUTE_MAC_MULTICAST,  /* MAC multicast packet */
  EROUTE_IP_BROADCAST,   /* IP broadcast packet */
  EROUTE_IP_MULTICAST,   /* IP multicast packet */
  EROUTE_GTPU_MESSAGE_TYPE_1,   /* GTP-U PING Request packet */
  EROUTE_GTPU_MESSAGE_TYPE_2,   /* GTP-U PING Response packet */
  EROUTE_GTPU_MESSAGE_TYPE_26,  /* GTP-U Error Indication packet */
  EROUTE_GTPU_MESSAGE_TYPE_31,  /* GTP-U Supported Header Notification packet */
  EROUTE_GTPU_MESSAGE_TYPE_254, /* GTP-U End Markr packet */
  EROUTE_GTPU_FAIL,             /* packet failed due to GTPU parsing error or unsupporte dmessage types */
  EROUTE_PPPoE_FAIL,            /* Packet failed due to PPPoE session packet parsing error */
  EROUTE_PPPoE_CTRL,            /* PPPoE session stage non-IP packets */
  EROUTE_802_1ag,               /* 802.1ag Packet*/
  EROUTE_IP_FAIL,               /* Packet failed due to invalid IP header */
  EROUTE_NAT_T_KEEPALIVE,       /* NAT-T Keep Alive packet where UDP Length = 9, data = 0xFF */
  EROUTE_NAT_T_CTRL,            /* NAT-T control packet where UDP Length > 12 and the first 4 payload bytes are equal to 0 */
  EROUTE_NAT_T_DATA,            /* NAT-T IPSEC ESP data packet where UDP Length > 12 and the first 4 payload bytes are not equal to 0 */
  EROUTE_NAT_T_FAIL,            /* Invalid NAT-T packet */
  EROUTE_N_MAX                  /* Number of error routes */
};

/* Configure the exception routes */
typedef struct pafrmComEroute_s  {

  uint32_t         routeBitMap;            /* Exception route vaild bitmap */
  pafrmForward_t   eRoute[EROUTE_N_MAX];   /* Array of exception routing information */
  
} pafrmComEroute_t;

/* event definitions */
#define  PAFRM_EVENT_C1_SOP                  (1 << 0)
#define  PAFRM_EVENT_C2_SOP                  (1 << 1)
#define  PAFRM_EVENT_M_SOP                   (1 << 2)
#define  PAFRM_EVENT_L1_MATCH                (1 << 3)
#define  PAFRM_EVENT_L2_MATCH                (1 << 4)
#define  PAFRM_EVENT_L1_NO_MATCH             (1 << 5)
#define  PAFRM_EVENT_L2_NO_MATCH             (1 << 6)
#define  PAFRM_EVENT_C1_PARSE_FAIL           (1 << 7)
#define  PAFRM_EVENT_C2_PARSE_FAIL           (1 << 8)
#define  PAFRM_EVENT_IP_FRAG                 (1 << 9)
#define  PAFRM_EVENT_C1_SILENT_DISCARD       (1 << 10)
#define  PAFRM_EVENT_C2_SILENT_DISCARD       (1 << 11)
#define  PAFRM_EVENT_VLAN_MAX_DEPTH          (1 << 12)
#define  PAFRM_EVENT_IP_MAX_DEPTH            (1 << 13)
#define  PAFRM_EVENT_GRE_MAX_DEPTH           (1 << 14)
#define  PAFRM_EVENT_IP_INCOMPLETE_ROUTE     (1 << 15)
#define  PAFRM_EVENT_LUT1_TIMEOUT            (1 << 16)
#define  PAFRM_EVENT_DISCARD_HELD_PACKET     (1 << 17)
#define  PAFRM_EVENT_NO_FREE_PKT_ID          (1 << 18)
#define  PAFRM_EVENT_INVALID_CTRL            (1 << 19)
#define  PAFRM_EVENT_SYSTEM_ERR              (1 << 20)

/* Event output enable */
typedef struct pafrmComEvents_s  {

  uint32_t validEvents;                /* The events that will trigger an int outside PA */
  
} pafrmComEvents_t;

/* PA Global Configuartions */
/* Custom LUT1 header processing */
typedef struct pafrmC1Custom_s  {

  uint8_t  idx;             /* Custom LUT1 index */
  uint8_t  nextHdr;         /* next header to be parsed */
  uint16_t offset;          
  uint16_t nextHdrOffset;   /* offset to the next header */
  uint16_t rsvd;            /* reserved for 32-bit alignment */
  uint8_t  bitMask[32];     /* 32 8-bit (256-bit) data masks */
  
} pafrmC1Custom_t;

/* Custom LUT2 header processing */
typedef struct pafrmC2Custom_s  {

  uint8_t  idx;         /* Custom LUT2 index */
  uint8_t  bitSet;      /* Use to set the most significant bits to distinguish the custom entry with others */
  uint8_t  ctrlBitMap;  /* Control Bit Map 
                         * b0: Use Link ==> us the upper link for the last match byte
                         */
#define PAFRM_C2_CUSTOM_CTRL_USE_LINK       0x01                         
  uint8_t  rsvd;        /* reserved for alignments */
  uint16_t offset[4];   /* array of offsets to the bytes used in LUT2 custom matching */
  uint8_t  bitMask[4];  /* array of masked bits that are valid in the custom LUT2 matching */
  
} pafrmC2Custom_t;

/* 802.1ag Detection Configuration */
typedef struct pafrm802p1agDet_s  {
  uint8_t  ctrlBitMap;  /* Control Bit Map
                         * b0: enable/disable
                         * b1: standard/draft
                         */
                           
  uint8_t  rsvd;        /* alignment */
  uint16_t rsvd2;       /* alignment */
} pafrm802p1agDet_t;

#define PAFRM_802_1ag_DET_ENABLE       (1 << 0)
#define PAFRM_802_1ag_DET_STANDARD     (1 << 1)


/* IPSEC NAT-T Detection Configuration */
typedef struct pafrmIpsecNatTDet_s  {
  uint8_t  ctrlBitMap;  /* Control Bit Map
                         * b0: enable/disable
                         */
                           
  uint8_t  rsvd;        /* alignment */
  uint16_t udpPort;     /* UDP port number which uniquely identifies the IPSEC NAT-T packets */
} pafrmIpsecNatTDet_t;

#define PAFRM_IPSEC_NAT_T_DET_ENABLE   (1 << 0)
       
/* PA global configuration command */
typedef struct pafrmCommandConfigPa_s
{
  uint8_t  validFlag;   /* valid bitmap as defined below */
  uint8_t  rsvd1;
  uint16_t rsvd2;       /* reserved for alignment */
  
  pafrmComMaxCount_t        maxCounts;      /* Maxmium counts configuration */
  pafrmIpReassmConfig_t     outIpReasm;     /* Outer IP Reassembly Configuration */
  pafrmIpReassmConfig_t     inIpReasm;      /* Inner IP Reassembly Configuration */
  pafrmCmdSetConfig_t       cmdSet;         /* Command Set Configuartion */
  pafrmUsrStatsConfig_t     usrStats;       /* User-defined Statistics Configuration */
  pafrmQueueDivertConfig_t  queueDivert;    /* LUT2 queue diversion configuration */
  pafrmPacketControlConfig_t pktCtrl;       /* Packet control configuration */ 
  pafrmComEvents_t          events;         /* Error events configuration */
  
} pafrmCommandConfigPa_t;

#define PAFRM_COMMAND_CONFIG_VALID_MAX_COUNTS       (1 << 0)
#define PAFRM_COMMAND_CONFIG_VALID_OUT_IP_REASSEM   (1 << 1)
#define PAFRM_COMMAND_CONFIG_VALID_IN_IP_REASSEM    (1 << 2)
#define PAFRM_COMMAND_CONFIG_VALID_CMDSET           (1 << 3)
#define PAFRM_COMMAND_CONFIG_VALID_USR_STATS        (1 << 4)
#define PAFRM_COMMAND_CONFIG_VALID_QUEUE_DIVERT     (1 << 5)
#define PAFRM_COMMAND_CONFIG_VALID_PKT_CTRL         (1 << 6)
#define PAFRM_COMMAND_CONFIG_VALID_EVENTS           (1 << 7)

/* PA system configuration command */
typedef struct pafrmCommandSysConfigPa_s
{
  uint8_t  cfgCode;     /* Specify the system configuration code as defined below */
  uint8_t  rsvd1;
  uint16_t rsvd2;       /* reserved for alignment */
  
  union {
    pafrmComEroute_t          eroute;         /* Exception routes configuration */
    pafrmC1Custom_t           customC1Config; /* Custom LUT1 configuration */
    pafrmC2Custom_t           customC2Config; /* Custom LUT2 configuration */
    pafrm802p1agDet_t         pa802p1agDet;   /* 802.1ag packet detection Configuration */
    pafrmIpsecNatTDet_t       ipsecNatTDet;   /* IPSEC NAT-T Packet detection configuratiuon */
  } u;  
  
} pafrmCommandSysConfigPa_t;

/* PA system configuration codes */
#define PAFRM_SYSTEM_CONFIG_CODE_EROUTE         0
#define PAFRM_SYSTEM_CONFIG_CODE_CUSTOM_LUT1    1
#define PAFRM_SYSTEM_CONFIG_CODE_CUSTOM_LUT2    2
#define PAFRM_SYSTEM_CONFIG_CODE_802_1AG        3
#define PAFRM_SYSTEM_CONFIG_CODE_IPSEC_NAT_T    4

/* Add standard (eth, IP) entry to LUT1 */
typedef struct pafrmComL1Standard_s  {
  
  /* LUT1 view 1 */
  uint8_t   dmac[6];      /* Destination mac */
  uint8_t   smac[6];      /* Source mac */
  uint16_t  etype;        /* Ethernrt type, the field is also used for the previous match PDSP number */
  uint16_t  vlan;         /* VLAN tag, the field is also used for the previous match LUT1 index */
  
  /* LUT1 view 2 */
  uint8_t   srcIp[16];    /* Source IP address */
  uint8_t   dstIp[16];    /* Destination IP address */
  
  /* LUT1 view 3 */
  uint32_t  spi;          /* ESP or AH header Security Parameters Index */
                          /* The field is also used for GRE protocol or SCTP destination port */
  uint32_t  flow;         /* IPv6 flow label in 20 lsbs */
  
  union {
    uint16_t  ports[2];   /* UDP/TCP Source port (0), destination port (1) */
    uint32_t  mpls;       /* mpls label in 20 Lsbs */
   } pm;
  
  uint8_t   protoNext;    /* Ipv4 Protocol fields, IPv6 next */
  uint8_t   tosTclass;    /* Ipv4 TOS, Ipv6 traffic class */
  uint8_t   inport;       /* Input EMAC port (1-based)*/
  uint8_t   key;          /* IP: Distinguishs spi/gre and mpls and ports
                           * LUT1_0: MAC/SRIO, 
                           * LUT1_1/LUT1_2: custom or standard 
                           */
  /* end LUT1 view 3 */
  
  /* Lookup cares/don't cares */
  uint16_t  matchFlags;   /* lookup matching valid flags as defined below */
  uint16_t  rsvd;         /* reserved for alignment */
  
} pafrmComL1Standard_t;


/* Standard match flag bits */
#define PAFRM_LUT1_MATCH_DMAC     (1 << 0)
#define PAFRM_LUT1_MATCH_SMAC     (1 << 1)
#define PAFRM_LUT1_MATCH_ETYPE    (1 << 2)
#define PAFRM_LUT1_MATCH_VLAN     (1 << 3)
#define PAFRM_LUT1_MATCH_LINK     (3 << 2)
#define PAFRM_LUT1_MATCH_SIP      (1 << 4)
#define PAFRM_LUT1_MATCH_DIP      (1 << 5)
#define PAFRM_LUT1_MATCH_SPI_GRE_SCTP  (1 << 6)
#define PAFRM_LUT1_MATCH_FLOW     (1 << 7)
#define PAFRM_LUT1_MATCH_SPORT    (1 << 8)
#define PAFRM_LUT1_MATCH_DPORT    (1 << 9)
#define PAFRM_LUT1_MATCH_PROTO    (1 << 10)
#define PAFRM_LUT1_MATCH_TOS      (1 << 11)
#define PAFRM_LUT1_MATCH_PORT     (1 << 12)
#define PAFRM_LUT1_MATCH_KEY      (1 << 13)
#define PAFRM_LUT1_MATCH_VALID    (1 << 15)

#define PAFRM_LUT1_MATCH_MPLS     (PAFRM_LUT1_MATCH_SPORT | PAFRM_LUT1_MATCH_DPORT)

/* Key values. The PDSP will set these bits as it parses the headers. */
/* LUT1_1 and LUT1_2 (L3): The following bit fields are used */
#define PAFRM_LUT1_KEY_IPV4   (1 << 3)
#define PAFRM_LUT1_KEY_IPV6   (1 << 4)

/* Use inport field to distinguish SPI, GRE and SCTP */
#define PAFRM_LUT1_SPI        (1 << 0)
#define PAFRM_LUT1_GRE        (1 << 1)
#define PAFRM_LUT1_SCTP       (1 << 2)

/* LUT1: Custom  (L3) */
#define PAFRM_LUT1_KEY_CUSTOM (1 << 7)     

/* LUT1_0: MAC and SRIO (L0-l2): The following bit fields are used */
#define PAFRM_LUT1_KEY_SRIO   (1 << 7)
#define PAFRM_LUT1_KEY_MAC    (1 << 0)

/* Add SRIO entry to LUT1 */
typedef struct pafrmComL1Srio_s  {

  /* LUT1 view 1 */
  uint8_t   rsvd1[4];     /* unused field: All zero's */
  uint16_t  srcId;        /* Source ID */
  uint16_t  destId;       /* Destination ID */
  uint8_t   rsvd2[4];     /* unused field: All zero's */
  uint16_t  etype;        /* upper link (previous match PDSP number) */
  uint16_t  vlan;         /* upper link (previous match LUT1 index) */
  
  /* LUT1 view 2 */
  uint8_t   rsvd3[16];    /* unused field: All zero's */
  uint8_t   rsvd4[14];    /* unused field: All zero's */
  uint16_t  typeParam1;   /* stream ID or mailbox */
  
  /* LUT1 view 3 */
  uint32_t  spi;          /* unused field: All zero's */
  uint32_t  flow;         /* unused field: All zero's */
  
  uint16_t  nextHdrOffset;  /* unused field: All zero's */
  uint8_t   nextHdr;        /* place holder for nextHdr and nextOffset         */
  uint8_t   rsvd5;          /* unused field: All zero's */
                           
  uint8_t   pri;          /* 3-bit Priority */
  uint8_t   typeParam2;   /* cos or letter */
  uint8_t   msgType;      /* SRIO message type */
  uint8_t   key;          /* IP: Distinguishs spi/gre and mpls and ports
                           * LUT1_0: MAC/SRIO, 
                           * LUT1_1/LUT1_2: custom or standard 
                           */
  /* end LUT1 view 3 */
  
  /* Lookup cares/don't cares */
  uint16_t  matchFlags;  /* lookup matching valid flags as defined below */
  uint16_t  rsvd;        /* reserved for alignment */
  
} pafrmComL1Srio_t;


/* SRIO match flag bits */
#define PAFRM_LUT1_SRIO_MATCH_SRCID         (1 << 0)
#define PAFRM_LUT1_SRIO_MATCH_DESTID        (1 << 1)
#define PAFRM_LUT1_SRIO_MATCH_LINK          (3 << 2)
#define PAFRM_LUT1_SRIO_MATCH_TYPEPARAM1    (1 << 5)
#define PAFRM_LUT1_SRIO_MATCH_PRI           (1 << 10)
#define PAFRM_LUT1_SRIO_MATCH_TYPEPARAM2    (1 << 11)
#define PAFRM_LUT1_SRIO_MATCH_MSGTYPE       (1 << 12)
#define PAFRM_LUT1_SRIO_MATCH_KEY           (1 << 13)
#define PAFRM_LUT1_SRIO_MATCH_VALID         (1 << 15)


/* Key values. The PDSP will set these bits as it parses the SRIO header */
#define PAFRM_LUT1_SRIO_KEY_TRANSPORT_8  (1 << 2)
#define PAFRM_LUT1_SRIO_KEY_TRANSPORT_16 (1 << 3)
#define PAFRM_LUT1_SRIO_KEY_SRIO         PAFRM_LUT1_KEY_SRIO 

/* Message Type value */
#define PAFRM_LUT1_SRIO_MSG_TYPE_9       (1 << 0)
#define PAFRM_LUT1_SRIO_MSG_TYPE_11      (1 << 1)


/* Add Custom entry to LUT1 */
typedef struct pafrmComL1Custom_s  {

  /* LUT1 view 1 */
  uint8_t   dmac[6];      /* unused field: All zero's */
  uint8_t   smac[6];      /* unused field: All zero's */
  uint16_t  etype;        /* upper link (previous match PDSP number) */
  uint16_t  vlan;         /* upper link (previous match LUT1 index) */
  
  /* LUT1 view 2 */
  uint8_t  matchValues[32];  /* 32 bytes to match   */
  
  /* LUT1 view 3 - offset from start */
  uint32_t  rsvd0;        /* unused field: All zero's */
  uint32_t  rsvd1;        /* unused field: All zero's */
  uint32_t  rsvd2;        /* unused field: All zero's */
  
  uint8_t   rsvd3;        /* unused field: All zero's */
  uint8_t   rsvd4;        /* unused field: All zero's */
  uint8_t   inport;       /* unused field: All zero's */
  uint8_t   key;          /* IP: Distinguishs spi/gre and mpls and ports
                           * LUT1_0: MAC/SRIO, 
                           * LUT1_1/LUT1_2: custom or standard 
                           */
  
  /* Lookup cares/dont cares */
  uint16_t  matchFlags;   /* lookup matching valid flags as defined below */
  uint16_t  rsvd5;        /* reserved for alignment */
  
} pafrmComL1Custom_t;

/* Custom match flag bits */
#define PAFRM_LUT1_CUSTOM_MATCH_ETYPE  (1 << 2)
#define PAFRM_LUT1_CUSTOM_MATCH_VLAN   (1 << 3)
#define PAFRM_LUT1_CUSTOM_MATCH_MATCH  (3 << 4)  /* Ipv6 source and dest entries */
#define PAFRM_LUT1_CUSTOM_MATCH_KEY    (1 << 13)
#define PAFRM_LUT1_CUSTOM_MATCH_VALID  (1 << 15)

/* Key values. The PDSP will set these bits as it parses the SRIO header */
#define PAFRM_LUT1_CUSTOM_KEY_CUSTOM   PAFRM_LUT1_KEY_CUSTOM
#define PAFRM_LUT1_CUSTOM_KEY_INDEX(index) ((index) << 0)  /* Vaild if custom type is set */

/* Add entry to LUT1 */
/* if PA_LUT1_INDEX_LAST_FREE is used then when the command returns, the value of index
 * will be replaced with the actual index used */
#define PAFRM_HW_LUT1_ENTRIES            64
#define PAFRM_LUT1_INDEX_LAST_FREE       PAFRM_HW_LUT1_ENTRIES

typedef struct pafrmCommandAddLut1_s  {

  uint8_t   index;        /* LUT1 index. */
  uint8_t   type;         /* Custom or standard */
  
  uint8_t  rsvd;          /* reserved for alignment */
  
  uint8_t  custIndex;     /* Vaild only if type is custom */
  
  union  {
  
    pafrmComL1Standard_t  ethIp;   /* matching information for MAC/IP entry */
    pafrmComL1Srio_t      srio;    /* matching information for SRIO entry */
    pafrmComL1Custom_t    custom;  /* matching information for custom LUT1 entry */
    
  } u;
  
  
  /* Routing information when a match is found */
  pafrmForward_t match;
  
  /* Routing information when subsequent match fails - a fragmented packet or
   * inner route. */
  pafrmForward_t nextFail;
  
} pafrmCommandAddLut1_t;

/* define LUT1 entry types */
#define PAFRM_COM_ADD_LUT1_STANDARD    0   /* MAC/IP */
#define PAFRM_COM_ADD_LUT1_SRIO        1   /* SRIO */
#define PAFRM_COM_ADD_LUT1_CUSTOM      2   /* Custom LUT1 */

/* Delete entry from LUT1 */
typedef struct pafrmCommandDelLut1_s
{
  uint8_t  index;        /*  LUT1 index */
  
} pafrmCommandDelLut1_t;

/* Add an entry to LUT2 */
/*
 *  Format a 4-byte byte array in big-endian format
 * 
 *  TCP/UDP:  0:port_hi:port_lo:previous matching link info
 *  32-bit port: port_hi:port_mhi:port_mlo:port_lo
 *  custom:   entry identifier or match0: match1: match2:mach3 or previous matching link info  
 *
 */
 
#define PAFRM_DEST_PORT_GTP      2152
 
typedef struct pafrmComL2Standard_s {

  uint8_t data[4];
        
} pafrmComL2Standard_t;

typedef struct pafrmComL2Custom_s
{

  uint8_t match[4];   /* If matching a LUT1 index, match[3] = (pdsp Id << 6) | lutIndex */
  
} pafrmComL2Custom_t;

/* In custom mode the last byte can be an optional link to a previous lookup */
#define PAFRM_FORM_CUSTOM_L4_LINK(pdspId,lutIndex)   \
    (CSL_FMKR(7,6,pdspId) | CSL_FMKR(5,0,lutIndex))
#define PAFRM_GET_PDSPID_FROM_LINK(lnk)   (lnk) >> 6
#define PAFRM_GET_LUTIDX_FROM_LINK(lnk)   (lnk) & 0x3F

/* Queue Divert information */
typedef struct pafrmComL2QueueDivert_s
{
  uint16_t  destQ;   /* Soutce Queue number */
  uint16_t  srcQ;    /* Destination Queue number */
  
} pafrmComL2QueueDivert_t;
    

typedef struct pafrmCommandAddLut2_s  {

  uint8_t  type;        /* Port16, port32 or custom */
  uint8_t  index;       /* LUT2 custom index if custom LUT2 */
  uint8_t  ctrlBitMap;  /* Control Bitmap      
                         * b0: replace entry
                         * b1: GTPU control
                         * b2-b7: reserved
                         */
#define PAFRM_COM_LUT2_CTRL_REPLACE         0x01   /* replace the entry: Do not increment the counter */  
#define PAFRM_COM_LUT2_CTRL_GTPU            0x02   /* Add: Disable GTPU parsing; Delete: Re-enable GTPU parsing */  
#define PAFRM_COM_LUT2_CTRL_QUEUE_DIVERT    0x04   /* Queue diversion: perform queue diversion */
#define PAFRM_COM_LUT2_CTRL_LINK            0x80   /* Upper linker is required (host only) */  
#define PAFRM_COM_LUT2_CTRL_PORT32          0x40   /* Port32 (host only) */                         
                       
                       
  uint8_t  inkTableIdx;       /* link table Index: host only */

  union  {
 
    pafrmComL2Standard_t  port;     /* standard (port16/32) matching information */
    pafrmComL2Custom_t    custom;   /* custom LUT2 matching information */
   
  } u;
  
  /* Routing information when a match is found */
  pafrmForward_t match;
  
  /* Queue Diversion information */
  pafrmComL2QueueDivert_t   qDivert;    /* Queue Divert information */
  
  
} pafrmCommandAddLut2_t;


/* define LUT2 entry types */
#define PAFRM_COM_ADD_LUT2_STANDARD    0
#define PAFRM_COM_ADD_LUT2_CUSTOM      1

/* Delete an entry in LUT2 */
typedef struct pafrmCommandDelLut2_s  {

  uint8_t  type;        /* Port16, port32 or custom */
  uint8_t  index;       /* LUT2 custom index if custom LUT2 */
  uint8_t  ctrlBitMap;  /* Control Bitmap      
                         * b0: reserved
                         * b1: GTPU control
                         * b1-b7: reserved
                         */
  uint8_t  rsvd2;       /* reserved for alignment */
  
  union  {
  
    pafrmComL2Standard_t port;    /* standard (port16/32) matching information */
    pafrmComL2Custom_t   custom;  /* custom LUT2 matching information */
    
  } u;

  
} pafrmCommandDelLut2_t;
  
  
/* define LUT2 entry types */
#define PAFRM_COM_DEL_LUT2_STANDARD   0
#define PAFRM_COM_DEL_LUT2_CUSTOM     1
  
/* Request the PA firmware version information */  
typedef struct pafrmCommandReqVer_s  {

  uint8_t   pdspNum;      /* PDSP Number: 0-5 */
  uint8_t   rsvd1;        /* reserved for alignment */
  uint16_t  rsvd2;        /* reserved for alignment */
  
  uint16_t  version[4];   /* Version ID - major, minor, tiny, nitpick */
  
} pafrmCommandReqVer_t;



#define PAFRM_STATS_TYPE_SYS         0    /* System Statistics  */
#define PAFRM_STATS_TYPE_USR         1    /* User Defined Statistics  */
 
/* Statistics Request */ 
typedef struct pafrmComReqStats_s
{
  uint8_t  clear;      /* If TRUE then stats are cleared after being read */
  uint8_t  type;       /* Statistics Type */
  uint16_t rsvd;
  uint32_t rsvd2;      /* 64-bit alignement for user-defined statistics */
} pafrmCommandReqStats_t;

/* CRC Engine Configuration */
#define PARAM_CRC_TABLE_SIZE    16

typedef struct pafrmCommandConfigCRC_s
{
  uint8_t   ctrlBitMap;     /* Control bit maps as defined below */
#define PARAM_CRC_SIZE_8         0
#define PARAM_CRC_SIZE_16        1
#define PARAM_CRC_SIZE_24        2
#define PARAM_CRC_SIZE_32        3

#define PARAM_CRC_CTRL_CRC_SIZE_MASK    0x3
#define PARAM_CRC_CTRL_LEFT_SHIFT       0x0
#define PARAM_CRC_CTRL_RIGHT_SHIFT      0x4
#define PARAM_CRC_CTRL_INV_RESULT       0x8

  uint8_t   rsvd1;                          /* reserved for alignment */
  uint16_t  rsvd2;                          /* reserved for alignment */
  uint32_t  initVal;                        /* Initial value to use in the CRC calcualtion */
  uint32_t  crcTbl[PARAM_CRC_TABLE_SIZE];   /* CRC tabls */

} pafrmCommandConfigCRC_t;

/* Command set Configuration */
#define PAFRM_MAX_CMD_SET_SIZE    124

typedef struct pafrmCommandCmdSet_s  {

  uint8_t   idx;      /* The index of the command set */
  uint8_t   numCmd;   /*  Number of commands */
  uint16_t  rsvd;     /* reserved for alignment */
  uint8_t   cmd[PAFRM_MAX_CMD_SET_SIZE];  /* commands  */ 
  
} pafrmCommandCmdSet_t;


/* User-defined Statistics counter entry */
typedef  struct pafrmUsrStatsEntry_s {
    uint16_t  index;      /* Index to the counter */ 
    uint16_t  lnkIndex;   /* Index to the next layer counter 
                           * b15: No link
                           * b14: 0: pkt counter
                           *      1: byte counter
                           */       
} pafrmUsrStatsEntry_t;  

#define PAFRM_USR_STATS_LNK_END         PA_USR_STATS_LNK_END /* 0x8000 */
#define PAFRM_USR_STATS_LNK_BYTE_CNT    0x4000       

/* User-defined Statistics counter configuration */
typedef struct pafrmUsrStatsCntCfg_s {
    uint8_t   ctrlFlags;     /* b0: reset */   
    uint8_t   rsvd;   
    uint16_t  nEntries;      /* Number of counter configuration blocks 1-256 */
}  pafrmUsrStatsCntCfg_t;  


#define PAFRM_USR_STATS_CFG_CLR_ALL      pa_USR_STATS_CONFIG_RESET 

/* multi-route entry  */
typedef struct pafrmMultiRouteEntry_s  {

  uint8_t  ctrlFlags;       /* b0:descriptor only;b7:active */
  uint8_t  flowId;          /* PKTDMA flow Id */
  uint16_t queue;           /* destination queue number */
  uint32_t swInfo0;         /* swInfo0 at CPPI descriptor */

} pafrmMultiRouteEntry_t;

/* Multi-route control flags */
#define PAFRM_MULTI_RUOTE_CTRL_DESC_ONLY   pa_NEXT_ROUTE_DESCRIPTOR_ONLY      /* Forward packet descriptor only (1 << 0) */
#define PAFRM_MULTI_RUOTE_CTRL_ACTIVE      (1<<7)


#define PAFRM_MAX_HOST_PKT_DUP             pa_MAX_MULTI_ROUTE_ENTRIES  /* 8 */
#define PAFRM_MULTI_ROUTE_NUM_ROUTES       pa_MAX_MULTI_ROUTE_SETS     /* 32 */
#define PAFRM_MULTI_ROUTE_NEXT_FREE_IDX    0xff

/* Configure PA multi-route set */
typedef struct pafrmCommandMultiRoute_s  {

  uint8_t   idx;      /* The index to read/write */
  uint8_t   mode;     /* Add/Delete or read */
  uint8_t   nRoutes;  /* Number of routes, 1 - PA_MAX_HOST_PKT_DUP    */ 
  uint8_t   rsvd;     /* reserved for alignment */
  
  pafrmMultiRouteEntry_t quFl[PAFRM_MAX_HOST_PKT_DUP];   /* The routes */
  
} pafrmCommandMultiRoute_t;

/* Multi-route operation definitions */
enum  {

  PAFRM_COMMAND_MULTI_ROUTE_MODE_ADD,
  PAFRM_COMMAND_MULTI_ROUTE_MODE_DEL,
  PAFRM_COMMAND_MULTI_ROUTE_MODE_GET
  
};

/* Commands to PA */
typedef struct pafrmCommand_s  {

  uint32_t commandResult; /* Returned to the host, ignored on entry to the PASS                         */

  uint8_t   command;      /* Command value                                                              */
  uint8_t   magic;        /* Magic value                                                                */
  uint16_t  comId;        /* Used by the host to identify command results                               */
  uint32_t  retContext;   /* Returned in swInfo to identify packet as a command                         */
  uint16_t  replyQueue;   /* Specifies the queue number for the message reply. 0xffff to toss the reply */
  uint8_t   replyDest;    /* Reply destination (host0, host1, discard are the only valid values)        */
  uint8_t   flowId;       /* Flow ID used to assign packet at reply                                     */
  
  uint32_t  cmd;          /* First word of the command */
  
/*  
 * refernce of the valid PA command structures
 *
  union  {
  
    pafrmCommandAddLut1_t     addLut1;
    pafrmCommandDelLut1_t     delLut1;
    pafrmCommandAddLut2_t     addLut2;
    pafrmCommandDelLut2_t     delLut2;
    pafrmCommandConfigPa_t    paConfig;
    pafrmCommandReqStats_t    reqStats;
    pafrmCommandReqVer_t      reqVersion;
    pafrmCommandMultiRoute_t  multiRoute; 
    pafrmCommandConfigCRC_t   crcCfg;
    pafrmCommandCmdSet_t      cmdSet;
    pafrmCommandSysConfigPa_t sysConfig;
    
  } u;
  
*/
  
  
} pafrmCommand_t;

/* Command values */
enum  {

  PAFRM_CONFIG_COMMAND_RSVD                 = 0,
  PAFRM_CONFIG_COMMAND_ADDREP_LUT1,
  PAFRM_CONFIG_COMMAND_DEL_LUT1,
  PAFRM_CONFIG_COMMAND_ADDREP_LUT2,
  PAFRM_CONFIG_COMMAND_DEL_LUT2,
  PAFRM_CONFIG_COMMAND_CONFIG_PA,
  PAFRM_CONFIG_COMMAND_REQ_STATS,
  PAFRM_CONFIG_COMMAND_REQ_VERSION,
  PAFRM_CONFIG_COMMAND_MULTI_ROUTE,
  PAFRM_CONFIG_COMMAND_CRC_ENGINE,
  PAFRM_CONFIG_COMMAND_CMD_SET,
  PAFRM_CONFIG_COMMAND_USR_STATS,
  PAFRM_CONFIG_COMMAND_SYS_CONFIG
};

/* Command magic value */
#define PAFRM_CONFIG_COMMAND_SEC_BYTE  0xce

/* Command return values */
enum  {

  PAFRM_COMMAND_RESULT_SUCCESS = 0,              /* Must be 0 */
  PAFRM_COMMAND_RESULT_NO_COMMAND_MAGIC,         /* Command magic value not found */
  
  PAFRM_COMMAND_RESULT_INVALID_CMD,              /* Invalid command identifier */
  
  /* Add entry to LUT1 fails */
  PAFRM_COMMAND_RESULT_LUT1_TYPE_INVALID,        /* Invalid type, custom or standard IP/ethernet */
  PAFRM_COMMAND_RESULT_LUT1_INDEX_INVALID,       /* Invalid LUT1 index (0-63) or no free indices available */
  PAFRM_COMMAND_RESULT_LUT1_MATCH_DEST_INVALID,  /* Sent a match packet to q0 on c1 or c2 - this is illegal. */
  PAFRM_COMMAND_RESULT_LUT1_NMATCH_INVALID,      /* Previous match forward info was somewhere in chunk domain */
  PAFRM_COMMAND_RESULT_LUT1_INVALID_KEYS,        /* Invalid combination found in the key value */
  
  /* Lut 2 entry warnings since the lut can be configured without pdsp */
  PAFRM_COMMAND_RESULT_WARN_OVER_MAX_ENTRIES,
  PAFRM_COMMAND_RESULT_WARN_NEGATIVE_ENTRY_COUNT,
  
  /* Lut 2 entry failures */
  PAFRM_COMMAND_RESULT_LUT2_ADD_BUSY,            /* LUT2 had a lookup and pending config */
  
  /* Not enough room in stats request packet for the reply */
  PAFRM_COMMAND_RESULT_WARN_STATS_REPLY_SIZE,
  
  /* Command sent to PDSP which couldn't handle it */
  PAFRM_COMMAND_RESULT_INVALID_DESTINATION,
  
  /* Add/Delete/Read entries to multi route table */
  PAFRM_COMMAND_RESULT_MULTI_ROUTE_NO_FREE_ENTRIES,    /* Asked to use a free entry, but none found */
  PAFRM_COMMAND_RESULT_MULTI_ROUTE_INVALID_IDX,        /* Illegal index value used */
  PAFRM_COMMAND_RESULT_MULTI_ROUTE_INVALID_MODE,       /* Illegal multi route mode used */
  
  /* Packet size didn't match command */
  PAFRM_COMMAND_RESULT_INVALID_PKT_SIZE,
  
  /* Coustom and Command set index */
  PAFRM_COMMAND_RESULT_INVALID_C1_CUSTOM_IDX,          /* Illegal Custom LUT1 index value used */
  PAFRM_COMMAND_RESULT_INVALID_C2_CUSTOM_IDX,          /* Illegal Custom LUT2 index value used */
  PAFRM_COMMAND_RESULT_INVALID_CMDSET_IDX,             /* Illegal Custom Command Set index value used */
  PAFRM_COMMAND_RESULT_USR_STATS_INVALID_CONFIG        /* Illegal User Stats Configuration */

  
};  

/* Destination (route) values */
#define PAFRM_DEST_PDSP0       0
#define PAFRM_DEST_PDSP1       1
#define PAFRM_DEST_PDSP2       2
#define PAFRM_DEST_PDSP3       3
#define PAFRM_DEST_PDSP4       4
#define PAFRM_DEST_PDSP5       5
#define PAFRM_DEST_PKTDMA      6   
#define PAFRM_DEST_ETH         7

#define PAFRM_DEST_SRIO        0  /* Virtual number used by next route command only */


#define PAFRM_DEST_DISCARD     10

/* Assigning names based on PDSP functions */
#define PAFRM_DEST_PA_C1_0         PAFRM_DEST_PDSP0
#define PAFRM_DEST_PA_C1_1         PAFRM_DEST_PDSP1
#define PAFRM_DEST_PA_C1_2         PAFRM_DEST_PDSP2 
#define PAFRM_DEST_PA_C2           PAFRM_DEST_PDSP3
#define PAFRM_DEST_PA_M_0          PAFRM_DEST_PDSP4
#define PAFRM_DEST_PA_M_1          PAFRM_DEST_PDSP5

/* The default queue for packets that arrive at the PA and don't match in
 * classify1 (right at init time) */
#define PAFRM_DEFAULT_INIT_Q   0x100

/* Ethertypes recognized by the firmware. */
#define PAFRM_ETHERTYPE_IP          0x0800
#define PAFRM_ETHERTYPE_IPV6        0x86dd
#define PAFRM_ETHERTYPE_VLAN        0x8100
#define PAFRM_ETHERTYPE_SPVLAN      0x88a8
#define PAFRM_ETHERTYPE_MPLS        0x8847
#define PAFRM_ETHERTYPE_MPLS_MULTI  0x8848

/* Next header type values  */
#define PAFRM_HDR_MAC               0
#define PAFRM_HDR_VLAN              1
#define PAFRM_HDR_MPLS              2
#define PAFRM_HDR_IPv4              3
#define PAFRM_HDR_IPv6              4
#define PAFRM_HDR_IPv6_EXT_HOP      5
#define PAFRM_HDR_IPv6_EXT_ROUTE    6
#define PAFRM_HDR_IPv6_EXT_FRAG     7
#define PAFRM_HDR_IPv6_EXT_DEST     8
#define PAFRM_HDR_GRE               9
#define PAFRM_HDR_ESP               10
#define PAFRM_HDR_ESP_DECODED       11
#define PAFRM_HDR_AUTH              12
#define PAFRM_HDR_CUSTOM_C1         13
#define PAFRM_HDR_FORCE_LOOKUP      14   /* A contrived header type used with custom SRIO to force
                                           a parse after looking at only the RIO L0-L2 */
#define PAFRM_HDR_SCTP              15
                                      
#define PAFRM_HDR_UNKNOWN           16
#define PAFRM_HDR_UDP               17
#define PAFRM_HDR_UDP_LITE          18
#define PAFRM_HDR_TCP               19
#define PAFRM_HDR_GTPU              20
#define PAFRM_HDR_ESP_DECODED_C2    21
#define PAFRM_HDR_CUSTOM_C2         22

/* Command related definitions */
#define PAFRM_CRC_FLAG_CRC_OFFSET_VALID        0x01
#define PAFRM_CRC_FLAG_CRC_OFFSET_FROM_DESC    0x02
#define PAFRM_CHKSUM_FALG_NEGATIVE             0x01


/* PAFRM receive commands related definitions */

/* 
 * There are the following two groups of PAFRM receive commands:
 * PAFRM short commands which can be used as part of the routing info 
 * PAFRM commands which can be used within a command set
 */
 
#define PAFRM_RX_CMD_NONE           0           /* Dummy command */

/* short commands */
#define PAFRM_RX_CMD_CMDSET             1       /* Execute a command set */
#define PAFRM_RX_CMD_INSERT             2       /* Insert up to two types at the current location */
#define PAFRM_RX_CMD_USR_STATS          3       /* Increment the specific user-statistics chain */
#define PAFRM_RX_CMD_CMDSET_USR_STATS   4       /* Increment User-defined Stats chain and  execute the command set */

/* command set commands */
#define PAFRM_RX_CMD_NEXT_ROUTE         11      /* Specify the next route */
#define PAFRM_RX_CMD_CRC_OP             12      /* CRC generation or verification */
#define PAFRM_RX_CMD_COPY_DATA          13      /* Copy data to the PS Info section */
#define PAFRM_RX_CMD_PATCH_DATA         14      /* Insert or pacth packet data at the specific location */
#define PAFRM_RX_CMD_REMOVE_HDR         15      /* Remove the parsed packet header */
#define PAFRM_RX_CMD_REMOVE_TAIL        16      /* Remove the parsed packet tail */
#define PAFRM_RX_CMD_MULTI_ROUTE        17      /* Duplicate packet to multiple destinations */
#define PAFRM_RX_CMD_VERIFY_PKT_ERROR   18      /* Verify packet error based on error flags */
#define PAFRM_RX_CMD_SPLIT              19       /* Payload splitting */


/* Rx command Header */
typedef struct
{
    uint8_t     cmd;      /* command code as defined above */
    uint8_t     len;      /* command total length */
    uint16_t    rsvd;     /* reserved for alignment */

} pafrmRxCmdHdr_t;


/* Rx command set */
typedef struct
{
    uint8_t     cmd;      /* PAFRM_RX_CMD_CMDSET */
    uint8_t     index;    /* command set index */

} pafrmRxCmdSet_t;

/* Rx insert command */
typedef struct
{
    uint8_t     cmd;        /* PAFRM_RX_CMD_INSERT */
    uint8_t     numBytes;   /* number of bytes to be inserted */
    uint8_t     data0;      /* data to be inserted */
    uint8_t     data1;      /* data to be inserted */
} pafrmRxCmdInsert_t;


/* Rx Usr Stats update command */
typedef struct
{
    uint8_t     cmd;      /* PAFRM_RX_CMD_USR_STATS */
    uint8_t     len;
    uint16_t    index;    /* user-statistics index */

} pafrmRxCmdUsrStats_t;

/*
 * Note: Both "Rx command set" and "Rx Usr Stats update" command  can be issued as part of the routing info.
 *       It is important because LUT1/LUT2 entries with different user-defined statistics can share the same command set
 *       out of the limited number of command sets.
 */
/* Rx command set and Usr Stats update command */
typedef struct
{
    uint8_t     cmd;        /* PAFRM_RX_CMD_CMDSET_USR_STATS */
    uint8_t     setIndex;   /* command set index */
    uint16_t    statsIndex; /* user-statistics index */

} pafrmRxCmdSetUsrStats_t;

/* 
 * Rx Next Route command
 * 
 * The SW Info should be present in the descriptor
 * The destination is always PKTDMA (Host, SA and ETH)
 *
 * Note: The routing to SRIO from command set is not required at this moment.
 *       This structure may need to be enhanced to support SRIO routing
 */       
typedef struct
{
    uint8_t    ctrlFlags;           /* b0: multi-route indication */
    uint8_t    multiRouteIndex;     /* multi-route index if required */
    uint16_t   rsvd;                /* reserved for alignment */
} pafrmRxCmdNextRoute_t;

#define PAFRM_RX_NEXT_ROUTE_CTRL_MULTI_ROUTE        0x01

/* Rx CRC verification command */
typedef struct
{
    uint8_t   ctrlFlags;    /* CRC operation control information as defined below */
    uint8_t   lenAdjust;    /* Payload length adjustment */
    uint16_t  startOffset;  /* Byte location, from the protocol header, where the CRC computation begins */
    uint16_t  len;          /* Number of bytes covered by the CRC computation */
    uint16_t  lenOffset;    /* Payload length field offset in the header */
    uint16_t  lenMask;      /* Payload length field mask */
    uint16_t  crcOffset;    /* Offset from the protocol header to the CRC field */
} pafrmRxCmdCrcOp_t;

#define PAFRM_RX_CRC_OP_CTRL_PAYLOAD_LENGTH_IN_HEADER        0x80
#define PAFRM_RX_CRC_OP_CTRL_CRC_FOLLOW_PAYLOAD              0x40
#define PAFRM_RX_CRC_OP_CTRL_FRAME_TYPE_INCLUDED             0x20
#define PAFRM_RX_CRC_OP_CTRL_LEN_OFFSET_NEGATIVE             0x10
#define PAFRM_RX_CRC_OP_CTRL_FRAME_TYPE_MASK                 0x0F

/*
 * WCDMA Iub HS-DSCH Type 2
 * offset = 6 + 2.5n       (n:even)
 *        = 6 + 2.5n + 0.5 (n:odd)
 */
 
#define PAFRM_RX_CRC_OP_CTRL_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE2         0

/*
 * WCDMA Iub HS-DSCH Type 3
 * offset = 4 + 2.5n       (n:even)
 *        = 4 + 2.5n + 0.5 (n:odd)
 */

#define PAFRM_RX_CRC_OP_CTRL_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE3         1


/* Rx Split command */
typedef struct
{
    uint8_t   ctrlFlags;    /* Split operation control information as defined below */
    uint8_t   startOffset;  /* Byte location, from the protocol header, where the payload or frame begins */
    uint8_t   frameType;    /* WCDMA Frame type */
    uint8_t   rsvd1;        /* alignment */
    uint8_t   rsvd2;        /* place holder for header size */
    uint8_t   flowId;       /* CPPI flow which instructs how link-buffer queues are used for sending payload packets */
    uint16_t  destQueue;    /* Host queue for the payload packet */
    
} pafrmRxCmdSplitOp_t;

#define PAFRM_RX_SPLIT_OP_CTRL_FRAME_TYPE_INCLUDED           0x80

/* Rx Copy Command */
typedef struct {
    uint8_t  ctrlFlags;     /* Copy operation control information as defined below */
    uint8_t  srcOffset;     /* Offset from the start of current protocol header for the data copy to begin */
    uint8_t  destOffset;    /* Offset from the top of the PSInfo for the data to be copied to */
    uint8_t  numBytes;      /* Number of bytes to be copied */
} pafrmRxCmdCopy_t;

#define PAFRM_RX_COPY_CTRL_FROM_END                        0x01

/* Rx Patch Command */
typedef struct {
    uint8_t  ctrlFlags;     /* Patch operation control flags as defined below */
    uint8_t  offset;        /* Offset from the start of the current header for the patch to begin */
    uint8_t  numBypes;      /* number of bytes to be patched */
    uint8_t  rsvd;          /* reserved for alignment */
    uint32_t data;          /* First set of patch data */ 
} pafrmRxCmdPatch_t;

#define PAFRM_RX_PATCH_CTRL_INSERT         (uint8_t)pa_PATCH_OP_INSERT
#define PAFRM_RX_PATCH_CTRL_MAC_HDR        (uint8_t)pa_PATCH_OP_MAC_HDR
#define PAFRM_RX_PATCH_CTRL_DELETE         (uint8_t)pa_PATCH_OP_DELETE


/* Rx MultiRoute Command */
typedef struct
{
    uint8_t     cmd;      /* PAFRM_RX_CMD_MULTI_ROUTE */
    uint8_t     len;      /* command total length */
    uint8_t     index;    /* index of the multi-route set */
    uint8_t     rsvd;     /* reserved for alignment */

} pafrmRxCmdMultiRoute_t;

/* Rx Packet Error verification command  */
typedef struct
{
    uint8_t     errMask;     /* Error Flag mask */
    uint8_t     forwardType; /* Discard or Host */
    uint8_t     flowId;      /* PKTDMA flow Id */
    uint8_t     rsvd1;       /* reserved for alignment */
    uint16_t    queue;       /* destination queue number */ 
    uint16_t    rsvd2;       /* reserved for alignment */
    uint32_t    swInfo0;     /* swInfo0 at CPPI descriptor */
}  pafrmRxCmdVerifyPktErr_t;

#define PAFRM_RX_PKT_ERR_IP_CHECKSUM       0x08
#define PAFRM_RX_PKT_ERR_L4_CHECKSUM       0x04
#define PAFRM_RX_PKT_ERR_CRC               0x02


#endif /* _PAFRM_H */




