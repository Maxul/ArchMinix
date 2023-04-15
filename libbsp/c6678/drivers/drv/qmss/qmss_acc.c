/**
 *   @file  qmss_acc.c
 *
 *   @brief   
 *      This is the Queue Manager accumulator and interrupt distributor implementation.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2011, Texas Instruments, Inc.
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
 *  \par
*/

/* QMSS Types includes */
#include <ti/drv/qmss/qmss_types.h>
#include <string.h>

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/include/qmss_pvt.h>

/* QMSS OSAL layer */
#include <ti/drv/qmss/qmss_osal.h>

/* RM LLD includes */
#include <ti/drv/rm/rm_public_lld.h>

/**********************************************************************
 ********************** Extern Variables ******************************
 **********************************************************************/

/* QMSS Local Object is valid flag */
extern uint8_t                  qmssLObjIsValid;

/* QMSS Local object */
extern Qmss_GlobalConfigParams  qmssLObj;

/**********************************************************************
 ************************** Functions *********************************
 **********************************************************************/

/** @addtogroup QMSS_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function programs the accumulator with values passed in the cfg structure
 *
 *  @param[in]  pdspId
 *      Accumulator to program
 *          Qmss_PdspId_PDSP1 selects firmware downloaded to PDSP 1
 *          Qmss_PdspId_PDSP2 selects firmware downloaded to PDSP 2
 *
 *  @param[in]  cfg
 *      Initialization structure that contains accumulator configurable fields
 *
 *  @post  
 *      Accumulator channel is programmed.
 *
 *  @retval
 *      Value reported by accumulator
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - QMSS_INVALID_PARAM
 *  @retval
 *      Failure - Accumulator status - QMSS_ACC_IDLE, QMSS_ACC_INVALID_COMMAND, QMSS_ACC_INVALID_CHANNEL, 
 *                  QMSS_ACC_CHANNEL_NOT_ACTIVE, QMSS_ACC_CHANNEL_ALREADY_ACTIVE, QMSS_ACC_INVALID_QUEUE_NUMBER,
 *                  QMSS_RESOURCE_INIT_DENIED
 */

