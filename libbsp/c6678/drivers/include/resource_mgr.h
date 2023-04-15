
/*
 *   @file  resource_mgr.h
 *
 *   @brief
 *      Resource Manager Common Definitions. The Resource Manager defines
 *		a set of APIs and definitions for managing platform resources (e.g.
 *		Interrupts) and initializing and using the PA, QMSS and CPPI
 *		subsystems.
 *
 */

/****************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
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


#ifndef _RESOURCE_MGR_H
#define _RESOURCE_MGR_H  /* #defined if this .h file has been included */

/**
 * @mainpage Resource Manager
 The Resource Manager defines a set of APIs and definitions for managing platform resources (e.g. Interrupts) and
 initializing and using the PA, QMSS and CPPI  subsystems. The main APIs are defined in the API functions under the Modules tab.
 @remark Each application must implement the code for the functions that are defined here. They are either application callbacks or in the case
 of the initialization functions expected to be created and called from the application. There are examples of doing this
 in the MCSDK demo and example programs. See the files there called platform_osal.c and resourcemgr.c.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "platform.h"

#include <ti/csl/tistdtypes.h>

/* CSL CHIP, SEM Functional layer includes */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_semAux.h>

/* CSL Cache module includes */
#include <ti/csl/csl_cacheAux.h>

/* CPPI LLD include */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>

/* QMSS LLD include */
#include <ti/drv/qmss/qmss_drv.h>

/* PA LLD include */
#include <ti/drv/pa/pa.h>

#if ( !defined( _LITTLE_ENDIAN ) && !defined( _BIG_ENDIAN ) ) \
||  ( defined(_LITTLE_ENDIAN ) && defined( _BIG_ENDIAN ) )
#error either _LITTLE_ENDIAN or _BIG_ENDIAN must be defined
#endif

/** @defgroup  Resource_Manager_Version  Resource Manager Version
 *  	Resource Manager Version
 */
/*@{*/
#define RESOURCEMGR_VERSION	"1.00.01.00"	/**< */
/*@}*/  /* defgroup */


/** @defgroup  Platform_hardware_semaphore  Hardware Semaphores
 *  	Hardware semaphore IDs. Only those which are assigned appear here.
 */


/*@{*/
#define PLATFORM_CPPI_HW_SEM        1		/**< CPPI Driver - Used by OSAL layer					*/
#define PLATFORM_QMSS_HW_SEM        2		/**< QMSS Driver - Used by OSAL Layer					*/
#define PLATFORM_SPI_HW_SEM			3		/**< SPI BUS arbitration - Used by platform library 	*/
/*@}*/  /* defgroup */



/** @defgroup  Platform_interrupts  Interrupts
 *  Interrupt and event IDs. Only those which are assigned appear here.
 *
 * We are currently combining all interrupts for the switch and they are set by the demo
 * applications in their .cfg files:
 * Ecm.eventGroupHwiNum[0] = 7;
 * Ecm.eventGroupHwiNum[1] = 8;
 * Ecm.eventGroupHwiNum[2] = 9;
 * Ecm.eventGroupHwiNum[3] = 10;
 */
/*@{*/
#define PLATFORM_ETH_EVENTID		48		/**< Ethernet Switch event - Used by NIMU library */
#define PLATFORM_ETH_INTERRUPT		7		/**< Ethernet Switch Interrupt - Used by NIMU library */
/*@}*/  /* defgroup */


/** @defgroup  Platform_dma  DMA
 * Assgined DMA Channels.
 */
/*@{*/
#define PRESOURCE_DMA_CHANNELS_GO_HERE	0
/*@}*/  /* defgroup */



/**********************************************************************
 *********** Resoure Manager QMSS configuration Definitions ***********
 **********************************************************************/

/** @defgroup  Qmss_Subsystem  QMSS Subsystem
* APIs and definitions for managing the QMSS subsystem.
*/
/*@{*/

/**
 *  @brief 	Queue handler type for FDQ.
 */
typedef enum {
    QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF,
    QHANDLER_QPOP_FDQ_ATTACHEDBUF
} QHANDLER_TYPE;



#define NIMU_NUM_TX_DESC                16u /**< Maximum number of TX descriptors used by NIMU */
#define NIMU_NUM_RX_DESC                110u /**< Maximum number of RX descriptors used by NIMU */
#define NIMU_MAX_NUM_TX_CMD_DESC        2u  /**< Maximum number of TX Command descriptors used by NIMU */

