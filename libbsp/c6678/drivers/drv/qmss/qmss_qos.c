/**
 *   @file  qmss_qos.c
 *
 *   @brief   
 *      This file has API that configure the Queue Manager QoS PDSP firmware.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2012, Texas Instruments, Inc.
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
#include <ti/drv/qmss/qmss_qos.h>
#include <ti/drv/qmss/include/qmss_pvt.h>

/* QMSS OSAL layer */
#include <ti/drv/qmss/qmss_osal.h>

/* CSL RL includes */
#include <ti/drv/qmss/include/qmss_qos_regs.h>

/* RM LLD includes */
#include <ti/drv/rm/rm_public_lld.h>
      
/**********************************************************************
 ********************** Extern Variables ******************************
 **********************************************************************/

/* QMSS Local Object is valid flag */
extern uint8_t               qmssLObjIsValid;

/**********************************************************************
 ************************** Functions *********************************
 **********************************************************************/
/* Private function to create hw queue ID for QoS Firmware */
static inline uint32_t qmss_convert_qmss_queue_to_bitfield (Qmss_Queue queue)
{
  /* Combine the Qmss_Queue structure into a number which can be 
   * passed to the QoS firmware.  The bitfield is the same as
   * a QueueHnd, but there is no SW requirement to keep QueueHnd
   * the same, hence this function.
   */
  return (uint32_t)Qmss_getQueueHandle (queue);
}

/* Private function to wait until command buf is ready */
static void qmss_wait_qos_command_buf(void) 
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                result;

    do
    {
        result = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND);  
    } while (result != 0);
}


/** @addtogroup QMSS_LLD_FUNCTION
@{ 
*/

/**
 *  @b Description
 *  @n  
 *      This function is used to set the QoS ingress base queue number. The QoS ingress queue 
 *      are restricted to a set of 64 starting at a fixed base which must be a multiple of 32.
 *
 *      Each device has a block of queues reserved for this purpose.  It is defined in CSL
 *      as QMSS_TRAFFIC_SHAPING_QUEUE_BASE and QMSS_MAX_TRAFFIC_SHAPING_QUEUE.  
 *
 *      On some devices, the QMSS_MAX_TRAFFIC_SHAPING_QUEUE may be limited to something less
 *      than 64.  If more queues are needed, then the traffic shaping/QoS subsystem allows 
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
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION,
 *                                                                   QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_setQosQueueBase (uint32_t queueNum)
{
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      
    
    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_QUEUE;  
        resourceData.res_info.qosQueue = queueNum;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
  
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                 cmdBuf = 0;

    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_SET_QUEUE_BASE);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, queueNum);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
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
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION
 */
Qmss_Result Qmss_getQosQueueBase (uint32_t *queueNum)
{
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];

    /* No RM check since readonly operation */
        
    CSL_FINS (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, 
                    QMSS_QCMD_GET_QUEUE_BASE);

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    *queueNum = CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX);
    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure the QoS credit interval timer
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  timerConstant
 *      The PDSP timer determines when credit is passed out. The timer is 
 *      configured by supplying a timer constant. The constant is computed as follows:
 *      
 *      timerConstant = (QMSS_Clock_Frequency * Desired_Interval) / 2
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION,
 *                                                                   QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_configureQosTimer (uint32_t timerConstant)
{
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                  cmdBuf = 0;
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;     

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_TIMER;  
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
            
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_TIMER_CONFIG);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, timerConstant);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to enable a QoS cluster. 
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to enable - Valid range is 0 to 7
 *      
 *  @pre  
 *      The cluster and queue configuration must be performed by the host before calling this function.
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION,
 *                                                                   QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_enableQosCluster (uint32_t clusterIndex)
{
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      
    
    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_CLUSTER;  
        resourceData.res_info.qosCluster = clusterIndex;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
  
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                  cmdBuf = 0;
        
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_ENABLE_CLUSTER);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION, 1);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, clusterIndex);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to disable a QoS cluster. Only disabled clusters can be modified by the host.
 *      When a cluster is disabled all packet in the QoS queue contained in that cluster are discarded.
 *
 *      **No validation is done on the input parameters**.
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to disable - Valid range is 0 to 7
 *      
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION,
 *                                                                   QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_disableQosCluster (uint32_t clusterIndex)
{
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      
    
    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_CLUSTER;  
        resourceData.res_info.qosCluster = clusterIndex;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
  
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                  cmdBuf = 0;
        
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_ENABLE_CLUSTER);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION, 0);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, clusterIndex);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure a QoS queue. Only disabled clusters can be modified by the host.
 *      When a cluster is disabled all packet in the QoS queue contained in that cluster are discarded.
 *
 *  @param[in]  queIndex
 *      Index of the queue to configure - Valid range is 0 to 31
 *      
 *  @param[in]  cfg
 *      Queue configuration parameters
 *
 *  @pre  
 *      The cluster must be disabled using Qmss_disableQosCluster() API before calling this function.
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX
 *  @retval
 *      Failure - QMSS_RESOURCE_INIT_DENIED
 */

