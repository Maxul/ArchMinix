#ifndef _PA_H
#define _PA_H

#ifdef __cplusplus
extern "C" {
#endif

/* System level header files */
#include <stdint.h>
#include <stdlib.h>

#include <ti/drv/pa/paver.h>

/* ============================================================= */
/**
 *   @file  pa.h
 *
 *   path  ti/drv/pa/pa.h
 *
 *   @brief  Packet Accelerator (PA) sub-system LLD API and Data Definitions
 *
 *  ============================================================================
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
 
/**  @mainpage Packet Accelerator Low Level Driver
 *
 *   \image html doxydoc.wmf
 *
 *   @section intro  Introduction
 *
 *   The packet accelerator sub-system (PASS) is designed to provide the input packet classification, checksum/CRC 
 *   verification and generation, data manipulation and etc. The PASS consists of the following resources
 *      - Six PDSPs for packet and command processing
 *      - Three 64-entry LUT1 (connected to PDSP0, PDSP1 and PDSP2) for Layer 2/3 or custom LUT1 lookup
 *      - One 8192-entry LUT2 (connected to PDSP3) for Layer 4/5 or custom LUT2 lookup
 *      - Six programmable CRC engines (connected to each PDSP respectively) for CRC computation and verification
 *      - Six 16-bit general purpose timers
 *
 *   The packet accelerator low level driver (PA LLD) provides configuration and control of the packet accelerator
 *   sub-system (PASS). The sub-system provides from network packet classification and routing based on  
 *   network header information (see @ref netlayers). The packet accelerator low level driver module 
 *   (referred to as the module) provides APIs to configure the criteria used for from-network packet
 *   routing.
 *
 *   The module attempts to abstract the operation of the PASS from the application. The module uses the following rules 
 *   when configuring the PASS:
 *      - All received packets from Ethernet and/or SRIO are routed to PDSP0
 *      - PDSP0 does L0-L2 (MAC/SRIO) lookup using LUT1-0. If the packet is IP, it is forwarded to PDSP1
 *      - PDSP1 does the outer IP or Custom LUT1 lookup using LUT1-1
 *      - PDSP2 does any subsequent IP or Custom LUT1 lookup using LUT1-2
 *      - PDSP3 does all TCP/UDP and Custom LUT2 lookup using LUT2
 *      - PDSP4 is used for post-lookup processes such as checksum/CRC result verification.
 *      - PDSP4/5 can be used for pre-transmission operation such as transmit checksum generation.
 *
 *   With the exception of some initial setup functions, the module does not communicate directly with
 *   the sub-system. The output of the module is a formatted data block along with a destination address.
 *   The module user must send the formatted data to the sub-system. This is typically done by linking the
 *   created data block to a host packet descriptor, and then using the addressing information to send
 *   the created packet to the sub-system through the queue manager and PKTDMA.
 *
 *   For packets to the network, the sub-system provides ones complement checksum or CRC generation over
 *   a range provided by the module user. The range is not determined by sub-system by parsing the
 *   to-network packet, since it is assumed that the creator of the packet already has the start offset,
 *   length, initial checksum value and etc.
 *
 *   The low level driver maintains two tables of layer 2 and layer 3 configuration information. The memory 
 *   for these tables is provided by the module user at run time. The module maintains ownership of these 
 *   tables and the module user must not write to the memory once provided to the module.
 *
 *   In multi-core devices the module can be used in two different configurations. In independent core
 *   mode each core in a device has a unique set of tables. Although it is legal for any core to
 *   reference handles from other cores, this is not typically done. In this case cache coherency and
 *   cross core semaphores are not implemented by the module user. In common core mode there is only
 *   one set of tables and they are shared by all cores. Each core that uses the module must initialize
 *   it, but each core will provide the exact same buffers to the module. The module user will have
 *   the first core to initialize the module also initialize the table. Other cores will initialize their
 *   internal state but not initalize the table. In this mode @ref cache coherency and cross core @ref semaphores
 *   must be implemented by the module user to insure the integrity of the tables.
 */
 
/* Define PALLD Module as a master group in Doxygen format and add all PA LLD API 
   definitions to this group. */
/** @defgroup palld_module PA LLD Module API
 *  @{
 */
/** @} */
   
/** @defgroup palld_api_functions PA LLD Functions
 *  @ingroup palld_module
 */
 
/** @defgroup palld_api_macros PA LLD Macros
 *  @ingroup palld_module
 */

/** @defgroup palld_api_structures PA LLD Data Structures
 *  @ingroup palld_module
 */

/** @defgroup palld_api_constants PA LLD Constants (enum's and define's)
 *  @ingroup palld_module
 */
 
/**
 *  @def  pa_PARAMS_NOT_SPECIFIED
 *        Used for unspecified classification parameters
 */
#define pa_PARAMS_NOT_SPECIFIED                      0xFFFF

/**
 *  @def  pa_LUT_INST_NOT_SPECIFIED
 *        Used if LUT1(or LUT2) instance is not specified
 *        In the case, the PA LLD will decide which LUT instance to use based on the API type and the previous link information 
 */
#define pa_LUT_INST_NOT_SPECIFIED                    -1

/**
 *  @def  pa_LUT1_INDEX_NOT_SPECIFIED
 *        Used if LUT1 index is not specified
 *        In the case, the PASS will use the first available entry 
 */
#define pa_LUT1_INDEX_NOT_SPECIFIED                  -1

/**
 *  @def  pa_MAX_NUM_LUT1_ENTRIES
 *        The maximum number of LUT1 entries
 *         
 */
#define pa_MAX_NUM_LUT1_ENTRIES                      64

/**
 *  @defgroup  ReturnValues  Function Return Values
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PALLD Function Return Codes
 *
 *  Error codes returned by PALLD API functions.
 */
/*@{*/
/**
 *  @def  pa_OK
 *        PA return code -- Function executed successfully
 */
#define pa_OK	                                 0

/**
 *  @def  pa_ERR_CONFIG
 *        Invalid configuration provided to PA
 */
#define pa_ERR_CONFIG                          -10

/**
 *   @def pa_INSUFFICIENT_CMD_BUFFER_SIZE
 *        The provided buffer was too small to hold the command
 */
#define pa_INSUFFICIENT_CMD_BUFFER_SIZE        -11

/**
 *  @def pa_INVALID_CMD_REPLY_DEST
 *       An invalid destination was provided for command replies
 */
#define pa_INVALID_CMD_REPLY_DEST              -12

/**
 *   @def  pa_DUP_ENTRY
 *         A duplicate active entry was found in the L2/L3 table.
 *         If the module user intends to replace the associate routing 
 *         information for the same entry, command packet should be
 *         delivered to the PASS via the PKTDMA sub-system     
 *         Otherwise, module user may decide to drop the command packet and 
 *         free the buffer.
 */ 
#define pa_DUP_ENTRY                           -13

/**
 *   @def  pa_INVALID_DUP_ENTRY
 *         A duplicate pending entry was found in the handle table
 *         This entry can not be handled until the pending entry
 *         becomes active
 */ 
#define pa_INVALID_DUP_ENTRY                   -14


/**
 *   @def  pa_INVALID_TABLE_MORE_SPECIFIC_ENTRY_PRESENT
 *         A more specific entry was found in the handle table
 */
#define pa_INVALID_TABLE_MORE_SPECIFIC_ENTRY_PRESENT   -15

/**
 *   @def  pa_INVALID_MPLS_LABEL
 *         An MPLS label exceeded 20 bits
 */
#define pa_INVALID_MPLS_LABEL                  -16

/**
 *   @def  pa_HANDLE_TABLE_FULL
 *         No room for an entry in the L2 table
 */
#define pa_HANDLE_TABLE_FULL                   -17

/**
 *   @def  pa_INVALID_INPUT_HANDLE
 *         Invalid handle provided
 */
#define pa_INVALID_INPUT_HANDLE                -18

/**
 *   @def  pa_HANDLE_INACTIVE
 *         Operation requested on an inactive handle
 */
#define pa_HANDLE_INACTIVE                     -19
 
/**
 *   @def  pa_INVALID_IP_FLOW
 *         A flow label exceeded 20 bits
 */ 
#define  pa_INVALID_IP_FLOW                    -20

/**
 *   @def  pa_WARN_ACTIVE_HANDLE_ACKED
 *         Sub-system reported activation of a handle already marked active
 */
#define pa_WARN_ACTIVE_HANDLE_ACKED            -21

/**
 *   @def  pa_LUT_ENTRY_FAILED
 *         Sub-system could not make an entry to the LUT1 table
 */
#define pa_LUT_ENTRY_FAILED                    -22


/**
 *   @def  pa_RESUBMIT_COMMAND
 *         Sub-system could not handle the command due to memory. Command must be resubmitted
 */
#define pa_RESUBMIT_COMMAND                    -23

/**
 *   @def  pa_SYSTEM_STATE_INVALID
 *         Tried to download an image to a running PDSP
 */
#define pa_SYSTEM_STATE_INVALID                -24

/**
 *   @def  pa_INVALID_LUT1_INDEX
 *         LUT1 index exceeds the LUT1 table range
 */
#define pa_INVALID_LUT1_INDEX                  -25

/**
 *   @def  pa_WARN_LNK_CNT_UNSYNC
 *         Warning: Link counter out of sync
 */
#define pa_WARN_LNK_CNT_UNSYNC                 -26

/**
 *   @def pa_CMDSET_TOO_BIG
 *        The total length of commads in the command set exceeds the limit
 */
#define pa_CMDSET_TOO_BIG                      -27

/**
 *   @def  pa_INVALID_LUT_INST
 *         The specified LUT1 or LUT2 instance does not exist
 */
#define pa_INVALID_LUT_INST                    -28


/** 
  *  @def pa_RESOURCE_INIT_DENIED
  *       The resource initialization permission denied 
  */
#define pa_RESOURCE_INIT_DENIED                 -29

/** 
  *  @def pa_RESOURCE_USE_DENIED
  *       The resource usage permission denied 
  */
#define pa_RESOURCE_USE_DENIED                  -30

/** 
  *  @def pa_FIRMWARE_REVISION_DIFFERENCE
  *       The firmware revision difference 
  */
#define pa_FIRMWARE_REVISION_DIFFERENCE         -31

/*@}*/
/** @} */


/**
 *  @defgroup  cmdMinBufSize Command buffer minimum size requirements
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Command buffer minimum sizes
 *
 *  Define command buffer minimum size requirements.
 */
/* @{ */

/**
 *  @def  pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_addSrio and @ref Pa_addCustomLUT1 function
 */
#define pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES          120

/**
 *  @def  pa_ADD_MAC_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_addMac function
 */
#define pa_ADD_MAC_MIN_CMD_BUF_SIZE_BYTES           pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES

/**
 *  @def  pa_DEL_HANDLE_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_delHandle function
 */
#define pa_DEL_HANDLE_MIN_CMD_BUF_SIZE_BYTES        20

/**
 *  @def  pa_DEL_L4_HANDLE_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_delL4Handle function
 */
#define pa_DEL_L4_HANDLE_MIN_CMD_BUF_SIZE_BYTES     24

/**
 *  @def  pa_ADD_IP_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_addIp function
 */
#define pa_ADD_IP_MIN_CMD_BUF_SIZE_BYTES            pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES 

/**
 *  @def  pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_addCustomLUT2 function
 */
#define pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES          44

/**
 *  @def  pa_ADD_PORT_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_addPort function
 */
#define pa_ADD_PORT_MIN_CMD_BUF_SIZE_BYTES          pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES

/**
 *  @def  pa_CONFIG_EXCEPTION_ROUTE_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_configExceptionRoute function
 */
#define pa_CONFIG_EXCEPTION_ROUTE_MIN_CMD_BUF_SIZE_BYTES   504

/**
 *  @def  pa_CONFIG_CRC_ENGINE_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_configCrcEngine function
 */
#define pa_CONFIG_CRC_ENGINE_MIN_CMD_BUF_SIZE_BYTES  88

/**
 *  @def  pa_CONFIG_MULTI_ROUTE_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_configMultiRoute function
 */
#define pa_CONFIG_MULTI_ROUTE_MIN_CMD_BUF_SIZE_BYTES 84

/**
 *  @def  pa_SET_CUSTOM_LUT1_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_setCustomLUT1 function
 */
#define pa_SET_CUSTOM_LUT1_MIN_CMD_BUF_SIZE_BYTES    60

/**
 *  @def  pa_SET_CUSTOM_LUT2_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_setCustomLUT2 function
 */
#define pa_SET_CUSTOM_LUT2_MIN_CMD_BUF_SIZE_BYTES    36

/**
 *  @def  pa_CONFIG_CMD_SET_MIN_CMD_BUF_SIZE_BYTES
 *        The minmium command buffer size allowed when using the @ref Pa_configCmdSet and @ref Pa_formatTxCmd function
 */
#define pa_CONFIG_CMD_SET_MIN_CMD_BUF_SIZE_BYTES     144

/**
 *  @def  pa_REQUEST_STATS_MIN_CMD_BUF_SIZE_BYTES
 *        The minimum command buffer size required when using the @ref Pa_requestStats and @ref Pa_requestUsrStats functions
 */
#define pa_REQUEST_STATS_MIN_CMD_BUF_SIZE_BYTES      24

/**
 *  @def  pa_CONFIG_USR_STATS_MIN_CMD_BUF_SIZE_BYTES
 *        The minmium command buffer size allowed when using the @ref Pa_configUsrStats function with the maximum number of 
 *        user-defined statistics. The size of command packet is calculated as 20 + (number of statistic entries) * 4. 
 */
#define pa_CONFIG_USR_STATS_MIN_CMD_BUF_SIZE_BYTES   2068

/**
 *  @def  pa_GLOBAL_CONFIG_MIN_CMD_BUF_SIZE_BYTES
 *        The minmium command buffer size allowed when using the @ref Pa_control (pa_CONTROL_SYS_CONFIG) function to perform PASS 
 *        global configuration.
 */
#define pa_GLOBAL_CONFIG_MIN_CMD_BUF_SIZE_BYTES      56

/**
 *  @def  pa_802_1ag_DET_MIN_CMD_BUF_SIZE_BYTES
 *        The minmium command buffer size allowed when using the @ref Pa_control (pa_CONTROL_802_1ag_CONFIG) function to configure 
 *        the 802.1ag packet detector.
 */
#define pa_802_1ag_DET_MIN_CMD_BUF_SIZE_BYTES        24

/**
 *  @def  pa_IPSEC_NAT_T_DET_MIN_CMD_BUF_SIZE_BYTES
 *        The minmium command buffer size allowed when using the @ref Pa_control (pa_CONTROL_IPSEC_NAT_T_CONFIG) function to configure 
 *        the IPSEC NAT-T packet detector.
 */
#define pa_IPSEC_NAT_T_DET_MIN_CMD_BUF_SIZE_BYTES    24



/**
 *  @def  pa_MAX_CMD_BUF_SIZE_BYTES
 *        The maximum command buffer size requested when using any PA API call which generates command packet.
 */
#define pa_MAX_CMD_BUF_SIZE_BYTES                    2068

/* @} */  
/** @} */


/**
 * @ingroup palld_api_structures
 * @brief MAC address specification
 *
 * @details  This type is used to pass MAC addresses (see @ref netlayers) to the module. The most significant byte
 *           of the mac address is placed in array element 0.
 */
#define pa_MAC_ADDR_SIZE       6 
typedef unsigned char paMacAddr_t[pa_MAC_ADDR_SIZE];

/**
 * @ingroup palld_api_structures
 * @brief  IPv4 address specification
 *
 * @details  This type is used to pass IPv4 addresses (see @ref netlayers) to the module. The most significant byte
 *           of the IP address is placed in array element 0.
 */
#define pa_IPV4_ADDR_SIZE      4
typedef unsigned char paIpv4Addr_t[pa_IPV4_ADDR_SIZE];

/**
 * @ingroup palld_api_structures
 * @brief  IPv6 address specificiation
 *
 * @details  This type is used to pass IPv6 addresses (see @ref netlayers) to the module. The most significant byte
 *           of the IP address is placed in array element 0.
 */
#define pa_IPV6_ADDR_SIZE      16 
typedef unsigned char paIpv6Addr_t[pa_IPV6_ADDR_SIZE];

/**
 * @ingroup palld_api_structures
 * @brief  IP address specification
 *
 * @details  This union is used to specify an IP address to the module. The type in the union is determined
 *           through other parameters passed to the module (see @ref IpValues).
 */
typedef union  {

  paIpv6Addr_t  ipv6;   /**< IPv6 address */
  paIpv4Addr_t  ipv4;   /**< IPv4 address */
  
} paIpAddr_t;

/**
 *  @defgroup  IpValues IP types
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name   IP Values
 *  @brief  Defines the IP version type used.
 *
 *  @details The packet accelerator module parses both IPv4 and IPv6 network layer headers (see @ref netlayers). 
 *           This group is used to distinguish which type of header will be used.
 */
/* @{ */
/**
 *  @def  pa_IPV4
 *        IPv4
 */
#define  pa_IPV4  4

/**
 *  @def   pa_IPV6
 *        IPv6
 */
#define  pa_IPV6  6
  
/*  @}  */  
/** @} */


/**
 * @ingroup palld_api_structures
 * @brief Specification of Pa_Handle 
 *
 * The Pa_Handle is used to identify a PA LLD instance
 */
typedef void*  Pa_Handle;

/**
 * @ingroup palld_api_structures
 * @brief  PA handle specification for L2 and L3 (LUT1) handles
 *
 * @details  This type is used to reference L2 and L3 (LUT1) routing information (see @ref netlayers). The module
 *           user is responsible for storing the handle and using it to refer to routing information already
 *           created through calls to @ref Pa_addMac, @ref Pa_addSrio, @ref Pa_addCustomLUT1 and @ref Pa_addIp.
 */
typedef void*  paHandleL2L3_t;

/**
 * @brief The un-linked inner IP handle
 *
 * @details This handle value is used to specify an inner IP (tunnel) which the application does not
 *          want to link to an outer IP address.
 */
#define PA_LLD_HANDLE_IP_INNER  ((paHandleL2L3_t)1)

/**
 *  @ingroup palld_api_structures
 *  @brief  PA handle specification for L4 (LUT2) handles
 *
 *  @details  This type is used to reference L4 (LUT2) routing information (see @ref netlayers). The module user
 *            is responsible for storing the handle. It is used again only to delete a created route.
 *            
 */
typedef uint32_t paHandleL4_t[2];


/**
 *  @ingroup palld_api_structures
 *  @brief A generic entry handle types
 *
 *  @details  The union of both entry handle types used by the module is used only in function @ref Pa_forwardResult.
 *            The function will return the corresponding entry type and its handle in the command response packets when a LUT1
 *            or LUT2 entry is added into the LUT1/LUT2 table successfully.
 *            The handle entry will be set to zero in all other cases 
 */
typedef union  {

  paHandleL2L3_t  l2l3Handle;  /**<  Level 2 or level 3 handle created by @ref Pa_addMac @ref Pa_addSrio, @ref Pa_addCustomLUT1 or @ref Pa_addIp */
  paHandleL4_t    l4Handle;    /**<  Level 4 handle created by @ref Pa_addPort or @ref Pa_addCustomLUT2 */

} paEntryHandle_t;

/**
 *  @ingroup salld_api_constants
 *  @{
 *  @brief  The number of bytes available for custom lookup
 *
 *  @details Custom lookup sizes are fixed by hardware
 */
#define pa_NUM_BYTES_CUSTOM_LUT1  32
#define pa_NUM_BYTES_CUSTOM_LUT2   4
/** @} */

/**
 *  @defgroup HandleTypes Handle Types
 *
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Handle Types
 *
 *  @brief  These values are used to describe what type of handle is referenced.
 *
 *  @details  These values are used only for function @ref Pa_forwardResult. The function returns with a copy
 *            of the handle, which the module user should already have, along with the type of handle. The
 *            module user can use this information to verify that a particular handle has been fully activated
 *            and can be used for linking reference in calls to @ref Pa_addIp, @ref Pa_addCustomLUT1, 
 *            @ref Pa_addCustomLUT2 or @ref Pa_addPort.
 */
/*  @{  */
/**
 *
  *  @def  pa_L2_HANDLE
  *        Level 2 (MAC/SRIO) handle
  */
#define  pa_L2_HANDLE   2

/**
 *  @def  pa_L3_HANDLE
 *        Level 3 (IP, Custom LUT1) handle
 */
#define  pa_L3_HANDLE   3

/**
 *  @def  pa_L4_HANDLE
 *        Level 4 (TCP/UDP/GTP-U/Custom LUT2) handle
 */
#define  pa_L4_HANDLE   4

/**
 *  @def  pa_INVALID_HANDLE
 *        Invalid handle type
 */
#define pa_INVALID_HANDLE   -1

/*  @}  */  
/** @} */


/**
 *  @defgroup ErouteTypes Exception Route Types
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Exception Route Types
 *
 *  @brief  These values are used to define exception route conditions.
 *
 *  @details  The exception route defines the global routing information when the exception condition such
 *            as LUT1 lookup failure, packet parsing failure, broadcast packet detection and etc. Multiple
 *            exception routes can be configured through @ref Pa_configExceptionRoute. All the exception
 *            routes are disabled by default.
 */
/*  @{  */
/**
 *
 *   @def  pa_EROUTE_L2L3_FAIL
 *         packet failed to match in L2/L3 (LUT1) table
 */
#define pa_EROUTE_L2L3_FAIL         0

/**
 *  @def  pa_EROUTE_VLAN_MAX_DEPTH
 *        packet exceeded maximum number of VLAN tags
 */
#define pa_EROUTE_VLAN_MAX_DEPTH    1

/**
 *  @def  pa_EROUTE_IP_MAX_DEPTH
 *        packet exceeded maximum number of IP headers
 */
#define pa_EROUTE_IP_MAX_DEPTH      2

/**
 *  @def  pa_EROUTE_MPLS_MAX_DEPTH
 *        packet exceeded maximum number of MPLS headers
 */
#define pa_EROUTE_MPLS_MAX_DEPTH    3

/**
 *  @def  pa_EROUTE_GRE_MAX_DEPTH
 *        packet exceeded maximum number of GRE headers
 */
#define pa_EROUTE_GRE_MAX_DEPTH     4

/**
 *  @def pa_EROUTE_PARSE_FAIL
 *       packet failed to parse
 */
#define pa_EROUTE_PARSE_FAIL        5

/**
 *  @def pa_EROUTE_L4_FAIL
 *       packet failed to match in L4 (LUT2) table
 */
#define pa_EROUTE_L4_FAIL           6

/**
 *  @def pa_EROUTE_IP_FRAG
 *       IP fragmented packet 
 */
#define pa_EROUTE_IP_FRAG           7

/**
 *  @def pa_EROUTE_IPV6_OPT_FAIL
 *       Packet failed due to unsupported IPV6 option header
 */
#define pa_EROUTE_IPV6_OPT_FAIL     8

/**
 *  @def pa_EROUTE_UDP_LITE_FAIL
 *       UDP lite packet had invalid checksum coverage
 */
#define pa_EROUTE_UDP_LITE_FAIL     9

/**
 *  @def pa_EROUTE_ROUTE_OPTION
 *       IP routing had incomplete routes
 */
#define pa_EROUTE_ROUTE_OPTION      10

/**
 *  @def  pa_EROUTE_SYSTEM_FAIL
 *        Sub-system detected internal error
 */
#define pa_EROUTE_SYSTEM_FAIL       11

/**
 *  @def pa_EROUTE_MAC_BROADCAST
 *       MAC broadcast packet which is not specified at the lookup table
 */
#define pa_EROUTE_MAC_BROADCAST     12

/**
 *  @def pa_EROUTE_MAC_MULTICAST
 *       MAC multicast packet which is not specified at the lookup table
 */
#define pa_EROUTE_MAC_MULTICAST     13

/**
 *  @def pa_EROUTE_IP_BROADCAST
 *       IP broadcast packet which is not specified at the lookup table
 */
#define pa_EROUTE_IP_BROADCAST      14

/**
 *  @def pa_EROUTE_IP_MULTICAST
 *       IP multicast packet which is not specified at the lookup table
 */
#define pa_EROUTE_IP_MULTICAST      15

/**
 *  @def pa_EROUTE_GTPU_MESSAGE_TYPE_1
 *       GTP-U PING Request packet
 */
#define pa_EROUTE_GTPU_MESSAGE_TYPE_1      16

/**
 *  @def pa_EROUTE_GTPU_MESSAGE_TYPE_2
 *       GTP-U PING Response packet
 */
#define pa_EROUTE_GTPU_MESSAGE_TYPE_2      17

/**
 *  @def pa_EROUTE_GTPU_MESSAGE_TYPE_26
 *       GTP-U Error Indication packet
 */
#define pa_EROUTE_GTPU_MESSAGE_TYPE_26     18

/**
 *  @def pa_EROUTE_GTPU_MESSAGE_TYPE_31
 *       GTP-U Supported Header Notification packet
 */
#define pa_EROUTE_GTPU_MESSAGE_TYPE_31     19

/**
 *  @def pa_EROUTE_GTPU_MESSAGE_TYPE_254
 *       GTP-U End Markr packet
 */
#define pa_EROUTE_GTPU_MESSAGE_TYPE_254    20

/**
 *  @def pa_EROUTE_GTPU_FAIL
 *       Packet failed due to GTPU parsing error or unsupporte dmessage types
 */
#define pa_EROUTE_GTPU_FAIL                21

/**
 *  @def pa_EROUTE_PPPoE_FAIL
 *       Packet failed due to PPPoE session packet parsing error
 */
#define pa_EROUTE_PPPoE_FAIL               22

/**
 *  @def pa_EROUTE_PPPoE_CTRL
 *       PPPoE session stage non-IP packets
 */
#define pa_EROUTE_PPPoE_CTRL               23

/**
 *  @def pa_EROUTE_802_1ag
 *       802.1ag Packet
 */
#define pa_EROUTE_802_1ag                  24

/**
 *  @def pa_EROUTE_IP_FAIL
 *       Packet failed due to invalid IP header
 */
#define pa_EROUTE_IP_FAIL                  25

/**
 *  @def pa_EROUTE_NAT_T_KEEPALIVE
 *       NAT-T Keep Alive packet where UDP Length = 9, data = 0xFF
 */
#define pa_EROUTE_NAT_T_KEEPALIVE          26

/**
 *  @def pa_EROUTE_NAT_T_CTRL
 *       NAT-T control packet where UDP Length > 12 and the first 4 payload bytes are equal to 0
 */
#define pa_EROUTE_NAT_T_CTRL               27

/**
 *  @def pa_EROUTE_NAT_T_DATA
 *       NAT-T IPSEC ESP data packet where UDP Length > 12 and the first 4 payload bytes are not equal to 0
 */
#define pa_EROUTE_NAT_T_DATA               28

/**
 *  @def pa_EROUTE_NAT_T_FAIL
 *       Invalid NAT-T packet
 */
#define pa_EROUTE_NAT_T_FAIL               29

/**
 *  @def   pa_EROUTE_MAX
 *         The maximum number of global route types
 */
#define pa_EROUTE_MAX                      30

/*  @}  */  
/** @} */


/**
 *  @defgroup NextHeaderTypes Next Header types 
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Next Header types
 *
 *  @brief  These values are used to define the next header (protocol) types for continus parsing after the 
 *          SRIO and custom parsing.
 *
 *  @details  The next header type can be derived from the upper layer header in a standard Ethernet packet.
 *            For SRIO and custom LUT1 lookup, the next header type should be provided by the user in function
 *            @ref Pa_setCustomLUT1 and @ref Pa_addSrio. 
 */
/*  @{  */
/**
 *
 *   @def  pa_HDR_TYPE_MAC
 *         MAC header
 */
#define pa_HDR_TYPE_MAC         0

/**
 *
 *  @def   pa_HDR_TYPE_IPV4
 *         IPv4 header
 */
#define pa_HDR_TYPE_IPV4        1


/**
 *
 *  @def   pa_HDR_TYPE_IPV6
 *         IPv6 header
 */
#define pa_HDR_TYPE_IPV6        2

/**
 *
 *  @def   pa_HDR_TYPE_CUSTOM_LUT1
 *         Custom LUT1 header
 */
#define pa_HDR_TYPE_CUSTOM_LUT1 3

/**
 *
 *  @def   pa_HDR_TYPE_UDP
 *         UDP header
 */
#define pa_HDR_TYPE_UDP         4

/**
 *
 *  @def   pa_HDR_TYPE_UDP_LITE
 */
#define pa_HDR_TYPE_UDP_LITE    5

/**
 *
 *  @def   pa_HDR_TYPE_TCP
 *         TCP header
 */
#define pa_HDR_TYPE_TCP         6

/**
 *
 *  @def   pa_HDR_TYPE_CUSTOM_LUT2
 *         Custom LUT2 header
 */
#define pa_HDR_TYPE_CUSTOM_LUT2 7

/**
 *
 *  @def   pa_HDR_TYPE_UNKNOWN
 *         next header type is not specified 
 */
#define pa_HDR_TYPE_UNKNOWN     8
 
/*  @}  */  
/** @} */

/** 
 * @ingroup palld_api_structures
 * @brief pa RM Handle
 */
typedef void *  pa_RmHnd;

/** 
 * @ingroup palld_api_structures
 * @brief PA start configuration structure
 */
typedef struct
{
    /** Provide a handle to the Resource Manager instance */
    pa_RmHnd rmHandle;
} paStartCfg_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  Pointer to the buffer where the PASS command is placed
 *
 *  @details  Functions in this module produce formatted commands that must be sent to the packet accelerator 
 *            sub-system. These commands are always referred to through this type.
 */
typedef void*  paCmd_t;


/**
 * @ingroup palld_api_structures
 * @brief  PA Size Configuration Structure
 *
 * @details The module is configured at run time with a maximum number of handles supported. The module
 *          maintains a set of handles and links between handles.
 */
typedef struct  {

  int nMaxL2;   /**< Maximum number of L2 handles supported */
  int nMaxL3;   /**< Maximum number of L3 handles supported */
  int nUsrStats;/**< Maximum number of user-defined statistics supported (maximum: 512)*/

} paSizeInfo_t;

/**
 *  @ingroup palld_api_structures
 *  @brief PA Initialization config structure
 *
 *  @details The parameters in this structure are used to do initial setup
 *           of the driver including its base address and other default settings.
 *           
 *  @note    The stream interface switch controls the destination of the traffic
 *           from the Ethernet switch. The default setting of the streaming
 *           interface switch is to route all traffic to the host. However, 
 *           this module is designed to receive the incoming packets at the PDSP0.
 *           If the initDeafultRoute is set to TRUE, this module will re-configure
 *           the stream interface switch to route all traffic to PDSP0. Otherwise,
 *           it is the module user's reponsibility to deliver incoming packets
 *           to PDSP0 via the CPPI/QMSS interface.           
 */
typedef struct  {
  uint16_t      initTable;          /**< If True then the L2/L3 tables are initialized */
  uint16_t      initDefaultRoute;   /**< If True then the switch default route is set to PASS PDSP0 */
  uint32_t      baseAddr;           /**< Specify the PASS base address */
  paSizeInfo_t* sizeCfg;            /**< Pointer to the size configuration information */
} paConfig_t;

/**
 *  @ingroup salld_api_constants
 *  @{
 *  @brief  Protocol Limit related constant definitions
 */
#define pa_PROTOCOL_LIMIT_NUM_VLANS_DEF    2    /**< Number of VLAN supported: default value */ 
#define pa_PROTOCOL_LIMIT_NUM_IP_DEF       2    /**< Number of IP layers supported: default value */ 
#define pa_PROTOCOL_LIMIT_NUM_GRE_DEF      2    /**< Number of GRE layers supported: default value */
#define pa_PROTOCOL_LIMIT_NUM_VLANS_MAX    3    /**< Number of VLAN supported: maximum value */ 
#define pa_PROTOCOL_LIMIT_NUM_IP_MAX       7    /**< Number of IP layers supported: maximum value */ 
#define pa_PROTOCOL_LIMIT_NUM_GRE_MAX      7    /**< Number of GRE layers supported: maximum value */
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  Protocol-specific Limitations.
 *
 *  @details  paProtocolLimit_t is used to defines the protocol-specific restrictions. For example, 
 *            it is necessary to limit the number of protocol layers such as GRE of the input packets 
 *            to prevent the irregular packets take too much processing time. 
 *            The PASS will detect the packets which violate the protocol-specific restrictions and either discard 
 *            or forward the packets to host queues which can be specified through API @ref Pa_configExceptionRoute.
 * 
 *  @note     The PASS will work when non-default values are used. However, it may limit the supported packet rate 
 *            below wire rate.
 */
typedef struct {

  uint8_t vlanMax; /**< Maximum number of VLANs supported, default = 2, maximum = 3 */
  uint8_t ipMax;   /**< Maximum number of IP layers supported, default = 2, maximum = 7 */
  uint8_t greMax;  /**< Maximum number of GRE layers supported, default = 2, maximum = 7 */

} paProtocolLimit_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  IP Reassembly Configuration Information.
 *
 *  @details  paIpReassmConfig_t is used to configure the PA-assisted IP reassembly operation. Two separate structures are used 
 *            for the outer IP and inner IP respectively. The IP reassembly assistance feature is disabled until 
 *            this information is provided. See section @ref appendix3 for deatiled description. 
 *  @note The maximum number of traffic flows is limited due to processing time and internal memory restriction.
 */
typedef struct {

  uint8_t numTrafficFlow; /**< Maximum number of IP reassembly traffic flows supported, default = 0, maximum = 32 */
  uint8_t destFlowId;     /**< CPPI flow which instructs how the link-buffer queues are used for forwarding packets */
  uint16_t destQueue;     /**< Destination host queue where PASS will deliver the packets which require IP reassembly assistance */

} paIpReassmConfig_t;

/**
 *  @ingroup palld_api_constants
 *  @brief   Define the maximum number of IP reassembly traffic flows 
 *
 */
#define pa_MAX_IP_REASM_TRAFFIC_FLOWS   32 

/**
 *  @ingroup palld_api_structures
 *  @brief  Command Set Configuration Information.
 *
 *  @details  paCmdSetConfig_t defines command set configuration parameters such as the maximum number of command sets.
 *            The PASS supports either 64 of 64-byte or 32 of 128-byte command sets. The number of command sets should 
 *            be configured at system startup. 
 */
typedef struct {

  uint8_t  numCmdSets; /**<  Number of command sets supported (32, 64), default = 64
                             @note If the number of command sets is set to 64, then each command entry will be limited to 64 bytes.
                                   If the number of command sets is set to 32, then each command entry will be limited to 128 bytes */
} paCmdSetConfig_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  User-defined Statistics Configuration Information.
 *
 *  @details  paUsrStatsConfig_t defines the configuration parameters for multi-level hierarchical user-defined statistics 
 *            operation such as the number of user-defined counters. There are up to 512 user-defined statistics consisting of 
 *            some 64-bit counters and some 32-bit counters whereas the total size of all counters cannot exceed 2048 bytes. 
 *            The user-defined statistics feature is disabled until this configuration is invoked through API @ref Pa_control. 
 *
 *            - 64-bit Counters index: 0 - (num64bCounters - 1)
 *            - 32-bit Counters index: num64bCounters - (numCounters - 1)
 */
typedef struct {

  uint16_t numCounters;     /**< Total number of user-defined counters, default = 0, maximum = 512 */
  uint16_t num64bCounters;  /**< Number of 64-bit user-defined counters, default = 0, maximum = 256 */
   
} paUsrStatsConfig_t;
 
/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum number of user-defined statistics the module supports.
 *
 */
#define pa_USR_STATS_MAX_COUNTERS        512

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum number of user-defined 64-bit statistics 
 *
 */
#define pa_USR_STATS_MAX_64B_COUNTERS    (pa_USR_STATS_MAX_COUNTERS/2)
/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum number of user-defined 32-bit statistics 
 *
 */
#define pa_USR_STATS_MAX_32B_COUNTERS    pa_USR_STATS_MAX_COUNTERS

/**
 *  @ingroup palld_api_structures
 *  @brief  Queue Diversion Configuration Information.
 *
 *  @details  The PASS supports optional queue diversion operation per LUT2 entry replacement. 
 *            paQueueDivertConfigl_t contains configuration information for the atomic queue diversion operation.
 *            The queue diversion feature is disabled until this configuration is invoked through API @ref Pa_control. 
 *
 */
typedef struct {

  uint16_t destQueue;     /**< Destination queue where PASS will deliver the LUT2 response packet which contains the 
                               queue diversion information */
  uint8_t  destFlowId;    /**< CPPI flow which instructs how the link-buffer queues are used for forwarding 
                               the LUT2 response packets */
} paQueueDivertConfig_t;


/**
 *  @defgroup pktControlInfo  PA Packet Control Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA Packet Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in @ref paPacketControlConfig_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_PKT_CTRL_HDR_VERIFY_PPPoE
 *        Control Info -- Set: Perform enhanced error check of the PPPoE header
 *                        Clear: Perform basic error check of the PPPoE header
 */
#define pa_PKT_CTRL_HDR_VERIFY_PPPoE       0x0001 
/**
 *  @def  pa_PKT_CTRL_HDR_VERIFY_IP
 *        Control Info -- Set: Perform enhanced error check of the IP header
 *                        Clear: Perform basic error check of the IP header
 */
#define pa_PKT_CTRL_HDR_VERIFY_IP          0x0002 
/**
 *  @def  pa_PKT_CTRL_MAC_PADDING_CHK
 *        Control Info -- Set: Perform MAC (802.3) padding check
 *                             The packet with illegal padding will be dropped  
 *                        Clear: Do not perform MAC (802.3) padding check
 */
#define pa_PKT_CTRL_MAC_PADDING_CHK        0x0004 
/**
 *  @def  pa_PKT_CTRL_IP_FRAGS_TO_EROUTE
 *        Control Info -- Set: Forward IP Fragments through the exception route regardless of the routing destination 
 *                        Clear: Forward IP Fragments through the exception route only if the routing destination is set to SASS or CONTINUE_PARSE
 */
#define pa_PKT_CTRL_IP_FRAGS_TO_EROUTE     0x0008 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  Packet Control Configuration Information.
 *
 *  @details  This data structure defines miscellaneous packet control information for some non-default PASS operations. 
 *            For example, PASS always performs basic protocol header verification to ensure that it can continue parsing the  
 *            current and next protocol header. The PASS will perform enhanced error check of protocol headers specified
 *            by this configuration. For example,
 *            PPPoE header in session mode:
 *              - Version = 1
 *              - Type = 1
 *              - Code = 0
 *
 *            IPv4 header:
 *              - Header length >= 20
 *              - Total length > 20
 *              - Source address is not broadcast
 *              - Destination address is not 0
 *              - TTL is not 0
 *
 *  @note refer to the @ref ErouteTypes for the corresponding exception routes.
 */
typedef struct {

  uint16_t ctrlBitMap;              /**< Packet control bit as defined at @ref pktControlInfo */
  uint16_t rxPaddingErrStatsIndex;  /**< Specify the user statistics index of Rx padding error counter */
  uint16_t txPaddingStatsIndex;     /**< Specify the user statistics index of Tx MAC padding counter */

} paPacketControlConfig_t;

/**
 * @ingroup palld_api_structures
 * @brief PA System Configuration Information structure
 *
 * @details paSysConfig_t contains pointers to the system-level configuration structures defined above. The null pointer 
 *          indicates the configuration of the corresponding sub-group is not required.
 */
typedef struct {
  paProtocolLimit_t*        pProtoLimit;           /**< Pointer to the protocol limit configuration structure */
  paIpReassmConfig_t*       pOutIpReassmConfig;    /**< Pointer to the outer IP Reassembly  configuration structure */
  paIpReassmConfig_t*       pInIpReassmConfig;     /**< Pointer to the inner IP Reassembly  configuration structure */
  paCmdSetConfig_t*         pCmdSetConfig;         /**< Pointer to the command set configuration structure */
  paUsrStatsConfig_t*       pUsrStatsConfig;       /**< Pointer to the user-defined statistics configuration structure */
  paQueueDivertConfig_t*    pQueueDivertConfig;    /**< Pointer to the queue-diversion configuration structure */
  paPacketControlConfig_t*  pPktControl;           /**< Pointer to the packet control configuration structure */

} paSysConfig_t;


/**
 *  @defgroup pa802p1agDetectInfo  PA 802.1ag Detector Control Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA 802.1ag Detector Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in @ref pa802p1agDetConfig_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_802_1ag_DETECT_ENABLE
 *        Control Info -- Set: Enable 802.1ag Detector
 *                        Clear: Disable 802.1ag Detector
 */
#define pa_802_1ag_DETECT_ENABLE           0x0001 
/**
 *  @def  pa_802_1ag_DETECT_STANDARD
 *        Control Info -- Set: Perform 802.1ag packet detection per 802.1ag formal standard
 *                        Clear:  Perform 802.1ag packet detection per 802.1ag draft 
 */
#define pa_802_1ag_DETECT_STANDARD         0x0002 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief   802.1ag Detection Configuration Information.
 *
 *  @details  The 802.1ag packet can be recognized with ether type equal to 0x8902 normally. However, the PASS can be 
 *            configured to further qualify the IEEE 802.1ag packet per one of the following criteria: 
 *            - 802.1ag standard: Destion MAC address = 01-80-c2-00-00-3x, Ether type = 0x8902
 *            - 802.1ag draft: Destion MAC address = 01-80-c2-xx-xx-xx, Ether type = 0x8902 
 *
 *  @note The 802.1ag detector is disabled by default.
 *  @note refer to the @ref ErouteTypes for the corresponding exception routes.
 * 
 */
typedef struct {
  uint16_t ctrlBitMap;     /**< 802.1ag Detector control info as defined at @ref pa802p1agDetectInfo */
} pa802p1agDetConfig_t;


/**
 *  @defgroup ipsecNatTCtrlInfo  PA IPSEC NAT-T Control Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA IPSEC NAT-T Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in @ref pa802p1agDetConfig_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_IPSEC_NAT_T_CTRL_ENABLE
 *        Control Info -- Set: Enable IPSEC NAT-T packet detection
 *                        Clear: Disable IPSEC NAT-T packet detection
 */
#define pa_IPSEC_NAT_T_CTRL_ENABLE           0x0001 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief   IPSEC NAT-T Packet Detection Configuration Information.
 *
 *  @details  paIpsecNatTConfig_t is used to configure the IPSEC NAT-T packet detector which is disabled
 *            until this configuration is invoked through API @ref Pa_control. 
 *
 *  @note The IPSEC NAT-T packet detector is disabled by default.
 *  @note refer to the @ref ErouteTypes for the corresponding exception routes.
 * 
 */
typedef struct {

  uint16_t ctrlBitMap;     /**< IPSEC NAT-T control info as defined at @ref ipsecNatTCtrlInfo */
  uint16_t udpPort;        /**< Specify the UDP port number which uniquely identifies the IPSEC NAT-T packets */
} paIpsecNatTConfig_t;

/**
 * @defgroup  paCtrlCode PA Control Code
 * @ingroup palld_api_constants
 * @{
 *
 * @name PA Control Code
 *
 * @brief Define the PA LLD control code  
 *
 */
/** @ingroup paCtrlCode */
/* @{ */
/**
 *  @def  pa_CONTROL_SYS_CONFIG
 *        system-level configuration 
 */
#define  pa_CONTROL_SYS_CONFIG              0  

/**
 *  @def  pa_CONTROL_802_1ag_CONFIG
 *        802.1ag Detector configuration 
 */
#define  pa_CONTROL_802_1ag_CONFIG          1 

/**
 *  @def  pa_CONTROL_IPSEC_NAT_T_CONFIG
 *        IPSEC NAT-T Packet Detector configuration 
 */
#define  pa_CONTROL_IPSEC_NAT_T_CONFIG      2  

/*  @}  */  
/** @} */


/**
 * @ingroup palld_api_structures
 * @brief PA Control Information structure
 *
 * @details Data structure defines PA control information used by API @ref Pa_control. 
 *
 */
typedef struct {
  uint16_t    code;                          /**< Specify the PA control code as defined at @ref paCtrlCode */
  union {
    paSysConfig_t           sysCfg;          /**< Specify system-level configuration parameters */
    pa802p1agDetConfig_t    pa802p1agDetCfg; /**< Specify 802.1ag Detector configuration parameters */
    paIpsecNatTConfig_t     ipsecNatTDetCfg; /**< Specify IPSEC NAT-T Detector configuration parameters */
  }params;                                   /**< Contain the control operation specific parameters */

} paCtrlInfo_t;
 

/**
 *  @ingroup palld_api_structures
 *  @brief  The return type for module functions
 *
 *  @details  Function calls to this module return values used to determine if the command was successful or
 *            the reason for failure (see @ref ReturnValues).
 */

typedef int paReturn_t;

/**
 *   @ingroup palld_api_structures
 *   @brief  paCmdReply_t is used to specify command result (from PASS) routing information
 *
 *   @details Commands sent to packet accelerator sub-system will generate replies. These replies
 *            can be either discarded by the sub-system or routed to a queue. Command replies that
 *            must be forwarded back to this module are detailed for each command. The module user
 *            typically either selects a unique destination queue for command replies, or else supplies
 *            a unique value for replyId. This value is placed into software info word 0 in the 
 *            packet descriptor for the returned command. The data in the returned packet is not
 *            typically examined by the module user, but passed directly back to this module through 
 *            API function @ref Pa_forwardResult to examine the results of the command.
 */
typedef struct  {

  int       dest;        /**<  Packet destination, must be pa_DEST_HOST or pa_DEST_DISCARD, see @ref pktDest */
  uint32_t  replyId;     /**<  Value placed in swinfo0 in reply packet */
  uint16_t  queue;       /**<  Destination queue for destination pa_DEST_HOST */
  uint8_t   flowId;      /**<  Flow ID used on command reply from PASS */
  
} paCmdReply_t;

/**
 *  @ingroup palld_api_constants
 *  @brief   Define the maximum number of buffers the module can request
 *
 */
#define pa_N_BUFS               4 

/**
 *  @defgroup  paBufIndex PA Memory Buffer Index
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name   PA Memory Buffer Index
 *  @brief  Define the buffer inedex of the PA LLD memory blocks.
 *
 */
/* @{ */
/**
 *  @def  pa_BUF_INST
 *        PA LLD instance buffer
 */
#define pa_BUF_INST             0
/**
 *  @def  pa_BUF_L2_TABLE
 *        PA LLD match table of Layer 2 (MAC/SRIO) entries
 */
#define pa_BUF_L2_TABLE         1
/**
 *  @def  pa_BUF_L3_TABLE
 *        PA LLD match table of Layer 3 (IP/CustomLUT1) entries
 */
#define pa_BUF_L3_TABLE         2
/**
 *  @def  pa_BUF_USR_STATS_TABLE
 *        PA LLD link table of user-defined statistics 
 */
#define pa_BUF_USR_STATS_TABLE  3

/*  @}  */  
/** @} */


/**
 *  @ingroup palld_api_functions
 *  @brief Pa_getBufferReq returns the memory requirements for the PA driver
 *
 *  @details This function returns the memory buffer requirements in term
 *           of the size and alignment array. The PA LLD requires up to 
 *           four memory blocks as described below:
 *           - PA Instance: PA instance data
 *           - L2 Table: Layer-2 (MAC/SRIO) entry information
 *           - L3 Table: Layer-3 (IP/Custom LUT1) entry information
 *           - User Statistics Link Table: User-defined Statistics entry information (Optional)
 *
 *  @param[in]   sizeCfg     Size configuration information
 *  @param[out]  sizes       Array of size requirements
 *  @param[out]  aligns      Array of alignment requirements
 *  @retval                  Value (@ref ReturnValues)
 *
 *  @note This function specifies the minimum memory buffer requirements, it is up to the
 *        module user to round up the buffer alignemnt and size to the cache line boundary
 *        to ensure cache coherency if cacheable memory is used.
 */
paReturn_t Pa_getBufferReq (paSizeInfo_t *sizeCfg, int sizes[], int aligns[]);

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_create creates the PA driver instance
 *
 *  @details This function initializes the PA driver based on user configuration
 *
 *  @param[in]  cfg     Configuration information
 *  @param[in]  bases   Array of the memory buffer base addresses 
 *  @param[out] pHandle Instance handle. This is a pointer to an initialized
 *                      instance structure. 
 *  @retval             Value (@ref ReturnValues)
 */
paReturn_t Pa_create (paConfig_t *cfg, void* bases[], Pa_Handle *pHandle);

/**
 *  @ingroup palld_api_functions
 *  @brief   Pa_startCfg Adds PA configuration
 *  @details This function needs to be called from all cores to initialize PA with 
 *           per core configurations
 *
 *  @param[in]  handle  The PA LLD instance identifier
 *  @param[in]  startCfg PA start configuration
 *  @retval     None
 */
void Pa_startCfg (Pa_Handle handle, paStartCfg_t *startCfg);

/**
 *  @ingroup palld_api_functions
 *  @brief Pa_close decativates the PA driver instance
 *
 *  @details This function deactivates the PA driver instance, all the associated
 *           memory buffers can be freed after this call. 
 *
 *  @param[in]  handle  The PA LLD instance identifier
 *  @param[out] bases   Array of the memory buffer base addresses 
 *  @retval             Value (@ref ReturnValues)
 */
paReturn_t Pa_close (Pa_Handle handle, void* bases[]);

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_control performs system-level control and configuration
 *
 *  @details This function performs PASS control operations including system-level figurations. 
 *           The system-level configurations are divided into several sub-groups which can be configured 
 *           independently. The default configuration will be used until this API is invoked.
 *
 *           On return the command buffer (cmd) contains a formatted command for the sub-system when the cmdSize
 *           is set to non-zero. The destination for the command is provided in cmdDest. The module user must send 
 *           the formatted command to the sub-system. The sub-system will generate a reply
 *           and this reply must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *
 *  @param[in]      handle      The PA LLD instance identifier
 *  @param[in]      ctrl        Control information
 *  @param[out]     cmd         Where the created command is placed
 *  @param[in,out]  cmdSize     Input the size of cmd buffer, on output the actual size used. @ref cmdMinBufSize
 *  @param[in]      reply       Where the sub-system sends the command reply
 *  @param[out]     cmdDest     Value (@ref cmdTxDest) 
 *  @retval                     Value (@ref ReturnValues)
 */
paReturn_t Pa_control (Pa_Handle      handle, 
                       paCtrlInfo_t  *ctrl, 
                       paCmd_t        cmd,
                       uint16_t       *cmdSize,
                       paCmdReply_t   *reply,
                       int            *cmdDest);


/**
 * @defgroup  pktDest Routed Packet Destinations
 * @ingroup palld_api_constants
 * @{
 *
 * @name Routed Packet Destinations
 *
 * @brief The module user specifies packet destinations for packets exiting the packet accelerator sub-system.
 *
 * @details  The destination of packets that leave the packet accelerator sub-system
 *           are provided to the module in the @ref paRouteInfo_t structure and passed
 *           to the module through the @ref Pa_addMac, @ref Pa_addSrio, @ref Pa_addIp, @ref Pa_addCustomLUT1, 
 *           @ref Pa_addCustomLUT2 and @ref Pa_addPort functions
 */
/** @ingroup pktDest */
/* @{ */

/** 
 *  @def  pa_DEST_DISCARD
 *        packet is discarded
 */
#define  pa_DEST_DISCARD  3  /**< Packet is discarded */

/** 
 *  @def  pa_DEST_CONTINUE_PARSE_LUT1
 *        packet remains in PA sub-system for more parsing and LUT1 classification
 */
#define  pa_DEST_CONTINUE_PARSE_LUT1  4 /**< Packet remains in PA sub-system for more parsing and LUT1 classification */

/** 
 *  @def  pa_DEST_CONTINUE_PARSE_LUT2
 *        packet remains in PA sub-system for more parsing and LUT2 classification. 
 */
#define  pa_DEST_CONTINUE_PARSE_LUT2  5  /**< Packet remains in PA sub-system for more parsing and LUT2 classification */

/**
 *  @def  pa_DEST_HOST
 *        host thread 
 */
#define  pa_DEST_HOST   6   /**< Packet is routed to host */

/** 
 *  @def  pa_DEST_EMAC
 *        ethernet mac port (of the switch)
 */
#define  pa_DEST_EMAC   7   /**< Packet is routed to  EMAC */

/** 
 *  @def  pa_DEST_SASS
 *        security accelerator destination 
 */
#define  pa_DEST_SASS   8   /**< Packet is routed to SA */

/** 
 *  @def  pa_DEST_SRIO
 *        SRIO interface
 */
#define  pa_DEST_SRIO   9   /**< Packet is routed to SRIO */
  
/*  @}  */  
/** @} */

/**
 * @defgroup  paEmacPort Ethernet MAC port
 * @ingroup palld_api_constants
 * @{
 *
 * @name Ethernet MAC port
 *
 * @brief The module user specifies the Ethernet MAC port of the ingress and egress packets.
 *
 * @details  In the from-network direction, the module user can specify the input port as one of classification parameters.
 *           In the to-network direction, the module user can force the egress packets to be sent over the specified 
 *           destination Ethernet MAC port of the switch regreless of its states or configurations.
 */
/** @ingroup customType */
/* @{ */
/**
 *  @def  pa_EMAC_PORT_NOT_SPECIFIED
 *        From-Netwprk: Don't care
 *        To-Network: Use standard switch forwarding 
 */
#define  pa_EMAC_PORT_NOT_SPECIFIED   0   

/* @def   pa_EMAC_PORT_0
 *        Use EMAC Port 0
 */
#define  pa_EMAC_PORT_0               1   

/* @def   pa_EMAC_PORT_1
 *        Use EMAC Port 1
 */
#define  pa_EMAC_PORT_1               2   

/*  @}  */  
/** @} */

/**
 * @defgroup emcOutputCtrlBits Ethernet MAC Output Control Bit Definitions
 * @ingroup palld_api_constants
 * @{
 *
 * @name Ethernet MAC Output Control Bit Definition
 *
 * Bitmap definition of the emacCtrl at @ref paRouteInfo_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  pa_EMAC_CTRL_PORT_MASK
 *        Control Info -- EMAC port mask
 */
#define pa_EMAC_CTRL_PORT_MASK            0x0F 
/**
 *  @def  pa_EMAC_CTRL_CRC_DISABLE
 *        Control Info -- 0:EMAC port computes and inserts CRC
 *                        1:EMAC port does not generate CRC  
 */
#define pa_EMAC_CTRL_CRC_DISABLE          0x80 

/*  @}  */  
/** @} */

/**
 * @defgroup  customType Custom Classification Types
 * @ingroup palld_api_constants
 * @{
 *
 * @name Custom Classification Types
 *
 * @brief The module user specifies the custom classification types.
 *
 * @details  The optional custom classification rule may be used to further parse and calssify the incoming
 *           packet.
 */
/** @ingroup customType */
/* @{ */
/**
 *  @def  pa_CUSTOM_TYPE_NONE
 *        Use standard classification 
 */
#define  pa_CUSTOM_TYPE_NONE   0   

/* @def   pa_CUSTOM_TYPE_LUT1
 *        Custom classification with LUT1
 */
#define  pa_CUSTOM_TYPE_LUT1   1   

/* @def   pa_CUSTOM_TYPE_LUT2
 *        Custom classification with LUT2
 */
#define  pa_CUSTOM_TYPE_LUT2   2   

/*  @}  */  
/** @} */

/**
 *   @brief  The maximum number of LUT1 Custom Types supported
 */
#define pa_MAX_CUSTOM_TYPES_LUT1   4


/**
 *   @brief  The maximum number of LUT2 Custom Types supported
 */
#define pa_MAX_CUSTOM_TYPES_LUT2   16


/**
 * @defgroup  cmdTxDest Command/Transmit Packet Destinations
 * @ingroup palld_api_constants
 * @{
 *
 * @name Command/Transmit Packet Destinations
 *
 * @brief These values are used by the module user to deliver the configuration packets to the specific PDSP within PASS.
 */
/* @{ */
/**
 *   @def  pa_CMD_TX_DEST_0
 *         Destination PDSP0
 */
#define  pa_CMD_TX_DEST_0    0  /**< Packet is sent to PDSP0 */

/**
 *   @def  pa_CMD_TX_DEST_1
 *         Destination PDSP1
 */
#define  pa_CMD_TX_DEST_1    1  /**< Packet is sent to PDSP1 */

/**
 *   @def  pa_CMD_TX_DEST_2
 *         Destination PDSP2
 */
#define  pa_CMD_TX_DEST_2    2  /**< Packet is sent to PDSP2 */

/**
 *   @def  pa_CMD_TX_DEST_3
 *         Destination PDSP3
 */
#define  pa_CMD_TX_DEST_3    3  /**< Packet is sent to PDSP3 */

/**
 *   @def  pa_CMD_TX_DEST_4
 *         Destination PDSP4
 */
#define  pa_CMD_TX_DEST_4    4  /**< Packet is sent to PDSP4 */

/**
 *   @def  pa_CMD_TX_DEST_5
 *         Destination PDSP5
 */
#define  pa_CMD_TX_DEST_5    5  /**< Packet is sent to PDSP5  */
 
 
/*  @}  */  
/** @} */

/**
 * @defgroup  paLut1Inst PA LUT1 Instance Destinations
 * @ingroup palld_api_constants
 * @{
 *
 * @name PA LUT1 Instance Destinations
 *
 * @brief These values are used by the module user to specify the LUT1 table instance used by the specified IP or customLUT1 entry
 */
/* @{ */
/**
 *   @def  pa_LUT1_INST_0
 *         LUT1 instance 0
 */
#define  pa_LUT1_INST_0    0   /**< LUT1 table connected to PDSP0 */

/**
 *   @def  pa_LUT1_INST_1
 *         LUT1 instance 1
 */
#define  pa_LUT1_INST_1    1   /**< LUT1 table connected to PDSP1 */

/**
 *   @def  pa_LUT1_INST_2
 *         LUT1 instance 2
 */
#define  pa_LUT1_INST_2    2   /**< LUT1 table connected to PDSP2 */

/**
 *   @def  pa_LUT1_INST_MAX
 *         Specify the maximum LUT1 instance
 */
#define  pa_LUT1_INST_MAX  pa_LUT1_INST_2   
 
/*  @}  */  
/** @} */


/**
 * @defgroup  paCmdCode Command Code
 * @ingroup palld_api_constants
 * @{
 *
 * @name PA Command Codes
 *
 * @brief Define the commands which can be executed in PASS 
 *
 * @details  A single command or a set of commands can be executed to support fully-offloaded 
 *           data path in both the transmit (to network) and receive (from network) directions.  
 *           In the to-network direction, the list of commands formatted by the module should 
 *           be stored as the protocol-specific information at the packet descriptor with the 
 *           packet. The commands will be executed in order at PASS and the associated security
 *           accelerator sub-system (SASS). The executed commands will be removed by PASS and 
 *           SASS so that the output packet will not contain any command.
 *           In the from-network direction, the list of commands formatted by the module will 
 *           be stored at the PASS as a command set which can be referred to by the command set 
 *           index. A single command including a command set can be executed per the enhanced 
 *           routing information @ref paRouteInfo_t after a LUT1 or LUT2 matches.
 *
 * @note     The packet offset specified at each command of the command list should be strictly 
 *           in ascending order becasue the PASS processes the list of commands in order and it
 *           can not move backwards. The command violating the order requirement may be detected
 *           and rejected by the API @ref Pa_formatTxCmd and @ref Pa_configCmdSet. In the case, 
 *           the order constraint can not be validated at the LLD, the violating command will 
 *           be ignored by the PASS.    
 */
/** @ingroup paCmdCode */
/* @{ */
/**
 *  @def  pa_CMD_NONE
 *        End of commands 
 */
#define  pa_CMD_NONE                        0   

/* @def   pa_CMD_NEXT_ROUTE
 *        Specifies next route 
 */
#define  pa_CMD_NEXT_ROUTE                  1   

/*  @def  pa_CMD_CRC_OP
 *        CRC generation or verification 
 */
#define  pa_CMD_CRC_OP                      2   

/*  @def  pa_CMD_COPY_DATA_TO_PSINFO
 *        Copy Data from the packet to the PS Info Area in the packet descriptor 
 */
#define  pa_CMD_COPY_DATA_TO_PSINFO         3

/*  @def  pa_CMD_PATCH_DATA
 *        Insert or patch packet data at the specific location  
 */
#define  pa_CMD_PATCH_DATA                  4  

/*  @def  pa_CMD_TX_CHECKSUM
 *        Compute and insert checksum  
 */
#define  pa_CMD_TX_CHECKSUM                 5 

/*  @def  pa_CMD_MULTI_ROUTE
 *        Duplicate packet to multiple destinations  
 */
#define  pa_CMD_MULTI_ROUTE                 6  

/*  @def  pa_CMD_REPORT_TX_TIMESTAMP
 *        Report the PA 32-bit timestamp at the timestamp field of the packet descriptor
 */
#define  pa_CMD_REPORT_TX_TIMESTAMP         7 

/*  @def  pa_CMD_REMOVE_HEADER
 *        Remove the parsed packet header 
 *  @note It should be the first command in the rx command set 
 */
#define  pa_CMD_REMOVE_HEADER               8 

/*  @def  pa_CMD_REMOVE_TAIL
 *        Remove the parsed packet tail 
 *
 *  @note It should be the last command next to the next route or multi-route command       
 */
#define  pa_CMD_REMOVE_TAIL                 9 


/*  @def  pa_CMD_CMDSET
 *        Specify the command set to be executed  
 */
#define  pa_CMD_CMDSET                      10   

/*  @def  pa_CMD_SA_PAYLOAD
 *        Specify the payload information required by SASS  
 */
#define  pa_CMD_SA_PAYLOAD                  11   

/*  @def  pa_CMD_IP_FRAGMENT
 *        Perform IPv4 fragmentation  
 */
#define  pa_CMD_IP_FRAGMENT                 12 

/*  @def  pa_CMD_USR_STATS
 *        Update the specified user-defined counter and the counters which are linked to this counter  
 */
#define  pa_CMD_USR_STATS                   13    
   
   
/*  @def  pa_CMD_CMDSET_AND_USR_STATS
 *        Combination of the CMDSET and USR_STATS commands. 
 *  @note It is only used as a command executed after the last classification per the enhanced routing 
 *        information      
 */
#define  pa_CMD_CMDSET_AND_USR_STATS        14   

/*  @def  pa_CMD_PATCH_MSG_LEN
 *        Update the message length field within some L2 protocol header such as 802.3 and PPPoE after the
 *        potential IP fragmentation operation
 *  @note This command is only used in conjunction with the pa_CMD_IP_FRAGMENT command.    
 */
#define  pa_CMD_PATCH_MSG_LEN               15 

/*  @def  pa_CMD_VERIFY_PKT_ERROR
 *        Verify the packet error based on the CPPI error flags as specified at @ref Appendix2 and forward
 *        the error packet to the specified destination 
 *  @note This packet error verification is not applicable to the CRC verification operation within the same
 *        command set.
 *  @note This command should be either the last command or the second last to the nextRoute command since 
 *        all commands following this operation will be ignored if packet error is found. 
 */
#define  pa_CMD_VERIFY_PKT_ERROR            16 


/*  @def  pa_CMD_SPLIT
 *        Split the packet into header and payload portion to be delivered to different queues with 
 *        different CPPI flows 
 *  @note This command is only supported in the from-network direction
 *  @note This command should be placed ahead of any pa_CMD_PATCH command so that the header size can be adjusted accordingly
 *  @note The first 8-byte of psInfo area is reserved for this operation, therefore, the destOffset of pa_CMD_COPY_DATA_TO_PSINFO
 *        commands within the same command set should be 8 or larger.
 *        
 */

#define  pa_CMD_SPLIT                      17
   
/*  @}  */  
/** @} */

/**
 *  @defgroup routeCtrlInfo  PA Routing Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA Routing Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paCmdNextRoute_t. 
 */ 
/*@{*/
/**
 *  @def  pa_NEXT_ROUTE_PARAM_PRESENT
 *        Control Info -- Set: Routing information such as flowId, queue are in command
 *                        Clear: Routing information such as flowId, queue are in packet
 */
#define pa_NEXT_ROUTE_PARAM_PRESENT              0x0001 
/**
 *  @def  pa_NEXT_ROUTE_PROC_NEXT_CMD
 *        Control Info -- Set: Process the next command prior to forward the packet to its final destination
 *                        Clear: Forward the packet to the next destination without executing any more command
 *  @note The data patch command (pa_CMD_PATCH_DATA) is the only one which can follow the next route command.
 */
#define pa_NEXT_ROUTE_PROC_NEXT_CMD              0x0002 
/**
 *  @def  pa_NEXT_ROUTE_PROC_MULTI_ROUTE
 *        Control Info -- Set: Multi-route is valid, the packet should be forwarded and then perform multi-route
 *                        Clear: Multi-route is invalid
 *  @note: This option is only valid in the receive (from-network) direction      
 */
#define pa_NEXT_ROUTE_PROC_MULTI_ROUTE           0x0004 
/**
 *  @def  pa_NEXT_ROUTE_TX_L2_PADDING
 *        Control Info -- Set: Perform MAC padding for packet with size smaller than 60
 *                        Clear: Do not perform MAC padding
 *  @note: This option is only valid in the transmit (to-network) direction      
 */
#define pa_NEXT_ROUTE_TX_L2_PADDING              0x0008 
/*@}*/
/** @} */


/**
 *  @ingroup palld_api_structures
 *  @brief  Next Route Command
 *
 *  @details paCmdNextRoute_t defines the final route information
 *           The next route command can be used in both to-network and from-network directions. 
 *           In the to-network direction, it may be used multiple times to route traffic between PASS and SASS 
 *           before the packet is finally forwarded to the network. For example, the following steps show the 
 *           SRTP over IPSEC AH to-network traffic:
 *  @verbatim 
               1. Packet is delivered to SASS for SRTP operation
               2. Packet is delivered to PASS for UDP checksum operation
               3. Packet is delivered to SASS for IPSEC AH operation
               4. Packet is delivered to PASS for AH authentication tag insertion
               5. Packet is delivered to the network.
    @endverbatim
 *           The next route commands are required for step 3 and 5. The complete routing information should be provided 
 *           in the to-network direction.
 *
 *           In the from-network direction, the next route command is used only if the multiple route is required. 
 *           In this case, only the parameter "ctrlBitfield" and "multiRouteIndex" are valid. After all the commands in the 
 *           command set are executed, the PASS will deliver packets to their desired destination based on the parameters 
 *           specified at the routing information upon the LUT1/LUT2 matching.  
 *           If the next route command is specified, it must be the last command within a command set. The commands following 
 *           the next route command will not be executed.  
 */

typedef struct {

  uint16_t    ctrlBitfield;    /**< Routing control information as defined at @ref routeCtrlInfo */
  int         dest;            /**< Packet destination as defined at @ref pktDest */
  uint8_t     pktType_emacCtrl;/**<  For destination SRIO, specify the 5-bit packet type toward SRIO 
                                     For destination HOST, EMAC, specify the EMAC control @ref emcOutputCtrlBits to the network */
  uint8_t     flowId;          /**< For host, SA or SRIO destinations, specifies return free descriptor setup */
  uint16_t    queue;           /**< For host, SA or SRIO destinations, specifies the dest queue */
  uint32_t    swInfo0;         /**< Placed in SwInfo0 for packets to host or SA; Placed in the PS Info for packets to SRIO*/
  uint32_t    swInfo1;         /**< Placed in SwInfo1 for packets to the SA; Placed in the PS Info for packets to SRIO */
  uint16_t    multiRouteIndex; /**< Multi-route index. It is valid in the from-network direction only */
} paCmdNextRoute_t;

/**
 *  @defgroup  crcFrameTypes CRC Frame types 
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name CRC Frame types
 *
 *  @brief  Define the frame types which are used to extract and derive the CRC operation parameters such as CRC starting 
 *          offset and CRC payload length from the frame header. 
 *
 *  @details  Both the payload length and the byte location where CRC calculation begins may vary in some protocl 
 *            frame such as WCDMA FP HS-DSCH Data Frame type 2 and type 3. The CRC Frame type is used for PASS to
 *            extract and/or derive the CRC starting offset and payload length.
 *
 *  @note     Only the following frame types are supported.
 */
/*  @{  */
/**
 *
 *   @def  pa_CRC_OP_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE2
 *         WCDMA FP HS-DSCH Data Frame Type 2
 */
#define pa_CRC_OP_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE2         0

/**
 *
 *  @def   pa_CRC_OP_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE3
 *         WCDMA FP HS-DSCH Data Frame Type 3
 */
#define pa_CRC_OP_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE3         1

#define pa_CRC_OP_FRAME_TYPE_MAX                          pa_CRC_OP_FRAME_TYPE_IUB_FP_HS_DSCH_TYPE3

 
/*  @}  */  
/** @} */


/**
 *  @defgroup crcOpCtrlInfo  PA CRC Command Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA CRC Command Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paCmdCrcOp_t. 
 */ 
/*@{*/
/**
 *  @def  pa_CRC_OP_CRC_VALIDATE
 *        Control Info -- Set: CRC Validate
 *                        Clear: CRC Computation
 */
#define pa_CRC_OP_CRC_VALIDATE              0x0001 
/**
 *  @def  pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER
 *        Control Info -- Set: CRC length field in the header
 *                        Clear: CRC length specified in command
 */
#define pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER  0x0002 
/**
 *  @def  pa_CRC_OP_PAYLOAD_LENGTH_OFFSET_IS_NEGATIVE
 *        Control Info -- Set: Payload length field resides prior to the parsed header offset 
 *                             length field offset = offset from the current parsed header - lenOffset
 *                        Clear: Payload length field resides after the parsed header offset
 *                             length field offset = offset from the current parsed header + lenOffset
 */
#define pa_CRC_OP_PAYLOAD_LENGTH_OFFSET_IS_NEGATIVE  0x0004 
/**
 *  @def  pa_CRC_OP_CRC_FRAME_TYPE
 *        Control Info -- Set: Frame Type is specified
 *                        Clear: Frame Type is not specified, use offset 
 *                               parameter
 */
#define pa_CRC_OP_CRC_FRAME_TYPE            0x0008 
/**
 *  @def  pa_CRC_OP_CRC_RESULT_FOLLOW_PAYLOAD
 *        Control Info -- Set: CRC field following payload
 *                        Clear: CRC offset specified in command
 */
#define pa_CRC_OP_CRC_RESULT_FOLLOW_PAYLOAD 0x0010 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  CRC Generation/Verification Command
 *
 *  @details paCmdCrcOp_t is used to create CRC operation command instruct the PASS to 
 *           perform CRC operation in both to-network and from-network directions. The
 *           module user is responsible for configuring the corresponding CRC engines
 *           which are used for the specified CRC operation. 
 *
 *           In the to-network direction, the payload offset, payload length and CRC offset 
 *           should be available in the command. The generated CRC will be inserted into
 *           the CRC location in the packet.
 *
 *           In the from-network direction, the payload length is either a constant or 
 *           available in the custom header. The CRC verification will be performed by
 *           the CRC engine connected to the PDSP where the CRC command is executed. 
 *           The CRC verification result will be indicated by the error flags within 
 *           the CPPI descriptor as described at section table @ref appendix2 
 */

typedef struct {

  uint16_t    ctrlBitfield;    /**< CRC operation control information as defined at @ref crcOpCtrlInfo */
  uint16_t    startOffset;     /**< Byte location, from SOP/Protocol Header, where the CRC computation begins 
                                    if frame type is not specified
                                    Byte location, from SOP/Protocol header, where the specific frame header begins
                                    if frame type is specified
                                    In to-network direction: offset from SOP
                                    In from-network direction: offset from the current parsed header 
                                    */
  uint16_t    len;             /**< Number of bytes covered by the CRC computation 
                                    valid only if pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER is clear */
  uint16_t    lenOffset;       /**< Payload length field offset in the custom header */
  uint16_t    lenMask;         /**< Payload length field mask */
  uint16_t    lenAdjust;       /**< Payload length adjustment: valid only if pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER is set */
  uint16_t    crcOffset;       /**< Offset from SOP/Protocol Header to the CRC field 
                                    In to-network direction: offset from SOP
                                    In from-network direction: offset from the current parsed header */
  uint16_t    frameType;       /**< Frame type @ref crcFrameTypes, vaild if pa_CRC_OP_CRC_FRAME_TYPE is set */                                      
} paCmdCrcOp_t;

/**
 *  @defgroup splitOpCtrlInfo  PA SPLIT Command Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA SPLIT Command Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paCmdSplitOp_t. 
 */ 
/*@{*/
/**
 *  @def  pa_SPLIT_OP_FRAME_TYPE
 *        Control Info -- Set: Frame Type is specified
 *                        Clear: Frame Type is not specified, use offset 
 *                               parameter
 */
#define pa_SPLIT_OP_FRAME_TYPE            0x0001 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  Split Command
 *
 *  @details paCmdSplitOp_t is used to create Split command to instruct the PASS to 
 *           divide the ingress packet into the header and payload portion and deliver them
 *           to specified destination queues with specified CPPI flows respectively.
 *           Where the destination information of the header packet is specified by the 
 *           classification routing information and the destination information of the payload  
 *           packet is specified in this structure. 
 *
 */
typedef struct {
  uint16_t    ctrlBitfield; /**< Split operation control information as defined at @ref splitOpCtrlInfo */
  uint16_t    startOffset;  /**< Byte location, from Protocol Header, where the payload begins 
                                 if frame type is not specified
                                 Byte location, from Protocol header, where the specific frame header begins
                                 if frame type is specified
                                 In from-network direction: offset from the current parsed header 
                                 */
  uint16_t    frameType;    /**< Frame type @ref crcFrameTypes, vaild if pa_SPLIT_OP_FRAME_TYPE is set */         
  uint16_t    destQueue;    /**< Host queue for the payload packet */
  uint16_t    flowId;       /**< CPPI flow which instructs how link-buffer queues are used for sending payload packets. */   
                            
} paCmdSplitOp_t;   

/**
 *  @ingroup palld_api_structures
 *  @brief  Transmit checksum configuration
 *
 *  @details  paTxChksum_t is used in the call to @ref Pa_formatTxRoute or @ref Pa_formatTxCmd to create a tx 
 *            command header that instructs the packet accelerator sub-system to generate ones' complement
 *             checksums into network packets. The checksums are typically used for TCP and UDP payload checksums as
 *            well as IPv4 header checksums. In the case of TCP and UDP payload checksums the psuedo header
 *            checksum must be pre-calculated and provided, the sub-system does not calculate it.
 */
typedef struct {

  uint16_t startOffset;   /**<  Byte location, from SOP, where the checksum calculation begins */
  uint16_t lengthBytes;   /**<  Number of bytes covered by the checksum. Must be even */
  uint16_t resultOffset;  /**<  Byte offset, from startOffset, to place the resulting checksum */
  uint16_t initialSum;    /**<  Initial value of the checksum */
  uint16_t negative0;     /**<  If TRUE, a computed value of 0 is written as -0 */

} paTxChksum_t;


/**
 *  @defgroup copyCtrlInfo  PA Copy Command Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA Copy Command Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paCmdCopy_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_COPY_OP_FROM_END
 *        Control Info -- Set: Copy data from the end of the payload
 *                        Clear: Copy data from the beginning of the payload
 */
#define pa_COPY_OP_FROM_END                 0x0001 
/*@}*/
/** @} */
 
/**
 *  @ingroup palld_api_structures
 *  @brief  Copy Command
 *
 *  @details paCmdCopy_t is used to define how to copy number of bytes from the data packet to 
 *           the descriptor. The copy command is used to instruct the PASS to copy up to 8 byte 
 *           from packet to the PS info section in the packet descriptor in the from-network direction. 
 *           If the desired copy area crosses over the packet boundary, then garbage data will be copied.
 *
 *  @note: There are 20-byte packet information stored in the PS Info section. It is recommended to copy 
 *         packet data after the packet information area. Otherwise, the packet information will be 
 *         overwritten. There are upto 12 bytes can be copied with the packet information or upto
 *         32 bytes can be copied without the packet information. 
 */

typedef struct {

  uint16_t    ctrlBitfield;    /**< Copy operation control information as defined at @ref copyCtrlInfo */
  uint16_t    srcOffset;       /**< Offset from the start of current protocol header for the data copy to begin */
  uint16_t    destOffset;      /**< Offset from the top of the PSInfo for the data to be copied to */
  uint16_t    numBytes;        /**< Number of bytes to be copied */   
} paCmdCopy_t;


/**
 *  @ingroup palld_api_structures
 *  @brief  Multi-route Command
 *
 *  @details paCmdMultiRoute_t is used to specify the desired PA multi-route set.
 *           The multi-route command instructs the PASS to route the packets to multiple 
 *           destinations in the from-network direction only. It must be the last command 
 *           within a command set. The commands following the multi-route command will
 *           not be executed.  
 */
typedef struct {

  uint16_t    index;        /**<  Multi-route set Index */
} paCmdMultiRoute_t;


/**
 *  @ingroup salld_api_constants
 *  @def  pa_MAX_CMD_SETS
 *        The maximum number of command sets supported
 */
#define pa_MAX_CMD_SETS     64

/**
 *  @ingroup palld_api_structures
 *  @brief  Command Set Command
 *
 *  @details paCmdSet_t is used to specify the desired PA command set. The command set command 
 *           instructs the PASS to execute a list of commands after a LUT1 or LUT2 match occurs. 
 *           It is one of the command which can be embedded within the @ref paRouteInfo_t. 
 */
typedef struct {

  uint16_t    index;        /**< Command Set Index */
} paCmdSet_t;

/**
 *   @ingroup salld_api_constants
 *   @def  pa_MAX_PATCH_BYTES
 *         The maximum number of bytes that a patch command can accept
 */
#define pa_MAX_PATCH_BYTES     16      /**< PATCH Command in to-netweok direction */
#define pa_MAX_RX_PATCH_BYTES  32      /**< PATCH Command within a command set */

/**
 *  @defgroup patchCtrlInfo  PA Patch Command Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA Patch Command Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paPatchInfo_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_PATCH_OP_INSERT
 *        Control Info -- Set: Insert data into the packet
 *                        Clear: Patch data replaces existing packet data
 */
#define pa_PATCH_OP_INSERT                 0x0001 
/**
 *  @def  pa_PATCH_OP_MAC_HDR
 *        Control Info -- Set: Replace MAC header with patch data
 *                        Clear: Normal Patch/Insert operation
 */
#define pa_PATCH_OP_MAC_HDR                0x0002 
/**
 *  @def  pa_PATCH_OP_DELETE
 *        Control Info -- Set: Delete data in the packet
 *                        Clear: Normal Patch/Insert operation
 */
#define pa_PATCH_OP_DELETE                 0x0004 
/*@}*/
/** @} */


/**
 *  @ingroup palld_api_structures
 *  @brief  Packet patching configuration
 *
 *  @details paPatchInfo_t is used to create data patch command. The patch command is used to patch 
 *           existing data or insert data in the packet in both to-network and from-network directions.
 *
 *           In the to-network direction, it can be used to patch the authentication tag provided by SASS 
 *           into the AH header within the packet. In this case, the patch data is not present at the command 
 *           when it is formatted and it is appended by the SASS. The @ref Pa_formatRoutePatch is used to create
 *           a command block along with a packet routing command to forward the packet after the patch is complete
 *
 *           In the from-network direction, it can be used to insert up to 32 bytes to the offset location
 *           as part of the command set to be executed after a LUT1 or LUT2 match. 
 *           This command can be used to patch the entire MAC header for MAC router functionality. It may be further 
 *           enhanced and combined with other commands to support IP forwarding operation in the future.
 *           A short version of the patch command can be used to insert up to 2 bytes into the current parsing
 *           location of the packet after a LUT2 match.
 */

typedef struct {

  uint16_t   ctrlBitfield;      /**<  Patch operation control information as defined at @ref patchCtrlInfo */
  uint16_t   nPatchBytes;       /**<  The number of bytes to be patched */
  uint16_t   totalPatchSize;    /**<  The number of patch bytes in the patch command, must be >= to nPatchBytes and a multiple of 4 bytes */
  uint16_t   offset;            /**<  Offset from the start of the packet for the patch to begin in the to-network direction 
                                      Offset from the start of the current header for the patch to begin in the from-network direction */
  uint8_t    *patchData;        /**<  Pointer to the patch data */

} paPatchInfo_t;



/**
 *  @ingroup palld_api_structures
 *  @brief  paPayloadInfo_t defines the packet payload information in the short format.
 *          It is required by the Security Accelerator sub-system (SASS)
 *
 *  @details paPayloadInfo_t defines the packet parsing information in terms of
 *           payload offset and payload length as described below
 *  @li      SRTP:      offset to the RTP header; RTP payload length including ICV
 *  @li      IPSEC AH:  offset to the Outer IP; IP payload length
 *  @li      IPSEC ESP: offset to the ESP header; ESP papload length including ICV
 */

typedef struct  {
    uint16_t  offset;    /**< The offset to where the SA packet parsing starts */
    uint16_t  len;       /**< The total length of the protocal payload to be processed by SA */
    uint32_t  supData;   /**< Optional supplement data such as the 32-bit CountC for some 3GPP operation modes */
} paPayloadInfo_t;


/**
 *  @ingroup palld_api_structures
 *  @brief   Tx timestamp reporting information
 *
 *  @details paCmdTxTimestamp_t specifies the tx timestamp reporting information. The report tx timestamp command is used to instruct 
 *           the PASS to report the PA timestamp when the packet is transmitting out of PASS in a return (null) packet to the specified 
 *           host queue. The transmit timestamp may be used for the Precision Timing Protocol (PTP).
 */

typedef struct  {
    uint16_t  destQueue; /**< Host queue for the tx timestamp reporting packet */
    uint16_t  flowId;    /**< CPPI flow which instructs how link-buffer queues are used for sending tx timestamp reporting packets. */
    uint32_t  swInfo0;   /**< 32 bit value returned in the descriptor as swInfo0 which can be used as event identifier */
} paCmdTxTimestamp_t;

/**
 *  @ingroup palld_api_structures
 *  @brief   IP fragmentation information
 *
 *  @details paCmdIpFrag_t is used to create the IPv4 fragment command. The IP fragment command is used to instruct the PASS to 
 *           perform IPv4 fragmentation operation. This operation can be applied to both inner IP prior to IPSEC encapsulation and 
 *           outer IP after IPSEC encapsulation.  This command should go with a next route command which provides the destination 
 *           information prior to the fragmentation operation. 
 *    
 *           For the inner IP fragmentation, follow the following procedure:
 *  @li	     Host sends packets with the IP fragment command and the destination queue set to a host queue to PASS PDSP5 
 *           for IP fragmentation operation.
 *  @li      All fragments will be delivered to the specified host queue.
 *  @li      Host adds the outer MAC/IP header, invokes the SA LLD sendData function and then sends the fragments to the SA queue.
 *  @li      Each fragment will be encrypted, authenticated and forwarded to the final destination.
 *
 *           For the outer IP fragmentation, the overall operation is stated below: 
 *  @li      Packet is delivered to SASS for IPSEC operation
 *  @li	     Packet is delivered to PASS for IP Fragmentation operation
 *  @li      The entire packet or its fragments are delivered to the network.
 *
 *  @note the next route command is required for step 2
 *  @note The IP fragment command can not be combined with some other tx commands such as checksum and CRC commands since
 *        those commands may require the PASS operation across multiple fragments. The workaround is to break the tx commands into
 *        two groups. The first group consists of the checksum, CRC, other commands and a next route command which routes the packet
 *        back to the same PDSP to execute the second command group which consists of the IP fragment command and the next route 
 *        command which points to the final destination.
 *
 *        The IP fragment command can be combined with a single blind patch command to support the IPSEC AH use case in which the SASS 
 *        passes the IPSEC AH packet with the blind patch command to the PASS so that the autentication tag can be inserted into the AH 
 *        header. The recommended order of the tx commands is as the followings:
 *        - pa_CMD_IP_FRAGMENT
 *        - pa_CMD_NEXT_ROUTE with flag pa_NEXT_ROUTE_PROC_NEXT_CMD set
 *        - pa_CMD_PATCH_DATA 
 *
 *        The IP fragment command can be also combined with up to two message length patching commands to support the message length 
 *        field updating for each IP fragment. This operation is required for certain L2 header which contains a length field such as
 *        802.3 and PPPoE. The order of tx command is as the followings:
 *        - pa_CMD_PATCH_MSG_LEN (optional)
 *        - pa_CMD_PATCH_MSG_LEN (optional)
 *        - pa_CMD_IP_FRAGMENT
 *        - pa_CMD_NEXT_ROUTE 
 */

typedef struct  {
    uint16_t  ipOffset; /**< Offset to the IP header. */
    uint16_t  mtuSize;  /**< Size of the maximum transmission unit (>= 68) */
} paCmdIpFrag_t;

/**
 *  @ingroup palld_api_structures
 *  @brief   Message length patching configuration
 *
 *  @details paPatchMsgLenInfo_t is used to create message length patch command which is used in conjunction with
 *           the Ipv4 fragmentation command. This command instruct the PASS to update the message length field within 
 *           some L2 protocol header such as 802.3 and PPPoE after the potential IP fragmentation operation.
 *
 *           The PASS support up to two message length patching operations per IP fragmentation command.
 */

typedef struct {

  uint8_t    msgLenSize;    /**<  Size of message length field in bytes (@note only 2-byte message length is supported) */
  uint8_t    offset;        /**<  Offset from the start of the packet to the message length field */ 
  uint16_t   msgLen;        /**<  Message length excluding the IP header and payload length */

} paPatchMsgLenInfo_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  User-defined Statistics Command
 *
 *  @details paCmdUsrStats_t is used to specify the desired User-defined counter. The user stats command 
 *           instructs the PASS to update the specified user-defined counter and all the counters which are 
 *           linked to this counter 
 *           It is one of the command which can be embedded within the @ref paRouteInfo_t. 
 */
typedef struct {
  uint16_t    index;        /**< User-defined statistics index */
} paCmdUsrStats_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  Command Set plus User-defined Statistics Command
 *
 *  @details paCmdSetUsrStats_t is used to specify the desired PA command set and User-defined counter. This command
 *           provides the module user a mechanism to specify different user-defined counters with the same command set 
 *           for different LUT entries and vice versa. 
 *           This command instructs the PASS to update the specified user-defined counter and all the counters which are 
 *           linked to this counter and then execute the specified command set.
 *           It is one of the command which can be embedded within the @ref paRouteInfo_t. 
 */
typedef struct {
  uint16_t    setIndex;          /**< Commad Set Index */
  uint16_t    statsIndex;        /**< User-defined statistics index */
} paCmdSetUsrStats_t;


/**
 *  @defgroup pktErrInfo  PA Packet Error Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA Packet Error Info Bit Definitions
 *
 *  Bitmap definition of the errorBitfield in @ref paCmdVerifyPktErr_t. 
 *         
 */ 
/*@{*/
/**
 *  @def  pa_PKT_ERR_IP_CHECKSUM
 *        Control Info -- Set: Re-direct packet if IP checksum error occurs
 *                        Clear: Ignore IP checksum Error
 */
#define pa_PKT_ERR_IP_CHECKSUM                 0x0001 
/**
 *  @def  pa_PKT_ERR_L4_CHECKSUM
 *        Control Info -- Set: Re-direct packet if UDP/TCP checksum error occurs
 *                        Clear: Ignore UDP/TCP checksum Error
 */
#define pa_PKT_ERR_L4_CHECKSUM                 0x0002 
/**
 *  @def  pa_PKT_ERR_CRC
 *        Control Info -- Set: Re-direct packet if CRC error occurs
 *                        Clear: Ignore CRC Error
 */
#define pa_PKT_ERR_CRC                         0x0004 
/*@}*/
/** @} */
 
/**
 *  @ingroup palld_api_structures
 *  @brief  Verify Packet Error Command
 *
 *  @details paCmdVerifyPktErr_t is used to construct the "Verify Packet Error" command. The  
 *           IPv4 header checksum, UDP/TCP checksum and SCTP CRC-32c checksum verification are performed by 
 *           the PASS autonomously while the CRC verification is performed per command. The corresponding error bit 
 *           in the CPPI descriptor will be set and can be verified by the application when packet is delivered 
 *           to the host. This command instructs PASS to examine the specified error flags and forward the error 
 *           packet accordingly.
 */

typedef struct {

  uint16_t errorBitfield;   /**<  Packet Error information as defined at @ref pktErrInfo */
  uint8_t  dest;            /**<  Packet destination as defined at @ref pktDest */
  uint8_t  flowId;          /**<  For host destination, specifies CPPI flow which defines free queues are used for receiving packets */
  uint16_t queue;           /**<  For host destination, specifies the destination queue */
  uint32_t swInfo0;         /**<  Placed in SwInfo0 for packets to host */
} paCmdVerifyPktErr_t;


/**
 * @ingroup palld_api_structures
 * @brief PA Command Information structure
 *
 * @details Data structure defines PA commands. The PA command can be invoked by the @ref paRouteInfo_t as a simple command. 
 *          They are the building blocks for function @ref Pa_configCmdSet to create a list of commands refered as a command
 *          set in the from-network direction. They can be used by the function @ref Pa_formatTxCmd to create or update the
 *          list of tx commands.
 *
 */
typedef struct {
  uint16_t    cmd;         /**< Specify the PA command code as defined at @ref paCmdCode */
  union {
    paCmdNextRoute_t   route;    /**< Specify nextRoute command specific parameters */
    paTxChksum_t       chksum;   /**< Specify Tx Checksum command specific parameters */
    paCmdCrcOp_t       crcOp;    /**< Specify CRC operation command specific parameters */
    paCmdCopy_t        copy;     /**< Specify Copy command specific parameters */
    paPatchInfo_t      patch;    /**< Specify Patch command specific parameters */
    paPayloadInfo_t    payload;  /**< Specify the payload information required by SA */
    paCmdSet_t         cmdSet;   /**< Specify Command Set command specific parameters */
    paCmdMultiRoute_t  mRoute;   /**< Specify Multi-route command specific parameters */
    paCmdTxTimestamp_t txTs;     /**< Specify Report Tx Timestamp command specific parameters */
    paCmdIpFrag_t      ipFrag;   /**< Specify IP fragmentation command specific parameters */
    paCmdUsrStats_t    usrStats; /**< Specify User-defined Statistics command specific parameters */
    paCmdSetUsrStats_t cmdSetUsrStats; /**< Specify Command Set and User-defined Statistics command specific parameters */
    paPatchMsgLenInfo_t patchMsgLen;   /**< Specify Patch Message Length command specific parameters */
    paCmdVerifyPktErr_t verifyPktErr;  /**< Specify Packet error Verification command specific parameters */
    paCmdSplitOp_t      split;   /**< Specify Split command sepcific parameters */
  }params;                      /**< Contain the command specific parameters */

} paCmdInfo_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  IP lookup information
 *
 *  @details  paIpInfo_t is used to specifiy the IPv4 or IPv6 parameters used in packet routing.
 *            With the exception of parameter tos, a value of 0 in any parameter means that that
 *            field is not used in packet routing. Since a value of 0 is valid for tos, the paramter
 *            tosCare is used to indicate if the tos field (IPv4) or traffic class (Ipv6) is used 
 *            for packet routing.
 */
typedef struct  {

  paIpAddr_t  src;       /**<  Source IP address */
  paIpAddr_t  dst;       /**<  Destination IP address */
  uint32_t    spi;       /**<  ESP or AH header Security Parameters Index */
  uint32_t    flow;      /**<  IPv6 flow label in 20 lsbs */
  int         ipType;    /**<  @ref IpValues */
  uint16_t    greProto;  /**<  GRE protocol field */
  uint8_t     proto;     /**<  IP Protocol (IPv4) / Next Header (IPv6) */
  uint8_t     tos;       /**<  IP Type of Service (IPv4) / Traffic class (IPv6) */
  uint16_t    tosCare;   /**<  TRUE if the tos value is used for matching */
  uint16_t    sctpPort;  /**<  SCTP Destination Port */
} paIpInfo_t;
  
/**
 *  @ingroup palld_api_structures
 *  @brief  MAC/Ethernet lookup information
 *
 *  @details  paEthInfo_t is used to specify the MAC/Ethernet parameters used in packet routing.
 *            A value in 0 for any of the fields indicates that the field is not used for
 *            packet routing.
 */
typedef struct  {
  paMacAddr_t        src;           /**< Source MAC addresss  */
  paMacAddr_t        dst;           /**< Destination MAC address */
  uint16_t           vlan;          /**< VLAN tag VID field, 12 lsbs  */
  uint16_t           ethertype;     /**< Ethertype field. */
  uint32_t           mplsTag;       /**< MPLS tag. Only the outer tag is examined */
  uint16_t           inport;        /**< Input EMAC port number as specified by @ref paEmacPort */
} paEthInfo_t;

/** 
 * @ingroup palld_api_structures
 * @brief SRIO Type11 header information
 *
 * @details  The structure describes the SRIO type 11 specific Lo-L2 header information.
 */
typedef struct paSrioType11Info_s
{
    uint16_t      mbox;     /**< Mail Box */
    uint16_t      letter;   /**< Letter Identifier */
} paSrioType11Info_t;

/**
 * @ingroup palld_api_structures
 * @brief SRIO Type9 header information
 *
 * @details  The structure describes the SRIO type 9 specific L0-L2 header information.
 */
typedef struct paSrioType9Info_s
{
    uint16_t        streamId;  /**< Stream identifier */
    uint16_t        cos;       /**< Class of service  */
} paSrioType9Info_t;


/**
 * @ingroup palld_api_structures
 * @brief  Srio message type specific header information
 *
 * @details  This union is used to specify the SRIO type specific header information to the module. 
 *            The type in the union is determined through other parameters passed to the module 
 *            (see @ref srioMessageTypes).
 */
typedef union  {

  paSrioType9Info_t  type9;   /**< SRIO type 9 specific information */
  paSrioType11Info_t type11;  /**< SRIO type 11 specific information */
  
} paSrioTypeInfo_t;

/**
 *  @defgroup  srioMessageTypes SRIO Message types
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name   SRIO Type Values
 *  @brief  Defines the SRIO message types.
 *
 *  @details The packet accelerator sub-system parses both SRIO Type 9 and Type 11 message headers (see @ref netlayers). 
 *           This group is used to distinguish which type of header will be used.
 */
/* @{ */
/**
 *  @def  pa_SRIO_TYPE_9
 *        SRIO Message Type 9
 */
#define  pa_SRIO_TYPE_9    9

/**
 *  @def  pa_SRIO_TYPE_11
 *        SRIO Message Type 11
 */
#define  pa_SRIO_TYPE_11   11
  
/*  @}  */  
/** @} */

/**
 *  @defgroup  srioTransportTypes SRIO Transport types
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name   SRIO Transport Type Values
 *  @brief  Defines the SRIO tansport types used.
 *
 */
/* @{ */
/**
 *  @def  pa_SRIO_TRANSPORT_TYPE_0
 *        SRIO Transport type 0: 8 bit device identifiers 
 */
#define  pa_SRIO_TRANSPORT_TYPE_0    0

/**
 *  @def  pa_SRIO_TRANSPORT_TYPE_1
 *        SRIO Transport type 1: 16 bit device identifiers
 */
#define  pa_SRIO_TRANSPORT_TYPE_1    1
  
/*  @}  */  
/** @} */

/**
 *  @defgroup paSrioInfoValidBits  PA SRIO Info Valid Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name PA SRIO Info Valid Bit Definitions
 *
 *  Bitmap definition of the validBitMap in @ref paSrioInfo_t. 
 */ 
/*@{*/
/**
 *  @def  pa_SRIO_INFO_VALID_SRC_ID
 *        - srcId is present
 */
#define pa_SRIO_INFO_VALID_SRC_ID               0x0001 
/**
 *  @def  pa_SRIO_INFO_VALID_DEST_ID
 *        - destId is present
 */
#define pa_SRIO_INFO_VALID_DEST_ID              0x0002 
/**
 *  @def  pa_SRIO_INFO_VALID_ID
 *        - Id is present
 */
#define pa_SRIO_INFO_VALID_ID                  (pa_SRIO_INFO_VALID_SRC_ID | pa_SRIO_INFO_VALID_DEST_ID)  

/**
 *  @def  pa_SRIO_INFO_VALID_CC
 *        - cc is present
 */
#define pa_SRIO_INFO_VALID_CC                   0x0004 
/**
 *  @def  pa_SRIO_INFO_VALID_PRI
 *        - pri is present
 */
#define pa_SRIO_INFO_VALID_PRI                  0x0008 
/**
 *  @def  pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID
 *        - typeInfo.type9.streamId is present
 */
#define pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID   0x0010 
/**
 *  @def  pa_SRIO_INFO_VALID_TYPE_INFO_COS
 *        - typeInfo.type9.cos is present
 */
#define pa_SRIO_INFO_VALID_TYPE_INFO_COS        0x0020
/**
 *  @def  pa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX
 *        - typeInfo.type11.mbox is present
 */
#define pa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX    0x0010 
/**
 *  @def  pa_SRIO_INFO_VALID_TYPE_INFO_LETTER
 *        - typeInfo.type11.letter is present
 */
#define pa_SRIO_INFO_VALID_TYPE_INFO_LETTER     0x0020
/**
 *  @def  pa_SRIO_INFO_VALID_TYPE_INFO
 *        - typeInfo is present
 */
#define pa_SRIO_INFO_VALID_TYPE_INFO            (pa_SRIO_INFO_VALID_TYPE_INFO_COS       |   \
                                                 pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID  |   \
                                                 pa_SRIO_INFO_VALID_TYPE_INFO_LETTER    |   \
                                                 pa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX )     
