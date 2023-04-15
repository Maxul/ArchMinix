#ifndef _PASAHOST_H
#define _PASAHOST_H
/**
 *   @file  pasahost.h
 *
 *   @brief   
 *      This file defines constants, data structures and macros used
 *      among the PA LLD, SA LLD and the host.  
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009-2010 Texas Instruments, Inc.
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

/** @defgroup pasaho_if_module PA/SA/Host Interface
 *  @{
 */
/** @} */

/** @defgroup pasaho_if_macros PA/SA/Host Macros
 *  @ingroup pasaho_if_module
 */

/** @defgroup pasaho_if_structures PA/SA/Host Data Structures
 *  @ingroup pasaho_if_module
 */

/** @defgroup pasaho_if_constants PA/SA/Host Constants (enum's and define's)
 *  @ingroup pasaho_if_module
 */



/**
 *  @defgroup pasahoCommands PA/SA/Host Commands
 *  @ingroup pasaho_if_constants
 *  @{
 *
 *  @name PA/SA/Host Commands
 *
 *  Define PASS Firmware Commands
 *  These values are placed in the 3 msbits of the protocol specific information
 */
/*@{*/
/**
 *  @def  PASAHO_CONFIGURE
 *        PA/SA Configuration command
 */

#define PASAHO_CONFIGURE			4

/* PA commands for receive packet PDSPs */
/**
 *  @def  PASAHO_PARX_PARSECMD
 *        Instruct PDSP to parse the receive packet
 */

#define PASAHO_PARX_PARSECMD		0

/**
 *  @def  PASAHO_PARX_MULTI_ROUTE
 *        Instruct PDSP to perform multiple routing
 */

#define PASAHO_PARX_MULTI_ROUTE 	5

/* PA commands for modify packet PDSPs */
/**
 *  @def  PASAHO_PAMOD_CMPT_CHKSUM
 *        Instruct PDSP to compute checksum
 */

#define PASAHO_PAMOD_CMPT_CHKSUM	0

/**
 *  @def  PASAHO_PAMOD_CMPT_CRC
 *        Instruct PDSP to compute CRC
 */

#define PASAHO_PAMOD_CMPT_CRC		1

/**
 *  @def  PASAHO_PAMOD_PATCH
 *        Instruct PDSP to perform blind patch
 */

#define PASAHO_PAMOD_PATCH			2

/**
 *  @def  PASAHO_PAMOD_NROUTE
 *        Provide PDSP with the next routing information
 */

#define PASAHO_PAMOD_NROUTE			3


/**
 *  @def  PASAHO_PAMOD_MULTI_ROUTE
 *        Instruct PDSP to perform multiple routing
 */

#define PASAHO_PAMOD_MULTI_ROUTE	5

/**
 *  @def  PASAHO_PAMOD_REPORT_TIMESTAMP
 *        Instruct PDSP to report the system timestamp at the timestamp field of the packet descriptor 
 *        when the tx packet is delivered out of the PASS 
 */

#define PASAHO_PAMOD_REPORT_TIMESTAMP  6   


/**
 *  @def  PASAHO_PAMOD_GROUP_7
 *        Define this group command so that several command can share the same command code. They will be distinguished
 *        by its unique 5-bit sub-command code as defined at
 */

#define PASAHO_PAMOD_GROUP_7        7   


/**
 *  @def  PASAHO_PAMOD_DUMMY
 *        No action is required. It is for SA alignment only
 */

#define PASAHO_PAMOD_DUMMY			PASAHO_PAMOD_GROUP_7

/**
 *  @def  PASAHO_PAMOD_IP_FRAGMENT
 *        Instruct PDSP to perform IPv4 fragmentation. The transmit IP packets will be divided into smaller 
 *        IP fragments with the updated IPv4 header and checksum based on the specified MTU size and forwarded 
 *        to the destination specified by the next route command. It is up to the module user to format the
 *        correct IPv4 header. The IP fragmentation command will be ignored if any error is detected.
 */
#define PASAHO_PAMOD_IP_FRAGMENT    PASAHO_PAMOD_GROUP_7  

/**
 *  @def  PASAHO_PAMOD_PATCH_MSG_LEN
 *        Instruct PDSP to perform message length patching after IPv4 fragmentation operation. This command is
 *        valid only if it is in conjunction with the PASAHO_PAMOD_IP_FRAGMENT command.
 */
#define PASAHO_PAMOD_PATCH_MSG_LEN  PASAHO_PAMOD_GROUP_7   

/* SA commands */
/**
 *  @def  PASAHO_SA_LONG_INFO
 *        Provide SA with the packet parsing information in the long form
 */

#define PASAHO_SA_LONG_INFO         0

/**
 *  @def  PASAHO_SA_SHORT_INFO
 *        Provide SA with the packet parsing information in the short form
 */

#define PASAHO_SA_SHORT_INFO        1


