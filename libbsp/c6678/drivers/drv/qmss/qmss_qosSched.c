/**
 *   @file  qmss_qosSched.c
 *
 *   @brief   
 *      This file has API that configure the Queue Manager QoS packet 
 *      scheduler firmware.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2012, Texas Instruments, Inc.
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

/* QMSS LLD includes */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/qmss_qosSched.h>
#include <ti/drv/qmss/include/qmss_pvt.h>
#include <ti/drv/qmss/include/qmss_qos_sched_regs.h>

/* QMSS OSAL layer */
#include <ti/drv/qmss/qmss_osal.h>

/**********************************************************************
 ********************** Extern Variables ******************************
 **********************************************************************/

/* QMSS Local Object is valid flag */
extern uint8_t               qmssLObjIsValid;

/**********************************************************************
 ************************** Functions *********************************
 **********************************************************************/
/* Private function to create hw queue ID for QoS Firmware */
static int qmss_qos_sched_act_types_is_bytes (uint32_t *isBytes, Qmss_QosSchedAcctType type)
{
    switch (type)
    {
        case Qmss_QosSchedAcctType_PACKETS:
            *isBytes = 0;
            break;
        case Qmss_QosSchedAcctType_BYTES:
            *isBytes = 1;
            break;
        default:
            return 1;
    }

    return 0;
}
/* Private function to create enum from bytes flag */
static Qmss_QosSchedAcctType qmss_qos_sched_is_bytes_to_act_types(uint32_t isBytes)
{
    return isBytes ? Qmss_QosSchedAcctType_BYTES 
                   : Qmss_QosSchedAcctType_PACKETS;
}

static inline uint32_t qmss_convert_qmss_queue_to_bitfield (Qmss_Queue queue)
{
    /* Combine the Qmss_Queue structure into a number which can be 
     * passed to the QoS firmware.  The bitfield is the same as
     * a QueueHnd, but there is no SW requirement to keep QueueHnd
     * the same, hence this function.
     */
    return (uint32_t)Qmss_getQueueHandle (queue);
}

static inline Qmss_Queue qmss_convert_bitfield_to_qmss_queue (uint32_t bitfield)
{
    /* This is inverse of qmss_convert_qmss_queue_to_bitfield */
    return Qmss_getQueueNumber ((Qmss_QueueHnd)bitfield);
}

/* Private function to wait until command buf is ready */
static void qmss_wait_qos_sched_command_buf(void) 
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           result;

    do
    {
        result = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0, 
                           QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND);  
    } while (result != 0);
}

/* Private function to error check port, group, and queue */
static Qmss_Result qmss_qos_sched_check_args (int zeroIsBad, uint32_t port, uint32_t group, uint32_t queue)
{
    if (port > QMSS_QOS_SCHED_MAX_PHYS_PORTS) 
    {
        return QMSS_QOS_SCHED_INVALID_PORT;
    }

    if ((zeroIsBad) && (group == 0))
    {
        return QMSS_QOS_SCHED_INVALID_GROUP;
    }

    if ((zeroIsBad) && (queue == 0))
    {
        return QMSS_QOS_SCHED_INVALID_QUEUE;
    }

    if (port >= QMSS_QOS_SCHED_FULL_MAX_PHYS_PORTS)
    {
        if (group > QMSS_QOS_SCHED_LITE_MAX_LOG_GROUPS) 
        {
            return QMSS_QOS_SCHED_INVALID_GROUP;
        }

        if (queue > QMSS_QOS_SCHED_LITE_MAX_QUEUES_PER_GROUP) 
        {
            return QMSS_QOS_SCHED_INVALID_QUEUE;
        }
    }
    else
    {
        if (group > QMSS_QOS_SCHED_FULL_MAX_LOG_GROUPS) 
        {
            return QMSS_QOS_SCHED_INVALID_GROUP;
        }

        if (queue > QMSS_QOS_SCHED_FULL_MAX_QUEUES_PER_GROUP) 
        {
            return QMSS_QOS_SCHED_INVALID_QUEUE;
        }
    }
    return QMSS_QOS_SCHED_RETCODE_SUCCESS;
}