/* @} */ /* ingroup */
/*@}*/
/** @} */


/**
 *  @ingroup palld_api_structures
 *  @brief  SRIO lookup information
 *
 *  @details  srioIpInfo_t is used to specifiy the SRIO type 9 and type 11 L0-L2 parameters used in packet routing.
 *            set the corresponding valid bit at validBitMap for the parameters required for SRIO message
 *            classification.  
 *            Where tt should be provided if srcId or destId is required
 *                  msgType should be provided if typeInfo is required 
 */
typedef struct  {

  uint16_t    validBitMap;   /**< Specify which parameters are valid as defined at @ref paSrioInfoValidBits */
  uint16_t    srcId;         /**< Source ID */
  uint16_t    destId;        /**< Destination ID */
  uint16_t    tt;            /**< Transport Type: 16 bit or 8 bit identifiers as defined at @ref srioTransportTypes */  
  uint16_t    cc;            /**< Completion code */
  uint16_t    pri;           /**< 3-bit priority */
  uint16_t    msgType;       /**< Message type as defined at @ref srioMessageTypes */
  paSrioTypeInfo_t typeInfo; /**< Message Type specific parameters */
} paSrioInfo_t;


/**
 *  @ingroup palld_api_structures
 *  @brief  Packet routing configuration
 *
 *  @details paRouteInfo_t is used to specify the physical routing of packets out of the packet accelerator
 *           sub-system. Not all fields are required for all destinations. 
 *  @li      pa_DEST_DISCARD: none
 *  @li      pa_DEST_CONTINUE_PARSE_LUT1: 
 *  @li      pa_DEST_CONTINUE_PARSE_LUT2: customType, customIndex
 *  @li      pa_DEST_HOST: flowId, queue, mRoutehandle, swInfo0, cmd
 *  @li      pa_DEST_SASS: flowId, queue, swInfo0, swInfo1, cmd
 *  @li      pa_DEST_ETH: emacCtrl
 *  @li      pa_DEST_SRIO: flowId, queue, swInfo0, swInfo2, pktType
 */