/**
 *  @def  PASAHO_SA_AIR_INFO
 *        Provide SA with the packet parsing information for the air ciphering 
 *        operation
 */

#define PASAHO_SA_AIR_INFO          2


/*@}*/
/** @} */

/** @name PASAHO Common Macros
 *  
 */
/*@{*/

/**
 *  @ingroup pasaho_if_macros
 *  @brief  PASAHO_READ_BITFIELD is used to read the specific bit fields
 *
 *  @details  It is one of the main macros for accessing configuration bit fields
 *            Input parameter a contains bit field
 *            b is bit offset withing bit field
 *            c is number of bits used by that parameter
 */

#define PASAHO_READ_BITFIELD(a,b,c)    (((a)>>(b)) & ((1UL<<(c))-1))

/**
 *  @ingroup ingroup pasaho_if_macros
 *  @brief  PASAHO_SET_BITFIELD is used to set the specific bit fields
 *
 *  @details  It is one of the main macros for accessing configuration bit fields
 *            Input parameter a contains bit field
 *            b is bit offset withing bit field
 *            c is number of bits used by that parameter
 *            x is new value of parameter that is packed in this bit field
 *
 *  @note    It enforces strict setting to prevent overflow into other bits, would
 *           cost program space for additional protection. 
 */

#define PASAHO_SET_BITFIELD(a,x,b,c)   (a) &= ~(((1UL<<(c))-1)<<(b)), \
                                       (a) |= (((x) & ((1UL<<(c))-1))<<(b))

/**
 *  @ingroup ingroup pasaho_if_macros
 *  @brief  PASAHO_SET_CMDID is used to set the command ID
 */
#define PASAHO_SET_CMDID(x,v)  PASAHO_SET_BITFIELD((x)->word0, (v), 29,3)

/**
 *  @ingroup ingroup pasaho_if_macros
 *  @brief  PASAHO_PACFG_CMD is used to set the PA configuration command only
 */
#define PASAHO_PACFG_CMD        (((uint32_t)PASAHO_CONFIGURE << 5) << 24)

/*@}*/ /* @name PASAHO Common Macros */


/**
 *  @defgroup pasahoHeaderTypes PASS Header Types
 *  @ingroup pasaho_if_constants
 *  @{
 *
 *  @name PASS Header Types
 *  Definition of protocol header types used at the PASS PDSP Firmwase. In the 
 *  long info field these values specify what the next header type will be
 *  at the next parse offset 
 */ 
/*@{*/

typedef enum {

  PASAHO_HDR_MAC        = 0,        /**< MAC */
  PASAHO_HDR_VLAN,                  /**< VLAN */
  PASAHO_HDR_MPLS,                  /**< MPLS */
  PASAHO_HDR_IPv4,                  /**< IPv4 */
  PASAHO_HDR_IPv6,                  /**< IPv6 */
  PASAHO_HDR_IPv6_EXT_HOP,          /**< IPv6 hop by hop extenstion header */
  PASAHO_HDR_IPv6_EXT_ROUTE,        /**< IPv6 routing extenstion header */
  PASAHO_HDR_IPv6_EXT_FRAG,         /**< IPv6 fragmentation extention header */
  PASAHO_HDR_IPv6_EXT_DEST,         /**< IPv6 destination options header */
  PASAHO_HDR_GRE,                   /**< Generic Routing Encapsulation header */
  PASAHO_HDR_ESP,                   /**< Encapsulating Security Payload header */
  PASAHO_HDR_ESP_DECODED,           /**< Decoded Encapsulating Security Payload header */
  PASAHO_HDR_AUTH,                  /**< Authentication header */
  PASAHO_HDR_CUSTOM_C1,             /**< Custom classify 1 header */
  PASAHO_HDR_FORCE_LOOKUP,          /**< A contrived header type used with custom SRIO to force
                                       * a parse after looking at only the SRIO L0-L2 */
  PASAHO_HDR_UNKNOWN,               /**< Next header type is unknown */
  PASAHO_HDR_UDP,                   /**< User Datagram Protocol header */
  PASAHO_HDR_UDP_LITE,              /**< Lightweight User Datagram Protocol header */
  PASAHO_HDR_TCP,                   /**< Transmission Control Protocol header */
  PASAHO_HDR_CUSTOM_C2              /**< Custom classify 2 header */
 
} pasaho_HeaderType_e;
/*@}*/
/** @} */

/**
 *  @defgroup pasahoSubCmdCode PASS Sub-Command Code
 *  @ingroup pasaho_if_constants
 *  @{
 *
 *  @name PASS Sub-Command Code
 *  Definition of the 5-bit sub-command codes which is used to specify the group 7 commands. 
 */ 
/*@{*/

typedef enum {

  PASAHO_SUB_CMD_DUMMY        = 0,   /**< Dummy */
  PASAHO_SUB_CMD_IP_FRAG,            /**< IPv4 fragmentation */
  PASAHO_SUB_CMD_PATCH_MSG_LEN       /**< Message length Patching */ 
} pasaho_SubCmdCode_e;
/*@}*/
/** @} */