/** @addtogroup QMSS_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function is used to set the QoS ingress base queue number. The QoS ingress queue 
 *      are restricted to a set of 80 starting at a fixed base which must be a multiple of 32.
 *
 *      The queues are statically mapped to ports and groups.  Assuming there are 8 queues
 *      per group and 5 groups per port the assignment is:
 *
 *      <TABLE>
 *      <TR><TH>Port</TH><TH>Group</TH><TH>Assigned Queues</TH></TR>
 *      <TR><TD>0</TD><TD>0</TD><TD>base + 0 - 7</TD></TR>
 *      <TR><TD>0</TD><TD>1</TD><TD>base + 8 -15</TD></TR>
 *      <TR><TD>0</TD><TD>2</TD><TD>base + 16-23</TD></TR>
 *      <TR><TD>0</TD><TD>3</TD><TD>base + 24-31</TD></TR>
 *      <TR><TD>0</TD><TD>4</TD><TD>base + 32-39</TD></TR>
 *      <TR><TD>1</TD><TD>0</TD><TD>base + 40-47</TD></TR>
 *      <TR><TD>1</TD><TD>1</TD><TD>base + 48-55</TD></TR>
 *      <TR><TD>1</TD><TD>2</TD><TD>base + 56-63</TD></TR>
 *      <TR><TD>1</TD><TD>3</TD><TD>base + 64-71</TD></TR>
 *      <TR><TD>1</TD><TD>4</TD><TD>base + 72-79</TD></TR>
 *      </TABLE>
 *
 *      Each device has a block of queues reserved for this purpose.  It is defined in CSL
 *      as QMSS_TRAFFIC_SHAPING_QUEUE_BASE and QMSS_MAX_TRAFFIC_SHAPING_QUEUE.  
 *
 *      On some devices, the QMSS_MAX_TRAFFIC_SHAPING_QUEUE may be limited to something less
 *      than 80.  If more queues are needed, then the traffic shaping/QoS subsystem allows 
 *      any range of general purpose queues used.  In other words, 
 *      QMSS_TRAFFIC_SHAPING_QUEUE_BASE is just a suggestion.
 *
 *      If general queues are used, they should be reserved through @ref Qmss_queueOpen,
 *      similar to example code in test_descAlloc.c inside QOS_TEST.
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  queueNum
 *      Base queue number 
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QOS_SCHED_INVALID_COMMAND, QMSS_QOS_SCHED_INVALID_INDEX, QMSS_QOS_SCHED_INVALID_OPTION
 */
Qmss_Result Qmss_setQosSchedQueueBase (uint32_t queueNum)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           cmdBuf = 0;
    Qmss_Result        retVal;
    void              *key    = Qmss_osalCsEnter();

    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND, 
              QMSS_QOSSCHED_COMMAND_SET_QUEUE_BASE);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX, 
              queueNum);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_sched_command_buf ();

    retVal = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, 
                       QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE);

    Qmss_osalCsExit(key);

    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to get the QoS ingress base queue number. The QoS ingress queue 
 *      are restricted to a set of 32 starting at a fixed base which must be a multiple of 32.
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[out]  queueNum
 *      Base queue number 
 *
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QOS_SCHED_INVALID_COMMAND, QMSS_QOS_SCHED_INVALID_INDEX, QMSS_QOS_SCHED_INVALID_OPTION
 */
Qmss_Result Qmss_getQosSchedQueueBase (uint32_t *queueNum)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           cmdBuf = 0;
    Qmss_Result        retVal;
    void              *key    = Qmss_osalCsEnter();

    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND, 
              QMSS_QOSSCHED_COMMAND_GET_QUEUE_BASE);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_sched_command_buf ();

    *queueNum = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0, 
                          QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX);

    retVal = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, 
                       QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE);

    Qmss_osalCsExit(key);

    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure the QoS packet scheduler's credit interval timer
 *
 *  @param[in]  timerConstant
 *      The PDSP timer determines when credit is passed out. The timer is 
 *      configured by supplying a timer constant. The constant is computed as follows:
 *      
 *      timerConstant = (QMSS_Clock_Frequency * Desired_Interval) / 2
 *
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_COMMAND
 *                QMSS_QOS_SCHED_INVALID_INDEX
 *                QMSS_QOS_SCHED_INVALID_OPTION\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 */