#define MAX_NUM_NIMU_DESC   (NIMU_NUM_TX_DESC + NIMU_NUM_RX_DESC + NIMU_MAX_NUM_TX_CMD_DESC) /**< Maximum number of descriptors used by NIMU */
#define MAX_NUM_DESC        (MAX_NUM_NIMU_DESC)   /**< Maximum number of descriptors used by all the modules */
#define	MAX_DESC_SIZE       128 /**< Maximum size of descriptor in bytes */

/**
 * @brief
 *  QMSS configuration information
 *
 * @details
 *  This structure defines the QMSS configuration during the system initialization
 */
typedef struct QMSS_CFG_Tag
{
    /**
     * @brief       Master core flag, 1: Master core; 0: non-master core.
     */
    uint32_t        master_core;

    /**
     * @brief       Maximum number of descriptors.
     */
    uint32_t        max_num_desc;

    /**
     * @brief       Size of descriptor in bytes.
     */
    uint32_t        desc_size;

    /**
     * @brief       Memory Region corresponding to the descriptor.
     */
    Qmss_MemRegion  mem_region;

} QMSS_CFG_T;


/**
 *   @n@b res_mgr_qmss_freeq
 *
 *   @b Description
 *   @n This API returns the handle to the global free queue for QMSS. The global free
 *   queue maintains all usable descriptors for anyone using QMSS.
 *
 *
 *   @return
 *
 */
Qmss_QueueHnd
res_mgr_qmss_get_freeq
(
    void
);


 /**
 *   @n@b QMSS_QPOP
 *
 *   @b Description
 *   @n This API implements a the queue pop logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  type
 *        Queue handler type
 *   @param[in]  pHostDescriptor
 *       pointer to  descriptor
 *
 *   @return
 *
 */
int32_t
QMSS_QPOP
(
    Qmss_QueueHnd           handler,
    QHANDLER_TYPE           type,
    Cppi_HostDesc**         pHostDescriptor
);

/**
 *   @n@b QMSS_QPUSH
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  packetSize
 *        packet Size
 *   @param[in]  descSize
 *        descriptor Size
 *   @param[in]  location
 *        Qmss_Location location
 *   @return
 *
 */
void
QMSS_QPUSH
(
    Qmss_QueueHnd          handler,
    void                   *descAddr,
    uint32_t               packetSize,
    uint32_t               descSize,
    Qmss_Location          location
);

/**
 *   @n@b QMSS_QPUSHDESCSIZE
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  descSize
 *        descriptor Size
 *   @return
 *
 */
void
QMSS_QPUSHDESCSIZE
(
    Qmss_QueueHnd          handler,
    void                   *descAddr,
    uint32_t               descSize
);

/**
 *   @n@b res_mgr_init_qmss
 *
 *   @b Description
 *   @n This API initializes the QMSS LLD.
 *
 *   @param[in]  *p_qmss_cfg Pointer to QMSS_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 *
 */
int32_t
res_mgr_init_qmss
(
    QMSS_CFG_T      *p_qmss_cfg
);

/**
 *   @n@b res_mgr_stop_qmss
 *
 *   @b Description
 *   @n This API de-initializes the QMSS LLD.
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 *
 */
int32_t
res_mgr_stop_qmss
(
    void
);


/**
 *  @n@b Osal_qmssCsEnter
 *
 *  @b  brief
 *  @n  This API ensures multi-core and multi-threaded
 *      synchronization to the caller.
 *
 *      This is a BLOCKING API.
 *
 *      This API ensures multi-core synchronization between
 *      multiple processes trying to access QMSS shared
 *      library at the same time.
 *
 *
 *  @return
 *      Handle used to lock critical section
 *
 */
Ptr Osal_qmssCsEnter (void);


/**
 *
 *  @n@b Osal_qmssCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cpswQmssCsEnter ()
 *      API. It resets the multi-core and multi-threaded lock,
 *      enabling another process/core to grab QMSS access.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return
 *  @n None
 *
 */
void Osal_qmssCsExit (Ptr CsHandle);


/**
 *
 *  @n@b Osal_qmssMtCsEnter
 *
 *  @b  brief
 *  @n  This API ensures ONLY multi-threaded
 *      synchronization to the QMSS user.
 *
 *      This is a BLOCKING API.
 *
 *
 *  @return
 *       Handle used to lock critical section
 *
 */
Ptr Osal_qmssMtCsEnter (void);


/**
 *
 *  @n@b Osal_qmssMtCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cpswQmssMtCsEnter ()
 *      API. It resets the multi-threaded lock, enabling another process
 *      on the current core to grab it.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return None
 *
 */
void Osal_qmssMtCsExit (Ptr CsHandle);


/**
 *
 *  @n@b Osal_qmssMalloc
 *
 *  @b  brief
 *  @n  This API allocates a memory block of a given
 *      size specified by input parameter 'num_bytes'.
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @return
 *      Allocated block address
 *
 */