/**
 *  @defgroup pasahoPktType PASS Packet Type
 *  @ingroup pasaho_if_constants
 *  @{
 *
 *  @name PASS Packet Type
 *  Definition of the MAC or IP packet types. 
 */ 
/*@{*/

typedef enum {

  PASAHO_PKT_TYPE_UNICAST        = 0,   /**< Unicast MAC/IP  */
  PASAHO_PKT_TYPE_BROADCAST,            /**< Broadcast MAC/IP */
  PASAHO_PKT_TYPE_MULTICAST             /**< Multicast MAC/IP */ 
} pasaho_pktType_e;
/*@}*/
/** @} */



/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoCmdInfo_t defines the general short command information
 *
 */

typedef struct pasahoCmdInfo_s {
    uint32_t  word0;    /**< Control block word 0 */
} pasahoCmdInfo_t;

/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoLongInfo_t defines the packet parsing information in the long format. 
 *          The information is structured as an array of 32 bit values. These values
 *          are broken down through macros. This allows the representation to be
 *          endian independent to the hardware which operates only on 32 bit values.
 *
 *  @details  
 */

typedef struct pasahoLongInfo_s  {

  uint32_t   word0;  /**< Control block word 0 */
  uint32_t   word1;  /**< Control block word 1 */
  uint32_t   word2;  /**< Control block word 2 */
  uint32_t   word3;  /**< Control block word 3 */
  uint32_t   word4;  /**< Control block word 4 */
  
} pasahoLongInfo_t;

/** 
 *  @defgroup PASAHO_long_info_command_macros  PASAHO Long Info Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Long Info Command Macros
 *  Macros used by the PASAHO Long Info Command
 */
/*@{*/

#define PASAHO_LINFO_READ_CMDID(x)          PASAHO_READ_BITFIELD((x)->word0,29,3)    /**< Extract the command ID defined at @ref pasahoCommands */
#define PASAHO_LINFO_READ_RECLEN(x)         PASAHO_READ_BITFIELD((x)->word0,24,5)    /**< Extract the block length */
#define PASAHO_LINFO_READ_START_OFFSET(x)   PASAHO_READ_BITFIELD((x)->word0,0,16)    /**< Extract the next parse start offset */

#define PASAHO_LINFO_IS_MAC_BROADCAST(x)    PASAHO_READ_BITFIELD((x)->word0,16,1)    /**< Indicate whether it is a broadcast MAC packet */
#define PASAHO_LINFO_IS_MAC_MULTICAST(x)    PASAHO_READ_BITFIELD((x)->word0,17,1)    /**< Indicate whether it is a multicast MAC packet */
#define PASAHO_LINFO_READ_MAC_PKTTYPE(x)    PASAHO_READ_BITFIELD((x)->word0,16,2)    /**< Extract the MAC packet type */

#define PASAHO_LINFO_IS_IP_BROADCAST(x)     PASAHO_READ_BITFIELD((x)->word0,18,1)    /**< Indicate whether it is a broadcast IP packet */
#define PASAHO_LINFO_IS_IP_MULTICAST(x)     PASAHO_READ_BITFIELD((x)->word0,19,1)    /**< Indicate whether it is a multicast IP packet */
#define PASAHO_LINFO_READ_IP_PKTTYPE(x)     PASAHO_READ_BITFIELD((x)->word0,18,2)    /**< Extract the IP packet type */

#define PASAHO_LINFO_READ_END_OFFSET(x)     PASAHO_READ_BITFIELD((x)->word1,16,16)   /**< Extract the end of packet parse offset */
#define PASAHO_LINFO_READ_EIDX(x)           PASAHO_READ_BITFIELD((x)->word1,11,5)    /**< Extract the error index */
#define PASAHO_LINFO_READ_PMATCH(x)         PASAHO_READ_BITFIELD((x)->word1,10,1)    /**< Extract the previous match flag */
#define PASAHO_LINFO_READ_L1_PDSP_ID(x)     PASAHO_READ_BITFIELD((x)->word1,6,3)     /**< Extract the first parse module ID */
#define PASAHO_LINFO_READ_L1_IDX(x)         PASAHO_READ_BITFIELD((x)->word1,0,6)     /**< Extract the first parse module match index */

#define PASAHO_LINFO_READ_L3_OFFSET(x)      PASAHO_READ_BITFIELD((x)->word2,24,8)    /**< Extract the offset to the level 3 header */
#define PASAHO_LINFO_READ_L4_OFFSET(x)      PASAHO_READ_BITFIELD((x)->word2,16,8)    /**< Extract the offset to the level 4 header */
#define PASAHO_LINFO_READ_L5_OFFSET(x)      PASAHO_READ_BITFIELD((x)->word2,8,8)     /**< Extract the offset to the level 5 header */
#define PASAHO_LINFO_READ_ESP_AH_OFFSET(x)  PASAHO_READ_BITFIELD((x)->word2,0,8)     /**< Extract the offset to the security header */