typedef struct  {

  int      dest;                  /**<  Packet destination as defined at @ref pktDest */
  uint8_t  flowId;                /**<  For host, SA or SRIO destinations, specifies CPPI flow which defines free queues are used for receiving packets */
  uint16_t queue;                 /**<  For host, SA or SRIO destinations, specifies the destination queue */
  int      mRouteIndex;           /**<  For host, Multi-queue routing index (0 to (@ref pa_MAX_MULTI_ROUTE_SETS - 1)) 
                                        or @ref pa_NO_MULTI_ROUTE if multi routing not used */
  uint32_t swInfo0;               /**<  Placed in SwInfo0 for packets to host or SA; Placed in the PS Info for packets to SRIO */
  uint32_t swInfo1;               /**<  Placed in SwInfo1 for packets to the SA; Placed in the PS Info for packets to SRIO */
  int      customType;            /**<  For CONTINUE_PARSE_LUT1/LUT2 only, specifies the custom type as defined at @ref customType */
  uint8_t  customIndex;           /**<  For CONTINUE_PARSE_LUT1/LUT2 only, specifies the custom classification entry index */                                
  uint8_t  pktType_emacCtrl;      /**<  For destination SRIO, specify the 5-bit packet type toward SRIO 
                                        For destination HOST, EMAC, specify the EMAC control @ref emcOutputCtrlBits to the network */
  paCmdInfo_t *pCmd;              /**<  Pointer to the Command info to be executed prior to the packet forwarding. 
                                        NULL: no commads 
                                        @note only the following commands are supported within paRouteInfo_t 
                                              - pa_CMD_PATCH_DATA (up to two bytes only) (LUT2 only)
                                              - pa_CMD_CMDSET
                                              - pa_CMD_USR_STATS
                                              - pa_CMD_CMDSET_AND_USR_STATS 
                                   */                                                                                    

} paRouteInfo_t;