Qmss_Result Qmss_configureQosQueue (uint32_t queIndex, Qmss_QosQueueCfg *cfg)
{
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;      
    
    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_QUEUE;  
        resourceData.res_info.qosQueue = queIndex;
        
        if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_INIT_DENIED;
        }
    }
  
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
        
    if (queIndex > QMSS_QOS_MAX_QUEUES)
        return QMSS_QCMD_INVALID_INDEX;

    CSL_FINS (regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD0, QM_QOS_PDSP_QOS_QUEUE_WORD0_EGRESS_QUEUE, cfg->egressQueNum);
    CSL_FINS (regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD0, QM_QOS_PDSP_QOS_QUEUE_WORD0_ITERATION_CREDIT, cfg->iterationCredit);
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD1 = 0;
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD2 = cfg->maxCredit;
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD3 = cfg->congestionThreshold;
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD4 = 0;
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD5 = 0;

    return QCMD_RETCODE_SUCCESS;
}

/* Internal function to configure modified token bucket clusters */
Qmss_Result Qmss_configureQosClusterTB (uint32_t clusterIndex, Qmss_QosClusterCfgTB *cfg)
{
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
        
    if (clusterIndex > QMSS_QOS_MAX_CLUSTERS)
        return QMSS_QCMD_INVALID_INDEX;

    /* Specify it is a modified token bucket */
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD5_FLAGS, 0);

    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD1 = cfg->maxGlobalCredit;

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_COUNT, 
                    cfg->qosQueCnt);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_0, 
                    cfg->qosQueNum[0]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_1, 
                    cfg->qosQueNum[1]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_2,
                    cfg->qosQueNum[2]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_3, 
                    cfg->qosQueNum[3]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_4, 
                    cfg->qosQueNum[4]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_5, 
                    cfg->qosQueNum[5]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_6,
                    cfg->qosQueNum[6]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_7, 
                    cfg->qosQueNum[7]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_8, 
                    cfg->qosQueNum[8]);

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, 
                    QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_REALTIME_FLAGS, cfg->qosQueRTFlags);
    
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, 
                    QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_COUNT, cfg->egressQueCnt);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_0, 
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[0]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD6, QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_1,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[1]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD6, QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_2,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[2]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD7, QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_3,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[3]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD7, QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_4,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[4]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD8, QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_5,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[5]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD8, QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_6, 
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[6]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD9, QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_7,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[7]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD9, QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_8,
                    qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[8]));

    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD10 = cfg->egressCongestionThreshold1;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD11 = cfg->egressCongestionThreshold2;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD12 = cfg->egressCongestionThreshold3;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD13 = cfg->egressCongestionThreshold4;

    return QCMD_RETCODE_SUCCESS;
}