#define PASAHO_LINFO_READ_HDR_BITMASK(x)    PASAHO_READ_BITFIELD((x)->word3,21,11)   /**< Extract the bitmask of parsed header types */
#define PASAHO_LINFO_READ_HDR_BITMASK2(x)   PASAHO_READ_BITFIELD((x)->word3, 4,4)    /**< Extract the bitmask2 of parsed header types */
#define PASAHO_LINFO_READ_NXT_HDR_TYPE(x)   PASAHO_READ_BITFIELD((x)->word3,16,5)    /**< Extract the next header to parse type */
#define PASAHO_LINFO_READ_VLAN_COUNT(x)     PASAHO_READ_BITFIELD((x)->word3,14,2)    /**< Extract the number of VLAN tags found */
#define PASAHO_LINFO_READ_IP_COUNT(x)       PASAHO_READ_BITFIELD((x)->word3,8,3)     /**< Extract the number of IP headers found */
#define PASAHO_LINFO_READ_GRE_COUNT(x)      PASAHO_READ_BITFIELD((x)->word3,11,3)    /**< Extract the number of GRE headers found */
#define PASAHO_LINFO_READ_FLAG_FRAG(x)      PASAHO_READ_BITFIELD((x)->word3,3,1)     /**< Extract the fragmentation found flag */
#define PASAHO_LINFO_READ_INPORT(x)         PASAHO_READ_BITFIELD((x)->word3,0,3)     /**< Extract the (1-based) input EMAC port number  
                                                                                          0: Indicates that the packet does not enter PASS 
                                                                                             through CPSW */
#define PASAHO_LINFO_READ_PSEUDO_CHKSM(x)   PASAHO_READ_BITFIELD((x)->word4,16,16)   /**< Extract the last pseudo-header checksum computed */

/* Extract Protocol Information */

/*@}*/ /* PASAHO_long_info_command_macros */
/** @}*/ /* @name PASAHO Long Info Command Macros */

/**
 *  @defgroup PASAHO_long_info_proto_ind_macros  PASAHO Long Info Protocol Indication Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Long Info Protocol Indication Macros
 */

#define PASAHO_LINFO_IS_MAC(x)              PASAHO_READ_BITFIELD((x)->word3,21,1)    /**< Indicate whether it is a MAC packet */
#define PASAHO_LINFO_IS_802_3(x)            PASAHO_READ_BITFIELD((x)->word3,7,1)     /**< Indicate whether it is a 802.3 packet */
#define PASAHO_LINFO_IS_WITH_VLAN(x)        PASAHO_LINFO_READ_VLAN_COUNT(x)          /**< Indicate whether it is a MAC packet with VLAN */
#define PASAHO_LINFO_IS_WITH_MPLS(x)        PASAHO_READ_BITFIELD((x)->word3,23,1)    /**< Indicate whether it is a MAC packet with MPLS */
#define PASAHO_LINFO_IS_PPPoE(x)            PASAHO_READ_BITFIELD((x)->word3,6,1)     /**< Indicate whether it is a PPPoE packet */
#define PASAHO_LINFO_IS_IP(x)               PASAHO_LINFO_READ_IP_COUNT(x)            /**< Indicate whether it is an IP packet */
#define PASAHO_LINFO_IS_IPSEC_ESP(x)        PASAHO_READ_BITFIELD((x)->word3,25,1)    /**< Indicate whether it is an IPSEC ESP packet */
#define PASAHO_LINFO_IS_IPSEC_AH(x)         PASAHO_READ_BITFIELD((x)->word3,26,1)    /**< Indicate whether it is an IPSEC AH packet */
#define PASAHO_LINFO_IS_UDP(x)              PASAHO_READ_BITFIELD((x)->word3,27,1)    /**< Indicate whether it is an UDP packet */
#define PASAHO_LINFO_IS_UDP_LITE(x)         PASAHO_READ_BITFIELD((x)->word3,28,1)    /**< Indicate whether it is an UDP Lite packet */
#define PASAHO_LINFO_IS_TCP(x)              PASAHO_READ_BITFIELD((x)->word3,29,1)    /**< Indicate whether it is a TCP packet */
#define PASAHO_LINFO_IS_GRE(x)              PASAHO_LINFO_READ_GRE_COUNT(x)           /**< Indicate whether it is a GRE packet */
#define PASAHO_LINFO_IS_GTPU(x)             PASAHO_READ_BITFIELD((x)->word3,30,1)    /**< Indicate whether it is a GTPU packet */
#define PASAHO_LINFO_IS_CUSTOM(x)           PASAHO_READ_BITFIELD((x)->word3,31,1)    /**< Indicate whether it is a Custom packet */
#define PASAHO_LINFO_IS_SCTP(x)             PASAHO_READ_BITFIELD((x)->word3,4,1)     /**< Indicate whether it is a SCTP packet */
#define PASAHO_LINFO_IS_IPSEC_NAT_T(x)      PASAHO_READ_BITFIELD((x)->word3,5,1)     /**< Indicate whether it is an IPSEC NAT-T packet */