Ptr Osal_qmssMalloc (uint32_t num_bytes);


/**
 *
 *  @n@b Osal_qmssFree
 *
 *  @b  brief
 *  @n  This API frees and restores a given memory location
 *      pointer 'dataPtr' of size 'num_bytes' to its
 *      original heap location. Frees up memory allocated using
 *      @a Osal_qmssMalloc ()
 *
 *  @param[in]  dataPtr
 *      Pointer to the memory block to be cleaned up.
 *
 *  @param[in]  num_bytes
 *      Size of the memory block to be cleaned up.
 *
 *  @return  None
 *
 */
void Osal_qmssFree (Ptr dataPtr, uint32_t num_bytes);


/**
 *  @b Description
 *  @n
 *      The function is used to indicate that a block of memory is
 *      about to be accessed. If the memory block is cached then this
 *      indicates that the application would need to ensure that the
 *      cache is updated with the data from the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be invalidated
 *
 *  @param[in]  size
 *       Size of the block to be invalidated

 *  @retval None
 */
void Osal_qmssBeginMemAccess (void *blockPtr, uint32_t size);


/**
 *  @b Description
 *  @n
 *      The function is used to indicate that the block of memory has
 *      finished being accessed. If the memory block is cached then the
 *      application would need to ensure that the contents of the cache
 *      are updated immediately to the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be written back
 *
 *  @param[in]  size
 *       Size of the block to be written back

 *  @retval None
 */
void Osal_qmssEndMemAccess (void *blockPtr, uint32_t size);


/*@}*/  /* defgroup */


/**********************************************************************
 *********** Resoure Manager PA configuration Definitions ***********
 **********************************************************************/

/** @defgroup  PA_Subsystem  Packet Accelerator (PA) Subsystem
* APIs and definitions for managing the Packet Accelerator (PA) subsystem.
*/

/*@{*/

/**
 *  @brief Number of PA Tx queues available
 */
#define         NUM_PA_TX_QUEUES            9

/**
 *  @brief Number of PA Rx queues available
 */
#define         NUM_PA_RX_CHANNELS          24

/**
 *  @brief Maximum number of L2 handles
 */
#define     MAX_NUM_L2_HANDLES          10

/**
 *  @brief Maximum number of L3 handles
 */
#define     MAX_NUM_L3_HANDLES          20

/**
 *  @brief Maximum number of L4 handles
 */
#define     MAX_NUM_L4_HANDLES          40

/*
 *   @n@b res_mgr_init_pass
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP.
 *
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 *
 */
int32_t
res_mgr_init_pass
(
    void
);

/**
 *   @n@b res_mgr_cppi_get_passhandle
 *
 *   @b Description
 *   @n This API returns the handle to the the CPPI DMA for the Packet Accelerator (PA).
 *
 *
 *   @return  Cppi_Handle
 *
 */
Cppi_Handle
res_mgr_cppi_get_passhandle (void);

/**
 *   @n@b res_mgr_get_painstance
 *
 *   @b Description
 *   @n This API returns the handle to the PA.
 *
 *
 *   @return  Pa_Handle
 *
 */
Pa_Handle
res_mgr_get_painstance (void);

/**
 *  @b Description
 *  @n
 *      The function is used to indicate that a block of memory is
 *      about to be accessed. If the memory block is cached then this
 *      indicates that the application would need to ensure that the
 *      cache is updated with the data from the actual memory.
 *
 *  @param[in]  addr
 *       Address of the block which is to be invalidated
 *
 *  @param[in]  size
 *       Size of the block to be invalidated

 *  @retval  None
 */
void Osal_paBeginMemAccess (Ptr addr, uint32_t size);


/**
 *  @b Description
 *  @n
 *      The function is used to indicate that the block of memory has
 *      finished being accessed. If the memory block is cached then the
 *      application would need to ensure that the contents of the cache
 *      are updated immediately to the actual memory.
 *
 *  @param[in]  addr
 *       Address of the block which is to be written back
 *
 *  @param[in]  size
 *       Size of the block to be written back

 *  @retval  None
 */
void Osal_paEndMemAccess (Ptr addr, uint32_t size);

/*@}*/  /* defgroup */


/**********************************************************************
 *********** Resoure Manager CPPI configuration Definitions ***********
 **********************************************************************/

/** @defgroup  Cppi_Subsystem CPPI Subsystem
* APIs and definitions for managing the CPPI subsystem.
*/
/*@{*/


/**
 *  @brief CPPI configuration type.
 */