/**
 *   @def  pa_NO_MULTI_ROUTE
 *         Multi Route not enabled in this route
 */
#define pa_NO_MULTI_ROUTE   -1

/**
 *   @def  pa_MAX_MULTI_ROUTE_SETS
 *         The maximum number of multi-route sets supported
 */
#define pa_MAX_MULTI_ROUTE_SETS     32

/**
 *   @def  pa_MAX_MULTI_ROUTE_ENTRIES
 *         The maximum number of multi-route entries per muli-route set
 */
#define pa_MAX_MULTI_ROUTE_ENTRIES   8
 
/**
 *  @defgroup mrEntryCtrlInfo   Multiroute Entry Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Multiroute Entry Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paMultiRouteEntry_t. 
 */ 
/*@{*/
/**
 *  @def  pa_MULTI_ROUTE_DESCRIPTOR_ONLY
 *        Control Info -- Set: Send descriptor without packet to the destination
 *                        Clear: Send both descriptor and the packet to the destination
 *  
 */
#define pa_MULTI_ROUTE_DESCRIPTOR_ONLY            0x01 
/*@{*/
/**
 *  @def  pa_MULTI_ROUTE_REPLACE_SWINFO
 *        Control Info -- Set: Replace the swInfo0 with the value provided here
 *                        Clear: Keep the original swInfo0 
 *  
 */