/*@}*/ /* PASAHO_long_info_proto_ind_macross */
/** @}*/ /* @name PASAHO Long Info Protocol Indication Macross */

/**
 *  @defgroup PASAHO_long_info_ipReassm_macros  PASAHO Long Info IP Reassembly Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Long Info IpReassm Macros
 *  Macros used by the PASAHO PASS-assisted IP Reassembly Operation
 *
 */
#define PASAHO_LINFO_READ_TFINDEX(x)        PASAHO_READ_BITFIELD((x)->word4,24,8)     /**< Extract the IP Reassembly Traffic Flow Index */
#define PASAHO_LINFO_READ_FRANCNT(x)        PASAHO_READ_BITFIELD((x)->word4,16,8)     /**< Extract the IP Reassembly Fragment count */

#define PASAHO_LINFO_SET_TFINDEX(x, v)      PASAHO_SET_BITFIELD((x)->word4,(v),24,8)  /**< Set the IP Reassembly Traffic Flow Index */
#define PASAHO_LINFO_SET_FRANCNT(x, v)      PASAHO_SET_BITFIELD((x)->word4,(v),16,8)  /**< Set the IP Reassembly Fragment count */

#define PASAHO_LINFO_IS_IPSEC(x)            PASAHO_READ_BITFIELD((x)->word3,25,2)     /**< Indicate whether it is an IPSEC packet */
#define PASAHO_LINFO_CLR_IPSEC(x)           PASAHO_SET_BITFIELD((x)->word3,0,25,2)    /**< Clear IPSEC indication bits */
#define PASAHO_LINFO_CLR_IPSEC_ESP(x)       PASAHO_SET_BITFIELD((x)->word3,0,26,1)    /**< Clear IPSEC ESP indication bit */
#define PASAHO_LINFO_CLR_IPSEC_AH(x)        PASAHO_SET_BITFIELD((x)->word3,0,25,1)    /**< Claer IPSEC AH indication bit */
#define PASAHO_LINFO_CLR_FLAG_FRAG(x)       PASAHO_SET_BITFIELD((x)->word3,0,3,1)     /**< Clear the fragmentation found flag */


#define PASAHO_LINFO_SET_START_OFFSET(x, v) PASAHO_SET_BITFIELD((x)->word0,(v),0,16)  /**< Update the next parse start offset */
#define PASAHO_LINFO_SET_END_OFFSET(x, v)   PASAHO_SET_BITFIELD((x)->word1,(v),16,16) /**< Update the end of packet parse offset */

#define PASAHO_LINFO_SET_NULL_PKT_IND(x, v) PASAHO_SET_BITFIELD((x)->word0,(v),21,1)  /**< Set the null packet flag which indicates that the packet should be dropped. 
                                                                                           This flag should be set for the null packet to be delivered to PASS when
                                                                                           the reassembly timeout occurs    */
/*@}*/ /* PASAHO_long_info_ipReassm_macros */
/** @}*/ /* @name PASAHO Long Info IpReassm Macros */

/**
 *  @def  PA_INV_TF_INDEX
 *        PASS-asssited IP reassembly traffic flow index to indicate that no traffic flow is available 
 */
#define PA_INV_TF_INDEX     0xFF    

/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoShortInfo_t defines the packet parsing information in the short format
 *
 *  @details pasahoShortInfo_t defines the packet parsing information in terms of
 *           payload offset and payload length as described below
 *           SRTP:      offset to the RTP header; RTP payload length including ICV
 *           IPSEC AH:  offset to the Outer IP; IP payload length
 *           IPSEC ESP: offset to the ESP header; ESP papload length including ICV
 */

typedef struct pasahoShortInfo_s {
    uint32_t  word0;   /**< Control block word 0 */
    uint32_t  word1;   /**< Optional supplement data (It may be padding for alignment only) */
} pasahoShortInfo_t;

/** 
 *  @defgroup PASAHO_short_info_command_macros  PASAHO Short Info Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Short Info Command Macros
 *  Macros used by the PASAHO Short Info Command
 *  
 */
/*@{*/

#define PASAHO_SINFO_READ_CMDID(x)          PASAHO_READ_BITFIELD((x)->word0,29,3)                           /**< Extract the command ID defined at @ref pasahoCommands */
#define PASAHO_SINFO_RESD_PAYLOAD_OFFSET(x) PASAHO_READ_BITFIELD((x)->word0,16,8)                           /**< Extract the offset to the packet payload */
#define PASAHO_SINFO_READ_PAYLOAD_LENGTH(x) PASAHO_READ_BITFIELD((x)->word0,0,16)                           /**< Extract the byte length of the payload */

