/**
 *   @file  csl_edma3GetHwChannelStatus.c
 *
 *   @brief   
 *      The file implements the API which is used to retreive the status of
 *      an opened EDMA Channel.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2002, 2003, 2004, 2005, 2008, 2009, Texas Instruments, Inc.
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

#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>

/** ============================================================================
 *   @n@b CSL_edma3GetHwChannelStatus
 *
 *   @b Description
 *   @n The function is to used to get the status of entities specific to an
 *      EDMA Channel. The channel specific entity to be queried is specified in 
 *      the argument.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            myQuery         Query to be performed
            response        Pointer to buffer to return the data requested by
                            the query passed 
     @endverbatim     
 *
 *   @sa
 *      CSL_Edma3HwChannelStatusQuery
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVQUERY (The query passed is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen()
 *       must be called successfully in that order before this API can be 
 *       invoked.If a Shadow region is used then care of the DRAE settings
 *       must be taken
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n TPCC_TPCC_ER_E0;TPCC_TPCC_ER_E1;TPCC_TPCC_ER_E2;TPCC_TPCC_ER_E3;
 *      TPCC_TPCC_ER_E4;TPCC_TPCC_ER_E5;TPCC_TPCC_ER_E6;TPCC_TPCC_ER_E7;
 *      TPCC_TPCC_ER_E8;TPCC_TPCC_ER_E9;TPCC_TPCC_ER_E10;TPCC_TPCC_ER_E11;
 *      TPCC_TPCC_ER_E12;TPCC_TPCC_ER_E13;TPCC_TPCC_ER_E14;TPCC_TPCC_ER_E15;
 *      TPCC_TPCC_ER_E16;TPCC_TPCC_ER_E17;TPCC_TPCC_ER_E18;TPCC_TPCC_ER_E19;
 *      TPCC_TPCC_ER_E20;TPCC_TPCC_ER_E21;TPCC_TPCC_ER_E22;TPCC_TPCC_ER_E23;
 *      TPCC_TPCC_ER_E24;TPCC_TPCC_ER_E25;TPCC_TPCC_ER_E26;TPCC_TPCC_ER_E27;
 *      TPCC_TPCC_ER_E28;TPCC_TPCC_ER_E29;TPCC_TPCC_ER_E30;TPCC_TPCC_ER_E31;
 *   @n TPCC_TPCC_ERH_E32;TPCC_TPCC_ERH_E33;TPCC_TPCC_ERH_E34;TPCC_TPCC_ERH_E35;
 *      TPCC_TPCC_ERH_E36;TPCC_TPCC_ERH_E37;TPCC_TPCC_ERH_E38;TPCC_TPCC_ERH_E39;
 *      TPCC_TPCC_ERH_E40;TPCC_TPCC_ERH_E41;TPCC_TPCC_ERH_E42;TPCC_TPCC_ERH_E43;
 *      TPCC_TPCC_ERH_E44;TPCC_TPCC_ERH_E45;TPCC_TPCC_ERH_E46;TPCC_TPCC_ERH_E47;
 *      TPCC_TPCC_ERH_E48;TPCC_TPCC_ERH_E49;TPCC_TPCC_ERH_E50;TPCC_TPCC_ERH_E51;
 *      TPCC_TPCC_ERH_E52;TPCC_TPCC_ERH_E53;TPCC_TPCC_ERH_E54;TPCC_TPCC_ERH_E55;
 *      TPCC_TPCC_ERH_E56;TPCC_TPCC_ERH_E57;TPCC_TPCC_ERH_E58;TPCC_TPCC_ERH_E59;
 *      TPCC_TPCC_ERH_E60;TPCC_TPCC_ERH_E61;TPCC_TPCC_ERH_E62;TPCC_TPCC_ERH_E63;
 *   @n TPCC_TPCC_QER_QER0;TPCC_TPCC_QER_QER1;TPCC_TPCC_QER_QER2;TPCC_TPCC_QER_QER3;
 *      TPCC_TPCC_QER_QER4;TPCC_TPCC_QER_QER5;TPCC_TPCC_QER_QER6;TPCC_TPCC_QER_QER7;
 *   @n TPCC_TPCC_EMR_EMR0;TPCC_TPCC_EMR_EMR1;TPCC_TPCC_EMR_EMR2;
 *      TPCC_TPCC_EMR_EMR3;TPCC_TPCC_EMR_EMR4;TPCC_TPCC_EMR_EMR5;
 *      TPCC_TPCC_EMR_EMR6;TPCC_TPCC_EMR_EMR7;TPCC_TPCC_EMR_EMR8;
 *      TPCC_TPCC_EMR_EMR9;TPCC_TPCC_EMR_EMR10;TPCC_TPCC_EMR_EMR11;
 *      TPCC_TPCC_EMR_EMR12;TPCC_TPCC_EMR_EMR13;TPCC_TPCC_EMR_EMR14;
 *      TPCC_TPCC_EMR_EMR15;TPCC_TPCC_EMR_EMR16;TPCC_TPCC_EMR_EMR17;
 *      TPCC_TPCC_EMR_EMR18;TPCC_TPCC_EMR_EMR19;TPCC_TPCC_EMR_EMR20;
 *      TPCC_TPCC_EMR_EMR21;TPCC_TPCC_EMR_EMR22;TPCC_TPCC_EMR_EMR23;
 *      TPCC_TPCC_EMR_EMR24;TPCC_TPCC_EMR_EMR25;TPCC_TPCC_EMR_EMR26;
 *      TPCC_TPCC_EMR_EMR27;TPCC_TPCC_EMR_EMR28;TPCC_TPCC_EMR_EMR29;
 *      TPCC_TPCC_EMR_EMR30;TPCC_TPCC_EMR_EMR31;
 *   @n TPCC_TPCC_EMRH_EMR32;TPCC_TPCC_EMRH_EMR33;TPCC_TPCC_EMRH_EMR34;
 *      TPCC_TPCC_EMRH_EMR35;TPCC_TPCC_EMRH_EMR36;TPCC_TPCC_EMRH_EMR37;
 *      TPCC_TPCC_EMRH_EMR38;TPCC_TPCC_EMRH_EMR39;TPCC_TPCC_EMRH_EMR40;
 *      TPCC_TPCC_EMRH_EMR41;TPCC_TPCC_EMRH_EMR42;TPCC_TPCC_EMRH_EMR43;
 *      TPCC_TPCC_EMRH_EMR44;TPCC_TPCC_EMRH_EMR45;TPCC_TPCC_EMRH_EMR46;
 *      TPCC_TPCC_EMRH_EMR47;TPCC_TPCC_EMRH_EMR48;TPCC_TPCC_EMRH_EMR49;
 *      TPCC_TPCC_EMRH_EMR50;TPCC_TPCC_EMRH_EMR51;TPCC_TPCC_EMRH_EMR52;
 *      TPCC_TPCC_EMRH_EMR53;TPCC_TPCC_EMRH_EMR54;TPCC_TPCC_EMRH_EMR55;
 *      TPCC_TPCC_EMRH_EMR56;TPCC_TPCC_EMRH_EMR57;TPCC_TPCC_EMRH_EMR58;
 *      TPCC_TPCC_EMRH_EMR59;TPCC_TPCC_EMRH_EMR60;TPCC_TPCC_EMRH_EMR61;
 *      TPCC_TPCC_EMRH_EMR62;TPCC_TPCC_EMRH_EMR63,
 *   @n TPCC_TPCC_SER_SER0;TPCC_TPCC_SER_SER1;TPCC_TPCC_SER_SER2;
 *      TPCC_TPCC_SER_SER3;TPCC_TPCC_SER_SER4;TPCC_TPCC_SER_SER5;
 *      TPCC_TPCC_SER_SER6;TPCC_TPCC_SER_SER7;TPCC_TPCC_SER_SER8;
 *      TPCC_TPCC_SER_SER9;TPCC_TPCC_SER_SER10;TPCC_TPCC_SER_SER11;
 *      TPCC_TPCC_SER_SER12;TPCC_TPCC_SER_SER13;TPCC_TPCC_SER_SER14;
 *      TPCC_TPCC_SER_SER15;TPCC_TPCC_SER_SER16;TPCC_TPCC_SER_SER17;
 *      TPCC_TPCC_SER_SER18;TPCC_TPCC_SER_SER19;TPCC_TPCC_SER_SER20;
 *      TPCC_TPCC_SER_SER21;TPCC_TPCC_SER_SER22;TPCC_TPCC_SER_SER23;
 *      TPCC_TPCC_SER_SER24;TPCC_TPCC_SER_SER25;TPCC_TPCC_SER_SER26;
 *      TPCC_TPCC_SER_SER27;TPCC_TPCC_SER_SER28;TPCC_TPCC_SER_SER29;
 *      TPCC_TPCC_SER_SER30;TPCC_TPCC_SER_SER31;
 *   @n TPCC_TPCC_SERH_SER32;TPCC_TPCC_SERH_SER33;TPCC_TPCC_SERH_SER34;
 *      TPCC_TPCC_SERH_SER35;TPCC_TPCC_SERH_SER36;TPCC_TPCC_SERH_SER37;
 *      TPCC_TPCC_SERH_SER38;TPCC_TPCC_SERH_SER39;TPCC_TPCC_SERH_SER40;
 *      TPCC_TPCC_SERH_SER41;TPCC_TPCC_SERH_SER42;TPCC_TPCC_SERH_SER43;
 *      TPCC_TPCC_SERH_SER44;TPCC_TPCC_SERH_SER45;TPCC_TPCC_SERH_SER46;
 *      TPCC_TPCC_SERH_SER47;TPCC_TPCC_SERH_SER48;TPCC_TPCC_SERH_SER49;
 *      TPCC_TPCC_SERH_SER50;TPCC_TPCC_SERH_SER51;TPCC_TPCC_SERH_SER52;
 *      TPCC_TPCC_SERH_SER53;TPCC_TPCC_SERH_SER54;TPCC_TPCC_SERH_SER55;
 *      TPCC_TPCC_SERH_SER56;TPCC_TPCC_SERH_SER57;TPCC_TPCC_SERH_SER58;
 *      TPCC_TPCC_SERH_SER59;TPCC_TPCC_SERH_SER60;TPCC_TPCC_SERH_SER61;
 *      TPCC_TPCC_SERH_SER62;TPCC_TPCC_SERH_SER63;
 *   @n TPCC_TPCC_QEMR_QEMR0;TPCC_TPCC_QEMR_QEMR1;TPCC_TPCC_QEMR_QEMR2;
 *      TPCC_TPCC_QEMR_QEMR3;TPCC_TPCC_QEMR_QEMR4;TPCC_TPCC_QEMR_QEMR5;
 *      TPCC_TPCC_QEMR_QEMR6;TPCC_TPCC_QEMR_QEMR7,
 *   @n TPCC_TPCC_QSER_QSER0;TPCC_TPCC_QSER_QSER1;TPCC_TPCC_QSER_QSER2;
 *      TPCC_TPCC_QSER_QSER3;TPCC_TPCC_QSER_QSER4;TPCC_TPCC_QSER_QSER5;
 *      TPCC_TPCC_QSER_QSER6;TPCC_TPCC_QSER_QSER7;
 *
 *   @b Example
 *   @verbatim

            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Edma3ChannelObj         chObj;
            CSL_Edma3ChannelHandle      hChannel;
            CSL_Edma3ChannelAttr        chAttr;
            CSL_Status                  status;
            CSL_Edma3ChannelErr         errStat;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj, 0, NULL,&status);

            // Channel 0 Open in context of Shadow region 0
            chAttr.regionNum = CSL_EDMA3_REGION_0;
            chAttr.chaNum    = 0;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            ...               
            // Get the Channel 0 Error Status
            CSL_edma3GetHwChannelStatus(hChannel, CSL_EDMA3_QUERY_CHANNEL_ERR, &errStat);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwChannelStatus, ".text:csl_section:edma3");
CSL_Status  CSL_edma3GetHwChannelStatus 
(
    CSL_Edma3ChannelHandle          hEdma,
    CSL_Edma3HwChannelStatusQuery   myQuery,
    void*                           response
)
{
    CSL_Status status = CSL_SOK;

    /* Validate the arguments. */    
    if (hEdma == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (response == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else 
    {
        /* Arguments look good.. Process the query... */
        switch(myQuery) 
        {
            case CSL_EDMA3_QUERY_CHANNEL_STATUS:
            {
                /* Are we querying for DMA or QDMA Channel? */
                if (hEdma->chaNum < hEdma->cfgInfo.numDMAChannel)
                {
                    /* DMA Channel. */
                    CSL_edma3IsDMAChannelEventPending ((CSL_Edma3Handle)hEdma, hEdma->chaNum, response);
                }
                else 
                {
                    /* QDMA Channel */
                    CSL_edma3IsQDMAChannelEventPending ((CSL_Edma3Handle)hEdma, 
                                                        hEdma->chaNum - hEdma->cfgInfo.numDMAChannel, response);
                }
                break;
            }                                
            case CSL_EDMA3_QUERY_CHANNEL_ERR:
            {
                /* Are we querying for DMA or QDMA Channel? */
                if (hEdma->chaNum < hEdma->cfgInfo.numDMAChannel) 
                {
                    /* DMA Channel: Set the Missed Event and Secondary Events Flags */
                    CSL_edma3IsDMAChannelMissedEventSet ((CSL_Edma3Handle)hEdma, hEdma->chaNum, 
                                                         &((CSL_Edma3ChannelErr*)response)->missed);
                    CSL_edma3IsDMAChannelSecondaryEventSet ((CSL_Edma3Handle)hEdma, 
                                                         hEdma->chaNum, &((CSL_Edma3ChannelErr*)response)->secEvt);
                }
                else
                {
                    /* QDMA Channel: Set the Missed Event and Secondary Events Flags */
                    CSL_edma3IsQDMAChannelMissedEventSet ((CSL_Edma3Handle)hEdma, 
                                                          hEdma->chaNum - hEdma->cfgInfo.numDMAChannel, 
                                                          &((CSL_Edma3ChannelErr*)response)->missed);
                    CSL_edma3IsQDMAChannelSecondaryEventSet ((CSL_Edma3Handle)hEdma, 
                                                          hEdma->chaNum - hEdma->cfgInfo.numDMAChannel, 
                                                          &((CSL_Edma3ChannelErr*)response)->secEvt);
                }
                break;
            }
            default:
                status = CSL_ESYS_INVQUERY;
                break;
        }
    }
    return (status);
}