#define pa_MULTI_ROUTE_REPLACE_SWINFO             0x02 
/*@}*/
/** @} */
 
/**
 *  @ingroup palld_api_structures
 *  @brief  Packet Multi-route entry configuration
 *
 *  @details paMultiRouteEntry_t is used to specify the physical routing of packets per multi-route entry.
 *           It is only a subset of the Routing information defined at @ref paRouteInfo_t because those common 
 *           parameters such as swInfo0, swInfo1 must be already present in the packet descriptor.
 *           There is no restriction of the destination as long as it is accessible through PKTDMA queue. 
 */
typedef struct  {

  uint8_t   ctrlBitfield;    /**< Multi-Routing control information as defined at @ref mrEntryCtrlInfo */
  uint8_t   flowId;          /**< For host, specifies the CPPI flow which defines the free queues are used for receiving packets */
  uint16_t  queue;           /**< For host, specifies the destination queue */
  uint32_t  swInfo0;         /**< Placed in SwInfo0 for packets to host */

} paMultiRouteEntry_t;

/**
 *  @defgroup paMultiRouteModes Multi-route group configuration mode
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Multi-route group configuration mode
 *
 *  Definition of Multi-route group configuration mode supported in PA sub-system
 */ 
/** @ingroup paMultiRouteModes */
/*@{*/
typedef enum {
  pa_MULTI_ROUTE_MODE_CONFIG = 0, /**< Add or reconfigure the multi-route group */
  pa_MULTI_ROUTE_MODE_RESET       /**< Delete the multi-route group */
} paMultiRouteModes_e;
/*@}*/
/** @} */

/**
 *  @defgroup paCrcSizes PA CRC Sizes
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name CRC Sizes
 *
 *  Definition of CRC sizes supported in PA sub-system
 */ 
/** @ingroup paCrcSizes */
/*@{*/
typedef enum {
  pa_CRC_SIZE_8 = 0,        /**< 8-bit CRC */
  pa_CRC_SIZE_16,           /**< 16-bit CRC */
  pa_CRC_SIZE_24,           /**< 24-bit CRC */
  pa_CRC_SIZE_32            /**< 32-bit CRC */
} paCrcSizes_e;
/*@}*/
/** @} */

/**
 *  @defgroup crcConfigCtrlInfo  CRC Engine Configuration Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name CRC Engine Configuration Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paCrcConfig_t. 
 */ 
/*@{*/
/**
 *  @def  pa_CRC_CONFIG_RIGHT_SHIFT
 *        Control Info -- Set: Right shift CRC (b0 to b7)
 *                        Clear: Left shift CRC (b7 to b0)
 */
#define pa_CRC_CONFIG_RIGHT_SHIFT           0x0001 
/**
 *  @def  pa_CRC_CONFIG_INVERSE_RESULT
 *        Control Info -- Set: a 'NOT' operation is applied to the final CRC result
 */
#define pa_CRC_CONFIG_INVERSE_RESULT        0x0002 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  CRC Engine configuration
 *
 *  @details paCrcConfig_t is used to configure the CRC engines within the PA sub-system.
 *           There is one CRC engine connected to each PDSP in the PA sub-system. It will 
 *           be used to perform CRC operation required by some network protocol such as
 *           SCTP and/or the user-specified CRC command. It only supports one type of CRC
 *           per configuration.
 *
 *  @note Only one type of CRC calcualtion is supported by one CRC engine per configuration.
 *        It is the responsibility of the module user to configure the specific CRC engine by 
 *        calling @ref Pa_configCrcEngine.
 */
 
typedef struct {

  uint16_t      ctrlBitfield;   /**< CRC configuration control information as defined at @ref crcConfigCtrlInfo */
  paCrcSizes_e  size;           /**< CRC sizes as defined at @ref paCrcSizes_e */
  uint32_t      polynomial;     /**< Specify the CRC polynomial in the format of 0xabcdefgh. For example,
                                     x32+x28+x27+x26+x25+x23+x22+x20+x19+x18+x14+x13+x11+x10+x9+x8+x6+1 
                                     ==> 0x1EDC6F41
                                     x16+x15+x2+1 ==>0x80050000 */
  uint32_t      initValue;      /**< CRC initial value */   
} paCrcConfig_t;


/**
 *  @defgroup timestampScalerFactor Timestamp Scaler Factor
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name Timestamp Scaler Factor
 *
 *  Definition of PA timestamp scaler factor supported in PA sub-system
 *
 *  @note  pa_TIMESTAMP_SCALER_FACTOR_1 is not supported. It is defined here
 *         for reference purpose.
 */ 
/** @ingroup timestampScalerFactor */
/*@{*/
typedef enum {
  pa_TIMESTAMP_SCALER_FACTOR_1 = -1,       
  pa_TIMESTAMP_SCALER_FACTOR_2 = 0,       
  pa_TIMESTAMP_SCALER_FACTOR_4,       
  pa_TIMESTAMP_SCALER_FACTOR_8,       
  pa_TIMESTAMP_SCALER_FACTOR_16,       
  pa_TIMESTAMP_SCALER_FACTOR_32,       
  pa_TIMESTAMP_SCALER_FACTOR_64,       
  pa_TIMESTAMP_SCALER_FACTOR_128,       
  pa_TIMESTAMP_SCALER_FACTOR_256,       
  pa_TIMESTAMP_SCALER_FACTOR_512,       
  pa_TIMESTAMP_SCALER_FACTOR_1024,       
  pa_TIMESTAMP_SCALER_FACTOR_2048,       
  pa_TIMESTAMP_SCALER_FACTOR_4096,       
  pa_TIMESTAMP_SCALER_FACTOR_8192       
} paTimestampScalerFactor_e;
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  Timestamp configuration
 *
 *  @details paTimestampConfig_t is used to configure the timer which is used to generate timestamp in
 *           the PA sub-system.
 *  @verbatim 
             The 16-bit timer connected to PDSP0 is reserved for timestamp generation.
             The timestamp will be 0 until the timer is enabled.
             The timestamp unit is equal to (the scaler factor)/350 us.  
    @endverbatim
 *
 *  @note: The PDSP timer does not support pa_TIMESTAMP_SCALER_FACTOR_1.
 *         The timer will be disabled if unspported scaler factor is used.
 */
 
typedef struct {
  uint16_t                   enable;     /**< Enable/Disable(1/0) the timestamp generation */
  paTimestampScalerFactor_e  factor;     /**< Timestamp scaler factor as defined at @ref timestampScalerFactor */
} paTimestampConfig_t;

/**
 *  @defgroup paUsrStatsTypes PA User-defined Ststaistics Counter Types
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name User-defined Ststaistics Counter Types
 *
 *  Definition of Counter types of the User-defined Statistics
 */ 
/** @ingroup paUsrStatsTypes */
/*@{*/
typedef enum {
  pa_USR_STATS_TYPE_PACKET = 0,   /**< Packet Counter */
  pa_USR_STATS_TYPE_BYTE          /**< Byte Counter */
} paUsrStatsTypes_e;
/*@}*/
/** @} */


/**
 *  @ingroup palld_api_structures
 *  @brief  User-defined statistics counter entry configuration information
 *
 *  @details paUsrStatsCounterEntryConfig_t defines the operation parameters of each user-defined statistics.
 */
 
typedef struct {
  uint16_t          cntIndex; /**< Index of the counter */
  uint16_t          cntLnk;   /**< Index of the next level counter. 0xFFFF: No linking counter */ 
  paUsrStatsTypes_e cntType;  /**< Counter type (packet counter */
} paUsrStatsCounterEntryConfig_t;

/**
 *   @def  pa_USR_STATS_LNK_END
 *         Indicate that there is no next layer counter
 */
#define pa_USR_STATS_LNK_END                0xFFFF


/**
 *  @defgroup usrStatsCounterConfigCtrlInfo  User-defined Statistics Counter Configuration Control Info Bit Definitions
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name User-defined Statistics Counter Configuration Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitField in @ref paUsrStatsCounterConfig_t 
 */ 
/*@{*/
/**
 *  @def  pa_USR_STATS_CONFIG_RESET
 *        Control Info -- Set: Reset all counter control blocks to its default setting (packet counter without link to the next layer)
 */
#define pa_USR_STATS_CONFIG_RESET           0x0001 
/*@}*/
/** @} */

/**
 *  @ingroup palld_api_structures
 *  @brief  User-defined statistics counter configuration information
 *
 *  @details paUsrStatsCounterConfig_t contains an array of the entry configuration information.
 */
 
typedef struct {
  uint16_t          ctrlBitfield;      /**< User-defined statistics counter configuration control information as defined at @ref usrStatsCounterConfigCtrlInfo */
  uint16_t          numCnt;            /**< Number of counters to be configured */
  paUsrStatsCounterEntryConfig_t* cntInfo;  /**< Array of counter configuration as specified at @ref paUsrStatsCounterEntryConfig_t */
} paUsrStatsCounterConfig_t;