#define PASAHO_SINFO_SET_PAYLOAD_OFFSET(x, v)  PASAHO_SET_BITFIELD((x)->word0, (v), 16, 8)                  /**< Set the offset to the payload */
#define PASAHO_SINFO_SET_PAYLOAD_LENGTH(x, v)  PASAHO_SET_BITFIELD((x)->word0, (v), 0,  16)                 /**< Set the payload length */
#define PASAHO_SINFO_FORMAT_CMD(offset, len)   (((offset) << 16) | (len) | (PASAHO_SA_SHORT_INFO << 29))    /**< Format the entire short info command */

/*@}*/ /* PASAHO_short_info_command_macros */
/** @}*/ /* @name PASAHO Short Info Command Macros */


/* Header bitmask bits */
/**
 *  @defgroup pasahoHeaderBitmap  PA/SA/HO Header Bitmap Bit Definitions
 *  @ingroup pasaho_if_constants
 *  @{
 *
 *  @name PA/SA/HO Header Bitmap Bit Definitions
 *  Bitmap definition of the protocol header bitmask at the long info. 
 */ 
/*@{*/

#define PASAHO_HDR_BITMASK_MAC     (1 << 0)     /**< MAC present */
#define PASAHO_HDR_BITMASK_VLAN    (1 << 1)     /**< VLAN present */
#define PASAHO_HDR_BITMASK_MPLS    (1 << 2)     /**< MPLS present */
#define PASAHO_HDR_BITMASK_IP      (1 << 3)     /**< IP present */
#define PASAHO_HDR_BITMASK_ESP     (1 << 4)     /**< IPSEC/ESP present */
#define PASAHO_HDR_BITMASK_AH      (1 << 5)     /**< IPSEC/AH present */
#define PASAHO_HDR_BITMASK_UDP     (1 << 6)     /**< UDP present */
#define PASAHO_HDR_BITMASK_UDPLITE (1 << 7)     /**< UDPLITE present */
#define PASAHO_HDR_BITMASK_TCP     (1 << 8)     /**< TCP present */
#define PASAHO_HDR_BITMASK_GTPU    (1 << 9)     /**< GTPU present */
#define PASAHO_HDR_BITMASK_CUSTOM  (1 << 10)    /**< Custom header present */

#define PASAHO_HDR_BITMASK2_SCTP          (1 << 0)     /**< SCTP present */
#define PASAHO_HDR_BITMASK2_IPSEC_NAT_T   (1 << 1)     /**< IPSEC NAT-T present */
#define PASAHO_HDR_BITMASK2_PPPoE         (1 << 2)     /**< PPPoE present */
#define PASAHO_HDR_BITMASK2_802_3         (1 << 3)     /**< 802.3 present */

/*@}*/
/** @} */


/* Next Route command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoNextRoute_t defines the next route command. The command structure is defined as 32 bit
 *          values to work with the hardware regardless of the device endianness.
 *
 *  @details 
 */

typedef struct pasahoNextRoute_s  {
    uint32_t  word0;          /**< Contains the next route command information. @ref PASAHO_next_route_command_macros */
    uint32_t  swInfo0;        /**< Information placed into returned descriptor. Used if next destination is the host */
    uint32_t  swInfo1;        /**< Information placed into returned descriptor. Used if next destination is the host */
    uint32_t  word1;          /**< Contains the optional information such as pktType for SRIO. @ref PASAHO_next_route_command_macros */
} pasahoNextRoute_t;

/** 
 *  @defgroup PASAHO_next_route_command_macros  PASAHO Next Route Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Next Route Command Macros
 *  Macros used by the PASAHO Next Route Command
 *  
 */
/*@{*/

#define PASAHO_SET_N(x,v)       PASAHO_SET_BITFIELD((x)->word0, (v), 28, 1)    /**< Sets the N bit which indicates the next command should be executed prior to the route command */
#define PASAHO_SET_E(x,v)       PASAHO_SET_BITFIELD((x)->word0, (v), 27, 1)    /**< Sets the E bit which indicates the extened parameters (packet type and/or control flags) are present for SRIO */
#define PASAHO_SET_DEST(x,v)    PASAHO_SET_BITFIELD((x)->word0, (v), 24, 3)    /**< Sets the destination of the route defined at @ref pktDest */
#define PASAHO_SET_FLOW(x,v)    PASAHO_SET_BITFIELD((x)->word0, (v), 16, 8)    /**< Specifies the flow to use for packets sent to the host */
#define PASAHO_SET_QUEUE(x,v)   PASAHO_SET_BITFIELD((x)->word0, (v), 0,  16)   /**< Specifies the queue to use for packets send to the host */
#define PASAHO_SET_PKTTYPE(x,v) PASAHO_SET_BITFIELD((x)->word1, (v), 24, 8)    /**< Specifies the packet type to use for packets send to the SRIO */
#define PASAHO_SET_TX_PADDING(x,v) PASAHO_SET_BITFIELD((x)->word1, (v), 0, 1)  /**< Sets the tx padding bit which indicates PASS should check padding condition and provide L2 zero padding if required */