Qmss_Result Qmss_programAccumulator (Qmss_PdspId pdspId, Qmss_AccCmdCfg *cfg)
{
    Qmss_AccCmd         cmd;
    volatile uint32_t   *cmdPtr, *reg;
    uint32_t            index;
    uint8_t             result;
    void                *key;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    if (cfg == NULL)
    {
        return QMSS_INVALID_PARAM;
    }

    if (rmPermCallTbl)
    {
        /* Check init permissions for the accumulator channel */      
        resourceData.resourceType = Rm_resource_QMSS_ACCUM_CH;
        resourceData.res_info.accumCh = cfg->channel;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();
    
    memset ((void *) &cmd, 0, sizeof (Qmss_AccCmd));
    CSL_FINSR (cmd.word0, 7, 0, cfg->channel);
    CSL_FINSR (cmd.word0, 15, 8, cfg->command);
    cmd.word1 = cfg->queueEnMask;
    cmd.word2 = cfg->listAddress;
    CSL_FINSR (cmd.word3, 15, 0, cfg->queMgrIndex);
    CSL_FINSR (cmd.word3, 31, 16, cfg->maxPageEntries);
    CSL_FINSR (cmd.word4, 15, 0, cfg->timerLoadCount);
    CSL_FINSR (cmd.word4, 17, 16, cfg->interruptPacingMode);
    CSL_FINSR (cmd.word4, 19, 18, cfg->listEntrySize);
    CSL_FINSR (cmd.word4, 20, 20, cfg->listCountMode);
    CSL_FINSR (cmd.word4, 21, 21, cfg->multiQueueMode);
    
    /* Point to the accumulator command register's last word */
    reg = (uint32_t *) ((uint8_t *) qmssLObj.qmPdspCmdReg[pdspId] + 4 * 4);

    /* Write command word last */
    cmdPtr = ((uint32_t *) &cmd) + 4;

    for (index = 0; index < 5; index++)
        *reg-- = *cmdPtr--;

    /* wait for the command to clear */
    reg++;
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    /* End Critical Section */
    Qmss_osalCsExit (key);

    return (Qmss_Result) (CSL_FEXTR (*reg, 31, 24));
}

/**
 *  @b Description
 *  @n  
 *      This function disables the accumulator functionality for the specified channel
 *
 *  @param[in]  pdspId
 *      Accumulator to program. 
 *          Qmss_PdspId_PDSP1 selects firmware downloaded to PDSP 1
 *          Qmss_PdspId_PDSP2 selects firmware downloaded to PDSP 2
 *
 *  @param[in]  channel
 *      Channel to disable
 *
 *  @post  
 *      Channel is disabled.
 *
 *  @retval
 *      Value reported by accumulator
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - QMSS_INVALID_PARAM
 *  @retval
 *      Failure - Accumulator status - QMSS_ACC_IDLE, QMSS_ACC_INVALID_COMMAND, QMSS_ACC_INVALID_CHANNEL, 
 *                  QMSS_ACC_CHANNEL_NOT_ACTIVE, QMSS_ACC_CHANNEL_ALREADY_ACTIVE, QMSS_ACC_INVALID_QUEUE_NUMBER,
 *                  QMSS_RESOURCE_INIT_DENIED
 */

Qmss_Result Qmss_disableAccumulator (Qmss_PdspId pdspId, uint8_t channel)
{
    Qmss_AccCmd         cmd;
    volatile uint32_t   *cmdPtr, *reg;
    uint32_t            index;
    uint8_t             result;
    void                *key;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;          

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    if (rmPermCallTbl)
    {
        /* Check init permissions for the accumulator channel */
        resourceData.resourceType = Rm_resource_QMSS_ACCUM_CH;
        resourceData.res_info.accumCh = channel;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    } 

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    memset ((void *) &cmd, 0, sizeof (Qmss_AccCmd));
    CSL_FINSR (cmd.word0, 7, 0, channel);
    CSL_FINSR (cmd.word0, 15, 8, Qmss_AccCmd_DISABLE_CHANNEL);

    /* Point to the accumulator command register's last word */
    reg = (uint32_t *) ((uint8_t *) qmssLObj.qmPdspCmdReg[pdspId] + 4 * 4);

    /* Write command word last */
    cmdPtr = ((uint32_t *) &cmd) + 4;

    for (index = 0; index < 5; index++)
        *reg-- = *cmdPtr--;

    /* Wait for the command to clear */
    reg++;
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    /* End Critical Section */
    Qmss_osalCsExit (key);
    
    return (Qmss_Result) (CSL_FEXTR (*reg, 31, 24));
}

/**
 *  @b Description
 *  @n  
 *      This function programs the timer constant used by the firmware to generate the timer tick.
 *
 *      The Accumulator time "tick" is controlled by a local timer connected to the PDSP core. 
 *      This timer has a programmable count based on the sub-system clock. When this count expires, 
 *      a local "tick" is registered in the firmware. The tick is used when timing channel interrupts 
 *      based on the "Timer Load Count" value supplied in the timer configuration.
 *
 *      The value of "Timer Constant" is the number of QM sub-system clocks divided by 2 that 
 *      comprise a single "tick" in the accumulator firmware.
 *
 *      For example, if the QM sub-system clock is 350MHz, and the desired firmware "tick" is 20us, 
 *      the proper Timer Constant for this command is computed as follows:
 *      
 *      Timer Constant = (350,000,000 cycles/sec) * (0.000020 sec) / (2 cycles)
 *      Timer Constant = 3,500
 *      
 *      The firmware initializes with a default constant value of 4375. This corresponds to firmware tick of 25us.
 * 
 *  @param[in]  pdspId
 *      Accumulator to program
 *          Qmss_PdspId_PDSP1 selects firmware downloaded to PDSP 1
 *          Qmss_PdspId_PDSP2 selects firmware downloaded to PDSP 2
 *
 *  @param[in]  timerConstant
 *      16 bit wide value calculated as explained above
 *
 *  @post  
 *      Timer constant is programmed and in use.
 *
 *  @retval
 *      Value reported by accumulator
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - Accumulator status - QMSS_ACC_IDLE, QMSS_ACC_INVALID_COMMAND   
 */

Qmss_Result Qmss_configureAccTimer (Qmss_PdspId pdspId, uint16_t timerConstant)
{

    volatile uint32_t   *reg;
    uint8_t             result;
    void                *key;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Point to the accumulator command register's last word */
    reg = (uint32_t *) ((uint8_t *) qmssLObj.qmPdspCmdReg[pdspId] + 4);
    *reg-- = timerConstant;
   
    *reg = Qmss_AccCmd_CONFIG_TIMER_CONSTANT << 8; 

    /* wait for the command to clear */
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    /* End Critical Section */
    Qmss_osalCsExit (key);

    return (Qmss_Result) (CSL_FEXTR (*reg, 31, 24));
}


/**
 *  @b Description
 *  @n  
 *      
 *      The Accumulator firmware also supports an optional reclamation queue which can be used for 
 *      packet discards. Any descriptor placed on the reclamation queue will be recycled in the 
 *      same manner as if it had been submitted to CDMA. 
 *
 *      The descriptor packet information field is used to determine the return queue and the 
 *      return handling, including options to unlink host descriptors and push to either the 
 *      front or the back of the return queue.
 *      Setting queue to zero disables the reclamation feature
 *
 *  @param[in]  pdspId
 *      Accumulator to program
 *          Qmss_PdspId_PDSP1 selects firmware downloaded to PDSP 1
 *          Qmss_PdspId_PDSP2 selects firmware downloaded to PDSP 2
 *
 *  @param[in]  hnd
 *      Reclamation queue number
 *
 *  @post  
 *      Reclamation queue is configured.
 *
 *  @retval
 *      Value reported by accumulator
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - Accumulator status - QMSS_ACC_IDLE, QMSS_ACC_INVALID_COMMAND
 */

Qmss_Result Qmss_programReclaimQueue (Qmss_PdspId pdspId, Qmss_QueueHnd hnd)
{
    volatile uint32_t   *reg;
    uint8_t             result;
    void                *key;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Point to the accumulator command register's last word */
    reg = (uint32_t *) ((uint8_t *) qmssLObj.qmPdspCmdReg[pdspId] + 4);
    *reg-- = hnd;
    *reg = Qmss_AccCmd_CONFIG_RECLAIM_QUEUE << 8;

    /* wait for the command to clear */
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    /* End Critical Section */
    Qmss_osalCsExit (key);

    return (Qmss_Result) (CSL_FEXTR (*reg, 31, 24));
}

/**
 *  @b Description
 *  @n  
 *      The Accumulator firmware supports an optional diversion queue which 
 *      can be used remotely cause a queue diversion. When enabled, any 
 *      descriptor placed on the diversion queue will be loaded, and its 
 *      Timestamp field will be written to the QM diversion register. 
 *      The descriptor pointer will then be pushed onto the diversion 
 *      completion queue.
 *
 *      Setting the diversion queue to �0� disables diversion queue
 *      monitoring. The firmware initializes with a default diversion 
 *      queue of �0� (disabled).
 *
 *  @param[in]  pdspId
 *      Accumulator to program
 *          Qmss_PdspId_PDSP1 selects firmware downloaded to PDSP 1
 *          Qmss_PdspId_PDSP2 selects firmware downloaded to PDSP 2
 *
 *  @param[in]  divQ
 *      Diversion queue handle.  This is the queue which is monitored for 
 *      diversion requests
 *
 *  @param[in]  divCompletionQ
 *      Return queue handle.  After diversion request is completed, the
 *      descriptor is returned in this queue.
 *
 *  @post  
 *      Diversion queue is configured.
 *
 *  @retval
 *      Value reported by accumulator
 *  @retval
 *      Success - QMSS_ACC_SOK
 *  @retval
 *      Failure - QMSS_NOT_INITIALIZED
 *  @retval
 *      Failure - QMSS_INVALID_PARAM
 *  @retval
 *      Failure - Accumulator status - QMSS_ACC_IDLE, QMSS_ACC_INVALID_COMMAND
 */

Qmss_Result Qmss_programDiversionQueue (Qmss_PdspId pdspId, Qmss_QueueHnd divQ,
                                        Qmss_QueueHnd divCompletionQ)
{
    volatile uint32_t   *reg;
    uint8_t             result;
    void                *key;

    if (qmssLObjIsValid == 0)
        return QMSS_NOT_INITIALIZED;

    /* Make sure queue numbers fit in bitfields.  This doesn't check if the
     * queue number exists in the system */
    if ((divQ > 0xffff) || (divCompletionQ > 0xffff)) {
        return QMSS_INVALID_PARAM;
    }

    /* Begin Critical Section before accessing shared resources. */
    key = Qmss_osalCsEnter ();

    /* Point to the accumulator command register's last word */
    reg = (uint32_t *) ((uint8_t *) qmssLObj.qmPdspCmdReg[pdspId] + 4);
    *reg-- = (divCompletionQ  << 16) | divQ;
    *reg = Qmss_AccCmd_CONFIG_DIVERSION_QUEUE << 8;

    /* wait for the command to clear */
    do
    {
        result = CSL_FEXTR (*reg, 15, 8);  
    } while (result != 0);

    /* End Critical Section */
    Qmss_osalCsExit (key);

    return (Qmss_Result) (CSL_FEXTR (*reg, 31, 24));
}

/**
@}
*/