/**
 *  @ingroup palld_api_structures
 *  @brief  User-defined statistics configuration information
 *
 *  @details paUsrStatsConfigInfo_t is used to perform user-defined statistics related configuration. It is used by 
 *           API function @ref Pa_configUsrStats.
 */
 
typedef struct {
  paUsrStatsCounterConfig_t* pCntCfg;  /**< Pointer to the user-defined statistics counter configuration. */
} paUsrStatsConfigInfo_t;

/**
 * @defgroup  paSubSysStates PA Sub-system Queries and States
 * @ingroup palld_api_constants
 * @{
 *
 * @name PA Sub-system Queries and States
 *
 *  PA Sub-system reset state and query arguments used by API function @ref Pa_resetControl
 */
/* @{ */
/**
 *  @def  pa_STATE_RESET  
 *        The Sub-system is in reset
 */
#define pa_STATE_RESET            0  /**< Sub-system state reset */

/**
 *  @def  pa_STATE_ENABLE
 *        The Sub-system state is enabled
 */
#define pa_STATE_ENABLE           1  /**< Sub-system state enable  */

/**
 *  @def  pa_STATE_QUERY
 *        Query the Sub-system state
 */
#define pa_STATE_QUERY            2  /**< Query the Sub-system state */

/**
 *  @def  pa_STATE_INCONSISTENT
 *        The Sub-system state is partially enabled
 */
#define pa_STATE_INCONSISTENT     3  /**< Sub-system is partially enabled */

/**
 *  @def  pa_STATE_INVALID_REQUEST
 *        Invalid state command to the Sub-system
 */
#define pa_STATE_INVALID_REQUEST  4  /**< Invalid state command to the Sub-system */

/**
 *  @def  pa_STATE_ENABLE_FAILED
 *        The Sub-system did not respond after restart
 */
#define pa_STATE_ENABLE_FAILED    5  /**<  The Sub-system did not respond after restart */

/* @}  */ 
/** @} */


/**
 *  @ingroup palld_api_structures
 *  @brief  paSState_t defines the operating state of the packet accelerator sub-system
 *
 *  @details  The values in @ref paSubSysStates are used both to set the state of the packet accelerator
 *            sub-system (pa_STATE_RESET and pa_STATE_ENABLE) as well as show the current state
 *            of the system (all values).
 */
typedef int paSSstate_t;

/**
 * @ingroup palld_api_structures
 * @brief PA Classify1 Statistics Structure
 *
 * @details This structures define the PA Classify1-specific statistics provided 
 *          with API function @ref Pa_formatStatsReply ().
 */
typedef struct paClassify1Stats_s {

  uint32_t nPackets;                /**< Number of packets entering Classify1 PDSPs */
  uint32_t nIpv4Packets;            /**< Number of IPv4 packets */
  uint32_t nIpv6Packets;            /**< Number of IPv6 packets */
  uint32_t nCustomPackets;          /**< Number of custom LUT1 packets */
  uint32_t nSrioPackets;            /**< Number of SRIO packets */
  uint32_t nLlcSnapFail;            /**< Number of packets with corrupt LLC Snap */
  uint32_t nTableMatch;             /**< Number of packets with table match found */
  uint32_t nNoTableMatch;           /**< Number of packets without table match found */
  uint32_t nIpFrag;                 /**< Number of Ingress fragmented IP packets */
  uint32_t nIpDepthOverflow;        /**< Number of packets with too many IP layers */
  uint32_t nVlanDepthOverflow;      /**< Number of packets with too many VLANs */
  uint32_t nGreDepthOverflow;       /**< Number of packets with too many GREs */
  uint32_t nMplsPackets;            /**< Number of MPLS packets */
  uint32_t nParseFail;              /**< Number of packets which can not be parsed */
  uint32_t nInvalidIPv6Opt;         /**< Number of IPv6 packets which contains invalid IPv6 options */
  uint32_t nTxIpFrag;               /**< Number of Egress fragmented IP packets */
  uint32_t reserved1;               /**< Reserved for future use */
  uint32_t nSilentDiscard;          /**< Number of packets dropped */
  uint32_t nInvalidControl;         /**< Number of packet received with invalid control information */
  uint32_t nInvalidState;           /**< Number of times the PA detected an illegal state and recovered */
  uint32_t nSystemFail;             /**< Number of times the PA detected an unrecoverable state and restarted */
  
} paClassify1Stats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA Classify2 Statistics Structure
 *
 * @details This structures define the PA Classify2-specific statistics provided 
 *          with API function @ref Pa_formatStatsReply ().
 */
typedef struct paClassify2Stats_s  {
  
  uint32_t nPackets;                /**< Number of packets entering Classify2 PDSP */ 
  uint32_t reserved2;               /**< Reserved for future use */
  uint32_t nUdp;                    /**< Number of UDP packets */
  uint32_t nTcp;                    /**< Number of TCP packets */
  uint32_t nCustom;                 /**< Number of custom LUT2 packets */
  uint32_t reserved3;               /**< Reserved for future use */
  uint32_t reserved4;               /**< Reserved for future use */
  uint32_t nSilentDiscard;          /**< Number of packets dropped */
  uint32_t nInvalidControl;         /**< Number of packet received with invalid control information */

} paClassify2Stats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA Modifier Statistics Structure
 *
 * @details This structures define the PA Modifier-specific statistics provided 
 *          with API function @ref Pa_formatStatsReply ().
 */
typedef struct paModifyStats_s   {
  uint32_t nCommandFail;            /**< Number of invalid commands */
  
} paModifyStats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA Common Statistics Structure
 *
 * @details This structures define the PA Common statistics provided 
 *          with API function @ref Pa_formatStatsReply ().
 */
typedef struct paCommonStats_s  {

  uint32_t reserved5;               /**< Reserved for future use */
  
} paCommonStats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA System Statistics Structure
 *
 * @details This structures define the PA System statistics provided 
 *          with API function @ref Pa_formatStatsReply ().
 */

typedef struct paSysStats_s  {

  paClassify1Stats_t classify1;     /**< Classify1-specific statistics */
  paClassify2Stats_t classify2;     /**< Classify2-specific statistics */
  paModifyStats_t    modify;        /**< Modifier-specific statistics */
  paCommonStats_t    common;        /**< Common statistics */
  
} paSysStats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA User-defined Statistics Structure
 *
 * @details This structures define the PA User-defined statistics provided 
 *          with API function @ref Pa_requestUsrStats ().
 */

typedef struct paUsrStats_s  {

  uint64_t   count64[pa_USR_STATS_MAX_64B_COUNTERS];     /**< Array of general purpose 64-bit counters */
  uint32_t   count32[pa_USR_STATS_MAX_32B_COUNTERS];     /**< Array of general purpose 32-bit counters */
  
} paUsrStats_t;

/**
 * @ingroup palld_api_structures
 * @brief PA Timestamp Structure
 *
 * This structure defines the 48-bit timestamp provided upon request with @ref Pa_getTimestamp ().
 */
typedef struct {
  uint32_t   hi;         /**< Upper 32 bits of the 48-bit PASS timestamp */
  uint16_t   lo;         /**< Lower 16 bits of the 48-bit PASS timestamp */
} paTimestamp_t;

/**
 *  @ingroup palld_api_functions
 *  @brief   Pa_addSrio adds a SRIO entry to the L2 table
 *
 *  @details This function is used to add or replace an entry into the L2 table (see @ref netlayers).
 *           A new entry is added if the SRIO configuration info is unique in the modules handle table. 
 *           If the value is not unique then the routing information for the existing entry is changed to
 *           the values provided in the function.
 *
 *           On return the command buffer (cmd) contains a formatted command for the sub-system. The 
 *           destination for the command is provided in cmdDest. The module user must send the formatted
 *           command to the sub-system. The sub-system will generate a reply
 *           and this reply must be sent back to this module through the API @ref Pa_forwardResult.
 *
 *           This command as well as @ref Pa_addIp operate with a strong dependence on entry order.
 *           See section table @ref order for a description on the operation of the sub-system and
 *           table entry ordering.
 *
 *  @param[in]      iHandle         The driver instance handle
 *  @param[in]      index           Specify the index of the LUT1 entry (0-63). Set to pa_LUT1_INDEX_NOT_SPECIFIED if not specified
 *  @param[in]      srioInfo        Value @ref paSrioInfo_t
 *  @param[in]      nextHdr         The next header type to be parsed following the SRIO classification
 *                                  Refer to @ref NextHeaderTypes for all supported protocols
 *                                  Set to pa_HDR_TYPE_UNKNOWN if no further prasing is required
 *  @param[in]      nextHdrOffset   Offset to the next header from the beginning of the packet
 *  @param[in]      routeInfo       Match packet routing information
 *  @param[in]      nextRtFail      Routing information for subsequent match failures
 *  @param[out]     handle          Pointer to L2 Handle
 *  @param[out]     cmd             Where the created command is placed
 *  @param[in,out]  cmdSize         Input the size of cmd buffer, on output the actual size used. @ref cmdMinBufSize
 *  @param[in]      reply           Where the sub-system sends the command reply
 *  @param[out]     cmdDest         Value (@ref cmdTxDest) 
 *  @retval                         Value (@ref ReturnValues)
 *  @pre                            A driver instance must be created and tables initialized
 *
 *  @note No table entry validation will be proformed if the LUT1 index is specified at this function
 *
 */

paReturn_t Pa_addSrio (  Pa_Handle         iHandle,
                         int               index,
                         paSrioInfo_t      *srioInfo,
                         uint16_t          nextHdr,
                         uint16_t          nextHdrOffset,
                         paRouteInfo_t     *routeInfo,
                         paRouteInfo_t     *nextRtFail,
                         paHandleL2L3_t    *handle,
                         paCmd_t           cmd,
                         uint16_t          *cmdSize,
                         paCmdReply_t      *reply,
                         int               *cmdDest);

/**
 *  @ingroup palld_api_functions
 *  @brief   Pa_addMac adds a mac address to the L2 table
 *
 *  @details This function is used to add or replace an entry into the L2 table (see @ref netlayers).
 *           A new entry is added if the MAC configuration info is unique in the modules handle table. If
 *           the value is not unique then the routing information for the existing entry is changed to
 *           the values provided in the function.
 *
 *           L2 values that are not to be used for packet routing are set to 0.
 *
 *           On return the command buffer (cmd) contains a formatted command for the sub-system. The 
 *           destination for the command is provided in cmdDest. The module user must send the formatted
 *           command to the sub-system. The sub-system will generate a reply
 *           and this reply must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *           This command as well as @ref Pa_addIp operate with a strong dependence on entry order.
 *           See section table @ref order for a description on the operation of the sub-system and
 *           table entry ordering.
 *
 *
 *  @param[in]      iHandle     The driver instance handle
 *  @param[in]      index       Specify the index of the LUT1 entry (0-63). Set to pa_LUT1_INDEX_NOT_SPECIFIED if not specified
 *  @param[in]      ethInfo     Value @ref paEthInfo_t
 *  @param[in]      routeInfo   Match packet routing information
 *  @param[in]      nextRtFail  Routing information for subsequent match failures
 *  @param[out]     handle      Pointer to L2 Handle
 *  @param[out]     cmd         Where the created command is placed
 *  @param[in,out]  cmdSize     Input the size of cmd buffer, on output the actual size used. @ref cmdMinBufSize
 *  @param[in]      reply       Where the sub-system sends the command reply
 *  @param[out]     cmdDest     Value (@ref cmdTxDest) 
 *  @retval                     Value (@ref ReturnValues)
 *  @pre                        A driver instance must be created and tables initialized
 *
 *  @note No table entry validation will be proformed if the LUT1 index is specified at this function
 *
 */

paReturn_t Pa_addMac  (  Pa_Handle         iHandle,
                         int               index,
                         paEthInfo_t       *ethInfo,
                         paRouteInfo_t     *routeInfo,
                         paRouteInfo_t     *nextRtFail,
                         paHandleL2L3_t    *handle,
                         paCmd_t           cmd,
                         uint16_t          *cmdSize,
                         paCmdReply_t      *reply,
                         int               *cmdDest);
                         
                         
/**
 *   @ingroup palld_api_functions
 *   @brief  Pa_delHandle deletes a MAC/SRIO or IP handle
 *
 *   @details  This function is used to remove an entry from the sub-system L2 or L3 (LUT1) lookup (see @ref netlayers).
 *             When a handle is deleted it can create stale handles. For example, an L3 handle can reference
 *             an L2 handle, and an L4 handle can reference an L3 handle. The module does not check for 
 *             references to a stale handle, the module user is responsible for maintaining reference coherency.
 *             It is recommended that the handle should not be deleted if the API function @ref Pa_getHandleRefCount 
 *             returns non-zero reference count.
 *
 *   @param[in]     iHandle     The driver instance handle
 *   @param[in]     handle      Pointer to the l2/l3 handle to delete
 *   @param[out]    cmd         Where the created command is placed
 *   @param[in]     cmdSize     The size of the cmd buffer
 *   @param[in]     reply       Where the sub-system sends the command reply
 *   @param[out]    cmdDest     Value (@ref cmdTxDest)
 *   @retval                    Value (@ref ReturnValues)
 *   @pre                       A driver instance must be created and tables initialized
 */
paReturn_t Pa_delHandle (Pa_Handle       iHandle,
                         paHandleL2L3_t  *handle, 
                         paCmd_t         cmd,
                         uint16_t        *cmdSize,
                         paCmdReply_t    *reply,
                         int             *cmdDest );

/**
 *   @ingroup palld_api_functions
 *   @brief  Pa_delL4Handle deletes a UDP/TCP/GTPU/CustomLUT2 handle
 *
 *   @details This function is used to remove an entry from the sub-system L4 (LUT2) handle entry. 
 *
 *   @param[in]      iHandle   The driver instance handle
 *   @param[in, out] handle    Pointer to the L4 handle to delete
 *   @param[out]     cmd       Where the created command is placed
 *   @param[in]      cmdSize   The size of the cmd buffer
 *   @param[in]      reply     Where the sub-system sends the reply
 *   @param[out]     cmdDest   Value (@ref cmdTxDest)
 *   @retval                   Value (@ref ReturnValues)
 *   @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_delL4Handle (Pa_Handle      iHandle,
                          paHandleL4_t    handle, 
                          paCmd_t         cmd,
                          uint16_t        *cmdSize,
                          paCmdReply_t    *reply,
                          int             *cmdDest );


/**
 *   @ingroup palld_api_functions
 *   @brief  Pa_addIp adds an IP address to the L3 table
 *
 *   @details  This function is used to add or replace an entry in the L3 table (see @ref netlayers).
 *             A new entry is added if the IP configuration info is unique in the modules handle table.
 *             If the value is not unique then the routing information for the existing entry is changed
 *             to the values provided in the function.
 *
 *             The LLD will determine where this entry is added based on following rules
 *             - If there is no previous link or the previous link is a L2 (MAC/SRIO) entry, this entry will be
 *               added into LUT1_1
 *             - If the previous link is L3 (IP/Custom LUT1), this entry will be added into LUT1_2
 *             
 *             The module user can overwrite the default rules by specifying the desired LUT1 instance. 
 *
 *             The PASS will determine which entry of the specified LUT1 table is used for this entry based on
 *             its internal algorithm if the module user does not specify the LUT1 index.  
 *
 *             L3 values that are used for packet routing should be set as described in @ref paIpInfo_t.
 *
 *             The @ref paHandleL2L3_t prevLink is used to link this entry to an L2 or L3 entry already made
 *             by a call to @ref Pa_addMac or Pa_addIp. If the link is enabled then a packet will match the IP 
 *             information provided in ipInfo only if the same packet has already matched at the L2 level as  
 *             described by prevLink. To disable linking the value of prevLink is set to NULL.
 *
 *             On return the command buffer (cmd) contains a formatted command for the sub-system. The
 *             destination for the command is provided in cmdDest. The module user must send the formatted
 *             command to the sub-system. The sub-system will generate a reply and this reply must be
 *             sent back to this module through the API @ref Pa_forwardResult.
 *
 *             This command as well as @ref Pa_addMac operates with a strong dependence on entry order.
 *             See section table @ref order for a description on the operation of the sub-system and
 *             table entry ordering.
 *
 *
 *
 *   @param[in]    iHandle     The driver instance handle
 *   @param[in]    lutInst     Specify which LUT1 (0-2) should be used.  Set to pa_LUT_INST_NOT_SPECIFIED if not specified
 *   @param[in]    index       Specify the index of the LUT1 entry (0-63). Set to pa_LUT1_INDEX_NOT_SPECIFIED if not specified
 *   @param[in]    ipInfo      Value @ref paIpInfo_t
 *   @param[in]    prevLink    An optional L2 or L3 handle
 *   @param[in]    routeInfo   Where to send a packet that matches
 *   @param[in]    nextRtFail  Where to send a packet that matches, but later fails
 *   @param[out]   retHandle   Pointer to the returned L3 handle
 *   @param[out]   cmd         Buffer where the PASS command is created
 *   @param[in]    cmdSize     The size of the cmd buffer
 *   @param[in]    reply       Where the response to the PASS command is routed
 *   @param[out]   cmdDest     Value (@ref cmdTxDest)
 *   @retval                   Value (@ref ReturnValues)
 *   @pre                      A driver instance must be created and tables initialized
 *
 *  @note No table entry validation will be proformed if the LUT1 index is specified at this function
 *
 */
paReturn_t  Pa_addIp  ( Pa_Handle          iHandle,
                        int                lutInst,
                        int                index, 
                        paIpInfo_t        *ipInfo,
                        paHandleL2L3_t     prevLink,
                        paRouteInfo_t     *routeInfo,
                        paRouteInfo_t     *nextRtFail,
                        paHandleL2L3_t    *retHandle,
                        paCmd_t            cmd,
                        uint16_t          *cmdSize,
                        paCmdReply_t      *reply,
                        int               *cmdDest );
                        
                        
/**
 *  @defgroup  paLut2PortSize LUT2 Port Size Values
 *  @ingroup palld_api_constants
 *  @{
 *
 *  @name   LUT2 Port Size Values
 *  @brief  Defines the LUT2 port size supported by PA.
 *
 *  @details The PA LUT2 supports both 16-bit and 32-bit entry matching. It can be used to classify 
 *           based on the UDP/IP 16-bit destination port with or without upper layer link or the GTP-U
 *           32-bit Tunnel ID. No other Layer 4 or Layer 5 protocol is supported. 
 */
/* @{ */
/**
 *  @def  pa_LUT2_PORT_SIZE_16
 *        16-bit port number such as UDP/TCP port
 *        
 */
#define  pa_LUT2_PORT_SIZE_16    0

/**
 *  @def  pa_LUT2_PORT_SIZE_32
 *        32-bit port number such as GTP-U Tunnel ID
 */
#define  pa_LUT2_PORT_SIZE_32    1
  
/*  @}  */  
/** @} */
                        
                        
/**
 *   @ingroup palld_api_functions
 *   @brief  Pa_addPort adds a destination port to the L4 (LUT2) table
 *
 *   @details This function is used to add an entry to the L4 (LUT2) table (see @ref netlayers). Only the
 *            destination port can be set, along with an optional link to a previous L3 handle 
 *            (see @ref Pa_addIp) through linkHandle.
 *
 *            This module does not keep track of the L4 handles, so calling the function
 *            a second time with the same destination port and link handle will simply replace the
 *            previous entry. It is recommended to set the replace flag to indicate that this entry is
 *            already at the LUT2 table. This feature may be used to change the routing information without 
 *            deleting and adding the matching port. 
 *            This API also initiates the atomic queue diversion operation, which means that the QMSS moves 
 *            the entries in the diverted queue to the destination queue, if the divertQ is specified and 
 *            fReplace flag is set. In this case, the PASS will complete the LUT2 update, wait for the queue 
 *            diversion to be complete and then resume processing incoming packets.
 *            Unlike entries in the L2 and L3 table, the order of entry is not important.  
 *
 *            The type of transport header (TCP/UDP) is not specified here. If the type of transport
 *            is part of the packet routing criteria it is specified in the protocol type field
 *            in @ref paIpInfo_t in the call to @ref Pa_addIp.
 *
 *            This function supports both 16-bit and 32-bit port specified by the parameter portSize.
 *            However, there are the following restrictions for 32-bit ports
 *  @verbatim 
               1. The optional link to the previous LUT1 match can not be used so that the destID 
                  should be unique regressless of the previous L3 adddreses
               2. The 32-bit LUT2 lookup can not be mixed with the other TCP/UDP or custom LUT2 lookup. 
    @endverbatim
 *   
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *   @param[in]     iHandle     The driver instance handle
 *   @param[in]     portSize    The input port size (@ref paLut2PortSize)
 *   @param[in]     destPort    The destination TCP/UDP port
 *   @param[in]     linkHandle  An L3 handle that is linked to the destination port
 *   @param[in]     fReplace    Flag to indicate whether the entry exists
 *   @param[in]     divertQ     The source queue for atomic queue diversion with LUT2 update
 *                              Set to pa_PARAMS_NOT_SPECIFIED if not specified
 *   @param[in]     routeInfo   Where to send a packet that matches
 *   @param[out]    retHandle   A blank handle where the return handle is placed
 *   @param[out]    cmd         Buffer where the PASS command is created
 *   @param[in]     cmdSize     The size of the cmd buffer
 *   @param[out]    reply       Where the response to the PASS command is routed
 *   @param[out]    cmdDest     Value (@ref cmdTxDest)
 *   @retval                    Value (@ref ReturnValues)
 *   @pre                       A driver instance must be created and tables initialized
 *
 */