/*@}*/ /* PASAHO_next_route_command_macros */
/** @}*/ /* @name PASAHO Next Route Command Macros */


/* Compute checksum command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoComChkCrc_t defines the checksum and CRC generation command. The command structure is defined as
 *          32 bit values to wrok with the hardware regardless of the device endianness.
 *
 *  @details 
 */

typedef struct pasahoComChkCrc_s  {
    uint32_t  word0;        /**<  @ref PASAHO_chksum_command_macros */
    uint32_t  word1;        /**<  @ref PASAHO_chksum_command_macros */
    uint32_t  word2;        /**<  @ref PASAHO_chksum_command_macros */

} pasahoComChkCrc_t;

/** 
 *  @defgroup PASAHO_chksum_command_macros  PASAHO Checksum/CRC Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Checksum/CRC Command Macros
 *  Macros used by the ASAHO Checksum/CRC Command 
 */
/*@{*/


#define PASAHO_CHKCRC_SET_NEG0(x,v)        PASAHO_SET_BITFIELD((x)->word0, (v), 23, 1)           /**< Sets the negative 0 flag - if set a checksum computed as 0 will be sent as 0xffff */
#define PASAHO_CHKCRC_SET_CTRL(x,v)        PASAHO_SET_BITFIELD((x)->word0, (v), 16, 4)           /**< Sets the optional flags of the CRC/Checksum command */
#define PASAHO_CHKCRC_SET_START(x,v)       PASAHO_SET_BITFIELD((x)->word0, (v), 0,  16)          /**< Sets the start offset of the checksum/crc */
#define PASAHO_CHKCRC_SET_LEN(x,v)         PASAHO_SET_BITFIELD((x)->word1, (v), 16, 16)          /**< Sets the length of the checksum/crc */
#define PASAHO_CHKCRC_SET_RESULT_OFF(x,v)  PASAHO_SET_BITFIELD((x)->word1, (v), 0,  16)          /**< Sets the offset to where to paste the checksum/crc into the packet */
#define PASAHO_CHKCRC_SET_INITVAL(x,v)     PASAHO_SET_BITFIELD((x)->word2, (v), 16, 16)          /**< Sets the initial value of the checksum/crc */

/*@}*/ /* @name PASAHO Checksum/CRC Command Macros */
/** @}*/ /* PASAHO_chksum_command_macros */


/* Blind patch command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoComBlindPatch_t defines the blind patch command. The command structure is defined as
 *          32 bit values to work with the hardware regardless of the device endianness.
 *
 *  @details 
 */

#define PASAHO_BPATCH_MAX_PATCH_WORDS   4
typedef struct pasahoComBlindPatch_s  {
    uint32_t   word0;                                   /**<  @ref PASAHO_blind_patch_command_macros */
    uint32_t   patch[PASAHO_BPATCH_MAX_PATCH_WORDS];    /**<  @ref PASAHO_blind_patch_command_macros */
    
} pasahoComBlindPatch_t;

/** 
 *  @defgroup PASAHO_blind_patch_command_macros  PASAHO Blind Patch Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Blind Patch Command Macros
 *  Macros used by the PASAHO Blind Patch Command
 */

#define PASAHO_BPATCH_SET_PATCH_NBYTES(x,v)    \
                          PASAHO_SET_BITFIELD((x)->word0, v, 24,  5)
/**< Sets the number of bytes to patch */
                          
#define PASAHO_BPATCH_SET_PATCH_CMDSIZE(x,v)   \
                          PASAHO_SET_BITFIELD((x)->word0, v, 20, 4)
/**< Sets the size of the command in 32 bit word units */
                          
#define PASAHO_BPATCH_SET_OVERWRITE(x,v)       \
                          PASAHO_SET_BITFIELD((x)->word0, v, 19, 1)
/**< Sets the overwrite flag. If set the patch will overwrite existing packet data, 
     otherwise data may be inserted */  
     
#define PASAHO_BPATCH_SET_DELETE(x,v)         \
                          PASAHO_SET_BITFIELD((x)->word0, v, 18, 1)
/**< Sets the delete flag. If set, no data will be inserted */                          
                             
                          
#define PASAHO_BPATCH_SET_OFFSET(x,v)         \
                          PASAHO_SET_BITFIELD((x)->word0, v, 0,  16)
/**< Sets the offset to the start of the patch */                          
                          
#define PASAHO_BPATCH_SET_PATCH_BYTE(x, byteNum, byte)  \
                                PASAHO_SET_BITFIELD((x)->patch[(byteNum) >> 2], byte, ((3 - (byteNum & 0x3)) << 3), 8)
/**< Sets the data to patch */                                                   
                                
/*@}*/ /* @name PASAHO Blind Patch Command Macros */
/** @}*/ /* PASAHO_blind_patch_command_macros */        