/* Internal function to configure round robin clusters */
Qmss_Result Qmss_configureQosClusterRR (uint32_t clusterIndex, Qmss_QosClusterCfgRR *cfg)
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
        
    /* Round robin is only available on cluster 7 */
    if (clusterIndex != 7)
        return QMSS_QCMD_INVALID_INDEX;

    /* Validate params */
    if ( (cfg->qosQueHighCnt != 4)     ||
         (cfg->qosQueNumHigh[0] != 56) ||
         (cfg->qosQueNumHigh[1] != 57) ||
         (cfg->qosQueNumHigh[2] != 58) ||
         (cfg->qosQueNumHigh[3] != 59) ) 
    {
        return QMSS_QCMD_INVALID_RR_HIGH_Q;
    }

    if ( (cfg->qosQueLowCnt != 4)     ||
         (cfg->qosQueNumLow[0] != 60) ||
         (cfg->qosQueNumLow[1] != 61) ||
         (cfg->qosQueNumLow[2] != 62) ||
         (cfg->qosQueNumLow[3] != 63) ) 
    {
        return QMSS_QCMD_INVALID_RR_LOW_Q;
    }
    
    if (cfg->egressQueCnt != 1)
    {
        return QMSS_QCMD_INVALID_RR_EGRESS_Q;
    }
    
    /* Specify it is a round robin cluster */
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD5_FLAGS, 1);

    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD1 = cfg->maxGlobalCredit;

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_COUNT, 
              cfg->qosQueHighCnt + cfg->qosQueLowCnt);

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_0, 
              cfg->qosQueNumHigh[0]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_1, 
              cfg->qosQueNumHigh[1]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD2, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD2_QOS_QUEUE_2,
              cfg->qosQueNumHigh[2]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_3, 
              cfg->qosQueNumHigh[3]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_4, 
              cfg->qosQueNumLow[0]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_5, 
              cfg->qosQueNumLow[1]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD3, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD3_QOS_QUEUE_6,
              cfg->qosQueNumLow[2]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_7, 
              cfg->qosQueNumLow[3]);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_8, 
              0);

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD4, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD4_QOS_QUEUE_REALTIME_FLAGS, 
              cfg->sizeAdjust);
    
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_COUNT, 
              cfg->egressQueCnt);

    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD5, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD5_EGRESS_QUEUE_0, 
              qmss_convert_qmss_queue_to_bitfield (cfg->egressQueNum[0]));
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD6, 
              QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_1,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD6,
              QM_QOS_PDSP_QOS_CLUSTER_WORD6_EGRESS_QUEUE_2,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD7,
              QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_3,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD7,
              QM_QOS_PDSP_QOS_CLUSTER_WORD7_EGRESS_QUEUE_4,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD8,
              QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_5,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD8,
              QM_QOS_PDSP_QOS_CLUSTER_WORD8_EGRESS_QUEUE_6, 
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD9,
              QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_7,
              0);
    CSL_FINS (regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD9,
              QM_QOS_PDSP_QOS_CLUSTER_WORD9_EGRESS_QUEUE_8,
              0);

    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD10 = cfg->iterationCredit;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD11 = cfg->maxEgressBacklog;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD12 = cfg->queueDisableMask;
    regPtr->QOS_CLUSTER[clusterIndex].QOS_CLUSTER_WORD13 = 0;

    return QCMD_RETCODE_SUCCESS;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure a QoS cluster. Only disabled clusters can be modified by the host.
 *      When a cluster is disabled all packet in the QoS queue contained in that cluster are discarded.
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to configure - Valid range is 0 to 7
 *
 *  @param[in]  cfg
 *      cluster configuration parameters
 *      
 *  @pre  
 *      The cluster must be disabled using Qmss_disableQosCluster() API before calling this function.
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX
 *                QMSS_QCMD_INVALID_MODE
 *                QMSS_QCMD_INVALID_RR_HIGH_Q
 *                QMSS_QCMD_INVALID_RR_LOW_Q
 *                QMSS_QCMD_INVALID_RR_EGRESS_Q
 *                QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_configureQosCluster (uint32_t clusterIndex, Qmss_QosClusterCfg *cfg)
{
  Qmss_Result retVal = QMSS_QCMD_INVALID_MODE;
  Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
  Rm_ResourceInfo resourceData;      
  
  if (rmPermCallTbl)
  {
      resourceData.resourceType = Rm_resource_QMSS_QOS_CLUSTER;  
      resourceData.res_info.qosCluster = clusterIndex;
      
      if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
      {
          return QMSS_RESOURCE_INIT_DENIED;
      }
  }  

  switch (cfg->mode) {
    case Qmss_QosMode_TokenBucket:
      retVal = Qmss_configureQosClusterTB (clusterIndex, &cfg->u.cfgTB);
      break;
    case Qmss_QosMode_RoundRobin:
      retVal = Qmss_configureQosClusterRR (clusterIndex, &cfg->u.cfgRR);
      break;
    default:
      break;
  }

  return retVal;
}
/**
 *  @b Description
 *  @n  
 *      This function is used to get the number of packets forwarded to the ergress queue
 *
 *  @param[in]  queIndex
 *      Index of the queue whose stats are to be read - Valid range is 0 to 31 
 *
 *  @retval
 *      Success - Forwarded packet count
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX
 */
Qmss_Result Qmss_getQosQueueForwardPktStats (uint32_t queIndex)
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];

    /* No RM check since readonly operation */

    if (queIndex > QMSS_QOS_MAX_QUEUES)
        return QMSS_QCMD_INVALID_INDEX;
        
    return (regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD4);
}

/**
 *  @b Description
 *  @n  
 *      This function is used to get the number of packets dropped due to congestion
 *
 *  @param[in]  queIndex
 *      Index of the queue whose stats are to be read - Valid range is 0 to 31
 *
 *  @retval
 *      Success - Dropped packet count
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX
 */
Qmss_Result Qmss_getQosQueueDroppedPktStats (uint32_t queIndex)
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];

    /* No RM check since readonly operation */
  
    if (queIndex > QMSS_QOS_MAX_QUEUES)
        return QMSS_QCMD_INVALID_INDEX;
        
    return (regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD5);
}

/**
 *  @b Description
 *  @n  
 *      This function is used to reset the statistics(Forwarded and Dropped packet count) of a queue
 *
 *  @param[in]  queIndex
 *      Index of the queue whose stats are to be reset - Valid range is 0 to 31
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX
 *  @retval
 *      Failure - QMSS_RESOURCE_INIT_DENIED
 */