Qmss_Result Qmss_configureQosSchedTimer (uint32_t timerConstant)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           cmdBuf = 0;
    Qmss_Result        retVal;
    void              *key    = Qmss_osalCsEnter();

    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND, 
              QMSS_QOSSCHED_COMMAND_TIMER_CONFIG);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX, 
              timerConstant);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_sched_command_buf ();

    retVal = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, 
                       QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE);

    Qmss_osalCsExit(key);

    return retVal;
}


/* Internal function to send a port specific command */
static Qmss_Result qmss_qos_sched_port_control (uint32_t port, uint32_t cmd, uint32_t option)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           cmdBuf = 0;
    Qmss_Result        retVal;
    void              *key;
        
    if (port > QMSS_QOS_SCHED_MAX_PHYS_PORTS) 
    {
        return QMSS_QOS_SCHED_INVALID_PORT;
    }

    key = Qmss_osalCsEnter();
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND, cmd);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_OPTION, option);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_INDEX, port);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_sched_command_buf ();

    retVal = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, 
                       QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE);

    Qmss_osalCsExit(key);

    return retVal;
}
/**
 *  @b Description
 *  @n  
 *      This function is used to enable a QoS port. 
 *
 *  @param[in]  port
 *      Port to enable - Valid range is 0 to 1
 *      
 *  @pre  
 *      The port's configuration must be performed by the host before calling this function.
 *
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_COMMAND
 *                QMSS_QOS_SCHED_INVALID_INDEX
 *                QMSS_QOS_SCHED_INVALID_OPTION\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 */
Qmss_Result Qmss_enableQosSchedPort (uint32_t port)
{
    return qmss_qos_sched_port_control (port, QMSS_QOSSCHED_COMMAND_ENABLE_PORT, 1);
}

/**
 *  @b Description
 *  @n  
 *      This function is used to disable a QoS scheduler port. Only disabled ports can be modified by the host.
 *      When a port is disabled all packet in the QoS queue contained in that port are discarded.
 *
 *  @param[in]  port
 *      Port to disable - Valid range is 0 to 1
 *      
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_COMMAND
 *                QMSS_QOS_SCHED_INVALID_INDEX
 *                QMSS_QOS_SCHED_INVALID_OPTION\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 */