/* Report Timestamp command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoReportTimestamp_t defines the report timestamp command. The command structure is defined as 32 bit
 *          values to work with the hardware regardless of the device endianness.
 *
 *  @details 
 */

typedef struct pasahoReportTimestamp_s  {
    uint32_t  word0;          /**< Contains the report timestamp command information. @ref PASAHO_report_timestamp_command_macros */
    uint32_t  swInfo0;        /**< Information placed into returned descriptor of the reporting packet.  */
} pasahoReportTimestamp_t;

/** 
 *  @defgroup PASAHO_report_timestamp_command_macros PASAHO Report Timestamp Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Report Timestamp Command Macros
 *  Macros used by the PASAHO Report Timestamp Command
 *  
 */
/*@{*/
#define PASAHO_SET_REPORT_FLOW(x,v)    PASAHO_SET_BITFIELD((x)->word0, (v), 16, 8)    /**< Specifies the flow to use for report packets sent to the host */
#define PASAHO_SET_REPORT_QUEUE(x,v)   PASAHO_SET_BITFIELD((x)->word0, (v), 0,  16)   /**< Specifies the queue to use for report packets send to the host */

/*@}*/ /* @name PASAHO Report Timestamp Command Macros */
/** @}*/ /* PASAHO_report_timestamp_command_macros */        

/* IP Fragmentation command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoIpFrag_t defines the IP Fragmentation command. The command structure is defined as 32 bit
 *          values to work with the hardware regardless of the device endianness.
 *
 *  @details 
 */

typedef struct pasahoIpFrag_s  {
    uint32_t  word0;          /**< Contains the ip fragmentation command information. @ref PASAHO_ip_frag_command_macros */
} pasahoIpFrag_t;

/** 
 *  @defgroup PASAHO_ip_frag_command_macros  PASAHO IP Fragmentation Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO IP Fragmentation Command Macros
 *  Macros used by the PASAHO IP Fragmentation Command
 *  
 */
/*@{*/

#define PASAHO_SET_SUB_CODE_IP_FRAG(x) PASAHO_SET_BITFIELD((x)->word0, PASAHO_SUB_CMD_IP_FRAG, 24, 5)    /**< Set sub-command code to indicate IP Fragmentation command */
#define PASAHO_SET_SUB_CODE(x,v)  PASAHO_SET_BITFIELD((x)->word0, (v), 24, 5)  /**< Specifies the sub-command code */
#define PASAHO_SET_IP_OFFSET(x,v) PASAHO_SET_BITFIELD((x)->word0, (v), 16, 8)  /**< Specifies the offset to the IP header to be fragmented */
#define PASAHO_SET_MTU_SIZE(x,v)  PASAHO_SET_BITFIELD((x)->word0, (v), 0,  16) /**< Specifies the MTU size */

/*@}*/ /* @name PASAHO IP Fragmentation Command Macros */
/** @}*/ /* PASAHO_ip_frag_command_macros */


/* Patch Message Length command */
/**
 *  @ingroup pasaho_if_structures
 *  @brief  pasahoPatchMsgLen_t defines the message length patching command. The command structure is defined as 32 bit
 *          values to work with the hardware regardless of the device endianness.
 *
 *  @details 
 */

typedef struct pasahoPatchMsgLen_s  {
    uint32_t  word0;          /**< Contains the message length patching command information. @ref PASAHO_patch_msg_len_command_macros */
} pasahoPatchMsgLen_t;

/** 
 *  @defgroup PASAHO_patch_msg_len_command_macros  PASAHO Message Length Patching Command Macros
 *  @ingroup pasaho_if_macros
 *  @{
 *  @name PASAHO Message Length Patching Command Macros
 *  Macros used by the PASAHO Message Length Patching Command
 *  
 */
/*@{*/

#define PASAHO_SET_SUB_CODE_PATCH_MSG_LEN(x) PASAHO_SET_BITFIELD((x)->word0, PASAHO_SUB_CMD_PATCH_MSG_LEN, 24, 5)    /**< Set sub-command code to indicate Message Length Patching command */
#define PASAHO_SET_MSGLEN_OFFSET(x,v) PASAHO_SET_BITFIELD((x)->word0, (v), 16, 8)  /**< Specifies the offset to the message length field to be patched */
#define PASAHO_SET_MSGLEN_SIZE(x,v) PASAHO_SET_BITFIELD((x)->word0, (v), 15,  1)   /**< Specifies the size of the length field (0: 16-bit; 1: 32-bit) */
#define PASAHO_SET_MSGLEN(x,v)  PASAHO_SET_BITFIELD((x)->word0, (v), 0,  15)       /**< Specifies the message length excluding the IP header and payload length */

/*@}*/ /* @name PASAHO Message Length Patching Command Macros */
/** @}*/ /* PASAHO_patch_msg_len_command_macros */



#ifdef __cplusplus
}
#endif

#endif  /* _PASAHO_H */



  
  