paReturn_t  Pa_addPort ( Pa_Handle       iHandle,
                         int             portSize,
                         uint32_t        destPort,
                         paHandleL2L3_t  linkHandle, 
                         uint16_t        fReplace,
                         uint16_t        divertQ,
                         paRouteInfo_t  *routeInfo,
                         paHandleL4_t    retHandle,
                         paCmd_t         cmd,
                         uint16_t       *cmdSize,
                         paCmdReply_t   *reply,
                         int            *cmdDest );
                         
                         
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_setCustomLUT1 performs the global configuration for level 3 (LUT1) custom lookups
 *
 *  @details  This command is typically issued once per system and is used to configure the
 *            PA for performing network layer 3 (LUT1) custom lookups. 
 *            It specifies the offset and byte masks which the PA
 *            subsystem uses for parsing a packet that has entered custom LUT1 
 *            classification directed from the previous match route.
 *            It also specifies the next header type and offset to be used for continuous 
 *            parsing
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *  @param[in]    iHandle          The driver instance handle
 *  @param[in]    custIndex        The level 3 (LUT1) custom index
 *  @param[in]    parseByteOffset  Where the PA begins custom match (relative to the L3 start)
 *  @param[in]    nextHdr          The next header type to be parsed following the custom header
 *                                 Refer to @ref NextHeaderTypes for all supported protocols
 *                                 Set to pa_HDR_TYPE_UNKNOWN if no further prasing is required
 *  @param[in]    nextHdrOffset    Offset to the next header from the beginning of the custom header
 *  @param[in]    byteMasks        The bitmap of bits in the parse that matter
 *  @param[out]   cmd              Buffer where the PASS command is created
 *  @param[in]    cmdSize          On entry the size of the cmd buffer, on exit the size of the command
 *  @param[in]    reply            Where the response to the PASS command is routed
 *  @param[out]   cmdDest          Value (@ref cmdTxDest)
 *  @retval                        Value (@ref ReturnValues)
 *  @pre                           A driver instance must be created and tables initialized
 * 
 *  @note There is up to @ref pa_MAX_CUSTOM_TYPES_LUT1 LUT1 custom types supported by PASS.
 */
paReturn_t Pa_setCustomLUT1 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              uint16_t        parseByteOffset,
                              uint16_t        nextHdr,
                              uint16_t        nextHdrOffset,
                              uint8_t         byteMasks[pa_NUM_BYTES_CUSTOM_LUT1],
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest );
                            
/**
 *  @ingroup palld_api_functions
 *  @brief Pa_AddCustomLUT1 adds a custom lookup entry to the lookup tables (LUT1).
 *
 *  @details  This command is called to add a specific match entry to the L3 (LUT1) lookup table. This
 *            function is called once per desired custom LUT1 match criteria.
 *
 *            The LLD will determine where this entry is added based on following rules
 *            - If there is no previous link or the previous link is a L2 (MAC/SRIO) entry, this entry will be
 *              added into LUT1_1
 *            - If the previous link is L3 (IP/Custom LUT1), this entry will be added into LUT1_2
 *            
 *            The module user can overwrite the default rules by specifying the desired LUT1 instance. 
 *
 *            The PASS will determine which entry of the specified LUT1 table is used for this entry based on
 *            its internal algorithm if the module user does not specify the LUT1 index.  
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *  @param[in]   iHandle          The driver instance handle
 *  @param[in]   custIndex        The level 3 (LUT1) custom index
 *  @param[in]   lutInst          Specify which LUT1 (0-2) should be used.  Set to pa_LUT_INST_NOT_SPECIFIED if not specified
 *  @param[in]   index            Specify the index of the LUT1 entry (0-63). Set to pa_LUT1_INDEX_NOT_SPECIFIED if not specified
 *  @param[in]   match            The byte values that describe the match entry
 *  @param[in]   prevLink         An optional L2 or L3 handle that links to this lookup
 *  @param[in]   routeInfo        Where to send a packet that matches
 *  @param[in]   nextRtFail       Where to send a packet that matches here, but fails next parse level
 *  @param[out]  retHandle        The returned L3 handle
 *  @param[out]  cmd              Buffer where the command is created
 *  @param[in]   cmdSize          On entry the size of the cmd buffer, on exit the size of the command
 *  @param[in]   reply            Where the response to the PASS command is routed
 *  @param[out]  cmdDest          Value (@ref cmdTxDest)
 *  @retval                       Value (@ref ReturnValues)
 *  @pre                          A driver instance must be created and tables initialized
 */
paReturn_t Pa_addCustomLUT1 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              int             lutInst,
                              int             index, 
                              uint8_t         match[pa_NUM_BYTES_CUSTOM_LUT1],
                              paHandleL2L3_t  prevLink,
                              paRouteInfo_t  *routeInfo,
                              paRouteInfo_t  *nextRtFail,
                              paHandleL2L3_t *retHandle,
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest );
                            
/**
 *  @ingroup palld_api_functions
 *  @brief Pa_setCustomLUT2 performs the global configuration for level 4 (LUT2) custom lookups
 *
 *  @details  This command is typically called once per system and is used to configure the
 *            PA for performing network layer 4 (LUT2) custom lookups. 
 *            If handleLink is true then only 3 bytes and 3 offsets are available
 *            for matching. The fourth one is used to store the previous match information.
 *            In this case the first 3 values in the byteOffsets and byteMasks arrays are
 *            valid.
 *
 *            If setMask is non-zero, it will be ORed with the first byteMask and the match byte.
 *            It is used to distinguish this LUT2 custom lookupo entry from other lUT2
 *            custom lookup and the standard lookup entries.
 *            
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the API @ref Pa_forwardResult.
 *
 *  @param[in]  iHandle         The driver instance handle
 *  @param[in]  custIndex       The level 4 (LUT2) custom index
 *  @param[in]  handleLink      Set to TRUE to use one byte of the match to hold previous match info
 *  @param[in]  byteOffsets     The offsets to the bytes to use in custom matching
 *  @param[in]  byteMasks       The bits that are valid in the custom matching
 *  @param[in]  setMask         The bits to be set at the first match byte
 *  @param[out] cmd             Buffer where the command is created
 *  @param[in]  cmdSize         On entry the size of the cmd buffer, on exit the size of the command
 *  @param[in]  reply           Where the response to the PASS command is routed
 *  @param[out] cmdDest         Value (@ref cmdTxDest)
 *  @retval                     Value (@ref ReturnValues)
 *  @pre                        A driver instance must be created and tables initialized
 *
 *  @note There is up to @ref pa_MAX_CUSTOM_TYPES_LUT2 LUT2 custom types supported by PASS.
 */
paReturn_t Pa_setCustomLUT2 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              uint16_t        handleLink,
                              uint16_t        byteOffsets[pa_NUM_BYTES_CUSTOM_LUT2],
                              uint8_t         byteMasks[pa_NUM_BYTES_CUSTOM_LUT2],
                              uint8_t         setMask,
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest );
                            
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_addCustomLUT2 adds a custom lookup to the LUT2 lookup tables
 * 
 *  @details  This command is called to add a specific entry to the L4 (LUT2) lookup table. This
 *            function is called once per desired custom LUT2 match criteria.
 *            This API also initiates the atomic queue diversion operation, which means that the QMSS moves 
 *            the entries in the diverted queue to the destination queue, if the divertQ is specified and 
 *            fReplace flag is set. In this case, the PASS will complete the LUT2 update, wait for the queue 
 *            diversion to be complete and then resume processing incoming packets.
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *  @param[in]  iHandle      The driver instance handle
 *  @param[in]  custIndex    The level 4 (LUT2) custom index
 *  @param[in]  match        The four match values, only 1st three valid if prevLink is non-NULL
 *  @param[in]  prevLink     An optional L2 or L3 handle that links to this lookup
 *  @param[in]  divertQ      The source queue for atomic queue diversion with LUT2 update
 *                           Set to pa_PARAMS_NOT_SPECIFIED if not specified
 *  @param[in]  fReplace     Flag to indicate whether the entry exists
 *  @param[in]  routeInfo    Where to send a packet that matches
 *  @param[out] retHandle    The returned L4 handle
 *  @param[out] cmd          The buffer where the command is created
 *  @param[in]  cmdSize      On entry the size of the cmd buffer, on exit the size of the command
 *  @param[in]  reply        Where the response to the PASS command is routed
 *  @param[out] cmdDest      Value (@ref cmdTxDest)
 *  @retval                  Value (@ref ReturnValues)
 *  @pre                     A driver instance must be created and tables initialized
 */
paReturn_t Pa_addCustomLUT2 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              uint8_t         match[pa_NUM_BYTES_CUSTOM_LUT2],
                              paHandleL2L3_t  prevLink,
                              uint16_t        fReplace,
                              uint16_t        divertQ,
                              paRouteInfo_t  *routeInfo,
                              paHandleL4_t    retHandle,
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest );
                            
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_forwardResult examines the reply of the sub-system to a command
 *
 *  @details  This command is used to pass the sub-system generated replies to commands back to
 *            this module. Functions @ref Pa_addMac, @ref Pa_addSrio, @ref Pa_addCustomLUT1 and 
 *            @ref Pa_addIp generate replies that must be
 *            forwarded to this module, or else handle deletion and link are not possible. Other
 *            commands generate replies that can be sent to this module which will return any
 *            warnings detected in the sub-system.
 *
 *  @param[in]    iHandle    The driver instance handle
 *  @param[in]    vresult    The command reply packet from the sub-system
 *  @param[out]   retHandle  Returns the handle associated with the command
 *  @param[out]   handleType Value @ref HandleTypes
 *  @param[out]   cmdDest    Value (@ref cmdTxDest)
 *  @retval                  Value (@ref ReturnValues)
 *  @pre                     A driver instance must be created and tables initialized
 */
paReturn_t Pa_forwardResult (Pa_Handle iHandle, void *vresult, paEntryHandle_t *retHandle, int *handleType, int *cmdDest);


/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configExceptionRoute configures the routing of packets based on a exception condition such as
 *          MAC briadcast, multicast or error packet
 *
 *  @details  This function is used to configure the sub-system to route packets that satisfy an exception
 *            rule or condition (see @ref ErouteTypes). For example,
 *            - failure to table match
 *            - parsing error i.e. the sub-system is not able to continuethe parse
 *            - MAC broadcast packets
 *            - IP multicast packets  
 *
 *            From one to @ref pa_EROUTE_MAX routes can be specified through a single call to this
 *            function.  Parameter nRoute is used to specify how many routes are contained in the
 *            routeTypes and eRoutes arrays. A value of 0 nRoutes results in no action by the function.
 *
 *            By default when each exception type is detected the packet is discarded silently. Once the
 *            route is changed through a call to this function it remains in the new state until the
 *            function is called again to explicitly change that route. The only way to revert back
 *            to the default of silent discard is to call this function again.
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the API @ref Pa_forwardResult.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    nRoute      The number of exception routes specified
 *  @param[in]    routeTypes  Array of exception routing types (@ref ErouteTypes)
 *  @param[in]    eRoutes     Array of exception packet routing configuration
 *  @param[out]   cmd         Buffer where the sub-system command is created
 *  @param[in]    cmdSize     The size of the passCmd buffer
 *  @param[in]    reply       Where the response to the PASS command is routed
 *  @param[out]   cmdDest     Value (@ref cmdTxDest)
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_configExceptionRoute (Pa_Handle       iHandle,
                                    int             nRoute,
                                    int            *routeTypes,
                                    paRouteInfo_t  *eRoutes,
                                    paCmd_t         cmd,
                                    uint16_t       *cmdSize,
                                    paCmdReply_t   *reply,
                                    int            *cmdDest);
                                 
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configCmdSet configures the command set which consists of a list of commands
 *
 *  @details  This function is used to configure the sub-system to format and store a list 
 *            of commands which are executed in order when a match occurs and the command set is 
 *            specified by the routing information.
 *
 *            The command set is created and refered to based on the command set index.  
 *            Once the command set is created through a call to this function it remains effective 
 *            until the function is called again to explicitly overwrite its content. It is not 
 *            recommended to update a command set when it is still used by one or more packet 
 *            routes.  
 *            There are @ref pa_MAX_CMD_SETS of command sets supported by the sub-system
 *
 *            The commands within the command set will be executed in order at PDSP4. The module user is 
 *            responsible for placing the commands in such ways that the packet offsets required by commands should
 *            be in ascending order, otherwise, the unexecutable command will be ignored. The command set
 *            should be terminated with a pa_CMD_NEXT_ROUTE or pa_CMD_MULTI_ROUTE command. If there is
 *            no final route command specified, the PASS will use the default next route command. Please note
 *            that all the commands following the pa_CMD_NEXT_ROUTE or pa_CMD_MULTI_ROUTE command will be ignored.  
 *            
 *            This API supports the following commands (@ref paCmdCode)
 *            @li  pa_CMD_REMOVE_HEADER
 *            @li  pa_CMD_COPY_DATA_TO_PSINFO 
 *            @li  pa_CMD_CRC_OP
 *            @li  pa_CMD_PATCH_DATA
 *            @li  pa_CMD_REMOVE_TAIL
 *            @li  pa_CMD_NEXT_ROUTE
 *            @li  pa_CMD_MULTI_ROUTE
 *            @li  pa_CMD_USR_STATS
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the API @ref Pa_forwardResult.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    index       The command set index
 *  @param[in]    nCmd        The number of commands specified
 *  @param[in]    cmdInfo     Array of command configuration information
 *  @param[out]   cmd         Buffer where the sub-system command is created
 *  @param[in]    cmdSize     The size of the passCmd buffer
 *  @param[in]    reply       Where the response to the PASS command is routed
 *  @param[out]   cmdDest     Value (@ref cmdTxDest)
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_configCmdSet (Pa_Handle       iHandle,
                            uint16_t        index,
                            int             nCmd,
                            paCmdInfo_t    *cmdInfo,
                            paCmd_t         cmd,
                            uint16_t       *cmdSize,
                            paCmdReply_t   *reply,
                            int            *cmdDest);
                            
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configMultiRouteSet configures the multi-route group which consists of packet multi-route
 *          entries
 *
 *  @details  This function is used to configure the sub-system to format and store a multi- 
 *            route set which contains routing information for up to @ref pa_MAX_MULTI_ROUTE_ENTRIES 
 *            destinations.
 *
 *            The multi-route group is created and refered to based on the multi-route index.  
 *            Once the multi-route group is created through a call to this function it remains effective 
 *            until the function is called again to explicitly overwrite its content. It is not 
 *            recommended to update a mult-route group when it is still used by one or more packet 
 *            routes.  
 *
 *            There are @ref pa_MAX_MULTI_ROUTE_SETS of multi-route sets supported by the sub-system
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the API @ref Pa_forwardResult.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    mode        The operation mode (CONFIG or RESET) refer to @ref paMultiRouteModes_e
 *  @param[in]    index       The multi-route index
 *  @param[in]    nRoute      The number of routing entries specified
 *  @param[in]    routeEntry  Array of routing configuration information
 *  @param[out]   cmd         Buffer where the sub-system command is created
 *  @param[in]    cmdSize     The size of the passCmd buffer
 *  @param[in]    reply       Where the response to the PASS command is routed
 *  @param[out]   cmdDest     Value (@ref cmdTxDest)
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_configMultiRoute (Pa_Handle               iHandle,
                                paMultiRouteModes_e     mode,
                                uint16_t                index,
                                uint16_t                nRoute,
                                paMultiRouteEntry_t    *routeEntry,
                                paCmd_t                 cmd,
                                uint16_t               *cmdSize,
                                paCmdReply_t           *reply,
                                int                    *cmdDest);
                                   
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configCrcEngine configures the specified CRC engine
 *
 *  @details  This function is used to configure the specified CRC engine by formating the 
 *            CRC configuration command packet. 
 *
 *            There are 6 CRC engines in the PA sun-system. Each CRC engine is connected to its 
 *            corresponding PDSP. It performs CRC operation required by the some network protocol 
 *            such as SCTP and/or the user-specified CRC command for its corresponding PDSP. 
 *            Therefore, it is referred by the PDSP number.  
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *  @note     Each CRC engine only supports one type of CRC per configuration.
 *            It is up to the module user to configure and use the CRC engine by calling this function
 *            for the specific use cases. For example, the CRC engine connected to PDSP2 should be 
 *            configured to perform CRC-32c checksum for SCTP over inner-IP use case.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    index       The CRC engine index
 *  @param[in]    cfgInfo     The CRC engine configuration information
 *  @param[out]   cmd         Buffer where the sub-system command is created
 *  @param[in]    cmdSize     The size of the passCmd buffer
 *  @param[in]    reply       Where the response to the PASS command is routed
 *  @param[out]   cmdDest     Value (@ref cmdTxDest)
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_configCrcEngine (Pa_Handle       iHandle,
                               uint16_t        index,
                               paCrcConfig_t  *cfgInfo,
                               paCmd_t         cmd,
                               uint16_t       *cmdSize,
                               paCmdReply_t   *reply,
                               int            *cmdDest);
                               
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configUsrStats configures the user-defined statistics operation
 *
 *  @details  This function performs the counter configuration for the multi-level hierarchical user-defined 
 *            statistics which consists of up to 64 64-bit counters and up to 192 32-bit counters. Each counter  
 *            can be linked to the next level counter. All counters in its linking chain will be incremented 
 *            when the lowest level counter is updated. The module user can specify the type of each counter and 
 *            how the counter is linked to the next level counter. 
 *            It is not recommended to re-configure the user-defined statistics when one or more counters are 
 *            still used by PASS. The command reply routing is optional because this command is always 
 *            processed by the PA sub-system.  
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the @ref Pa_forwardResult API.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    cfgInfo     The user-defined statistics configuration information
 *  @param[out]   cmd         Buffer where the sub-system command is created
 *  @param[in]    cmdSize     The size of the passCmd buffer
 *  @param[in]    reply       Where the response to the PASS command is routed
 *  @param[out]   cmdDest     Value (@ref cmdTxDest)
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 */
paReturn_t Pa_configUsrStats (Pa_Handle                iHandle,
                              paUsrStatsConfigInfo_t  *cfgInfo,
                              paCmd_t                  cmd,
                              uint16_t                *cmdSize,
                              paCmdReply_t            *reply,
                              int                     *cmdDest);

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_configTimestamp configures the PA timer which is used to generate 48-bit timestamp
 *
 *  @details  This function is used to configure the 16-bit timer reserved for the 48-bit system 
 *            timestamp. The lower 32-bit of the system timestamp will be inserted into the timestamp 
 *            field in the packet descriptor for all input packets. It can be also inserted into 
 *            the timestamp report packets triggered by the egress packets per tx command.
 *            The 16-bit timer connected to PDSP0 is reserved for timestamp generation.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[in]    cfgInfo     The timestamp configuration information
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 *
 */
paReturn_t Pa_configTimestamp (Pa_Handle            iHandle,
                               paTimestampConfig_t  *cfgInfo);
                               
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_getTimestamp returns the 48-bit system timestamp
 *
 *  @details  This function is called to retrieve the current value of 48-bit PASS system timestamp.
 *
 *  @param[in]    iHandle     The driver instance handle
 *  @param[out]   pTimestamp  Pointer to the 48-bit timestamp
 *  @retval                   Value (@ref ReturnValues)
 *  @pre                      A driver instance must be created and tables initialized
 *
 */
paReturn_t Pa_getTimestamp (Pa_Handle            iHandle,
                            paTimestamp_t        *pTimestamp);
                               

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_requestStats requests sub-system statistics
 *
 *  @details  This function is used to request the operating statistics from the sub-system. 
 *            The statistics can be optionally cleared after reading through the doClear parameter.
 *            The statistics apply to the entire sub-system, and are not core dependent on multi-core
 *            devices.
 *
 *            On return the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. The sub-system will generate a reply and this reply
 *            must be sent back to this module through the API @ref Pa_formatStatsReply.
 *
 *  @param[in]    iHandle    The driver instance handle
 *  @param[in]    doClear    If TRUE then stats are cleared after being read
 *  @param[out]   cmd        Buffer where the sub-system command is created
 *  @param[in]    cmdSize    The size of the cmd buffer
 *  @param[in]    reply      Where the response of the PASS command is routed
 *  @param[out]   cmdDest    Value (@ref cmdTxDest)
 *  @retval                  Value (@ref ReturnValues)
 *  @pre                     A driver instance must be created and tables initialized
 */
paReturn_t Pa_requestStats (Pa_Handle      iHandle,
                            uint16_t       doClear, 
                            paCmd_t        cmd, 
                            uint16_t      *cmdSize, 
                            paCmdReply_t  *reply, 
                            int           *cmdDest);
                            
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_formatStatsReply formats the stats reply from the PA
 *
 *  @details  This function is used to convert the stats from the sub-system into a format
 *            useful for the application
 *
 *  @param[in]    handle    The driver instance handle
 *  @param[in]    cmd       The buffer returned with the request stats response from PA
 *  @retval                 A pointer to the formatted stats
 *  @pre                    A call to @ref Pa_requestStats with output sent to PA and a 
 *                          reply generated from PA.
 */
paSysStats_t* Pa_formatStatsReply (Pa_Handle    handle,
                                   paCmd_t      cmd);
                                   
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_requestUsrStats queries user-defined statistics
 *
 *  @details  This function is used to query the user-defined statistics from the sub-system. 
 *            The statistics will be formatted and copied to the buffer provided.
 *            The sub-system statistics can be then optionally cleared if doClear is set. 
 *            In this case, the command buffer (cmd) contains a formatted command for the sub-system. 
 *            The destination for the command is provided in cmdDest. The module user must send the
 *            formatted command to the sub-system. 
 *
 *  @param[in]    iHandle    The driver instance handle
 *  @param[in]    doClear    If TRUE then stats are cleared after being read
 *  @param[out]   cmd        Buffer where the sub-system command is created
 *  @param[in]    cmdSize    The size of the cmd buffer
 *  @param[in]    reply      Where the response of the PASS command is routed
 *  @param[out]   cmdDest    Value (@ref cmdTxDest)
 *  @param[out]   pUsrStats  Pointer to the usrStats buffer
 *  @retval                  Value (@ref ReturnValues)
 *  @pre                     A driver instance must be created and tables initialized
 */
paReturn_t Pa_requestUsrStats (Pa_Handle      iHandle,
                               uint16_t       doClear, 
                               paCmd_t        cmd, 
                               uint16_t      *cmdSize, 
                               paCmdReply_t  *reply, 
                               int           *cmdDest,
                               paUsrStats_t  *pUsrStats);

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_formatTxRoute formats the commands to add checksums and route a Tx packet
 *
 *  @details  This function is used to create the command block which is used by the packet accelerator
 *            sub-system to forward the packet with optional checksum generation.
 *            The module user can combine this block with other command blocks that control the security 
 *            accelerator. The combined block is then provided for the transmitted packets in the Protocol 
 *            specific section of the packet descriptor. This API needs only to be called once, and the same 
 *            protocol specific section can be used for every packet in the channel. If the length of the 
 *            checksum area changes with each packet, update the command buffer with the macro 
 *            PASS_SET_TX_CHKSUM_LENGTH()
 *
 *  @note     The Tx commands can be executed at either PDSP4 or PDSP5. However, it is highly
 *            recommended to use PDSP5 for load balance since PDSP4 will be used to execute 
 *            multi-routing and from-network command set.
 *
 *  @param[in]   chk0       Checksum 0 configuration. NULL if no checksum computation required
 *  @param[in]   chk1       Checksum 1 configuration. NULL if no checksum computation required
 *  @param[in]   route      Next packet routing from sub-system
 *  @param[out]  cmdBuffer  The routing command is formed in this buffer
 *  @param[in]   cmdSize    On entry the size of cmdBuffer. On exit the size of the command
 *  @retval                 Value (@ref ReturnValues)
 */