Qmss_Result Qmss_disableQosSchedPort (uint32_t port)
{
    return qmss_qos_sched_port_control (port, QMSS_QOSSCHED_COMMAND_ENABLE_PORT, 0);
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure a QoS scheduler port.  The configuration
 *      operates on a shadow configuration such that the configuration can occur
 *      whether or not the port is enabled.  
 *
 *      The entire configuration must be specified.  If unknown the previous 
 *      config can be retrieved with @ref Qmss_getCfgQosSchedPort then modified.
 *
 *      This function copies the new configuration to the shadow area, then
 *      sends a command to the firmware to accept the change.  This minimizes
 *      network downtime during reconfiguration.
 *
 *      If the queues or groups are renumbered by changing counts, the descriptors
 *      are left in the original queues.  Thus if renumbering is needed,
 *      it is suggested to disable the port before reconfiguring.  If
 *      only the credits are changed, the reconfiguration should be
 *      done with the port enabled.
 *
 *  @param[in]  port
 *      Port to configure - Valid range is 0 to @ref QMSS_QOS_SCHED_MAX_PHYS_PORTS
 *
 *  @param[in]  cfg
 *      Port configuration parameters
 *      
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_INDEX\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 *                QMSS_QOS_SCHED_INVALID_GROUP
 *                QMSS_QOS_SCHED_INVALID_QUEUE
 *                QMSS_QOS_SCHED_INVALID_WRRTYPE
 *                QMSS_QOS_SCHED_INVALID_CIRTYPE
 *                QMSS_QOS_SCHED_INVALID_CONGTYPE
 *                QMSS_QOS_SCHED_INVALID_QUEUE_NUM
 */
Qmss_Result Qmss_putCfgQosSchedPort (uint32_t port, Qmss_QosSchedPortCfg *cfg)
{
    Qmss_QosSchedRegs      *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    Qmss_QosSchedPortRegs  *pShadowReg;
    Qmss_QosSchedGroupCfg  *pGroupCfg;
    Qmss_QosSchedGroupRegs *pGroupReg;
    Qmss_QosSchedQueueCfg  *pQueueCfg;
    Qmss_QosSchedQueueRegs *pQueueReg;
    uint32_t                wrrIsBytes, cirIsBytes, congIsBytes;
    uint32_t                temp;
    int                     group, queue;
    Qmss_Result             retVal;

    /* Check input arguments */
    if ((retVal = qmss_qos_sched_check_args (1, port, cfg->groupCount, 1)) != 
        QMSS_QOS_SCHED_RETCODE_SUCCESS)
    {
        return retVal;
    }
    
    if (qmss_qos_sched_act_types_is_bytes (&wrrIsBytes, cfg->wrrType))
    {
        return QMSS_QOS_SCHED_INVALID_WRRTYPE;
    }

    if (qmss_qos_sched_act_types_is_bytes (&cirIsBytes, cfg->cirType))
    {
        return QMSS_QOS_SCHED_INVALID_CIRTYPE;
    }
    
    if (qmss_qos_sched_act_types_is_bytes (&congIsBytes, cfg->congestionType))
    {
        return QMSS_QOS_SCHED_INVALID_CONGTYPE;
    }

    if ((0xffffffffu - cfg->cirMax) < cfg->cirIteration)
    {
        /* cfg->cirMax + cfg->cirIteration is an overflow */
        return QMSS_QOS_SCHED_INVALID_CIR_MAX;
    }

    /* Check group configuration */
    for (group = 0; group < cfg->groupCount; group++) 
    {
        pGroupCfg = &cfg->group[group];

        if ((retVal = qmss_qos_sched_check_args (1, port, cfg->groupCount, pGroupCfg->totQueueCount)) != 
            QMSS_QOS_SCHED_RETCODE_SUCCESS)
        {
            return retVal;
        }

        if ((pGroupCfg->spQueueCount + pGroupCfg->wrrQueueCount) > pGroupCfg->totQueueCount)
        {
            return QMSS_QOS_SCHED_INVALID_QUEUE;
        }

        if ((0xffffffffu - pGroupCfg->cirMax) < pGroupCfg->cirIteration)
        {
            /* pGroupCfg->cirMax + pGroupCfg->cirIteration is an overflow */
            return QMSS_QOS_SCHED_INVALID_CIR_MAX;
        }

        if ((0xffffffffu - pGroupCfg->pirMax) < pGroupCfg->pirIteration)
        {
            /* pGroupCfg->pirMax + pGroupCfg->pirIteration is an overflow */
            return QMSS_QOS_SCHED_INVALID_PIR_MAX;
        }
    }

    /* Program the port */
    pShadowReg = &regPtr->PORT_SHADOW;
    temp = 0;
    CSL_FINS (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_WRR_IS_BYTES,
              wrrIsBytes);
    CSL_FINS (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CIR_IS_BYTES,
              cirIsBytes);
    CSL_FINS (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CONG_THRESH_IS_BYTES,
              congIsBytes);
    CSL_FINS (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_NUM_GROUPS,
              cfg->groupCount);
    CSL_FINS (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_OUT_QNUM_QMGR,
              qmss_convert_qmss_queue_to_bitfield (cfg->outputQueue));

    pShadowReg->PORT_FLAGS           = temp;
    pShadowReg->CIR_ITERATION_CREDIT = cfg->cirIteration;
    pShadowReg->CIR_MAX              = cfg->cirMax;

    /* Program the groups */
    for (group = 0; group < cfg->groupCount; group++) 
    {
        pGroupCfg = &cfg->group[group];
        pGroupReg = &pShadowReg->GROUPS[group];

        temp = 0;
        CSL_FINS (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_TOT,
                  pGroupCfg->totQueueCount);
        CSL_FINS (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_SP,
                  pGroupCfg->spQueueCount);
        CSL_FINS (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_WRR,
                  pGroupCfg->wrrQueueCount);

        pGroupReg->QUEUE_COUNTS       = temp;
        pGroupReg->CIR_ITERATION      = pGroupCfg->cirIteration;
        pGroupReg->PIR_ITERATION      = pGroupCfg->pirIteration;
        pGroupReg->CIR_MAX            = pGroupCfg->cirMax;
        pGroupReg->PIR_MAX            = pGroupCfg->pirMax;
        pGroupReg->WRR_INITIAL_CREDIT = pGroupCfg->wrrInitialCredit;

        /* Program the queues */
        for (queue = 0; queue < pGroupCfg->totQueueCount; queue++) 
        {
            pQueueCfg = &pGroupCfg->Queue[queue];
            pQueueReg = &pGroupReg->QUEUE[queue];

            pQueueReg->WRR_INITIAL_CREDIT = pQueueCfg->wrrInitialCredit;;
            pQueueReg->CONGESTION_THRESH  = pQueueCfg->congestionThresh;
        }
    }

    /* Now that the configuration is written, tell the FW to commit it */
    return qmss_qos_sched_port_control (port, QMSS_QOSSCHED_COMMAND_SHADOW_PORT, 1);
}

/**
 *  @b Description
 *  @n  
 *      This function is used to retrieve the current configuration of a QoS
 *      scheduler port.  It sends a command to the firmware which
 *      copies the configuration from the active area to the shadow area
 *      (the active area continues to run!)
 *
 *      This function can be used either for diagnostic purposes to verify
 *      the configuration, or to query the current configuration, modify
 *      it, and put it back using @ref Qmss_putCfgQosSchedPort.
 *
 *  @param[in]  port
 *      Port to query - Valid range is 0 to 1
 *
 *  @param[out]  cfg
 *      Port configuration parameters
 *      
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure - 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_INDEX\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 */
Qmss_Result Qmss_getCfgQosSchedPort (uint32_t port, Qmss_QosSchedPortCfg *cfg)
{
    Qmss_QosSchedRegs      *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    Qmss_QosSchedPortRegs  *pShadowReg;
    Qmss_QosSchedGroupCfg  *pGroupCfg;
    Qmss_QosSchedGroupRegs *pGroupReg;
    Qmss_QosSchedQueueCfg  *pQueueCfg;
    Qmss_QosSchedQueueRegs *pQueueReg;
    uint32_t                temp;
    int                     group, queue;
    Qmss_Result             retVal;

    /* Request firmware to copy port's config to shadow */
    retVal = qmss_qos_sched_port_control (port, QMSS_QOSSCHED_COMMAND_SHADOW_PORT, 0);
    if (retVal != QMSS_QOS_SCHED_RETCODE_SUCCESS) 
    {
        return retVal;
    }

    /* Convert from registers to *cfg */
    memset (cfg, 0, sizeof(*cfg));

    pShadowReg          = &regPtr->PORT_SHADOW;
    temp                = pShadowReg->PORT_FLAGS;
    cfg->wrrType        = qmss_qos_sched_is_bytes_to_act_types(
                              CSL_FEXT (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_WRR_IS_BYTES));
    cfg->cirType        = qmss_qos_sched_is_bytes_to_act_types(
                              CSL_FEXT (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CIR_IS_BYTES));
    cfg->congestionType = qmss_qos_sched_is_bytes_to_act_types(
                              CSL_FEXT (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_CONG_THRESH_IS_BYTES));
    cfg->groupCount     = CSL_FEXT (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_NUM_GROUPS);
    cfg->outputQueue    = qmss_convert_bitfield_to_qmss_queue (
                              CSL_FEXT (temp, QMSS_QOSSCHED_PORT_REGS_PORT_FLAGS_OUT_QNUM_QMGR));

    cfg->cirIteration   = pShadowReg->CIR_ITERATION_CREDIT;
    cfg->cirMax         = pShadowReg->CIR_MAX;

    /* Convert the groups */
    for (group = 0; group < cfg->groupCount; group++) 
    {
        pGroupCfg = &cfg->group[group];
        pGroupReg = &pShadowReg->GROUPS[group];

        temp                     = pGroupReg->QUEUE_COUNTS;
        pGroupCfg->totQueueCount = CSL_FEXT (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_TOT);
        pGroupCfg->spQueueCount  = CSL_FEXT (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_SP);
        pGroupCfg->wrrQueueCount = CSL_FEXT (temp, QMSS_QOSSCHED_GROUP_REGS_QUEUE_COUNTS_WRR);

        pGroupCfg->cirIteration     = pGroupReg->CIR_ITERATION;
        pGroupCfg->pirIteration     = pGroupReg->PIR_ITERATION;
        pGroupCfg->cirMax           = pGroupReg->CIR_MAX;
        pGroupCfg->pirMax           = pGroupReg->PIR_MAX;
        pGroupCfg->wrrInitialCredit = pGroupReg->WRR_INITIAL_CREDIT;

        /* Program the queues */
        for (queue = 0; queue < pGroupCfg->totQueueCount; queue++) 
        {
            pQueueCfg = &pGroupCfg->Queue[queue];
            pQueueReg = &pGroupReg->QUEUE[queue];

            pQueueCfg->wrrInitialCredit = pQueueReg->WRR_INITIAL_CREDIT;
            pQueueCfg->congestionThresh = pQueueReg->CONGESTION_THRESH;
        }
    }

    return QMSS_QOS_SCHED_RETCODE_SUCCESS;
}

/**
 *  @b Description
 *  @n  
 *      This function returns the bytes and packets forwarded and discarded by the
 *      port.  The statistics are synchronously and atomically collected and reset.
 *
 *  @param[out]  stats
 *      Statistics structure to contain returned stats.  NULL is valid in order to
 *      reset and discard the stats.
 *
 *  @param[in]  port
 *      Port to request stats - Valid range is 0 to 1
 *
 *  @param[in]  group
 *      Group to request stats - Valid range is 0 to 4
 *
 *  @param[in]  queue
 *      Queue in group to request stats - Valid range is 0 to 7
 *
 *  @param[in]  which_reset
 *      Bitfield listing the stats to reset.  Can be one or more of
 *         QMSS_QOS_SCHED_STATS_FORWARDED_BYTES   |
 *         QMSS_QOS_SCHED_STATS_FORWARDED_PACKETS |
 *         QMSS_QOS_SCHED_STATS_DISCARDED_BYTES   |
 *         QMSS_QOS_SCHED_STATS_DISCARDED_PACKETS
 *
 *  @retval
 *      Success - QMSS_QOS_SCHED_RETCODE_SUCCESS
 *  @retval
 *      Failure 
 *             The following errors are detected by the firmware:
 *                QMSS_QOS_SCHED_INVALID_INDEX\n
 *             The following errors are detected by the LLD:
 *                QMSS_QOS_SCHED_INVALID_PORT
 *                QMSS_QOS_SCHED_INVALID_GROUP
 *                QMSS_QOS_SCHED_INVALID_QUEUE
 */
Qmss_Result Qmss_getQosSchedStats (Qmss_QosSchedStats *stats, uint32_t port, uint32_t group, uint32_t queue, uint32_t which_reset)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t           cmdBuf = 0;
    Qmss_Result        retVal;
    void              *key;

    /* Clear stats in case of error */
    memset (stats, 0, sizeof(*stats));

    if ((retVal = qmss_qos_sched_check_args (0, port, group, queue)) != 
        QMSS_QOS_SCHED_RETCODE_SUCCESS)
    {
        return retVal;
    }

    key = Qmss_osalCsEnter();

    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_COMMAND, 
              QMSS_QOSSCHED_COMMAND_STATS_REQUEST);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_BUFFER_WORD0_OPTION, 
              which_reset);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_INDEX_PORT, 
              port);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_INDEX_GROUP, 
              group);
    CSL_FINS (cmdBuf, QMSS_QOSSCHED_COMMAND_INDEX_QUEUE, 
              queue);

    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_sched_command_buf ();

    retVal = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, 
                       QMSS_QOSSCHED_COMMAND_BUFFER_WORD1_RETURN_CODE);

    if (stats && (retVal == QMSS_QOS_SCHED_RETCODE_SUCCESS))
    {
       /* Must read the results inside a critical section since more than one 
        * core could ask for stats at the same time
        */
       stats->bytesForwarded   = (((uint64_t)regPtr->STATS.BYTES_FORWARDED_MSW) << 32) |
                                 (((uint64_t)regPtr->STATS.BYTES_FORWARDED_LSW)      );
       stats->bytesDiscarded   = (((uint64_t)regPtr->STATS.BYTES_DISCARDED_MSW) << 32) |
                                 (((uint64_t)regPtr->STATS.BYTES_DISCARDED_LSW)      );
       stats->packetsForwarded = regPtr->STATS.PACKETS_FORWARDED;
       stats->packetsDiscarded = regPtr->STATS.PACKETS_DISCARDED;
    }

    Qmss_osalCsExit(key);

    return retVal;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the version information of the current
 *      QoS scheduler firmware running on the PDSP.
 *
 *  @retval
 *      Version Information.
 */
uint32_t Qmss_getQosSchedFwVersion (void)
{
    Qmss_QosSchedRegs *regPtr = (Qmss_QosSchedRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];

    return regPtr->VERSION;
}


/**
@}
*/