Qmss_Result Qmss_resetQosQueueStats (uint32_t queIndex)
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) qmssLObj.qmRmHandle;
    Rm_ResourceInfo resourceData;  

    if (rmPermCallTbl)
    {
        resourceData.resourceType = Rm_resource_QMSS_QOS_QUEUE;  
        resourceData.res_info.qosQueue = queIndex;
        
        if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED)
        {
            return QMSS_RESOURCE_USE_DENIED;
        }
    }

    if (queIndex > QMSS_QOS_MAX_QUEUES)
        return QMSS_QCMD_INVALID_INDEX;
        
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD4 = 0;
    regPtr->QOS_QUEUE[queIndex].QOS_QUEUE_WORD5 = 0;

    return QCMD_RETCODE_SUCCESS;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to configure the SRIO tracking cluster.  The cluster must
 *      be disabled before calling this function.
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to configure - Valid value is 0
 *
 *  @param[in]  cfg
 *      cluster configuration parameters
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_SRIO_NUM_Q
 */
Qmss_Result Qmss_configureQosSrioCluster(uint32_t clusterIndex, Qmss_QosSrioCfg *cfg)
{
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t scratch;
    int q;

    if (clusterIndex != 0)
    {
        return QMSS_QCMD_INVALID_INDEX;
    }

    if ((cfg->queCount < QMSS_QOS_SRIO_MIN_TX_Q) ||
        (cfg->queCount > QMSS_QOS_SRIO_MAX_TX_Q))
    {
        return QMSS_QCMD_INVALID_SRIO_NUM_Q;
    }
        
    scratch = 0;
    CSL_FINS (scratch, QM_QOS_PDSP_SRIO_QCNT_QBASE_QCNT, cfg->queCount);
    CSL_FINS (scratch, QM_QOS_PDSP_SRIO_QCNT_QBASE_QBASE, cfg->queBase);
    regPtr->SRIO_CLUSTER.QCNT_QBASE = scratch;

    for (q = 0; q < cfg->queCount; q++)
    {
      scratch = 0;
      CSL_FINS (scratch, QM_QOS_PDSP_SRIO_TXQS_HWTXQ, 
		         qmss_convert_qmss_queue_to_bitfield (cfg->TXQs[q].txQ));
      CSL_FINS (scratch, QM_QOS_PDSP_SRIO_TXQS_THRESHOLD, cfg->TXQs[q].threshold);
      regPtr->SRIO_CLUSTER.TXQS[q] = scratch;
    }

    return QCMD_RETCODE_SUCCESS;
}

/**
 *  @b Description
 *  @n  
 *      This function is used to enable a QoS SRIO cluster. 
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to enable - Valid value is 0
 *      
 *  @pre  
 *      The cluster and queue configuration must be performed by the host before calling this function.
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION
 */
Qmss_Result Qmss_enableQosSrioCluster (uint32_t clusterIndex)
{
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                  cmdBuf = 0;
        
    /* Only one cluster is supported */
    if (clusterIndex != 0) 
    {
        return QMSS_QCMD_INVALID_INDEX;
    }

    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_SRIO_ENABLE);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION, 1);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, 0);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}

/**
 *  @b Description
 *  @n  
 *      This function is used to disable a QoS SRIO cluster. 
 *
 *  @param[in]  clusterIndex
 *      Index of the cluster to enable - Valid value is 0
 *      
 *  @pre  
 *      The cluster and queue configuration must be performed by the host before calling this function.
 *
 *  @retval
 *      Success - QCMD_RETCODE_SUCCESS
 *  @retval
 *      Failure - Command buffer return status - QMSS_QCMD_INVALID_COMMAND, QMSS_QCMD_INVALID_INDEX, QMSS_QCMD_INVALID_OPTION
 */
Qmss_Result Qmss_disableQosSrioCluster(uint32_t clusterIndex)
{
    /* No validation of input parameters is done */
    CSL_Qm_qos_pdspRegs     *regPtr = (CSL_Qm_qos_pdspRegs *) qmssLObj.qmPdspCmdReg[QMSS_QOS_PDSP_NUM];
    uint32_t                  cmdBuf = 0;
        
    /* Only one cluster is supported */
    if (clusterIndex != 0) 
    {
        return QMSS_QCMD_INVALID_INDEX;
    }

    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_COMMAND, QMSS_QCMD_SRIO_ENABLE);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_OPTION, 0);
    CSL_FINS (cmdBuf, QM_QOS_PDSP_COMMAND_BUFFER_WORD0_INDEX, 0);
    regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD0 = cmdBuf;

    /* Wait till command is executed */
    qmss_wait_qos_command_buf ();

    return (CSL_FEXT (regPtr->COMMAND_BUFFER.COMMAND_BUFFER_WORD1, QM_QOS_PDSP_COMMAND_BUFFER_WORD1_RETURN_CODE));
}
/**
@}
*/