paReturn_t Pa_formatTxRoute  (paTxChksum_t  *chk0,
                              paTxChksum_t  *chk1,
                              paRouteInfo_t *route,
                              void          *cmdBuffer,
                              uint16_t      *cmdSize );
                             
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_formatRoutePatch formats the commands to route a packet and blind patch
 *
 *  @details  This function is used to create the command block which is used by the packet accelerator
 *            sub-system to perform blind patches on the packet. This function user optionally combines
 *            the generated block with other blocks to create compound commands. The command blocks are
 *            attached to data packets in the Protocol specific section of the packet descriptor.
 *
 *  @note     The Tx commands can be executed at either PDSP4 or PDSP5. However, it is highly
 *            recommended to use PDSP5 for load balance since PDSP4 will be used to execute 
 *            multi-routing and from-network command set.
 *
 *  @param[in]   route      Specifies where the packet is sent after the patch is complete
 *  @param[in]   patch      The patch information
 *  @param[out]  cmdBuffer  The routing command is formed in this buffer
 *  @param[in]   cmdSize    On entry this size of cmdBuffer. On exit the amound of cmdBuffer used
 *  @retval                 Value (@ref ReturnValues)
 */
                             
paReturn_t Pa_formatRoutePatch (paRouteInfo_t *route,
                                paPatchInfo_t *patch,
                                void          *cmdBuffer,
                                uint16_t      *cmdSize);
                                
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_formatTxCmd formats a list of commands to be executed on the packets to be transmitted
 *          over the network
 *
 *  @details  This function is used to create, append and update the list of commands which will be 
 *            executed by the packet accelerator and security accelerator sub-systems to perform a sequence 
 *            of actions on the packet. The command block should be attached to data packets in the 
 *            protocol specific section of the packet descriptor. 
 *
 *            This API may be called multiple times to add or update the command block.
 *            The same protocol specific section can be used for every packet in the channel after the
 *            command list is constructed. Multiple MACROs may be used to update some parameters
 *            such as packet length in the command buffer for each packet. 
 *
 *            This API supports the following commands (@ref paCmdCode):
 *            @li pa_CMD_NEXT_ROUTE
 *            @li pa_CMD_CRC_OP
 *            @li pa_CMD_PATCH_DATA
 *            @li pa_CMD_TX_CHECKSUM
 *            @li pa_CMD_REPORT_TX_TIMESTAMP
 *            @li pa_CMD_SA_PAYLOAD
 *            @li pa_CMD_IP_FRAGMENT
 *            @li pa_CMD_PATCH_MSG_LEN
 *
 *  @note     The Tx commands can be executed at either PDSP4 or PDSP5. However, it is highly
 *            recommended to use PDSP5 for load balance since PDSP4 will be used to execute 
 *            multi-routing and from-network command set.
 *
 *  @param[in]   nCmd       The number of commands specified
 *  @param[in]   cmdInfo    Array of command configuration information
 *  @param[in]   offset     The command buffer location where the new commands are inserted
 *  @param[out]  cmdBuffer  Buffer where the sub-system command is created
 *  @param[in]   cmdSize    On entry this size of cmdBuffer. On exit the amound of cmdBuffer used
 *  @retval                 Value (@ref ReturnValues)
 *
 *  @note The command buffer should be 4-byte aligned
 */
                             
paReturn_t Pa_formatTxCmd (int             nCmd,
                           paCmdInfo_t    *cmdInfo,
                           uint16_t        offset,
                           void           *cmdBuffer,
                           uint16_t       *cmdSize);
                             
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_resetControl controls the reset state of the Sub-system
 *
 *  @details  This function is used to assert or release reset for the sub-system. Asserting reset does not
 *            reset any of the sub-system tables (L2, L3 or L4, see @ref netlayers), but only the packet
 *            processing modules. To achieve a complete system reset the system level reset must be asserted
 *            through the power controller.
 *
 *  @param[in]   iHandle    The driver instance handle
 *  @param[in]   newState   Value @ref paSubSysStates
 *  @retval                 Value @ref paSubSysStates
 *  @pre                    None
 *
 *  @note This function will access the PA sub-system registers. It is up to the module user to provide critical
 *        section protection so that only one core or task should use this function at a time.
 */
paSSstate_t Pa_resetControl (Pa_Handle iHandle, paSSstate_t newState);
                             
                             
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_downloadImage downloads a PDSP image to a sub-system with the packet processing modules in reset.
 *
 *  @details This function is used to download an executable PDSP image to the specific packet processing module.
 *           See section table @ref appendix1 for a description of PDSP images provided by this module
 *
 *  @param[in]   iHandle   The driver instance handle
 *  @param[in]   modId     The PDSP number (0-5)
 *  @param[in]   image     The image to download
 *  @param[in]   sizeBytes The size of the image
 *  @retval                Value (@ref ReturnValues)
 *  @pre                   The packet processing modules must be in reset. See @ref Pa_resetControl.
 *
 *  @note This function will access the PA sub-system registers. It is up to the module user to provide critical
 *        section protection so that only one core or task should use this function at a time.
 */
paReturn_t Pa_downloadImage (Pa_Handle iHandle, int modId, void* image, int sizeBytes);

/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_getHandleRefCount returns the number of reference channels linked to the LUT1 handle
 *
 *  @details  The LLD maintains the reference counter for LUT1 handles: MAC/IP. Given a handle, 
 *            the LLD would return how many references are being used in next header entry by invoking
 *            the function. For example, Query on MAC handle need to return how many IP handles are 
 *            referencing the MAC handles. Query on IP handle need to return how many next protocol 
 *            entries: IP/UDP are referencing to the IP handle.
 *            Therefore this function can be used to verify whether the LUT1 entry associated with
 *            the handle can be reomved.
 *
 *  @param[in]   iHandle    The driver instance handle
 *  @param[in]   l2l3handle The L2 or L3 handle to be queryed
 *  @param[out]  refCount   The number of reference channels
 *  @retval                 Value (@ref ReturnValues)
 */
paReturn_t Pa_getHandleRefCount ( Pa_Handle       iHandle,
                                  paHandleL2L3_t  l2l3handle,
                                  uint16_t        *refCount );
                                  
/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_getPDSPVersion returns the PA PDSP version information.
 *
 *  @details This function is used to get the PA PDSP version information in 0xAABBCCDD format.
 *           where Arch (AA); API Changes (BB); Major (CC); Minor (DD
 *
 *  @param[in]   iHandle   The driver instance handle
 *  @param[in]   modId     The PDSP number (0-5)
 *  @param[out]  pVersion  The pointer to PDSP version number
 *  @retval                Value (@ref ReturnValues)
 *  @pre                   The PDSP image should be downloaded successfully.
 *
 */
paReturn_t Pa_getPDSPVersion (Pa_Handle iHandle, int modId, uint32_t *pVersion);


/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_getVersion returns the PA LLD version information
 *
 *  @details This function is used to get the version information of the PA LLD in 0xAABBCCDD format.
 *           where Arch (AA); API Changes (BB); Major (CC); Minor (DD)
 *
 *  @retval                32-bit version information
 */
uint32_t Pa_getVersion (void);


/**
 *  @ingroup palld_api_functions
 *  @brief  Pa_getVersionStr returns the PA LLD version string
 *
 *  @details This function is used to get the version string of the PA LLD.
 *
 *  @retval                Version string
 */
const char* Pa_getVersionStr (void);

/**
 *  @ingroup palld_api_macros
 *  @brief  pa_RESET_SUBSYSTEM is used to reset the Sub-system
 *
 *  @details  This macro is used to put the packet processing sub-system into reset. It performs the same function
 *            as @ref Pa_resetControl, but in macro form. The module user must define the macro SYSTEM_WRITE32.
 *
 *  @pre      The module user must define a macro called SYSTEM_WRITE32(address, value) which writes a 32 bit
 *            value (value) to global address (address).
 *            
 */
#define pa_RESET_SUBSYSTEM()      \
{                                 \
      CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_PA_SS_CFG_REGS;    \
                                                                        \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[0].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[1].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[2].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[3].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[4].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[5].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PKT_ID.PKT_ID_SOFT_RESET, 1);    \
      SYSTEM_WRITE32(&(passRegs->STATS.STATS_SOFT_RESET, 1);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[0].TIMER_CNTRL_REG,   0);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[1].TIMER_CNTRL_REG,   0);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[2].TIMER_CNTRL_REG,   0);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[3].TIMER_CNTRL_REG,   0);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[4].TIMER_CNTRL_REG,   0);    \
      SYSTEM_WRITE32(&(passRegs->PDSP_TIMER[5].TIMER_CNTRL_REG,   0);    \
}      
      
/**
 *  @ingroup palld_api_macros
 *  @brief  pa_ENABLE_SUBSYSTEM enables the subsystem.
 *
 *  @details This macro is used to release reset from the packet processing sub-system. It performs the same
 *           function as @ref Pa_resetControl, but in macro from. The module user must define the macro SYSTEM_WRITE32
 *           and SYSTEM_READ32.
 *
 *  @pre     The module user must define the macro SYSTEM_WRITE32(address, value) and SYSTEM_READ32 (address) which
 *           read and write to global address (address). 
 */
#define pa_ENABLE_SUBSYSTEM()    \
{                                 \
      CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_PA_SS_CFG_REGS;    \
                                                                        \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[0].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[1].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[2].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[3].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[4].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      SYSTEM_WRITE32(&(passRegs->PDSP_CTLSTAT[5].PDSP_CONTROL), (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK));   \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[0].MBOX_SLOT[0])) == 0);          \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[1].MBOX_SLOT[0])) == 0);          \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[2].MBOX_SLOT[0])) == 0);          \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[3].MBOX_SLOT[0])) == 0);          \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[4].MBOX_SLOT[0])) == 0);          \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[5].MBOX_SLOT[0])) == 0);          \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[1], 1);                    \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[0], 0);                    \
      while (SYSTEM_READ32(&(passRegs->MAILBOX[0].MBOX_SLOT[1])) == 1);          \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[1], 0);                    \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[2], 0);                    \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[3], 0);                    \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[4], 0);                    \
      SYSTEM_WRITE32(&(passRegs->MAILBOX[0].MBOX_SLOT[5], 0);                    \
}      
     
     
/**
 *  @ingroup palld_api_macros
 *  @brief  pa_DOWNLOAD_MODULE downloads an image
 *
 *  @details  This macro provides the same function as @ref Pa_downloadImage. A single image is downloaded to
 *            one of the packet processing modules.
 *
 *  @pre      The module user must define macro SYSTEM_COPY(dest, src, sizeWords) which copies sizeWords from
 *            address src to address dst. The packet processing module must have reset asserted.
 */
#define pa_DOWNLOAD_MODULE(id,img,size)   \
{                                 \
      CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_PA_SS_CFG_REGS;    \
                                                                        \
      SYSTEM_COPY(&(passRegs->PDSP_IRAM[id].PDSP_RAM[0]), img, size);   \
}      
      
/**
 *   @ingroup palld_api_macros
 *   @brief  pa_GET_SYSETM_STATE returns the state of the subsystem
 *
 *   @details  This macro provides the same functionality as @ref Pa_resetControl and returns the 
 *             current state in the macro argument.
 */
#define pa_GET_SYSTEM_STATE(x)      \
  {  int enable=0; int disable=0;   \
      CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_PA_SS_CFG_REGS;    \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[0].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[1].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[2].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[3].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[4].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (SYSTEM_READ32(&(passRegs->PDSP_CTLSTAT[5].PDSP_CONTROL)) & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == \
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)    ) \
       enable++;  else disable++;                                                        \
     if ( (enable > 0) && (disable > 0) )  (x) = pa_STATE_INCONSISTENT;                  \
     else if (enable > 0)  (x) = pa_STATE_ENABLE;                                        \
     else (x) = pa_STATE_RESET;                                                          \
  }  (x) = (x)
  
  
/**
 *  @ingroup palld_api_macros
 *  @brief  pa_SET_TX_CHKSUM_LENGTH sets the tx checksum length in a tx route block
 *
 *  @details  This macro is used to modify the length of a checksum field in a command packet
 *            created by a call to @ref Pa_formatTxRoute. In many cases packets in an outbound packet
 *            stream will have the same source and destination information (see @ref netlayers) but
 *            differ in the packet length. This macro will change the checksum calculation information
 *            which is sent to the sub-system. The length fields in L2, L3 and L4 must be changed by
 *            the module user before sending the packet, they are not changed by this macro. In the
 *			  case of IP L3 and TCP or UDP L4 the psuedo header checksum must also be changed
 *			  to reflect the change in packet length.
 */
#define pa_SET_TX_CHKSUM_LENGTH(datap,cnum,len)  \
    PASAHO_CHKCRC_SET_LEN        ((&(((pasahoComChkCrc_t *)datap)[cnum])), len)

/**
 *  @ingroup palld_api_macros
 *  @brief pa_SET_TX_INITVAL sets the initial value in a tx route block
 *
 *  @details  This macro is used to modify the initial value of a checksum field in a command packet
 *            created by a call to @ref Pa_formatTxRoute. This macro is used when a single call
 *			  to @ref Pa_formatTxRoute is desired, The application typically follows this with an update 
 *            to the length fields in network headers, either directly or through a blind patch. 
 *            For updates with IPv4 or IPv6 the pseudo header checksum must be updated as well, and this 
 *            macro is used to update the value. Typically the pseudo header checksum will be computed 
 *            with all values except the length, and then updated for each packet with a single ones' complement add.
 */
#define pa_SET_TX_CHKSUM_INITVAL(datap,cnum,val)  \
	PASAHO_CHKCRC_SET_INITVAL   ((&(((pasahoComChkCrc_t *)datap)[cnum])), val)

/**
 * @page netlayers
 *
 *  Network layers define a hierarchy of services delineated by functionality. Each layer can use the functionality
 *  of the next layer below, and offers services to the next layer above. The packet accelerator sub-system examines
 *  and routes packets based on fields in up to three layers of the Ethernet packets or L0-L2 header of the SRIO packets. 
 *  
 *  In layer 2, the MAC (Media Access Control) layer, 
 *  the sub-system classifies IEEE 802.3 packets based on (optionally) the destination MAC, source MAC, Ethertype, and 
 *  VLAN tags. 
 *
 *  In Layer 3, the network layer, IPv4 (Internet Protocol Version 4) and IPv6 (Internet Protocol
 *  Version 6) packets are routed based (optionally) on source IP address, destination IP address, IPv4 protocol,
 *  IPv6 next header, IPv4 Type of Service (recently changed to IPv4 differentiated service in RFC 2474), IPv6
 *  traffic class, and IPv6 flow label. For IP packets with security services the SPI (Security Parameters Index)
 *  is also included in the classification information. For IP packets with SCTP (Stream Control Transmission Protocol) 
 *  the SCTP destination port is also included in the classification information.
 *
 *  In layer 4, the transport layer, UDP (User Datagram Protocol) and TCP (Transmission Control Protocol) packets
 *  are routed based on the destination port. However, the GTP-U (GPRS Tunnelling Protocol User Plane) over UDP packets 
 *  are routed based on its 32-bit TEID (Tunnel ID).   
 *
 *  For SRIO (Serial RapidIO), L0-L2 header information
 *  the sub-system classifies SRIO packets based on (optional) the source ID, destination ID, transport type, priority,
 *  message type, SRIO type 11 mailbox and letter, SRIO type 9 stream ID and class of service.
 * 
 */
 
 
/**
 * @page cache
 *
 * The packet accelerator low level driver module will make call backs to the module user when it
 * is about to read from one of the two tables provided by the module user. If the module user
 * is operating in a multi-core environment with a single set of tables shared by all the cores,
 * then this function is used to tell a local core that it must invalidate its cache, without writeback.
 * This is necessary if cross core cache coherency is not maintained by the hardware in the device.
 * 
 * Without this it is possible for one core to be operating from a locally cached version of the 
 * tables which does not reflect any additions or deletions done by other cores.
 *
 * An alternative is to place the tables into non-cached memory.
 *
 */
  
/**
 *  @page semaphores
 *
 *  The packet accelerator low level driver module will make call backs to the module user when it
 *  is about to modify from one of the two tables provided by the module user. If the module user
 *  is operating in a multi-core environment with a single set of tables shared by all the cores,
 *  then this function is used to tell the application to apply a cross core semaphore. 
 *
 *  When table modification is done the module will again make a call back to the module user
 *  to inform it to release the semaphore.
 */
   
   
/**   
 *  @page order
 *
 *  The sub-system examines the L2 and L3 (LUT1) information (see @ref netlayers) in packets based on internal 
 *  table location. When function @ref Pa_addMac and @ref Pa_addIp are executed and the resulting packet 
 *  forwarded to the sub-system, the sub-system places the new entries at the highest free
 *  table location. When incoming packets are examined, the table is searched from lowest entry location
 *  to highest entry location until the first matching entry is found. That entry is used to route the
 *  packet.
 *
 *  Because of this it is required that entries into the table be made in order from the most general
 *  to the most specific. For example, when adding a mac address it is common to want to route the following:
 *  @li  dest mac only  -  Forward packet to host
 *  @li  dest mac + ethertype - Continue parsing
 *  @li  dest mac + source mac + ethertype - Forward packet to host
 *
 *  To get the desired routing the @ref Pa_addMac commands must be executed and the command packets forwarded
 *  to the sub-system in the order shown above. If they are entered in the reverse order then every packet
 *  which has the value dest MAC will be forwarded to the host since it matches the first entry in the list.
 *
 *  The order dependency applies to calls to @ref Pa_addMac and @ref Pa_addIp, but not to calls between these functions. 
 *  So all MAC entries can be made followed by all IP entries, or in the reverse order (provided the IP entries
 *  do not reference the MAC entries) without changing the operation of the sub-system.
 *
 */
 
/**   
 *  @page appendix1 PDSP image
 *
 *  The sub-sustem contains six PDSPs wihich perform the command and packet processing. There are three PDSP
 *  images provided by the module under the pa/fw directory:
 *  @li Packet Classifier 1 image: classify1_bin.c for PDSP0, PDSP1 and PDSP2
 *  @li Packet Classifier 2 image: classify2_bin.c for PDSP3
 *  @li Packet Modifier image: pam_bin.c for PDSP4 and PDSP5
 *
 *  The PDSP executable images are provided to the module user as c-file contains the binary image. They should
 *  be included by the application and loaded into the corresponding PASS PDSP by invoking the API 
 *  @ref Pa_downloadImage at system startup.
 *
 */
 
 
 /**
  *  @page appendix2 CPPI Error Flags
  *
  *  The sub-system performs IPv4 header checksum, UDP/TCP checksum and SCTP CRC-32c checksum autonomously.
  *  The sub-system can also perform the CRC verification for incoming packet as one of the actions specified 
  *  by the post-classification command set.
  *
  *  The checksum and CRC verification results are recorded at the 4-bit error flags in the CPPI packet descriptor
  *  as described below:
  *  @li bit 3: IPv4 header checksum error
  *  @li bit 2: UDP/TCP or SCTP CRC-32c checksum error
  *  @li bit 1: Custom CRC checksum error
  *  @li bit 0: reserved
  * 
  */
  
 /**
  *  @page appendix3 PA-assisted IP Reassembly Operation
  *
  *  The current version of PASS does not support IP reassembly, the IP fragments can be detected by PASS, forwarded to 
  *  and reassembled at host. The reassembled IP packet may be forwarded back to PASS for continuous classification. 
  *  The drawback of this approach is that the order of the incoming packets will not be maintained.
  *
  *  To provide better support for IP reassembly, the PA-assisted IP Reassembly operation is introduced and summarized below:
  *  @li Array of traffic flows which consist of source IP, destination IP, protocol and counter are maintained at PASS PDSP.
  *  @li A traffic flow is activated by the PDSP when the first IP fragment with the source and destination IP and protocol is 
  *      detected and forwarded.
  *  @li The traffic flow is freed when its packet count reaches 0
  *  @li All packets belong to any active traffic flow will be forwarded to the host so the packet order will be maintained.
  *  @li IP fragments should be forwarded to host with "not availeable" traffic flow id if no traffic flow is available. 
  *      In this case, the packet order is not guaranteed to be maintained.
  *  @li PASS supports up to 32 active traffic flows for outer IP (PDSP1) and inner IP (PDSP2) respectively.
  *  @li The PA-assisted IP Reassembly Operation will be enabled by invoking API @ref Pa_control with the IP reassembly 
  *      configuration @ref paIpReassmConfig_t.
  *
  *  @note The minimum size packet wire rate throughput will not be guaranteed when this feature is enabled and there are active 
  *        traffic flows.
  *  
  *  The host IP reassembly module should interact with PASS and perform the full IP reassembly operation. The module user may choose 
  *  to implement a simplified version of IP reassembly algorithm to save CPU cycle in controlled IP environment. A sample reassembly
  *  module is provided in the PA LLD release package, which demonstrates how to interact with the NetCP to perform the IP reassembly
  *  operation. 
  *
  *  The sample code implements a simplified version of IP reassembly algorithm which supports non-overlapping segments only. The sample 
  *  code performs the following tasks:
  *  @li Maintain the IP reassembly contexts consist of source IP, destination IP, IP identification, protocol, fragments count and the 
  *      corresponding traffic flow id.
  *  @li Forward the non-fragmented IP packet with its flow id and count = 1 to PA PDSP queue. This avoids reordering the non-fragmented packets.
  *  @li For IPSEC inner IP fragments, call SA LLD to perform the post-decryption operation including padding check and IPSEC header 
  *      and authentication tag removal.
  *  @li Forward the reassembled IP packet with its flow id and fragments count to PA PDSP queue.
  *  @li Send a null packet with its flow id and fragments count to PA PDSP queue if the fragments are discarded due to timeout or other error.
  *
  */
  
  
#ifdef __cplusplus
}
#endif
  

#endif  /* _PA_H */