typedef enum {
    CPPI_CFG_PASS   = 0,
    CPPI_CFG_SRIO
} CPPI_CFG_TYPE;

/**
 *  @brief Maximum number of CPPI configuration
 */
#define         MAX_CPPI_CFG                2

/**
 * @brief
 *  CPPI configuration information
 *
 * @details
 *  This structure defines the CPPI configuration during the system initialization
 */
typedef struct CPPI_CFG_Tag
{
    /**
     * @brief       Master core flag, 1: Master core; 0: non-master core.
     */
    uint32_t        master_core;

    /**
     * @brief       CPPI DMA number.
     */
    Cppi_CpDma      dma_num;

    /**
     * @brief       Maximum number of TX queues.
     */
    uint32_t        num_tx_queues;

    /**
     * @brief       Maximum number of RX channels.
     */
    uint32_t        num_rx_channels;
} CPPI_CFG_T;

/**
 *   @n@b res_mgr_init_cppi
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]  *p_cppi_cfg  Pointer to CPPI_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 *
 */
int32_t
res_mgr_init_cppi
(
    CPPI_CFG_T      *p_cppi_cfg
);

/**
 *   @n@b res_mgr_stop_cppi
 *
 *   @b Description
 *   @n This API stops the CPPI subsystem.
 *
 *   @param[in]  cfg_type  Configuration to stop, e.g. CPPI_CFG_PASS
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 *
 */
int32_t
res_mgr_stop_cppi
(
    CPPI_CFG_TYPE       cfg_type
);


/**
 *
 *  @n@b Osal_cppiCsEnter
 *
 *  @b  brief
 *  @n  This API ensures multi-core and multi-threaded
 *      synchronization to the caller.
 *
 *      This is a BLOCKING API.
 *
 *      This API ensures multi-core synchronization between
 *      multiple processes trying to access CPPI shared
 *      library at the same time.
 *
 *
 *  @return
 *  @n  Handle used to lock critical section
 *
 */
Ptr Osal_cppiCsEnter (void);


/**
 *
 *  @n@b Osal_cppiCsExit
 *
 *  @b  brief
 *  @n  This API needs to be called to exit a previously
 *      acquired critical section lock using @a Osal_cppiCsEnter ()
 *      API. It resets the multi-core and multi-threaded lock,
 *      enabling another process/core to grab CPPI access.
 *
 *  @param[in]  CsHandle
 *      Handle for unlocking critical section.
 *
 *  @return  None
 *
 */
void Osal_cppiCsExit (Ptr CsHandle);



/**
 *
 *  @n@b Osal_cppiMalloc
 *
 *  @b  brief
 *  @n  This API allocates a memory block of a given
 *      size specified by input parameter 'num_bytes'.
 *
 *      This API should allocate memory from shared memory if the test applications
 *      are to be run on multiple cores.
 *
 *  @param[in]  num_bytes
 *      Number of bytes to be allocated.
 *
 *  @return
 *      Allocated block address
 *
 */
Ptr Osal_cppiMalloc (uint32_t num_bytes);


/**
 *
 *  @n@b Osal_cppiFree
 *
 *  @b  brief
 *  @n  This API frees and restores a given memory location
 *      pointer 'dataPtr' of size 'num_bytes' to its
 *      original heap location. Frees up memory allocated using
 *      @a Osal_cppiMalloc ()
 *
 *  @param[in]  dataPtr
 *      Pointer to the memory block to be cleaned up.
 *
 *  @param[in]  num_bytes
 *      Size of the memory block to be cleaned up.
 *
 *  @return  None
 *
 */
void Osal_cppiFree (Ptr dataPtr, uint32_t num_bytes);


/**
 *  @b Description
 *  @n
 *      The function is used to indicate that a block of memory is
 *      about to be accessed. If the memory block is cached then this
 *      indicates that the application would need to ensure that the
 *      cache is updated with the data from the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be invalidated
 *
 *  @param[in]  size
 *       Size of the block to be invalidated

 *  @retval  None
 */
void Osal_cppiBeginMemAccess (void *blockPtr, uint32_t size);


/**
 *  @b Description
 *  @n
 *      The function is used to indicate that the block of memory has
 *      finished being accessed. If the memory block is cached then the
 *      application would need to ensure that the contents of the cache
 *      are updated immediately to the actual memory.
 *
 *  @param[in]  blockPtr
 *       Address of the block which is to be written back
 *
 *  @param[in]  size
 *       Size of the block to be written back

 *  @retval  None
 */
void Osal_cppiEndMemAccess (void *blockPtr, uint32_t size);


/*@}*/  /* defgroup */



#endif /* _RESOURCE_MGR_H */
