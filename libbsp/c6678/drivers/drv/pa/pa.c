/******************************************************************************
 * FILE PURPOSE:  Main Routines for PA LLD
 ******************************************************************************
 * FILE NAME:   pa.c  
 *
 * DESCRIPTION: Main PA LLD functions 
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
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

#if defined(_LITTLE_ENDIAN)
  #define SWIZ(x)  (sizeof((x)) == 1 ? (x) : (sizeof((x)) == 2 ? swiz16((x)) : (sizeof((x)) == 4 ? swiz32((x)) : 0)))
#else
  #define SWIZ(x)  (x)
  #define swizFcmd(x)
  #define swizStatsRep(x)
#endif

#include <string.h>
#include <stddef.h>
#include "pa.h"
#include "paloc.h"
#include "pafrm.h"
#include "pa_osal.h"
#include "pasahost.h"
#include <ti/csl/cslr_pa_ss.h>
#include <ti/drv/rm/rm_public_lld.h>

#define pa_mac_zero(x)      pa_uint8_zero((uint8_t *)(x), 6)
#define pa_ip_zero(x)       pa_uint8_zero((uint8_t *)((x).ipv6), 16)

/* Global Variable which describes the PA LLD Version Information */
const char paLldVersionStr[] = PA_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/* Next header conversion table */
const uint8_t pa_next_hdr_tbl[] = {
    PAFRM_HDR_MAC,
    PAFRM_HDR_IPv4,
    PAFRM_HDR_IPv6,
    PAFRM_HDR_CUSTOM_C1,
    PAFRM_HDR_UDP,
    PAFRM_HDR_UDP_LITE,
    PAFRM_HDR_TCP,                                    
    PAFRM_HDR_CUSTOM_C2,
    PAFRM_HDR_UNKNOWN
};  

#if defined(_LITTLE_ENDIAN)
/*********************************************************************
 * FUNCTION PURPOSE: Swizzling
 *********************************************************************
 * DESCRIPTION: The PA sub-system requires all multi-byte fields in
 *              big endian format.
 *********************************************************************/
static inline uint16_t swiz16(uint16_t x)
{
  return ((x >> 8) | (x << 8));
}

static inline uint32_t swiz32 (uint32_t x)
{
  return (((x) >> 24) | (((x) >> 8) & 0xff00L) | (((x) << 8) & 0xff0000L) | ((x) << 24));
}

static void swizFcmd (pafrmCommand_t *fcmd)
{
  fcmd->commandResult =  SWIZ(fcmd->commandResult);
  fcmd->comId         =  SWIZ(fcmd->comId);
  fcmd->retContext    =  SWIZ(fcmd->retContext);
  fcmd->replyQueue    =  SWIZ(fcmd->replyQueue);
}

static void swizStatsRep (paSysStats_t *s)
{
  s->classify1.nPackets             = SWIZ(s->classify1.nPackets);
  s->classify1.nIpv4Packets         = SWIZ(s->classify1.nIpv4Packets);
  s->classify1.nIpv6Packets         = SWIZ(s->classify1.nIpv6Packets);
  s->classify1.nCustomPackets       = SWIZ(s->classify1.nCustomPackets);
  s->classify1.nSrioPackets         = SWIZ(s->classify1.nSrioPackets);
  s->classify1.nLlcSnapFail         = SWIZ(s->classify1.nLlcSnapFail);
  s->classify1.nTableMatch          = SWIZ(s->classify1.nTableMatch);
  s->classify1.nNoTableMatch        = SWIZ(s->classify1.nNoTableMatch);
  s->classify1.nIpFrag              = SWIZ(s->classify1.nIpFrag);
  s->classify1.nIpDepthOverflow     = SWIZ(s->classify1.nIpDepthOverflow); 
  s->classify1.nVlanDepthOverflow   = SWIZ(s->classify1.nVlanDepthOverflow);
  s->classify1.nGreDepthOverflow    = SWIZ(s->classify1.nGreDepthOverflow);
  s->classify1.nMplsPackets         = SWIZ(s->classify1.nMplsPackets);
  s->classify1.nParseFail           = SWIZ(s->classify1.nParseFail);
  s->classify1.nInvalidIPv6Opt      = SWIZ(s->classify1.nInvalidIPv6Opt);
  s->classify1.nTxIpFrag            = SWIZ(s->classify1.nTxIpFrag);
  s->classify1.nSilentDiscard       = SWIZ(s->classify1.nSilentDiscard);
  s->classify1.nInvalidControl      = SWIZ(s->classify1.nInvalidControl);
  s->classify1.nInvalidState        = SWIZ(s->classify1.nInvalidState);
  s->classify1.nSystemFail          = SWIZ(s->classify1.nSystemFail);

  s->classify2.nPackets             = SWIZ(s->classify2.nPackets);
  s->classify2.nUdp                 = SWIZ(s->classify2.nUdp);
  s->classify2.nTcp                 = SWIZ(s->classify2.nTcp);
  s->classify2.nCustom              = SWIZ(s->classify2.nCustom);
  s->classify2.nSilentDiscard       = SWIZ(s->classify2.nSilentDiscard);
  s->classify2.nInvalidControl      = SWIZ(s->classify2.nInvalidControl);

  s->modify.nCommandFail = SWIZ(s->modify.nCommandFail);


}
#endif


/********************************************************************
 * FUNCTION PURPOSE: Determine if a uint8_t array is identically 0
 ********************************************************************
 * DESCRIPTION: Returns TRUE if every element in a uint8_t is zero
 ********************************************************************/
static uint16_t pa_uint8_zero (uint8_t *v, int n)
{
  int i;

  for (i = 0; i < n; i++)
    if (v[i] != 0)
      return (FALSE);

  return (TRUE);

}

/********************************************************************
 * FUNCTION PURPOSE: Determine if two uint8_t arrays are identical
 ********************************************************************
 * DESCRIPTION: Returns TRUE if two uint8_t arrays are equal
 ********************************************************************/
static uint16_t pa_uint8_match (uint8_t *v1, uint8_t *v2, int n)
{
  int i;

  for (i = 0; i < n; i++)
    if (v1[i] != v2[i])
      return (FALSE);

  return (TRUE);

}

/********************************************************************
 * FUNCTION PURPOSE: Determine if two MAC addresses table match
 ********************************************************************
 * DESCRIPTION: Returns TRUE if the MAC addresses table match.
 *              If the first mac address is 0 then this entry in
 *              the table is a don't care which would be a table
 *              match
 ********************************************************************/
static uint16_t pa_mac_match (paMacAddr_t mac1, paMacAddr_t mac2)
{
  /* Mac1 is all zero indicates a don't care, which is a match */
  if (pa_mac_zero (mac1))
    return (TRUE);

  /* Compare the mac addresses */
  return (pa_uint8_match ((uint8_t *)mac1, (uint8_t *)mac2, 6));

}

/***********************************************************************
 * FUNCTION PURPOSE: Determine if two IP addresses table match
 ***********************************************************************
 * DESCRIPTION: Returns TRUE if the IP addresses table match.
 *              If the first IP address is 0 then this entry in 
 *              the table is a don't care which would always be a
 *              table match
 ***********************************************************************/
static uint16_t pa_ip_match (paIpAddr_t ip1,  paIpAddr_t ip2)
{
  /* Ip1 ll zero indicates a don't care, which is a match */
    if (pa_ip_zero (ip1))
      return (TRUE);

  /* Compare the addresses. For IPV4 addresses the 12 msbs will
   * be zero. */
  return (pa_uint8_match ((uint8_t *)(ip1.ipv6), (uint8_t *)(ip2.ipv6),  16));

}

/***********************************************************************
 * FUNCTION PURPOSE: Determine if two IP TOS fields table match
 ***********************************************************************
 * DESCRIPTION: Returns TRUE if the two TOS fields table match.
 *              If the first TOS is a don't care (TRUE), then this is
 *              a table match 
 ************************************************************************/
static uint16_t pa_tos_match (uint8_t tos1, uint16_t tosCare, uint8_t tos2)
{
  if (!tosCare)
    return (TRUE);

  if (tos1 == tos2)
    return (TRUE);

  return (FALSE);

}


/**********************************************************************
 * FUNCTION PURPOSE: Check for table match on uint8_t
 **********************************************************************
 * DESCRIPTION: Returns TRUE if two uint8_t values will table match.
 *              If the first value is 0 then in the table it is a 
 *              don't care, which is an automatic match.
 **********************************************************************/
static uint16_t pa_u8_match (uint8_t v1, uint8_t v2)
{
  /* If v1 is 0 it is a don't care, which is a match */
  if (v1 == 0)
    return (TRUE);

  if (v1 == v2)
    return (TRUE);

  return (FALSE);

}

/**********************************************************************
 * FUNCTION PURPOSE: Check for table match on uint16_t
 **********************************************************************
 * DESCRIPTION: Returns TRUE if two uint16_t values will table match.
 *              If the first value is 0 then in the table it is a 
 *              don't care, which is an automatic match.
 **********************************************************************/
static uint16_t pa_u16_match (uint16_t v1,  uint16_t v2)
{
  /* If v1 is 0 it is a don't care, which is a match */
  if (v1 == 0)
    return (TRUE);

  if (v1 == v2)
    return (TRUE);

  return (FALSE);

}

/*************************************************************************
 * FUNCTION PURPOSE: Check for table match on uint32_t
 ************************************************************************* 
 * DESCRIPTION: Returns TRUE if two uint32_t values will table match
 *************************************************************************/
static uint16_t pa_u32_match (uint32_t v1,  uint32_t v2)
{
  /* If v1 is 0 it is a don't care, which is a match */
  if (v1 == 0)
    return (TRUE);

  if (v1 == v2)
    return (TRUE);

  return (FALSE);

}

/*************************************************************************
 * FUNCTION PURPOSE: Convert Routing Info
 ************************************************************************* 
 * DESCRIPTION: Convert the application routing information into the
 *              firmware forward information
 * Returns FALSE if error occurs
 *************************************************************************/
static paReturn_t pa_conv_routing_info (paInst_t *paInst, pafrmForward_t *fwdInfo, paRouteInfo_t *routeInfo, int cmdDest, uint16_t failRoute)
{

  /*
   * The command location is the same in all destination modes except SRIO.
   * Initialize pCmd here to save code space.
   *
   * CmdSet is supported in all destination modes except CONTINUE_PARSE
   * Initialize fcmdSetNotSupport to FALSE as default
   */
                                      
  uint8_t psFlags;                                    
  uint8_t *pCmd = fwdInfo->u.host.cmd;
  int     fcmdSetNotSupport = FALSE;
  
  fwdInfo->flowId = routeInfo->flowId;
  fwdInfo->queue = SWIZ(routeInfo->queue);
  
  if ((routeInfo->dest == pa_DEST_HOST) || (routeInfo->dest == pa_DEST_EMAC)) {
    psFlags      = (routeInfo->pktType_emacCtrl & pa_EMAC_CTRL_CRC_DISABLE)?
                   PAFRM_ETH_PS_FLAGS_DISABLE_CRC:0;
    psFlags      |= ((routeInfo->pktType_emacCtrl & pa_EMAC_CTRL_PORT_MASK) << PAFRM_ETH_PS_FLAGS_PORT_SHIFT);
  }
  
  if (routeInfo->dest == pa_DEST_HOST)  {
    fwdInfo->forwardType    = PAFRM_FORWARD_TYPE_HOST;
    fwdInfo->u.host.context = SWIZ(routeInfo->swInfo0);
    fwdInfo->u.host.psFlags = SWIZ(psFlags);

    if (routeInfo->mRouteIndex >= 0)  {
      if (routeInfo->mRouteIndex >= pa_MAX_MULTI_ROUTE_SETS) {
        return (pa_ERR_CONFIG);
      }
      fwdInfo->u.host.multiRoute   = 1;
      fwdInfo->u.host.multiIdx     = (uint8_t)routeInfo->mRouteIndex;
      fwdInfo->u.host.paPdspRouter = PAFRM_DEST_PA_M_0;
    }
    
  } else if (routeInfo->dest == pa_DEST_DISCARD)  {
    fwdInfo->forwardType = PAFRM_FORWARD_TYPE_DISCARD;

  } else if (routeInfo->dest == pa_DEST_EMAC)  {
    fwdInfo->forwardType        = PAFRM_FORWARD_TYPE_ETH;
    fwdInfo->u.eth.psFlags      = SWIZ(psFlags);
  
  } else if (failRoute) {
    
    return (pa_ERR_CONFIG);

  } else if (((routeInfo->dest == pa_DEST_CONTINUE_PARSE_LUT1) && (routeInfo->customType != pa_CUSTOM_TYPE_LUT2))|| 
             ((routeInfo->dest == pa_DEST_CONTINUE_PARSE_LUT2) && (routeInfo->customType != pa_CUSTOM_TYPE_LUT1)))  {
             
    /* Custom Error check */
    if (((routeInfo->customType == pa_CUSTOM_TYPE_LUT1) && (routeInfo->customIndex >= pa_MAX_CUSTOM_TYPES_LUT1)) || 
        ((routeInfo->customType == pa_CUSTOM_TYPE_LUT2) && (routeInfo->customIndex >= pa_MAX_CUSTOM_TYPES_LUT2)) ) {
       return(pa_ERR_CONFIG); 
    }    
             
    fwdInfo->forwardType = PAFRM_FORWARD_TYPE_PA;
    fwdInfo->u.pa.customType = (uint8_t)routeInfo->customType;
    fwdInfo->u.pa.customIdx = SWIZ(routeInfo->customIndex); 

    if (routeInfo->dest == pa_DEST_CONTINUE_PARSE_LUT2) {
        
       fwdInfo->u.pa.paDest = PAFRM_DEST_PA_C2; 
        
    }
    else {
      /*
       * cmdDest is provided by calling function 
       * There is no need to check error case 
       */
      fwdInfo->u.pa.paDest = (cmdDest == pa_CMD_TX_DEST_0)?PAFRM_DEST_PA_C1_1:PAFRM_DEST_PA_C1_2; 
    }
    
    fcmdSetNotSupport = TRUE;

  }  else if (routeInfo->dest == pa_DEST_SASS)  {
    fwdInfo->forwardType        = PAFRM_FORWARD_TYPE_SA;
    fwdInfo->u.sa.swInfo0       = SWIZ(routeInfo->swInfo0);
    fwdInfo->u.sa.swInfo1       = SWIZ(routeInfo->swInfo1);
    
  }  else if (routeInfo->dest == pa_DEST_SRIO)  {
    fwdInfo->forwardType        = PAFRM_FORWARD_TYPE_SRIO;
    fwdInfo->u.srio.psInfo0     = SWIZ(routeInfo->swInfo0);
    fwdInfo->u.srio.psInfo1     = SWIZ(routeInfo->swInfo1);
    fwdInfo->u.srio.pktType     = SWIZ(routeInfo->pktType_emacCtrl);
    pCmd = NULL;
  
  } else  {

    return (pa_ERR_CONFIG);
  }
  
  if (pCmd && routeInfo->pCmd)  {
  
    paCmdInfo_t* paCmd = routeInfo->pCmd;
    paPatchInfo_t       *pPatchInfo;
    paCmdSet_t          *pCmdSet;
    paCmdUsrStats_t     *pUsrStats;
    paCmdSetUsrStats_t  *pCmdSetUsrStats; 
    
    switch (paCmd->cmd)
    {
      case pa_CMD_PATCH_DATA:
        pPatchInfo = &paCmd->params.patch;
        
        if((pPatchInfo->nPatchBytes > 2) || (!(pPatchInfo->ctrlBitfield & pa_PATCH_OP_INSERT)) || (pPatchInfo->patchData == NULL))
          return (pa_ERR_CONFIG); 
          
        pCmd[0] = PAFRM_RX_CMD_PATCH_DATA;
        pCmd[1] = pPatchInfo->nPatchBytes;
        pCmd[2] = pPatchInfo->patchData[0];
        pCmd[3] = pPatchInfo->patchData[1];
        break;
      
      case pa_CMD_CMDSET:
        pCmdSet = &paCmd->params.cmdSet;
        if(fcmdSetNotSupport || (pCmdSet->index >= paInst->cfg.cmdSetConfig.numCmdSets))
          return (pa_ERR_CONFIG); 
        pCmd[0] = PAFRM_RX_CMD_CMDSET;
        pCmd[1] = (uint8_t)pCmdSet->index; 
        break;
        
      case pa_CMD_USR_STATS:
        pUsrStats = &paCmd->params.usrStats;
        if(pUsrStats->index >= paInst->cfg.usrStatsConfig.numCounters)
          return (pa_ERR_CONFIG); 
        pCmd[0] = PAFRM_RX_CMD_USR_STATS;
        pCmd[1] = 4; 
        pCmd[2] = pUsrStats->index >> 8;
        pCmd[3] = pUsrStats->index & 0xFF;
        break;
        
      case pa_CMD_CMDSET_AND_USR_STATS:
        pCmdSetUsrStats = &paCmd->params.cmdSetUsrStats;
        if(fcmdSetNotSupport || 
           (pCmdSetUsrStats->setIndex >= paInst->cfg.cmdSetConfig.numCmdSets) || 
           (pCmdSetUsrStats->statsIndex >= paInst->cfg.usrStatsConfig.numCounters))
          return (pa_ERR_CONFIG); 
        pCmd[0] = PAFRM_RX_CMD_CMDSET_USR_STATS;
        pCmd[1] = (uint8_t)pCmdSetUsrStats->setIndex; 
        pCmd[2] = pCmdSetUsrStats->statsIndex >> 8;
        pCmd[3] = pCmdSetUsrStats->statsIndex & 0xFF;
        
        break;
      
      default:
        return(pa_ERR_CONFIG);
    }    
  
  }
  return (pa_OK);
} /* pa_conv_routing_info */

/*************************************************************************
 * FUNCTION PURPOSE: Format Firmware Command Header
 ************************************************************************* 
 * DESCRIPTION: Clear and construct the firmware command header
 * Returns pointer to the firmware command
 *************************************************************************/
static pafrmCommand_t* pa_format_fcmd_header (void *pCmd, paCmdReply_t *reply, uint8_t cmd, uint16_t comId, uint16_t cmdSize)
{
  pafrmCommand_t *fcmd = (pafrmCommand_t *) pCmd;
  
  memset(fcmd, 0, cmdSize);

  fcmd->command       = SWIZ(cmd);
  fcmd->magic         = PAFRM_CONFIG_COMMAND_SEC_BYTE;
  fcmd->comId         = SWIZ(comId);
  fcmd->retContext    = SWIZ(reply->replyId);
  fcmd->replyQueue    = SWIZ(reply->queue);
  fcmd->flowId        = SWIZ(reply->flowId);
  
  return(fcmd);
}  

/****************************************************************************
 * FUNCTION PURPOSE: Add a MAC address to the lookup table
 ****************************************************************************
 * DESCRIPTION: A MAC address is added to the L2 table.
 *              All L2 lookups are handled through PDSP0
 ****************************************************************************/
paReturn_t Pa_addMac  (  Pa_Handle          iHandle,
                         int                index,
                         paEthInfo_t       *ethInfo,
                         paRouteInfo_t     *routeInfo,
                         paRouteInfo_t     *nextRtFail,
                         paHandleL2L3_t    *handle,
                         paCmd_t            cmd,
                         uint16_t          *cmdSize,
                         paCmdReply_t      *reply,
                         int               *cmdDest )
{
  /* The entry is created in the stack and then copied to the table. This allows
   * for a comparison with other table entries to verify there is not an identical
   * entry or an entry that would supercede this one */

  paInst_t               *paInst    = (paInst_t *)iHandle;
  paL2Entry_t            *l2Table;

  paL2Entry_t             l2Entry;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut1_t  *al1;
  int                     i;
  uint16_t                csize;
  uint8_t                 lut1Index = (index == pa_LUT1_INDEX_NOT_SPECIFIED)?PAFRM_LUT1_INDEX_LAST_FREE:(uint8_t)index;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;

  if (rmPermCallTbl) {

      resourceData.res_info.lutEntry = pa_CMD_TX_DEST_0;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
          return pa_RESOURCE_USE_DENIED;
      }
  }
 
  /* Sanity chek the LUT1 index */
  if (lut1Index > PAFRM_HW_LUT1_ENTRIES) {
    return(pa_INVALID_LUT1_INDEX);
  }

  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+sizeof(pafrmCommandAddLut1_t)-sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);
  
  
  /* Form the table entry. Status is not required */
  memset (&l2Entry, 0, sizeof(paL2Entry_t));
  memcpy (l2Entry.cfg.mac.dstMac,  ethInfo->dst,  sizeof(paMacAddr_t));
  memcpy (l2Entry.cfg.mac.srcMac,  ethInfo->src,  sizeof(paMacAddr_t));
  l2Entry.cfg.mac.vlan       =  ethInfo->vlan;
  l2Entry.cfg.mac.ethertype  =  ethInfo->ethertype;
  l2Entry.cfg.mac.mplsTag    =  ethInfo->mplsTag;
  l2Entry.cfg.mac.inport     =  ethInfo->inport;
  
  /* Refresh PA Instance for read only operation */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  /* Signal the application that a table modification will be done */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base, 
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
                         
  l2Table   = (paL2Entry_t *)paInst->paBufs[PA_BUFFNUM_L2_TABLE].base;  

  /* Look for an identical entry in the table. If one is found, return it */
  /* perform entry check only if the LUT1 index is not specified by user */
  if (lut1Index == PAFRM_LUT1_INDEX_LAST_FREE) {
    for (i = 0; i < paInst->l2n;  i++)  {

      if ( ((l2Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) ||
            (l2Table[i].hdr.status == PA_TBL_STAT_ACTIVE)             ) &&
           (l2Table[i].hdr.subType == PA_TABLE_ENTRY_SUBTYPE_MAC )) {

        if (!memcmp (&(l2Table[i].cfg), &(l2Entry.cfg), sizeof(paL2InCfg_t)))  {
          /* Identical entry identified */
          if (l2Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) {
            *handle = (paHandleL2L3_t)&l2Table[i];
            ret = pa_INVALID_DUP_ENTRY;
            goto Pa_addMac_end;
          }                     
          else {
              /*  
               * Identical entry is identified, reuse the same table entry
               * Keep the entry status since it simply replaces the original entry
               * and the user may decide not to forward the packet
               */ 
            lut1Index = l2Table[i].hdr.lutIdx;
            ret = pa_DUP_ENTRY;   
            goto Pa_addMac_send_cmd;
          }                  
        }
      }
    }


    /* Check to see if this entry is more general then one already in the table.
     * If so then this entry would 'steal' packets from that entry.
     */

    for (i = 0; i < paInst->l2n; i++)  {

      if ( (l2Table[i].hdr.status != PA_TBL_STAT_PENDING_SUBS_REPLY)  &&
           (l2Table[i].hdr.status != PA_TBL_STAT_ACTIVE)              )
        continue;
        
      if ( l2Table[i].hdr.subType != PA_TABLE_ENTRY_SUBTYPE_MAC )  
        continue;

      if (!pa_mac_match (l2Entry.cfg.mac.dstMac, l2Table[i].cfg.mac.dstMac))
        continue;

      if (!pa_mac_match (l2Entry.cfg.mac.srcMac, l2Table[i].cfg.mac.srcMac))
        continue;

      if (!pa_u16_match (l2Entry.cfg.mac.vlan, l2Table[i].cfg.mac.vlan))
        continue;

      if (!pa_u16_match (l2Entry.cfg.mac.ethertype, l2Table[i].cfg.mac.ethertype))
        continue;

      if (!pa_u32_match (l2Entry.cfg.mac.mplsTag, l2Table[i].cfg.mac.mplsTag))
        continue;
        
      if (!pa_u16_match (l2Entry.cfg.mac.inport, l2Table[i].cfg.mac.inport))
        continue;
        

      /* This is a more general entry then one already in the table */
      ret = pa_INVALID_TABLE_MORE_SPECIFIC_ENTRY_PRESENT;
      goto Pa_addMac_end;
    }
  }

  /* The MPLS label is restricted to 20 bits */
  if (l2Entry.cfg.mac.mplsTag & ~PA_MPLS_LABEL_MASK)  {
    ret = pa_INVALID_MPLS_LABEL;
    goto Pa_addMac_end;
  }

  /* Find a free entry in the table */
  for (i = 0; i < paInst->l2n; i++)  {

    if (l2Table[i].hdr.status == PA_TBL_STAT_INACTIVE)
      break;

  }

  if (i == paInst->l2n)  {
    ret = pa_HANDLE_TABLE_FULL;
    goto Pa_addMac_end;
  }
  
Pa_addMac_send_cmd:
  
  /* The handle is just a pointer to the table entry */
  *handle = (paHandleL2L3_t)&l2Table[i];

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT1, PA_COMID_L2  | i, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al1  = (pafrmCommandAddLut1_t *) &(fcmd->cmd);

  al1->index         = lut1Index;
  al1->type          = PAFRM_COM_ADD_LUT1_STANDARD;

  if (!pa_mac_zero (l2Entry.cfg.mac.dstMac))
  {
    memcpy (al1->u.ethIp.dmac, l2Entry.cfg.mac.dstMac, sizeof(paMacAddr_t));
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_DMAC;
  }  

  if (!pa_mac_zero (l2Entry.cfg.mac.srcMac))
  {
    memcpy (al1->u.ethIp.smac, l2Entry.cfg.mac.srcMac, sizeof(paMacAddr_t));
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_SMAC;
  }  

  if (l2Entry.cfg.mac.ethertype != 0)
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_ETYPE;
    al1->u.ethIp.etype = SWIZ(l2Entry.cfg.mac.ethertype);
  }
  
  if (l2Entry.cfg.mac.mplsTag != 0)  {
    al1->u.ethIp.matchFlags |= PAFRM_LUT1_MATCH_MPLS;
    al1->u.ethIp.pm.mpls     = SWIZ(l2Entry.cfg.mac.mplsTag);
  }

  if (l2Entry.cfg.mac.vlan != 0)
  {
    al1->u.ethIp.matchFlags |= PAFRM_LUT1_MATCH_VLAN;
    al1->u.ethIp.vlan  = SWIZ(l2Entry.cfg.mac.vlan);
  }  
    
  if (l2Entry.cfg.mac.inport != 0)  {
    al1->u.ethIp.matchFlags |= PAFRM_LUT1_MATCH_PORT;
    al1->u.ethIp.inport = (uint8_t)l2Entry.cfg.mac.inport;
  }

  al1->u.ethIp.key        |= PAFRM_LUT1_KEY_MAC;
  al1->u.ethIp.key         = SWIZ(al1->u.ethIp.key);
  al1->u.ethIp.matchFlags |= PAFRM_LUT1_CUSTOM_MATCH_KEY;
  al1->u.ethIp.matchFlags  = SWIZ(al1->u.ethIp.matchFlags);

  /* Forwarding information */
  ret1 = pa_conv_routing_info(paInst, &al1->match, routeInfo, pa_CMD_TX_DEST_0, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addMac_end;
  }

  /* Next fail information */
  ret1 = pa_conv_routing_info(paInst, &al1->nextFail, nextRtFail, pa_CMD_TX_DEST_0, TRUE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addMac_end;
  }

  if (ret != pa_DUP_ENTRY)
  {
    /* Initialze, the table entry, add the status and pdsp NUM */
    l2Entry.hdr.type     = PA_TABLE_ENTRY_TYPE_L2;
    l2Entry.hdr.subType  = PA_TABLE_ENTRY_SUBTYPE_MAC;
    l2Entry.hdr.status   = PA_TBL_STAT_PENDING_SUBS_REPLY;
    l2Entry.hdr.pdspNum  = PASS_PDSP0;
    l2Entry.hdr.lutIdx   = -1;
    l2Entry.hdr.tableIdx = i;

    memcpy (&l2Table[i], &l2Entry, sizeof(paL2Entry_t));
  }

  /* The destination for this command must be PDSP 0 */
  *cmdDest = pa_CMD_TX_DEST_0;
  
Pa_addMac_end:
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  
  
  return (ret);

} /* Pa_addMac */


/****************************************************************************
 * FUNCTION PURPOSE: Add a SRIO entry to the lookup table
 ****************************************************************************
 * DESCRIPTION: A SRIO entry is added to the L2 table. (LUT1_0)
 *              All L2 lookups are handled through PDSP0
 ****************************************************************************/
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
                         int               *cmdDest)
{
  /* The entry is created in the stack and then copied to the table. This allows
   * for a comparison with other table entries to verify there is not an identical
   * entry or an entry that would supercede this one */

  paInst_t               *paInst    = (paInst_t *)iHandle;
  paL2Entry_t            *l2Table;

  paL2Entry_t             l2Entry;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut1_t  *al1;
  int                     i;
  uint16_t                csize;
  uint8_t                 lut1Index = (index == pa_LUT1_INDEX_NOT_SPECIFIED)?PAFRM_LUT1_INDEX_LAST_FREE:(uint8_t)index;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;

  if (rmPermCallTbl) {
  
   resourceData.res_info.lutEntry = pa_CMD_TX_DEST_0;
   
   if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
       return pa_RESOURCE_USE_DENIED;
   }
  }

  /* Sanity chek the LUT1 index */
  if (lut1Index > PAFRM_HW_LUT1_ENTRIES) {
    return(pa_INVALID_LUT1_INDEX);
  }
  
  if (nextHdr > pa_HDR_TYPE_UNKNOWN)
    return ( pa_ERR_CONFIG ); 

  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+sizeof(pafrmCommandAddLut1_t)-sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);
  
  /* Form the table entry. Status is not required */
  memset(&l2Entry, 0, sizeof(paL2Entry_t));
  
  l2Entry.cfg.srio.validBitMap = srioInfo->validBitMap;
  
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_ID)   {
  
    if((srioInfo->tt != pa_SRIO_TRANSPORT_TYPE_0) && (srioInfo->tt != pa_SRIO_TRANSPORT_TYPE_1))
      return(pa_ERR_CONFIG);
      
    l2Entry.cfg.srio.tt       = (uint8_t) srioInfo->tt; 
      
    if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_SRC_ID) {
        l2Entry.cfg.srio.srcId = (srioInfo->tt == pa_SRIO_TRANSPORT_TYPE_0)?
                                 srioInfo->srcId & 0x00FF: srioInfo->srcId;   
    }  
    
    if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_DEST_ID) {
        l2Entry.cfg.srio.destId = (srioInfo->tt == pa_SRIO_TRANSPORT_TYPE_0)?
                                  srioInfo->destId & 0x00FF: srioInfo->destId;   
    }  
  }
  
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_CC)
    l2Entry.cfg.srio.cc       =  (uint8_t) srioInfo->cc;
    
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_PRI)
    l2Entry.cfg.srio.pri       = (uint8_t) srioInfo->pri;
    
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO)   {
  
    if((srioInfo->msgType != pa_SRIO_TYPE_9) && (srioInfo->msgType != pa_SRIO_TYPE_11))
      return(pa_ERR_CONFIG);
      
    l2Entry.cfg.srio.msgType  = (uint8_t) srioInfo->msgType; 
      
    if (srioInfo->msgType == pa_SRIO_TYPE_9) {
    
        if(srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID)
          l2Entry.cfg.srio.typeParam1 = srioInfo->typeInfo.type9.streamId;
          
        if(srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_COS)
          l2Entry.cfg.srio.typeParam2 = (uint8_t)srioInfo->typeInfo.type9.cos;
          
    }  
    
    if (srioInfo->msgType == pa_SRIO_TYPE_11) {
    
    
        if(srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX)
          l2Entry.cfg.srio.typeParam1 = srioInfo->typeInfo.type11.mbox;
    
        if(srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_LETTER)
          l2Entry.cfg.srio.typeParam2 = (uint8_t)srioInfo->typeInfo.type11.letter;
          
    }  
  }
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  l2Table   = (paL2Entry_t *)paInst->paBufs[PA_BUFFNUM_L2_TABLE].base;
  
  /* Signal the application that a table modification will be done */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base, 
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);

  /* Look for an identical entry in the table. If one is found, return it */
  /* perform entry check only if the LUT1 index is not specified by user */
  if (lut1Index == PAFRM_LUT1_INDEX_LAST_FREE) {
    for (i = 0; i < paInst->l2n;  i++)  {

      if ( ((l2Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) ||
            (l2Table[i].hdr.status == PA_TBL_STAT_ACTIVE)             ) &&
           (l2Table[i].hdr.subType == PA_TABLE_ENTRY_SUBYTPE_SRIO )) {

        if (!memcmp (&(l2Table[i].cfg), &(l2Entry.cfg), sizeof(paL2InCfg_t)))  {

          /* Identical entry identified */
          if (l2Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) {
            *handle = (paHandleL2L3_t)&l2Table[i];
            ret = pa_INVALID_DUP_ENTRY;
            goto Pa_addSrio_end;                 
          }                     
          else {
              /*  
               * Identical entry is identified, reuse the same table entry
               * Keep the entry status since it simply replaces the original entry
               * and the user may decide not to forward the packet
               */ 
            lut1Index = l2Table[i].hdr.lutIdx;
            ret = pa_DUP_ENTRY;   
            goto Pa_addSrio_send_cmd;
          }                  
        }
      }
    }


    /* Check to see if this entry is more general then one already in the table.
     * If so then this entry would 'steal' packets from that entry.
     */

    for (i = 0; i < paInst->l2n; i++)  {

      if ( (l2Table[i].hdr.status != PA_TBL_STAT_PENDING_SUBS_REPLY)  &&
           (l2Table[i].hdr.status != PA_TBL_STAT_ACTIVE)              )
        continue;
        
      if ( l2Table[i].hdr.subType != PA_TABLE_ENTRY_SUBYTPE_SRIO )  
        continue;
        
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_ID) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_ID))
           continue;
         else if (l2Table[i].cfg.srio.tt != l2Entry.cfg.srio.tt)
           continue;  
      }
        
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_SRC_ID) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_SRC_ID))
           continue;
         else if (l2Table[i].cfg.srio.srcId != l2Entry.cfg.srio.srcId)
           continue;  
      }
      
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_DEST_ID) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_DEST_ID))
           continue;
         else if (l2Table[i].cfg.srio.destId != l2Entry.cfg.srio.destId)
           continue;  
      }
      
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_CC) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_CC))
           continue;
         else if (l2Table[i].cfg.srio.cc != l2Entry.cfg.srio.cc)
           continue;  
      }
      
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_PRI) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_PRI))
           continue;
         else if (l2Table[i].cfg.srio.pri != l2Entry.cfg.srio.pri)
           continue;  
      }
      
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO))
           continue;
         else if (l2Table[i].cfg.srio.msgType != l2Entry.cfg.srio.msgType)
           continue;  
      }
      
      
      /* Note: It is the same aspa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX */ 
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID))
           continue;
         else if (l2Table[i].cfg.srio.typeParam1 != l2Entry.cfg.srio.typeParam1)
           continue;  
      }
      
      /* Note: It is the same aspa_SRIO_INFO_VALID_TYPE_INFO_LETTER */ 
      if (l2Entry.cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_COS) {
         if(!(l2Table[i].cfg.srio.validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_COS))
           continue;
         else if (l2Table[i].cfg.srio.typeParam2 != l2Entry.cfg.srio.typeParam2)
           continue;  
      }
      
        
      /* This is a more specific entry then one already in the table */
      ret = pa_INVALID_TABLE_MORE_SPECIFIC_ENTRY_PRESENT;                     
      goto Pa_addSrio_end;                 
    }
  }

  /* Find a free entry in the table */
  for (i = 0; i < paInst->l2n; i++)  {

    if (l2Table[i].hdr.status == PA_TBL_STAT_INACTIVE)
      break;

  }

  if (i == paInst->l2n)  {
    ret = pa_HANDLE_TABLE_FULL;
    goto Pa_addSrio_end;                 
  }
  
Pa_addSrio_send_cmd:
  
  /* The handle is just a pointer to the table entry */
  *handle = (paHandleL2L3_t)&l2Table[i];

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT1, PA_COMID_L2  | i, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al1  = (pafrmCommandAddLut1_t *) &(fcmd->cmd);

  al1->index         = SWIZ(lut1Index);
  al1->type          = PAFRM_COM_ADD_LUT1_SRIO;

  /* Form the matchflags and the key */
  al1->u.ethIp.matchFlags = PAFRM_LUT1_CUSTOM_MATCH_KEY;
  al1->u.srio.key         = PAFRM_LUT1_SRIO_KEY_SRIO;
                           
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_SRC_ID) {
    al1->u.srio.srcId        = SWIZ(l2Entry.cfg.srio.srcId);
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_SRCID; 
  }
  
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_DEST_ID) {
    al1->u.srio.destId       = SWIZ(l2Entry.cfg.srio.destId);
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_DESTID; 
  }
  
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_PRI) {
    al1->u.srio.pri          = SWIZ(l2Entry.cfg.srio.pri);
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_PRI; 
  }
  
  /* pa_SRIO_INFO_VALID_TYPE_INFO_MAILBOX */
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_STREAMID) {
    al1->u.srio.typeParam1   = SWIZ(l2Entry.cfg.srio.typeParam1);
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_TYPEPARAM1; 
  }
  
  /* pa_SRIO_INFO_VALID_TYPE_INFO_LETTER */
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO_COS) {
    al1->u.srio.typeParam2   = SWIZ(l2Entry.cfg.srio.typeParam2);
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_TYPEPARAM2; 
  }
  
  if(srioInfo->tt ==  pa_SRIO_TRANSPORT_TYPE_0)
    al1->u.srio.key |= PAFRM_LUT1_SRIO_KEY_TRANSPORT_8;
  else
    al1->u.srio.key |= PAFRM_LUT1_SRIO_KEY_TRANSPORT_16;
    
  if (srioInfo->validBitMap & pa_SRIO_INFO_VALID_TYPE_INFO)
  {  
    if(srioInfo->msgType == pa_SRIO_TYPE_9)
        al1->u.srio.msgType = PAFRM_LUT1_SRIO_MSG_TYPE_9;
    else  
        al1->u.srio.msgType = PAFRM_LUT1_SRIO_MSG_TYPE_11;
        
    al1->u.srio.msgType     = SWIZ(al1->u.srio.msgType);    
    al1->u.srio.matchFlags  |= PAFRM_LUT1_SRIO_MATCH_MSGTYPE;        
  }  
  
  al1->u.srio.nextHdr       = SWIZ(pa_next_hdr_tbl[nextHdr]);
  al1->u.srio.nextHdrOffset = SWIZ(nextHdrOffset);
  al1->u.srio.matchFlags    = SWIZ(al1->u.srio.matchFlags);
  al1->u.srio.key           = SWIZ(al1->u.srio.key);

  /* Forwarding information */
  ret1 = pa_conv_routing_info(paInst, &al1->match, routeInfo, pa_CMD_TX_DEST_0, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1; 
    goto Pa_addSrio_end;                 
  }

  /* Next fail information */
  ret1 = pa_conv_routing_info(paInst, &al1->nextFail, nextRtFail, pa_CMD_TX_DEST_0, TRUE);
  if (ret != pa_OK) {
    ret = ret1;
    goto Pa_addSrio_end;                 
     
  }

  if (ret != pa_DUP_ENTRY) {
     /* Initialze, the table entry, add the status and pdsp NUM */
     l2Entry.hdr.type     = PA_TABLE_ENTRY_TYPE_L2;
     l2Entry.hdr.subType  = PA_TABLE_ENTRY_SUBYTPE_SRIO;
     l2Entry.hdr.status   = PA_TBL_STAT_PENDING_SUBS_REPLY;
     l2Entry.hdr.pdspNum  = PASS_PDSP0;
     l2Entry.hdr.lutIdx   = -1;
     l2Entry.hdr.tableIdx = i;

     memcpy (&l2Table[i], &l2Entry, sizeof(paL2Entry_t));
  }

  /* The destination for this command must be PDSP 0 */
  *cmdDest = pa_CMD_TX_DEST_0;
  
Pa_addSrio_end:  
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  
  
  return (ret);

} /* Pa_addSrio */

/***************************************************************************************
 * FUNCTION PURPOSE: Delete an L2/L3 handle
 ***************************************************************************************
 * DESCRIPTION: The handle is deleted. Dependent handles are left intact
 ***************************************************************************************/
paReturn_t Pa_delHandle (Pa_Handle       iHandle,
                         paHandleL2L3_t  *handle, 
                         paCmd_t         cmd,
                         uint16_t        *cmdSize,
                         paCmdReply_t    *reply,
                         int             *cmdDest )

{
  paInst_t              *paInst  = (paInst_t *)iHandle;
  paL3Entry_t           *l3Table;
  paL2L3Header_t        *hdr     = (paL2L3Header_t *)*handle;
  paL3Entry_t           *l3e     = NULL;
  pafrmCommand_t        *fcmd;
  pafrmCommandDelLut1_t *del;
  uint16_t              csize, comId;
  paReturn_t            ret = pa_OK;
  uint32_t              mtCsKey;  
  
  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+sizeof(pafrmCommandDelLut1_t)-sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if ((reply->dest != pa_DEST_HOST) && (reply->dest != pa_DEST_DISCARD))
    return (pa_INVALID_CMD_REPLY_DEST);
    
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;

  /* Inform the host a table entry will be changed. Need to do both
   * since the table must be read before determining what type of
   * handle it is */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  /* Basic sanity check. The base address of the table is not verified, however */
  if ((hdr == NULL) || ((hdr->type != PA_TABLE_ENTRY_TYPE_L2) && (hdr->type != PA_TABLE_ENTRY_TYPE_L3))) {
    ret = pa_INVALID_INPUT_HANDLE;
    goto Pa_delHandle_end;
  }

  if (hdr->status == PA_TBL_STAT_INACTIVE)  {
    ret = pa_HANDLE_INACTIVE;
    goto Pa_delHandle_end;
  }

  if (hdr->type == PA_TABLE_ENTRY_TYPE_L2)
    comId = PA_COMID_L2 | hdr->tableIdx;
  else {
    comId = PA_COMID_L3 | hdr->tableIdx;
    l3e = &l3Table[hdr->tableIdx];
    
    if (l3e->pHandle) {
      paL2L3Header_t *hdr1 = (paL2L3Header_t *)l3e->pHandle;
      if (hdr1->lnkCnt > 0) 
      {
        hdr1->lnkCnt--;
      }
      else
      {
        ret = pa_WARN_LNK_CNT_UNSYNC;
      }
    }
    
  }
  
  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_DEL_LUT1, comId, csize);
  
  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  del = (pafrmCommandDelLut1_t *)&(fcmd->cmd);

  del->index = hdr->lutIdx;
  del->index = SWIZ(del->index);

  /* The command must be sent to the PDSP that owns the lut */
  *cmdDest = hdr->pdspNum + pa_CMD_TX_DEST_0;

  /* Mark the entry as disabled */
  hdr->status = PA_TBL_STAT_INACTIVE;
  
Pa_delHandle_end:
  
  /* Release the table */
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
                       
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  /* clear the handle */
  *handle = NULL;

  return (ret);

} /* Pa_delHandle */

/*******************************************************************************************
 * FUNCTION PURPOSE: Return number of linking channels
 *******************************************************************************************
 * DESCRIPTION: The function returns the number of channels linked to the LUT1 handle. 
 *
 ********************************************************************************************/
paReturn_t Pa_getHandleRefCount ( Pa_Handle       iHandle,
                                  paHandleL2L3_t  l2l3handle,
                                  uint16_t        *refCount )
{
  paInst_t              *paInst  = (paInst_t *)iHandle;
  paL2L3Header_t        *hdr     = (paL2L3Header_t *)l2l3handle;
  paReturn_t            ret = pa_OK;
  uint32_t              mtCsKey;  
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  /* Inform the host a table entry will be changed. Need to do both
   * since the table must be read before determining what type of
   * handle it is */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  
  if ((hdr == NULL) || ((hdr->type != PA_TABLE_ENTRY_TYPE_L2) && (hdr->type != PA_TABLE_ENTRY_TYPE_L3))) {
    *refCount = 0;                     
    ret = pa_INVALID_INPUT_HANDLE;
  }
  else
  {
    *refCount = hdr->lnkCnt;
  }
  /* Release the table */
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);

} /* Pa_getHandleRefCount */

/*******************************************************************************************
 * FUNCTION PURPOSE: Add an IP address to the lookup table
 *******************************************************************************************
 * DESCRIPTION: The IP address is verified and added to the table. 
 *
 *              If the IP address is linked to a MAC address then the entry is sent to PDSP1
 *              If the entry is not linked to a MAC address then the entry is sent to PDSP1
 *
 *              If the IP address is linked to another IP address the entry is sent to PDSP2
 *
 ********************************************************************************************/
paReturn_t  Pa_addIp  ( Pa_Handle       iHandle,
                        int             lutInst,
                        int             index, 
                        paIpInfo_t     *ipInfo,
                        paHandleL2L3_t  prevLink,
                        paRouteInfo_t  *routeInfo,
                        paRouteInfo_t  *nextRtFail,
                        paHandleL2L3_t *retHandle,
                        paCmd_t         cmd,
                        uint16_t       *cmdSize,
                        paCmdReply_t   *reply,
                        int            *cmdDest )
{

  /* The entry is created in the stack and then copied to the table. This allows
   * for a comparison with other table entries to verify there is not an identical
   * entry or an entry that would supercede this one */
  paInst_t               *paInst =  (paInst_t *)iHandle;
  paL3Entry_t            *l3Table;
  
  paL3Entry_t             l3Entry;
  paL2L3Header_t         *hdr;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut1_t  *al1;
  int                     i;
  uint16_t                csize;
  uint16_t                l2Release;
  uint8_t                 lut1Index = (index == pa_LUT1_INDEX_NOT_SPECIFIED)?PAFRM_LUT1_INDEX_LAST_FREE:(uint8_t)index;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint8_t                 numNextLayers = 0;   /* Specify the number of next matching layers */
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;
  
  /* Sanity check the LUT1 index */
  if (lut1Index > PAFRM_HW_LUT1_ENTRIES) {
    return(pa_INVALID_LUT1_INDEX);
  }
  
  /* Sanity check the LUT1 instance */
  if ((lutInst != pa_LUT_INST_NOT_SPECIFIED) && (lutInst > pa_LUT1_INST_MAX)) {
    return(pa_INVALID_LUT_INST);
  }
  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+sizeof(pafrmCommandAddLut1_t)-sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);

  /* It is not valid to have a GRE and SPI and SCTP in the search */
  if (ipInfo->spi != 0) numNextLayers++;
  if (ipInfo->greProto != 0) numNextLayers++;
  if (ipInfo->sctpPort != 0) numNextLayers++;
  if (numNextLayers > 1)
    return (pa_ERR_CONFIG);

  /* Create the entry on the stack, and zero the entire entry. This will
   * allow simple compare of entries using memcmp even though some fields
   * will be unused, or there will be gaps between fields */
  memset (&l3Entry, 0, sizeof(paL3Entry_t));

  /* For IPv4, copy the 4 address bytes to the last 4 bytes in an IPv6 sized byte array */
  if (ipInfo->ipType == pa_IPV4)  {
    memcpy (&(l3Entry.u.ipInfo.src.ipv6[sizeof(paIpv6Addr_t)-sizeof(paIpv4Addr_t)]), 
             ipInfo->src.ipv4, sizeof(paIpv4Addr_t));

    memcpy (&(l3Entry.u.ipInfo.dst.ipv6[sizeof(paIpv6Addr_t)-sizeof(paIpv4Addr_t)]), 
             ipInfo->dst.ipv4, sizeof(paIpv4Addr_t));

  }  else if (ipInfo->ipType == pa_IPV6)  {
    memcpy (&l3Entry.u.ipInfo.src.ipv6, ipInfo->src.ipv6, sizeof(paIpv6Addr_t));
    memcpy (&l3Entry.u.ipInfo.dst.ipv6, ipInfo->dst.ipv6, sizeof(paIpv6Addr_t));

  }  else
    return (pa_ERR_CONFIG);

  l3Entry.u.ipInfo.spi      =  ipInfo->spi;
  l3Entry.u.ipInfo.flow     =  ipInfo->flow;
  l3Entry.u.ipInfo.ipType   =  ipInfo->ipType;
  l3Entry.u.ipInfo.greProto =  ipInfo->greProto;
  l3Entry.u.ipInfo.proto    =  ipInfo->proto;
  l3Entry.u.ipInfo.tos      =  ipInfo->tos;
  l3Entry.u.ipInfo.tosCare  =  ipInfo->tosCare;
  l3Entry.u.ipInfo.sctpPort =  ipInfo->sctpPort;
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  /* Signal the application that a table modification will be done. */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  l2Release = FALSE;

  l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;

  /* If there is a link to the l2 table then that table must be
   * protected as well. Of course the previous link could be an l3
   * link as well as an l2 link. If so then the l2 protection is released */
  if (prevLink != NULL)  {

    l2Release = TRUE;
    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);

    hdr = (paL2L3Header_t *)prevLink;
    /* Must be linked to a complete entry */
    if (hdr->status != PA_TBL_STAT_ACTIVE)  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addIp_end;
    }

    if (hdr->type == PA_TABLE_ENTRY_TYPE_L3)  {
      Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
      l2Release = FALSE;
    }  else if (hdr->type != PA_TABLE_ENTRY_TYPE_L2)  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addIp_end;
    }

  }

  /* Look for an identical entry in the table. If one is found, return it */
  /* perform entry check only if the LUT1 index is not specified by user */
  if (lut1Index == PAFRM_LUT1_INDEX_LAST_FREE) {
    for (i = 0; i < paInst->l3n; i++)  {

      if ( ((l3Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) ||
            (l3Table[i].hdr.status == PA_TBL_STAT_ACTIVE)               ) &&
            (l3Table[i].hdr.subType == PA_TABLE_ENTRY_SUBYTPE_IP))    {

        if (!memcmp (&l3Table[i].u.ipInfo, &l3Entry.u.ipInfo, sizeof(paIpInfo_t)))  { 

          if (l3Table[i].pHandle == prevLink)  {

            if (l3Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) {
              *retHandle = (paHandleL2L3_t)&(l3Table[i]);
              ret = pa_INVALID_DUP_ENTRY;
              goto Pa_addIp_end;
                                   
            }                     
            else {
              /*  
               * Identical entry is identified, reuse the same table entry
               * Keep the entry status since it simply replaces the original entry
               * and the user may decide not to forward the packet
               */ 
              lut1Index = l3Table[i].hdr.lutIdx;
              ret = pa_DUP_ENTRY;   
              goto Pa_addIp_send_cmd;
            }                  
          }
        }
      }
    }


    /* Check to see if this entry is more general than one already in the table.
     * If so then this entry would 'steal' packets from that entry.
     * The order of the args to each of these function is important.
     */

    for (i = 0; i < paInst->l3n; i++)  {

      if ( (l3Table[i].hdr.status != PA_TBL_STAT_PENDING_SUBS_REPLY)  &&
           (l3Table[i].hdr.status != PA_TBL_STAT_ACTIVE)              )
        continue;
        
      if(l3Table[i].hdr.subType != PA_TABLE_ENTRY_SUBYTPE_IP)  
        continue;

      if (prevLink != l3Table[i].pHandle)
        continue;

      if (!pa_ip_match (l3Entry.u.ipInfo.src,  l3Table[i].u.ipInfo.src))
        continue;

      if (!pa_ip_match (l3Entry.u.ipInfo.dst,  l3Table[i].u.ipInfo.dst))
        continue;

      if (!pa_u32_match (l3Entry.u.ipInfo.spi,  l3Table[i].u.ipInfo.spi))
        continue;

      if (!pa_u32_match (l3Entry.u.ipInfo.flow, l3Table[i].u.ipInfo.flow))
        continue;

      if (!pa_u16_match (l3Entry.u.ipInfo.greProto, l3Table[i].u.ipInfo.greProto))
        continue;

      if (!pa_u8_match (l3Entry.u.ipInfo.proto, l3Table[i].u.ipInfo.proto))
        continue;

      if (!pa_tos_match (l3Entry.u.ipInfo.tos, l3Entry.u.ipInfo.tosCare, l3Table[i].u.ipInfo.tos))
        continue;
        
      if (!pa_u16_match (l3Entry.u.ipInfo.sctpPort, l3Table[i].u.ipInfo.sctpPort))
        continue;

      /* This is a more specific entry than one already in the table */
      ret = pa_INVALID_TABLE_MORE_SPECIFIC_ENTRY_PRESENT;
      goto Pa_addIp_end;

    }
  }
  
  /* The flow label is restricted to 20 bits */
  if (l3Entry.u.ipInfo.flow & ~PA_IP_FLOW_MASK)  {
    ret = pa_INVALID_IP_FLOW;
    goto Pa_addIp_end;
    
  }

  /* Find a free entry in the table */
  for (i = 0; i < paInst->l3n; i++)  {

    if (l3Table[i].hdr.status == PA_TBL_STAT_INACTIVE)
      break;
  }

  if (i == paInst->l3n)  {
    ret = pa_HANDLE_TABLE_FULL;
    goto Pa_addIp_end;
  }
  
Pa_addIp_send_cmd:
  
  /* The handle is just a pointer to the table entry */
  *retHandle = (paHandleL2L3_t)&l3Table[i];

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT1, PA_COMID_L3  | i, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al1  = (pafrmCommandAddLut1_t *) &(fcmd->cmd);

  al1->index         = SWIZ(lut1Index);
  al1->type          = PAFRM_COM_ADD_LUT1_STANDARD;

  /* Ethertype holds the linked PDSP ID */
  /* vlan holds the linked PDSP LUT index */
  if (prevLink != NULL)  {
    al1->u.ethIp.etype = SWIZ((uint16_t)hdr->pdspNum);
    al1->u.ethIp.vlan  = SWIZ((uint16_t)hdr->lutIdx);
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_LINK;
  }

  /* The IP type has already been validated as either IPv4 or IPv6. IPv4
   * addresses have been pre-padded with 0 to be t`he same size as IPv6 */

  if (!pa_ip_zero (l3Entry.u.ipInfo.src))
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_SIP;
    memcpy (al1->u.ethIp.srcIp, &l3Entry.u.ipInfo.src, sizeof(paIpv6Addr_t));
  }  

  if (!pa_ip_zero (l3Entry.u.ipInfo.dst))
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_DIP;
    memcpy (al1->u.ethIp.dstIp, &l3Entry.u.ipInfo.dst, sizeof(paIpv6Addr_t));
  }  
  
  if (al1->u.ethIp.matchFlags & (PAFRM_LUT1_MATCH_SIP|PAFRM_LUT1_MATCH_DIP))
  {
    al1->u.ethIp.matchFlags |= PAFRM_LUT1_MATCH_KEY;
    
    /* The validity of ipType was already checked */
    if (l3Entry.u.ipInfo.ipType == pa_IPV4)
        al1->u.ethIp.key  |=  PAFRM_LUT1_KEY_IPV4;
    else
        al1->u.ethIp.key  |=  PAFRM_LUT1_KEY_IPV6;
    
  }

  if (l3Entry.u.ipInfo.spi != 0)  {
    al1->u.ethIp.spi         =  SWIZ(l3Entry.u.ipInfo.spi);
    al1->u.ethIp.inport     =  PAFRM_LUT1_SPI;
  }

  if (l3Entry.u.ipInfo.greProto != 0)  {
    al1->u.ethIp.spi         =  SWIZ((uint32_t)l3Entry.u.ipInfo.greProto);
    al1->u.ethIp.inport     =  PAFRM_LUT1_GRE;
  }
  
  if (l3Entry.u.ipInfo.sctpPort != 0)  {
    al1->u.ethIp.spi        =   SWIZ((uint32_t)l3Entry.u.ipInfo.sctpPort);
    al1->u.ethIp.inport     =  PAFRM_LUT1_SCTP;
  }
  
  if (al1->u.ethIp.inport)
  {
    al1->u.ethIp.matchFlags |=  (PAFRM_LUT1_MATCH_SPI_GRE_SCTP|PAFRM_LUT1_MATCH_PORT);
    al1->u.ethIp.inport     =   SWIZ(al1->u.ethIp.inport);
  }
  
  if (l3Entry.u.ipInfo.flow != 0)
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_FLOW;
    al1->u.ethIp.flow        =  SWIZ(l3Entry.u.ipInfo.flow);
  }  
  
  if (l3Entry.u.ipInfo.proto != 0)
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_PROTO;
    al1->u.ethIp.protoNext = l3Entry.u.ipInfo.proto;
  }  

  if (l3Entry.u.ipInfo.tosCare)
  {
    al1->u.ethIp.matchFlags |=  PAFRM_LUT1_MATCH_TOS;
    al1->u.ethIp.tosTclass = l3Entry.u.ipInfo.tos;
  }  

  al1->u.ethIp.matchFlags = SWIZ(al1->u.ethIp.matchFlags);  
  al1->u.ethIp.key        = SWIZ(al1->u.ethIp.key); 
  
  ret1 = pa_conv_routing_info(paInst, &al1->match, routeInfo, *cmdDest, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addIp_end;
  }

  /* Next fail information */
  ret1 = pa_conv_routing_info(paInst, &al1->nextFail, nextRtFail, *cmdDest, TRUE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addIp_end;
  }
  
  /* find the command destination */
  if (lutInst != pa_LUT_INST_NOT_SPECIFIED) {
    switch (lutInst)
    {
        case pa_LUT1_INST_0:
            l3Entry.hdr.pdspNum = PASS_PDSP0;
            *cmdDest            = pa_CMD_TX_DEST_0;
            break;
            
        case pa_LUT1_INST_1:
            l3Entry.hdr.pdspNum = PASS_PDSP1;
            *cmdDest            = pa_CMD_TX_DEST_1;
            break;
            
        case pa_LUT1_INST_2:
            l3Entry.hdr.pdspNum = PASS_PDSP2;
            *cmdDest            = pa_CMD_TX_DEST_2;
            break;
    }
  
  }
  else if (prevLink == NULL)  {

    l3Entry.hdr.pdspNum = PASS_PDSP1;
    *cmdDest            = pa_CMD_TX_DEST_1;

  }  else  {

    if (hdr->type == PA_TABLE_ENTRY_TYPE_L2)  {
      l3Entry.hdr.pdspNum = PASS_PDSP1;
      *cmdDest            = pa_CMD_TX_DEST_1;
    }  else  {
      l3Entry.hdr.pdspNum = PASS_PDSP2;
      *cmdDest            = pa_CMD_TX_DEST_2;
    }

  }
  

  if (rmPermCallTbl) {

      resourceData.res_info.lutEntry = *cmdDest;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
          ret = pa_RESOURCE_USE_DENIED;
          goto Pa_addIp_end;
      }
  }
   

  if (ret != pa_DUP_ENTRY)
  {
    /* Add the status and pdsp NUM */
    l3Entry.hdr.status   = PA_TBL_STAT_PENDING_SUBS_REPLY;
    l3Entry.pHandle      = prevLink;
    l3Entry.hdr.type     = PA_TABLE_ENTRY_TYPE_L3;
    l3Entry.hdr.subType  = PA_TABLE_ENTRY_SUBYTPE_IP;
    l3Entry.hdr.tableIdx = i;
    l3Entry.hdr.lutIdx   = -1;
  
    memcpy (&l3Table[i], &l3Entry, sizeof(paL3Entry_t));
  }
  
  
Pa_addIp_end:  

  if (l2Release) Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                                      paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
                       
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);

} /* Pa_addIp */

/****************************************************************************
 * FUNCTION PURPOSE: Add a destination TCP/UDP port to the lookup
 ****************************************************************************
 * DESCRIPTION: The destination port is added to the lookup criteria.
 ****************************************************************************/
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
                         int            *cmdDest )

{

  paInst_t               *paInst  = (paInst_t *)iHandle;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut2_t  *al2;
  paL2L3Header_t         *hdr;
  paL4Entry_t            *hl4;
  uint16_t                csize;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;

  if (rmPermCallTbl) {

      resourceData.res_info.lutEntry = pa_CMD_TX_DEST_3;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
          return pa_RESOURCE_USE_DENIED;
      }
  }
  
  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandAddLut2_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);
  
  *cmdSize = csize;

  /* The reply generated by the PASS must go to the host */
  if  (reply->dest != pa_DEST_HOST) 
    return (pa_INVALID_CMD_REPLY_DEST);
    
  /* Sanity check: 16-bit or 32-bit ports only */
  if ((portSize != pa_LUT2_PORT_SIZE_16) && (portSize != pa_LUT2_PORT_SIZE_32))
    return ( pa_ERR_CONFIG );
    
  if((!fReplace) && (divertQ != pa_PARAMS_NOT_SPECIFIED))
    return ( pa_ERR_CONFIG );  

  /* Information from the linked handle */
  hdr = (paL2L3Header_t *) linkHandle;

  /* Initialize the return handle */
  hl4 = (paL4Entry_t *)    retHandle;
  memset (hl4, 0, sizeof(paL4Entry_t));
  hl4->lnkType = PA_TABLE_ENTRY_TYPE_NONE;
  hl4->subType = (portSize == pa_LUT2_PORT_SIZE_16)?PA_TABLE_ENTRY_SUBYTPE_PORT16:
                                                    PA_TABLE_ENTRY_SUBYTPE_PORT32;
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  /* Verify the link */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  if ((hdr != NULL) && (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT16))  {

    if ( (hdr->type != PA_TABLE_ENTRY_TYPE_L3) ||
         (hdr->status == PA_TBL_STAT_INACTIVE)     )  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addPort_end;
    }

    hl4->lnkType = PA_TABLE_ENTRY_TYPE_L3;
    hl4->lnkTableIdx = hdr->tableIdx;
    hl4->u.portInfo.lnkP  = hdr->pdspNum;
    hl4->u.portInfo.lnkId = hdr->lutIdx;

  }

  if (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT16) {
    hl4->u.portInfo.destPort = (uint16_t)destPort;
  }
  else {
    hl4->u.port = destPort;
  }

  /* Create the command */   
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT2, 0, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al2 = (pafrmCommandAddLut2_t *) &(fcmd->cmd);

  al2->type  = PAFRM_COM_ADD_LUT2_STANDARD;
  al2->ctrlBitMap |= (fReplace)?PAFRM_COM_LUT2_CTRL_REPLACE:0;
  
  if (hl4->lnkType == PA_TABLE_ENTRY_TYPE_L3) {
    al2->inkTableIdx = hl4->lnkTableIdx;
    al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_LINK;
  }
  
  if (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT16) {
  
    al2->u.port.data[1] = (uint8_t) (destPort >> 8);
    al2->u.port.data[2] = destPort & 0xFF;
    al2->u.port.data[3] = PAFRM_FORM_CUSTOM_L4_LINK(hl4->u.portInfo.lnkP, hl4->u.portInfo.lnkId);
    
    if(destPort == PAFRM_DEST_PORT_GTP)
        al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_GTPU;
    
  } else  {
    al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_PORT32;
    al2->u.port.data[0] = (uint8_t) (destPort >> 24);
    al2->u.port.data[1] = (uint8_t) (destPort >> 16);
    al2->u.port.data[2] = (uint8_t) (destPort >> 8);
    al2->u.port.data[3] = destPort & 0xFF;;
  }
  
  /* Queue Divert information */
  if (divertQ != pa_PARAMS_NOT_SPECIFIED) {
    al2->qDivert.srcQ  = SWIZ(divertQ);
    al2->qDivert.destQ = SWIZ(routeInfo->queue);
    al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_QUEUE_DIVERT;
  }
  al2->ctrlBitMap   = SWIZ(al2->ctrlBitMap);  
  al2->inkTableIdx  = SWIZ(al2->inkTableIdx);
     
  /* Forwarding information */
  ret1 = pa_conv_routing_info(paInst, &al2->match, routeInfo, pa_CMD_TX_DEST_3, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addPort_end;
  }
  
  /* Update the link counter */
  if ((hdr != NULL) && (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT16)) {
    hdr->lnkCnt++;
  }
  
  /* Only one valid destination for a LUT2 add */
  *cmdDest = pa_CMD_TX_DEST_3;
  
Pa_addPort_end:  

  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);

} /* Pa_addPort */

/***************************************************************************************
 * FUNCTION PURPOSE: Configure the PA L3 (LUT1)  custom lookup
 ***************************************************************************************
 * DESCRIPTION: The per-system L3 (LUT1)custom configuration parameters are setup
 ***************************************************************************************/
paReturn_t Pa_setCustomLUT1 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              uint16_t        parseByteOffset,
                              uint16_t        nextHdr,
                              uint16_t        nextHdrOffset,
                              uint8_t         byteMasks[pa_NUM_BYTES_CUSTOM_LUT1],
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest )
{
  uint16_t                csize;
  pafrmCommand_t         *fcmd;
  pafrmCommandSysConfigPa_t *ccfg;
  
  /* Sanity check: custom index range check */
  if (custIndex >= pa_MAX_CUSTOM_TYPES_LUT1)
    return ( pa_ERR_CONFIG );
    
  if (nextHdr > pa_HDR_TYPE_UNKNOWN)
    return ( pa_ERR_CONFIG ); 

  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+offsetof(pafrmCommandSysConfigPa_t, u) - sizeof(uint32_t) + sizeof(pafrmC1Custom_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_SYS_CONFIG, 0, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  ccfg = (pafrmCommandSysConfigPa_t *) &(fcmd->cmd);

  ccfg->cfgCode = PAFRM_SYSTEM_CONFIG_CODE_CUSTOM_LUT1;
  ccfg->u.customC1Config.idx    = (uint8_t)custIndex;
  ccfg->u.customC1Config.offset = SWIZ(parseByteOffset);
  ccfg->u.customC1Config.nextHdr = SWIZ(pa_next_hdr_tbl[nextHdr]);
  ccfg->u.customC1Config.nextHdrOffset = SWIZ(nextHdrOffset);
  
  memcpy (ccfg->u.customC1Config.bitMask, byteMasks, sizeof(ccfg->u.customC1Config.bitMask));

  /* Any PDSP can be used to handle the command. PDSP5 is chosen
   * since it is typically lightly loaded */
  *cmdDest = pa_CMD_TX_DEST_5;

  return (pa_OK);

} /* Pa_setCustomLUT1 */
                            
/*********************************************************************
 * FUNCTION PURPOSE: Add a custom L3 (LUT1) lookup to the tables
 *********************************************************************
 * DESCRIPTION: The entry is added to the lookup table (LUT1)
 *********************************************************************/
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
                              int            *cmdDest )
{
  /* The entry is created in the stack and then copied to the table. This allows
   * for a comparison with other table entries to verify there is not an identical
   * entry or an entry that would supercede this one */
  paInst_t               *paInst =  (paInst_t *)iHandle;
  paL3Entry_t            *l3Table;
  
  paL3Entry_t             l3Entry;
  paL2L3Header_t         *hdr;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut1_t  *al1;
  int                     i;
  uint16_t                csize;
  uint16_t                l2Release;
  uint8_t                 lut1Index = (index == pa_LUT1_INDEX_NOT_SPECIFIED)?PAFRM_LUT1_INDEX_LAST_FREE:(uint8_t)index;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;
  
  /* Sanity check: custom index range check */
  if (custIndex >= pa_MAX_CUSTOM_TYPES_LUT1)
    return ( pa_ERR_CONFIG );

  /* Sanity check the LUT1 instance */
  if ((lutInst != pa_LUT_INST_NOT_SPECIFIED) && (lutInst > pa_LUT1_INST_MAX)) {
    return(pa_INVALID_LUT_INST);
  }
  
  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+sizeof(pafrmCommandAddLut1_t)-sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Create the entry on the stack, and zero the entire entry. This will
   * allow simple compare of entries using memcmp even though some fields
   * will be unused, or there will be gaps between fields */
  memset (&l3Entry, 0, sizeof(paL3Entry_t));

  /* Copy the custom information into the proposed table entry */
  memcpy (l3Entry.u.customInfo.match, match, sizeof(l3Entry.u.customInfo.match));
  l3Entry.u.customInfo.customIdx = (uint8_t)custIndex;
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;

  /* Signal the application that a table modification will be done. */
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  l2Release = FALSE;

  /* If there is a link to the l2 table then that table must be
   * protected as well. Of course the previous link could be an l3
   * link as well as an l2 link. If so then the l2 protection is released */
  if (prevLink != NULL)  {
    l2Release = TRUE;
    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);

    hdr = (paL2L3Header_t *)prevLink;

    /* Must be linked to a complete entry */
    if (hdr->status != PA_TBL_STAT_ACTIVE)  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addCustomLUT1_end;
    }

    if (hdr->type == PA_TABLE_ENTRY_TYPE_L3) {
      Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
      l2Release = FALSE;
    }  else if (hdr->type != PA_TABLE_ENTRY_TYPE_L2)  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addCustomLUT1_end;
    }

  }

  /* Look for an identical entry in the table. If one is found, return it */
  /* perform entry check only if the LUT1 index is not specified by user */
  if (lut1Index == PAFRM_LUT1_INDEX_LAST_FREE) {
    for (i = 0; i < paInst->l3n; i++)  {

      if ( ((l3Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) ||
            (l3Table[i].hdr.status == PA_TBL_STAT_ACTIVE)               ) &&
            (l3Table[i].hdr.subType == PA_TABLE_ENTRY_SUBYTPE_CUSTOM)        )    {

        if (!memcmp (&l3Table[i].u.customInfo, &l3Entry.u.customInfo, sizeof(l3Entry.u.customInfo)))  {

          if (l3Table[i].pHandle == prevLink)  {

            if (l3Table[i].hdr.status == PA_TBL_STAT_PENDING_SUBS_REPLY) {
              *retHandle = (paHandleL2L3_t)&(l3Table[i]);
              ret = pa_INVALID_DUP_ENTRY;   
              goto Pa_addCustomLUT1_end;
                                
            }                     
            else {
              /*  
               * Identical entry is identified, reuse the same table entry
               * Keep the entry status since it simply replaces the original entry
               * and the user may decide not to forward the packet
               */ 
              lut1Index = l3Table[i].hdr.lutIdx;
              ret = pa_DUP_ENTRY;   
              goto Pa_addCustomLut1_send_cmd;
            }                  
          }
        }
      }
    }
  }

  /* Find a free entry in the table */
  for (i = 0; i < paInst->l3n; i++)  {

    if (l3Table[i].hdr.status == PA_TBL_STAT_INACTIVE)
      break;

  }

  if (i == paInst->l3n)  {
    ret = pa_HANDLE_TABLE_FULL;
    goto Pa_addCustomLUT1_end;
  }
  
Pa_addCustomLut1_send_cmd:
  
  /* The handle is just a pointer to the table entry */
  *retHandle = (paHandleL2L3_t)&l3Table[i];

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT1, PA_COMID_L3  | i, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al1  = (pafrmCommandAddLut1_t *) &(fcmd->cmd);

  al1->index         = SWIZ(lut1Index);
  al1->type          = PAFRM_COM_ADD_LUT1_CUSTOM;

  /* Ethertype holds the linked PDSP ID */
  /* vlan holds the linked PDSP LUT index */
  if (prevLink != NULL)  {
    al1->u.custom.etype = SWIZ((uint16_t)hdr->pdspNum);
    al1->u.custom.vlan  = SWIZ((uint16_t)hdr->lutIdx);
  }

  memcpy (al1->u.custom.matchValues, &l3Entry.u.customInfo.match, sizeof(l3Entry.u.customInfo.match));

  /* Form the matchflags and the key */
  al1->u.custom.matchFlags = PAFRM_LUT1_CUSTOM_MATCH_MATCH | PAFRM_LUT1_CUSTOM_MATCH_KEY ;
  al1->u.custom.key        = PAFRM_LUT1_CUSTOM_KEY_CUSTOM + PAFRM_LUT1_CUSTOM_KEY_INDEX(custIndex);

  if (prevLink != NULL) 
    al1->u.custom.matchFlags |=  (PAFRM_LUT1_CUSTOM_MATCH_ETYPE | PAFRM_LUT1_CUSTOM_MATCH_VLAN);
    
  al1->u.custom.matchFlags = SWIZ(al1->u.custom.matchFlags); 
  al1->u.custom.key        = SWIZ(al1->u.custom.key);  
    

  /* Forwarding information */
  ret1 = pa_conv_routing_info(paInst, &al1->match, routeInfo, *cmdDest, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addCustomLUT1_end;
  }

  /* Next fail information */
  ret1 = pa_conv_routing_info(paInst, &al1->nextFail, nextRtFail, *cmdDest, TRUE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addCustomLUT1_end;
  }
  
  /* find the command destination */
  if (lutInst != pa_LUT_INST_NOT_SPECIFIED) {
    switch (lutInst)
    {
        case pa_LUT1_INST_0:
            l3Entry.hdr.pdspNum = PASS_PDSP0;
            *cmdDest            = pa_CMD_TX_DEST_0;
            break;
            
        case pa_LUT1_INST_1:
            l3Entry.hdr.pdspNum = PASS_PDSP1;
            *cmdDest            = pa_CMD_TX_DEST_1;
            break;
            
        case pa_LUT1_INST_2:
            l3Entry.hdr.pdspNum = PASS_PDSP2;
            *cmdDest            = pa_CMD_TX_DEST_2;
            break;
    }
  
  }
  else if (prevLink == NULL)  {

    l3Entry.hdr.pdspNum = PASS_PDSP1;
    *cmdDest            = pa_CMD_TX_DEST_1;

  }  else  {

    if (hdr->type == PA_TABLE_ENTRY_TYPE_L2)  {
      l3Entry.hdr.pdspNum = PASS_PDSP1;
      *cmdDest            = pa_CMD_TX_DEST_1;
    }  else  {
      l3Entry.hdr.pdspNum = PASS_PDSP2;
      *cmdDest            = pa_CMD_TX_DEST_2;
    }

  if (rmPermCallTbl) {

      resourceData.res_info.lutEntry = *cmdDest;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
          ret = pa_RESOURCE_USE_DENIED;
          goto Pa_addCustomLUT1_end;
      }
  }

  }

  if (ret != pa_DUP_ENTRY)
  {
    /* Add the status and pdsp NUM */
    l3Entry.hdr.status  = PA_TBL_STAT_PENDING_SUBS_REPLY;
    l3Entry.pHandle      = prevLink;
    l3Entry.hdr.type     = PA_TABLE_ENTRY_TYPE_L3;
    l3Entry.hdr.subType  = PA_TABLE_ENTRY_SUBYTPE_CUSTOM;
    l3Entry.hdr.tableIdx = i;
    l3Entry.hdr.lutIdx   = -1;
  
    memcpy (&l3Table[i], &l3Entry, sizeof(paL3Entry_t));
  }
  
Pa_addCustomLUT1_end:  

  if (l2Release) Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                                      paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
                       
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);
} /* Pa_addCustomLUT1 */

/***************************************************************************************
 * FUNCTION PURPOSE: Configure the PA L4 custom lookup
 ***************************************************************************************
 * DESCRIPTION: The per-system L4 custom configuration parameters are setup
 ***************************************************************************************/
paReturn_t Pa_setCustomLUT2 ( Pa_Handle       iHandle,
                              uint16_t        custIndex,
                              uint16_t        handleLink,
                              uint16_t        byteOffsets[pa_NUM_BYTES_CUSTOM_LUT2],
                              uint8_t         byteMasks[pa_NUM_BYTES_CUSTOM_LUT2],
                              uint8_t         setMask,
                              paCmd_t         cmd,
                              uint16_t       *cmdSize,
                              paCmdReply_t   *reply,
                              int            *cmdDest )
{
  uint16_t                csize;
  pafrmCommand_t         *fcmd;
  pafrmCommandSysConfigPa_t *ccfg;
  int                     i;
  
  /* Sanity check: custom index range check */
  if (custIndex >= pa_MAX_CUSTOM_TYPES_LUT2)
    return ( pa_ERR_CONFIG );

  /* Sanity check the command buffer */
  csize = sizeof(pafrmCommand_t)+offsetof(pafrmCommandSysConfigPa_t, u) - sizeof(uint32_t) + sizeof(pafrmC2Custom_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  /* Return the actual size of the buffer used */
  *cmdSize = csize;

  /* Sanity check the command reply information.  */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);

  /* The byte offsets must be in increasing order */
  for (i = 0; i < pa_NUM_BYTES_CUSTOM_LUT2-1; i++)
    if (byteOffsets[i] >= byteOffsets[i+1])
      return (pa_ERR_CONFIG);

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_SYS_CONFIG, 0, csize);
  
  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  ccfg = (pafrmCommandSysConfigPa_t *) &(fcmd->cmd);

  ccfg->cfgCode = PAFRM_SYSTEM_CONFIG_CODE_CUSTOM_LUT2;
  ccfg->u.customC2Config.idx         = (uint8_t)custIndex;
  ccfg->u.customC2Config.bitSet      = SWIZ(setMask);
  ccfg->u.customC2Config.ctrlBitMap  = (handleLink)?PAFRM_C2_CUSTOM_CTRL_USE_LINK:0;
  ccfg->u.customC2Config.ctrlBitMap  = SWIZ(ccfg->u.customC2Config.ctrlBitMap); 
    for(i = 0; i < 4; i++)
    ccfg->u.customC2Config.offset[i] = SWIZ(byteOffsets[i]);
  memcpy (ccfg->u.customC2Config.bitMask, byteMasks,   sizeof(ccfg->u.customC2Config.bitMask));

  /* Only PDSP0 or PDSP3 can be used due to constant register configuration */
  *cmdDest = pa_CMD_TX_DEST_0;

  return (pa_OK);

} /* Pa_setCustomLUT2 */

/*********************************************************************
 * FUNCTION PURPOSE: Add a custom LUT2 lookup to the tables
 *********************************************************************
 * DESCRIPTION: The custom entry is added to the lookup table (LUT2)
 *********************************************************************/
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
                              int            *cmdDest )
{
  paInst_t               *paInst  = (paInst_t *)iHandle;
  pafrmCommand_t         *fcmd;
  pafrmCommandAddLut2_t  *al2;
  paL2L3Header_t         *hdr;
  paL4Entry_t            *hl4;
  uint16_t                csize;
  paReturn_t              ret = pa_OK;
  paReturn_t              ret1;
  uint32_t                mtCsKey;  
  Rm_LldPermCallouts     *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo         resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_LUT;

  if (rmPermCallTbl) {

      resourceData.res_info.lutEntry = pa_CMD_TX_DEST_3;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED) {
          return pa_RESOURCE_USE_DENIED;
      }
  }
  
  /* Sanity check: custom index range check */
  if (custIndex >= pa_MAX_CUSTOM_TYPES_LUT2)
    return ( pa_ERR_CONFIG );
    
  if((!fReplace) && (divertQ != pa_PARAMS_NOT_SPECIFIED))
    return ( pa_ERR_CONFIG );  

  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandAddLut2_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);
  
  *cmdSize = csize;

  /* The reply generated by the PASS must go to the host */
  if  (reply->dest != pa_DEST_HOST) 
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Information from the linked handle */
  hdr = (paL2L3Header_t *) prevLink;

  /* Initialize the return handle */
  hl4 = (paL4Entry_t *)    retHandle;
  memset (hl4, 0, sizeof(paL4Entry_t));
  hl4->lnkType = PA_TABLE_ENTRY_TYPE_NONE;
  hl4->subType = PA_TABLE_ENTRY_SUBYTPE_CUSTOM;
  hl4->customIndex = (uint8_t)custIndex;
  
  /* Copy all the match bytes even if the last one is used for link */
  memcpy (hl4->u.customInfo, match, sizeof(hl4->u.customInfo));

  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  /* Verify the link */
  if (hdr != NULL)  {
    if ( (hdr->type   != PA_TABLE_ENTRY_TYPE_L3) ||
         (hdr->status == PA_TBL_STAT_INACTIVE)     )  {
      ret = pa_INVALID_INPUT_HANDLE;
      goto Pa_addCustomLUT2_end;
    }
    
    hl4->lnkType = PA_TABLE_ENTRY_TYPE_L3;
    hl4->lnkTableIdx = hdr->tableIdx;
    hl4->u.customInfo[3] = PAFRM_FORM_CUSTOM_L4_LINK(hdr->pdspNum, hdr->lutIdx);
  }
  
  /* Create the command */   
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_ADDREP_LUT2, 0, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  al2 = (pafrmCommandAddLut2_t *) &(fcmd->cmd);

  al2->type  = PAFRM_COM_ADD_LUT2_CUSTOM;
  al2->index = (uint8_t)custIndex;
  al2->ctrlBitMap |= (fReplace)?PAFRM_COM_LUT2_CTRL_REPLACE:0;
  
  if (hl4->lnkType == PA_TABLE_ENTRY_TYPE_L3) {
    al2->inkTableIdx = hl4->lnkTableIdx;
    al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_LINK;
  }

  memcpy (al2->u.custom.match, hl4->u.customInfo, sizeof (al2->u.custom.match));
  
  /* Queue Divert information */
  if (divertQ != pa_PARAMS_NOT_SPECIFIED) {
    al2->qDivert.srcQ  = SWIZ(divertQ);
    al2->qDivert.destQ = SWIZ(routeInfo->queue);
    al2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_QUEUE_DIVERT;
  }
  
  al2->ctrlBitMap   = SWIZ(al2->ctrlBitMap);  
  al2->inkTableIdx  = SWIZ(al2->inkTableIdx);

  /* Forwarding information */
  ret1 = pa_conv_routing_info(paInst, &al2->match, routeInfo, pa_CMD_TX_DEST_3, FALSE);
  if (ret1 != pa_OK) {
    ret = ret1;
    goto Pa_addCustomLUT2_end;
  }
  
  /* Update the link counter */
  if (hdr != NULL) {
    hdr->lnkCnt++;
  }
  
  /* Only one valid destination for a LUT2 add */
  *cmdDest = pa_CMD_TX_DEST_3;
  
Pa_addCustomLUT2_end:  

  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);

} /* Pa_addCustomLUT2 */

/***************************************************************************************
 * FUNCTION PURPOSE: Delete an L4(LUT2) handle
 ***************************************************************************************
 * DESCRIPTION: The handle is deleted. Dependent handles are left intact
 ***************************************************************************************/
paReturn_t Pa_delL4Handle (Pa_Handle      iHandle,
                           paHandleL4_t   handle, 
                           paCmd_t        cmd,
                           uint16_t      *cmdSize,
                           paCmdReply_t  *reply,
                           int           *cmdDest )

{
  paInst_t              *paInst  = (paInst_t *)iHandle;
  paL4Entry_t           *hl4;
  paL3Entry_t           *l3Table;
  paL3Entry_t           *l3e = NULL;
  paL2L3Header_t        *hdr;
  pafrmCommand_t        *fcmd;
  pafrmCommandDelLut2_t *dl2;
  uint16_t               csize;
  paReturn_t             ret = pa_OK;
  uint32_t               mtCsKey;  
  
  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandDelLut2_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;
  
  /* The reply generated by the PASS must go to the host */
  if (reply->dest != pa_DEST_HOST)
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Sanity check the handle */
  hl4 = (paL4Entry_t *)handle;
  if ((hl4->lnkType != PA_TABLE_ENTRY_TYPE_L3) && (hl4->lnkType != PA_TABLE_ENTRY_TYPE_NONE))
    return (pa_INVALID_INPUT_HANDLE);
    
  if (hl4->lnkType == PA_TABLE_ENTRY_TYPE_L3) {
    /* Refresh PA Instance for read only */
    Pa_osalMtCsEnter(&mtCsKey);
    Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
    l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;
  
    /* Inform the host a table entry will be changed */ 
    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
                           
      l3e = &l3Table[hl4->lnkTableIdx];
      hdr = &(l3e->hdr);
      if (hdr->lnkCnt > 0)
      {
        hdr->lnkCnt--; 
      }
      else
      {
        ret = pa_WARN_LNK_CNT_UNSYNC;  
      }                        

    Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
    Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
    Pa_osalMtCsExit(mtCsKey);  
  }

  /* Create the command */   
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_DEL_LUT2, 0, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  dl2 = (pafrmCommandDelLut2_t *) &(fcmd->cmd);
  dl2->type = PAFRM_COM_DEL_LUT2_STANDARD;
  
  if (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT16) {
  
    dl2->u.port.data[1] = (uint8_t) (hl4->u.portInfo.destPort >> 8);
    dl2->u.port.data[2] = hl4->u.portInfo.destPort & 0xFF;
    dl2->u.port.data[3] = PAFRM_FORM_CUSTOM_L4_LINK(hl4->u.portInfo.lnkP, hl4->u.portInfo.lnkId);
    
    if(hl4->u.portInfo.destPort == PAFRM_DEST_PORT_GTP)
        dl2->ctrlBitMap |= PAFRM_COM_LUT2_CTRL_GTPU;
    
  } else if (hl4->subType == PA_TABLE_ENTRY_SUBYTPE_PORT32){
  
    dl2->u.port.data[0] = (uint8_t) (hl4->u.port >> 24);
    dl2->u.port.data[1] = (uint8_t) (hl4->u.port >> 16);
    dl2->u.port.data[2] = (uint8_t) (hl4->u.port >> 8);
    dl2->u.port.data[3] = hl4->u.port & 0xFF;;
    
  } else {

    dl2->type = PAFRM_COM_DEL_LUT2_CUSTOM;
    dl2->index =  hl4->customIndex;
    memcpy (dl2->u.custom.match, hl4->u.customInfo, sizeof(dl2->u.custom.match));
  }
  dl2->index        = SWIZ(dl2->index);
  dl2->ctrlBitMap   = SWIZ(dl2->ctrlBitMap);
  
  /* Only one valid destination for a LUT2 add */
  *cmdDest = pa_CMD_TX_DEST_3;
  
  /* clear the L4 handle */
  memset(hl4, 0, sizeof(hl4));

  return (ret);

} /* Pa_delL4Handle */

/******************************************************************************************************
 * FUNCTION PUROPSE: Process result from PASS subsystem
 ******************************************************************************************************
 * DESCRIPTION: The sub-system responded to a command, the result is verified
 ******************************************************************************************************/
paReturn_t Pa_forwardResult (Pa_Handle iHandle, void *vresult, paEntryHandle_t *retHandle, int *handleType, int *cmdDest)
{
  paInst_t              *paInst = (paInst_t *)iHandle;
  paL2Entry_t           *l2Table;
  paL3Entry_t           *l3Table;
  pafrmCommand_t        *fcmd;
  pafrmCommandAddLut1_t *al1;
  pafrmCommandAddLut2_t *al2;
  paL2Entry_t           *l2e;
  paL3Entry_t           *l3e = NULL;
  paL2L3Header_t        *hdr;
  paL4Entry_t           *pL4Entry = (paL4Entry_t *)&retHandle->l4Handle;
  int8_t                 origStatus;
  paReturn_t             ret = pa_OK;
  uint32_t               mtCsKey;  

  /* The buffer contains the complete formatted command */
  fcmd = (pafrmCommand_t *)vresult;
  swizFcmd(fcmd);

  memset(retHandle, 0, sizeof(paEntryHandle_t));
  *handleType = pa_INVALID_HANDLE;
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  l2Table = (paL2Entry_t *)paInst->paBufs[PA_BUFFNUM_L2_TABLE].base;
  l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;
  
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  if (fcmd->command == PAFRM_CONFIG_COMMAND_ADDREP_LUT1)  {

    al1 = (pafrmCommandAddLut1_t *)&(fcmd->cmd);

    if ((fcmd->comId & PA_COMID_L_MASK) == PA_COMID_L2)  {

      l2e = &l2Table[fcmd->comId & PA_COMID_IDX_MASK];
      hdr = &(l2e->hdr);
      retHandle->l2l3Handle = (paHandleL2L3_t)l2e;
      *handleType           =  pa_L2_HANDLE;

    }  else  {

      l3e = &l3Table[fcmd->comId & PA_COMID_IDX_MASK];
      hdr = &(l3e->hdr);
      retHandle->l2l3Handle = (paHandleL2L3_t)l3e;
      *handleType           =  pa_L3_HANDLE;
    }

    origStatus = hdr->status;

    if (fcmd->commandResult != PAFRM_COMMAND_RESULT_SUCCESS)  {
      hdr->status = PA_TBL_STAT_INACTIVE;
     
      if (origStatus == PA_TBL_STAT_ACTIVE)
      {
        /* Duplicate entry, remove the link */
        if (l3e && l3e->pHandle)
        {
            hdr = (paL2L3Header_t *)l3e->pHandle;
            hdr->lnkCnt--;
        }
      }
      
      ret = pa_LUT_ENTRY_FAILED;

    }  else  {

      origStatus  = hdr->status;
      hdr->status = PA_TBL_STAT_ACTIVE;
      hdr->lutIdx = SWIZ(al1->index);
      
      if (l3e && l3e->pHandle && (origStatus != PA_TBL_STAT_ACTIVE)) {
        hdr = (paL2L3Header_t *)l3e->pHandle;
        hdr->lnkCnt++;
      }

    }

  }  else if (  (fcmd->command == PAFRM_CONFIG_COMMAND_ADDREP_LUT2)  ||
                (fcmd->command == PAFRM_CONFIG_COMMAND_DEL_LUT2)      )   {

    al2 = (pafrmCommandAddLut2_t *)&(fcmd->cmd);

    /* The handle must be recreated */
    pL4Entry->lnkType =  PA_TABLE_ENTRY_TYPE_NONE;
    
    if (SWIZ(al2->ctrlBitMap) & PAFRM_COM_LUT2_CTRL_LINK) {
      pL4Entry->lnkType =  PA_TABLE_ENTRY_TYPE_L3;
      pL4Entry->lnkTableIdx = SWIZ(al2->inkTableIdx);
    }
    
    if (SWIZ(al2->type) == PAFRM_COM_ADD_LUT2_STANDARD) {
      if (SWIZ(al2->ctrlBitMap) & PAFRM_COM_LUT2_CTRL_PORT32) {
        pL4Entry->subType = PA_TABLE_ENTRY_SUBYTPE_PORT32;
        pL4Entry->u.port = (al2->u.port.data[0] << 24) +
                           (al2->u.port.data[1] << 16) +
                           (al2->u.port.data[2] << 8) +
                           (al2->u.port.data[3]); 
      
      }
      else {
        pL4Entry->subType = PA_TABLE_ENTRY_SUBYTPE_PORT16;
        pL4Entry->u.portInfo.destPort = (al2->u.port.data[1] << 8) + al2->u.port.data[2];
        pL4Entry->u.portInfo.lnkP  =  PAFRM_GET_PDSPID_FROM_LINK(al2->u.port.data[3]);
        pL4Entry->u.portInfo.lnkId =  PAFRM_GET_LUTIDX_FROM_LINK(al2->u.port.data[3]);
      }
    
    }
    else {
      /* Custom type */
      pL4Entry->subType = PA_TABLE_ENTRY_SUBYTPE_CUSTOM;
      pL4Entry->customIndex = SWIZ(al2->index);
      memcpy (pL4Entry->u.customInfo, al2->u.custom.match, sizeof (al2->u.custom.match));
    }

    *handleType         = pa_L4_HANDLE;

    /* The LUT2 can be busy with a different add. There is only room
     * to store one add while one is in progress */
    if (fcmd->commandResult == PAFRM_COMMAND_RESULT_LUT2_ADD_BUSY)  {
      *cmdDest = pa_CMD_TX_DEST_3;
      ret = pa_RESUBMIT_COMMAND;
    }

  }  else  {

    /* For other commands the Sub-System return value was not really required */

  }
  
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (ret);  

}  /* Pa_forwardResult */
        
/************************************************************************************
 * FUNCTION PURPOSE: Configure sub-system routing of exception conditions
 ************************************************************************************
 * DESCRIPTION: Packets which satifies the exception conditions can be routed to the 
 *              host 
 ************************************************************************************/
paReturn_t Pa_configExceptionRoute(Pa_Handle       iHandle,
                                   int             nRoute,
                                   int            *routeTypes,
                                   paRouteInfo_t  *eRoutes,
                                   paCmd_t         cmd,
                                   uint16_t       *cmdSize,
                                   paCmdReply_t   *reply,
                                   int            *cmdDest)
{
  paInst_t                  *paInst    = (paInst_t *)iHandle;
  pafrmCommand_t            *fcmd;
  pafrmCommandSysConfigPa_t *cpa;
  int                       i;
  uint16_t                  csize;
  paReturn_t                retCode;

  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + offsetof(pafrmCommandSysConfigPa_t, u) - sizeof(uint32_t) + sizeof(pafrmComEroute_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* The destination for the command must be host or discard */
  if (  (reply->dest != pa_DEST_HOST)     &&
        (reply->dest != pa_DEST_DISCARD)  )
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Make sure there is at least one route */
  if (nRoute <= 0)
    return (pa_ERR_CONFIG);

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_SYS_CONFIG, 0, csize);

  /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  cpa = (pafrmCommandSysConfigPa_t *)&(fcmd->cmd);
  
  cpa->cfgCode = PAFRM_SYSTEM_CONFIG_CODE_EROUTE;

  cpa->u.eroute.routeBitMap = 0;
  
  /* Refresh PA Instance for read only */
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));

  for (i = 0; i < nRoute; i++)  {

    if (  (eRoutes[i].dest != pa_DEST_HOST)    &&
          (eRoutes[i].dest != pa_DEST_EMAC)    &&
          (eRoutes[i].dest != pa_DEST_DISCARD)  )
      return (pa_ERR_CONFIG);
      
    if ( routeTypes[i] >= pa_EROUTE_MAX  )
      return (pa_ERR_CONFIG);  

    cpa->u.eroute.routeBitMap |=  (1 << routeTypes[i]);
    
    retCode =  pa_conv_routing_info (paInst, &cpa->u.eroute.eRoute[routeTypes[i]], &eRoutes[i], pa_CMD_TX_DEST_5, FALSE);
    if(retCode != pa_OK)
        return(retCode);
    
  }
  
  cpa->u.eroute.routeBitMap = SWIZ(cpa->u.eroute.routeBitMap);

  /* Destination can be any PDSP, but 5 is used since it is lightly loaded */
  *cmdDest = pa_CMD_TX_DEST_5;

  return (pa_OK);

} /* Pa_configExceptionRoute */

/************************************************************************************
 * FUNCTION PURPOSE: Configure multi-route groups
 ************************************************************************************
 * DESCRIPTION: Configures the multi-route group which consists of multiple
 *              packet routing entries
 ************************************************************************************/
paReturn_t Pa_configMultiRoute (Pa_Handle               iHandle,
                                paMultiRouteModes_e     mode,
                                uint16_t                index,
                                uint16_t                nRoute,
                                paMultiRouteEntry_t    *routeEntry,
                                paCmd_t                 cmd,
                                uint16_t               *cmdSize,
                                paCmdReply_t           *reply,
                                int                    *cmdDest)
{
  pafrmCommand_t           *fcmd;
  pafrmCommandMultiRoute_t *mr;
  int                       i;
  uint16_t                  csize;

  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandMultiRoute_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* The destination for the command must be host or discard */
  if (  (reply->dest != pa_DEST_HOST)     &&
        (reply->dest != pa_DEST_DISCARD)  )
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Make sure there is at least one entry  */
  if ((nRoute == 0) || (nRoute > pa_MAX_MULTI_ROUTE_ENTRIES))
    return (pa_ERR_CONFIG);
    
  /* Sanity check: multi-route set index range and operation mode */
  if(index >= pa_MAX_MULTI_ROUTE_SETS)
    return (pa_ERR_CONFIG);  
    
  if((mode != pa_MULTI_ROUTE_MODE_CONFIG) && (mode != pa_MULTI_ROUTE_MODE_RESET))
    return (pa_ERR_CONFIG);  

  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_MULTI_ROUTE, 0, csize);

  /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  mr = (pafrmCommandMultiRoute_t *)&(fcmd->cmd);
  
  mr->idx = (uint8_t)index;
  
  if (mode == pa_MULTI_ROUTE_MODE_CONFIG) {
    mr->mode = PAFRM_COMMAND_MULTI_ROUTE_MODE_ADD;
    mr->nRoutes = (uint8_t)nRoute;
  
    for (i = 0; i < nRoute; i++)  {
      mr->quFl[i].ctrlFlags = routeEntry[i].ctrlBitfield | PAFRM_MULTI_RUOTE_CTRL_ACTIVE;
      mr->quFl[i].ctrlFlags = SWIZ(mr->quFl[i].ctrlFlags);
      mr->quFl[i].flowId = SWIZ(routeEntry[i].flowId);
      mr->quFl[i].queue = SWIZ(routeEntry[i].queue);
      mr->quFl[i].swInfo0 = SWIZ(routeEntry[i].swInfo0);
    }
  }
  else {
    mr->mode = PAFRM_COMMAND_MULTI_ROUTE_MODE_DEL;
  }                                                

  /* Destination can be any PDSP, but 5 is used since it is lightly loaded */
  *cmdDest = pa_CMD_TX_DEST_5;

  return (pa_OK);

} /* Pa_configMultiRoute */

/************************************************************************************
 * FUNCTION PURPOSE: Initialize the CRC table
 ************************************************************************************
 * DESCRIPTION: Initialize the CRC table based on the 32-bit polynomial 
 ************************************************************************************/
static void pa_init_crc_table4(uint32_t polynomial, uint32_t *crc_table4)
{
  uint32_t  i,bit;
  
  /* 16 values representing all possible 4-bit values */
  for(i=0; i<=16; i++) {
    crc_table4[i]=i<<28;
    for (bit=0; bit<4; bit++) {
      /* If shifting out a zero, then just shift */
      if( !(crc_table4[i] & 0x80000000) )
        crc_table4[i] = (crc_table4[i] << 1);
      /* Else add in the polynomial as well */
      else
        crc_table4[i] = (crc_table4[i] << 1) ^ polynomial;
    }
    crc_table4[i] = SWIZ(crc_table4[i]);
  }
}

/************************************************************************************
 * FUNCTION PURPOSE: Configure CRC engine
 ************************************************************************************
 * DESCRIPTION: This function is used to configure the specified CRC engine by 
 *   formating the required CRC configuration command packet.
 ************************************************************************************/
paReturn_t Pa_configCrcEngine (Pa_Handle       iHandle,
                               uint16_t        index,
                               paCrcConfig_t  *cfgInfo,
                               paCmd_t         cmd,
                               uint16_t       *cmdSize,
                               paCmdReply_t   *reply,
                               int            *cmdDest)
{
  pafrmCommand_t           *fcmd;
  pafrmCommandConfigCRC_t  *ccrc;
  uint16_t                  csize;

  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandConfigCRC_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* The destination for the command must be host or discard */
  if (  (reply->dest != pa_DEST_HOST)     &&
        (reply->dest != pa_DEST_DISCARD)  )
    return (pa_INVALID_CMD_REPLY_DEST);

  /* Sanity check: there is only 6 CRC engines */
  if(index >= PAFRM_NUM_CRC_ENGINES)
    return (pa_ERR_CONFIG);  
    
  if(cfgInfo->size > pa_CRC_SIZE_32)
    return (pa_ERR_CONFIG);   
    
  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_CRC_ENGINE, 0, csize);

  /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  ccrc = (pafrmCommandConfigCRC_t *)&(fcmd->cmd);
  
  ccrc->ctrlBitMap = (uint8_t) cfgInfo->size;
  ccrc->ctrlBitMap |= (cfgInfo->ctrlBitfield & pa_CRC_CONFIG_RIGHT_SHIFT)?PARAM_CRC_CTRL_RIGHT_SHIFT:
                                                                          PARAM_CRC_CTRL_LEFT_SHIFT;
  ccrc->ctrlBitMap |= (cfgInfo->ctrlBitfield & pa_CRC_CONFIG_INVERSE_RESULT)? PARAM_CRC_CTRL_INV_RESULT:0;
                                                                          
  ccrc->ctrlBitMap = SWIZ(ccrc->ctrlBitMap);
  ccrc->initVal = SWIZ(cfgInfo->initValue);
  
  pa_init_crc_table4(cfgInfo->polynomial, ccrc->crcTbl);

  /* Destination is the PDSP corresping to the CRC engine index */
  *cmdDest = (int)index;

  return (pa_OK);

} /* Pa_configCrcEngine */

/************************************************************************************
 * FUNCTION PURPOSE: Configure Command Set
 ************************************************************************************
 * DESCRIPTION: This function is used to configure the command set which consists 
 *   of a list of commands 
 ************************************************************************************/
paReturn_t Pa_configCmdSet (Pa_Handle       iHandle,
                            uint16_t        index,
                            int             nCmd,
                            paCmdInfo_t    *cmdInfo,
                            paCmd_t         cmd,
                            uint16_t       *cmdSize,
                            paCmdReply_t   *reply,
                            int            *cmdDest)
{
  paInst_t             *paInst =  (paInst_t *)iHandle;
  pafrmCommand_t       *fcmd;
  pafrmCommandCmdSet_t *cmdSet;
  uint16_t              csize;
  uint8_t               buf[PAFRM_MAX_CMD_SET_SIZE + 20];
  int                   i;
  uint8_t               maxCmdSets;
  uint16_t              offset, cmdOffset;
  uint16_t              maxCmdSize, maxUsrCounters; 
  uint32_t              mtCsKey;  
  
  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandCmdSet_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* The destination for the command must be host or discard */
  if (  (reply->dest != pa_DEST_HOST)     &&
        (reply->dest != pa_DEST_DISCARD)  )
    return (pa_INVALID_CMD_REPLY_DEST);
    
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  maxCmdSets     = paInst->cfg.cmdSetConfig.numCmdSets;
  maxUsrCounters = paInst->cfg.usrStatsConfig.numCounters;
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  /* Sanity check: command set index range check */
  if (index >= maxCmdSets)
  {
    return (pa_ERR_CONFIG); 
  }   
    
  maxCmdSize = (maxCmdSets == 32)?128 - offsetof(pafrmCommandCmdSet_t, cmd):
                                  64 - offsetof(pafrmCommandCmdSet_t, cmd);  
    
  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_CMD_SET, 0, csize);

  /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  cmdSet = (pafrmCommandCmdSet_t *)&(fcmd->cmd);
  
  cmdSet->idx = (uint8_t)index;
  cmdSet->numCmd = (uint8_t)nCmd;
  
  offset = 0;
  cmdOffset = 0;
  
  for (i = 0; i < nCmd; i++) {
    pafrmRxCmdHdr_t *pCmdHdr = (pafrmRxCmdHdr_t *)&buf[cmdOffset];
    
    switch (cmdInfo[i].cmd)
    {
      
      case pa_CMD_NEXT_ROUTE:
        {
          paCmdNextRoute_t* route = &cmdInfo[i].params.route;
          pafrmRxCmdNextRoute_t* nr = (pafrmRxCmdNextRoute_t *)&buf[cmdOffset + sizeof(pafrmRxCmdHdr_t)];
          
          /* 
           * All the routing information should be in the packet already 
           */
          if(route->ctrlBitfield & pa_NEXT_ROUTE_PARAM_PRESENT)
            return (pa_ERR_CONFIG);
            
          if (route->ctrlBitfield & pa_NEXT_ROUTE_PROC_MULTI_ROUTE) {
            if(route->multiRouteIndex >= pa_MAX_MULTI_ROUTE_SETS)
              return (pa_ERR_CONFIG);  
            nr->ctrlFlags |= PAFRM_RX_NEXT_ROUTE_CTRL_MULTI_ROUTE;
            nr->multiRouteIndex = (uint8_t)route->multiRouteIndex;
          }
          
          pCmdHdr->cmd = PAFRM_RX_CMD_NEXT_ROUTE;
          pCmdHdr->len = sizeof(pafrmRxCmdNextRoute_t) + sizeof(pafrmRxCmdHdr_t);

        }
        break;
      
      case pa_CMD_CRC_OP:
        {
          paCmdCrcOp_t  *crc = &cmdInfo[i].params.crcOp;
          pafrmRxCmdCrcOp_t *ptx = (pafrmRxCmdCrcOp_t *)&buf[cmdOffset  + sizeof(pafrmRxCmdHdr_t)];
          
          if (!(crc->ctrlBitfield & pa_CRC_OP_CRC_VALIDATE))
          {
            /* 
             * Support CRC verification only in rx direction
             */
            return (pa_ERR_CONFIG); 
          
          } 
          
          if (!(crc->ctrlBitfield & pa_CRC_OP_CRC_FRAME_TYPE))
          {
            /*
             * It is not possible to verify the start offset with variable-length header
             */
            if(crc->startOffset < offset)
                return(pa_ERR_CONFIG);
                
            offset = crc->startOffset;
               
          }
          
          ptx->ctrlFlags = (crc->ctrlBitfield & pa_CRC_OP_CRC_RESULT_FOLLOW_PAYLOAD)?PAFRM_RX_CRC_OP_CTRL_CRC_FOLLOW_PAYLOAD:0;
          ptx->ctrlFlags |= (crc->ctrlBitfield & pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER)?PAFRM_RX_CRC_OP_CTRL_PAYLOAD_LENGTH_IN_HEADER:0;
          ptx->ctrlFlags |= (crc->ctrlBitfield & pa_CRC_OP_PAYLOAD_LENGTH_OFFSET_IS_NEGATIVE)?PAFRM_RX_CRC_OP_CTRL_LEN_OFFSET_NEGATIVE:0;
          if (crc->ctrlBitfield & pa_CRC_OP_CRC_FRAME_TYPE)
          {
            ptx->ctrlFlags |= PAFRM_RX_CRC_OP_CTRL_FRAME_TYPE_INCLUDED;
            
            /* Make sure that the frame type is supported */
            if(crc->frameType > pa_CRC_OP_FRAME_TYPE_MAX)
                return(pa_ERR_CONFIG);
                
            ptx->ctrlFlags |= crc->frameType;    
          }
          ptx->ctrlFlags = SWIZ(ptx->ctrlFlags);
          ptx->lenAdjust = (uint8_t)crc->lenAdjust;
          ptx->startOffset = SWIZ(crc->startOffset);
          ptx->lenOffset = SWIZ(crc->lenOffset);
          ptx->lenMask = SWIZ(crc->lenMask);
          ptx->crcOffset = SWIZ(crc->crcOffset);
          ptx->len = SWIZ(crc->len);
          
          pCmdHdr->cmd = PAFRM_RX_CMD_CRC_OP;
          pCmdHdr->len = sizeof(pafrmRxCmdCrcOp_t) + sizeof(pafrmRxCmdHdr_t);
          
        }
        break;
        
      case pa_CMD_COPY_DATA_TO_PSINFO:
        {
          paCmdCopy_t* pCopy = &cmdInfo[i].params.copy;
          pafrmRxCmdCopy_t *pCmdCopy = (pafrmRxCmdCopy_t *)&buf[cmdOffset  + sizeof(pafrmRxCmdHdr_t)];
          
          if((pCopy->numBytes + pCopy->destOffset) > 32)
            return(pa_ERR_CONFIG);
          
          pCmdCopy->ctrlFlags  = (pCopy->ctrlBitfield & pa_COPY_OP_FROM_END)?PAFRM_RX_COPY_CTRL_FROM_END:0;
          pCmdCopy->ctrlFlags  = SWIZ(pCmdCopy->ctrlFlags);
          pCmdCopy->srcOffset  = (uint8_t)pCopy->srcOffset;
          pCmdCopy->destOffset = (uint8_t)pCopy->destOffset;
          pCmdCopy->numBytes   = (uint8_t)pCopy->numBytes;
          
          pCmdHdr->cmd = PAFRM_RX_CMD_COPY_DATA;
          pCmdHdr->len = sizeof(pafrmRxCmdCopy_t) + sizeof(pafrmRxCmdHdr_t);

        }
        break;
      
      case pa_CMD_PATCH_DATA:
        {
          paPatchInfo_t* patch = &cmdInfo[i].params.patch;
          pafrmRxCmdPatch_t *pCmdPatch = (pafrmRxCmdPatch_t *)&buf[cmdOffset  + sizeof(pafrmRxCmdHdr_t)];
          int fData = !(patch->ctrlBitfield & pa_PATCH_OP_DELETE);
          
          if((patch->offset < offset)  || (patch->nPatchBytes > pa_MAX_RX_PATCH_BYTES) || 
             (fData && (patch->patchData == NULL)))
            return(pa_ERR_CONFIG);
            
          pCmdPatch->ctrlFlags = (uint8_t)patch->ctrlBitfield;
          pCmdPatch->offset    = (uint8_t)patch->offset;
          pCmdPatch->numBypes  = (uint8_t)patch->nPatchBytes; 
          
          if(fData)
            memcpy(&pCmdPatch->data, patch->patchData, patch->nPatchBytes);
          
          pCmdHdr->cmd = PAFRM_RX_CMD_PATCH_DATA;
          pCmdHdr->len = sizeof(pafrmRxCmdPatch_t) + sizeof(pafrmRxCmdHdr_t);
          
          if((patch->nPatchBytes > 4) && fData)
            pCmdHdr->len += (((patch->nPatchBytes - 4) + 3)/4)*4; 
          
          offset = patch->offset;

        }
        break;
        
      case pa_CMD_SPLIT:
        {
          paCmdSplitOp_t      *split = &cmdInfo[i].params.split;
          pafrmRxCmdSplitOp_t *pCmdSplit = (pafrmRxCmdSplitOp_t *)&buf[cmdOffset + sizeof(pafrmRxCmdHdr_t)];
                
          pCmdHdr->cmd = PAFRM_RX_CMD_SPLIT;
          pCmdHdr->len = sizeof(pafrmRxCmdSplitOp_t) + sizeof(pafrmRxCmdHdr_t);
          
          pCmdSplit->ctrlFlags = 0;
          if (split->ctrlBitfield & pa_SPLIT_OP_FRAME_TYPE)
          {
            pCmdSplit->ctrlFlags |= PAFRM_RX_SPLIT_OP_CTRL_FRAME_TYPE_INCLUDED;
            
            /* Make sure that the frame type is supported */
            if(split->frameType > pa_CRC_OP_FRAME_TYPE_MAX)
                return(pa_ERR_CONFIG);
                
            pCmdSplit->frameType = (uint8_t)split->frameType;    
          }
          pCmdSplit->ctrlFlags = (uint8_t)pCmdSplit->ctrlFlags;
          pCmdSplit->flowId    = (uint8_t)split->flowId;
          pCmdSplit->destQueue = SWIZ(split->destQueue);
          pCmdSplit->startOffset = (uint8_t)split->startOffset;
          
        }
        break;
        
      
      case pa_CMD_REMOVE_HEADER:
        {
          /* It must be the first command before packet data has been used */
          if(offset)
            return (pa_ERR_CONFIG);
            
          pCmdHdr->cmd = PAFRM_RX_CMD_REMOVE_HDR;
          pCmdHdr->len = sizeof(pafrmRxCmdHdr_t);
            
        }
        break;
        
      case pa_CMD_REMOVE_TAIL:
        {
          
          pCmdHdr->cmd = PAFRM_RX_CMD_REMOVE_TAIL;
          pCmdHdr->len = sizeof(pafrmRxCmdHdr_t);
            
        }
        break;
        
      
      case pa_CMD_MULTI_ROUTE:
        {
          paCmdMultiRoute_t      *mRoute = &cmdInfo[i].params.mRoute;
          pafrmRxCmdMultiRoute_t *pCmdMroute = (pafrmRxCmdMultiRoute_t *)pCmdHdr;
          
          if(mRoute->index >= pa_MAX_MULTI_ROUTE_SETS)
            return (pa_ERR_CONFIG);  
          
          pCmdMroute->cmd = PAFRM_RX_CMD_MULTI_ROUTE;
          pCmdMroute->len = sizeof(pafrmRxCmdMultiRoute_t);
          pCmdMroute->index = (uint8_t)mRoute->index;
        
        }
        break;
        
      case pa_CMD_USR_STATS:
        {
          paCmdUsrStats_t      *usrStats = &cmdInfo[i].params.usrStats;
          pafrmRxCmdUsrStats_t *pCmdUsrStats = (pafrmRxCmdUsrStats_t *)pCmdHdr;
          
          if(usrStats->index >= maxUsrCounters)
            return (pa_ERR_CONFIG);  
          
          pCmdUsrStats->cmd = PAFRM_RX_CMD_USR_STATS;
          pCmdUsrStats->len = sizeof(pafrmRxCmdUsrStats_t);
          pCmdUsrStats->index = SWIZ(usrStats->index);
        
        }
        break;
        
      case pa_CMD_VERIFY_PKT_ERROR:
        {
          paCmdVerifyPktErr_t      *verifyPktErr = &cmdInfo[i].params.verifyPktErr;
          pafrmRxCmdVerifyPktErr_t *pCmdVerifyPktErr = (pafrmRxCmdVerifyPktErr_t *)&buf[cmdOffset  + sizeof(pafrmRxCmdHdr_t)];
          
          pCmdHdr->cmd = PAFRM_RX_CMD_VERIFY_PKT_ERROR;
          pCmdHdr->len = sizeof(pafrmRxCmdVerifyPktErr_t) + sizeof(pafrmRxCmdHdr_t);
          
          switch (verifyPktErr->dest)
          {
            case pa_DEST_DISCARD:
                pCmdVerifyPktErr->forwardType = PAFRM_FORWARD_TYPE_DISCARD;
                break;
                
            case pa_DEST_HOST:
                pCmdVerifyPktErr->forwardType = PAFRM_FORWARD_TYPE_HOST;   
                pCmdVerifyPktErr->flowId  = SWIZ(verifyPktErr->flowId);
                pCmdVerifyPktErr->queue   = SWIZ(verifyPktErr->queue);
                pCmdVerifyPktErr->swInfo0 = SWIZ(verifyPktErr->swInfo0);
                break;
                    
            default:
                return (pa_ERR_CONFIG);  
          }
          
          pCmdVerifyPktErr->errMask = (verifyPktErr->errorBitfield & pa_PKT_ERR_IP_CHECKSUM)?PAFRM_RX_PKT_ERR_IP_CHECKSUM:0;
          pCmdVerifyPktErr->errMask |= (verifyPktErr->errorBitfield & pa_PKT_ERR_L4_CHECKSUM)?PAFRM_RX_PKT_ERR_L4_CHECKSUM:0;
          pCmdVerifyPktErr->errMask |= (verifyPktErr->errorBitfield & pa_PKT_ERR_CRC)?PAFRM_RX_PKT_ERR_CRC:0;
          pCmdVerifyPktErr->errMask = SWIZ(pCmdVerifyPktErr->errMask);
          pCmdVerifyPktErr->forwardType = SWIZ(pCmdVerifyPktErr->forwardType);
        
        }
        break;
      
      default:
        return (pa_ERR_CONFIG);
    }
  
    cmdOffset += pCmdHdr->len;
    pCmdHdr->len = SWIZ(pCmdHdr->len);
    
    if(cmdOffset > maxCmdSize)
      return (pa_CMDSET_TOO_BIG);
  }
  
  /* copy the constructed commands */
  memcpy((uint8_t *)cmdSet->cmd, buf, maxCmdSize);

  /* Destination can be any PDSP, but 5 is used since it is lightly loaded */
  *cmdDest = pa_CMD_TX_DEST_5;

  return (pa_OK);

} /* Pa_configCmdSet */

/************************************************************************************
 * FUNCTION PURPOSE: Verify link table of the user-defined statistics
 ************************************************************************************
 * DESCRIPTION: The function verify the link table by detecting close loop which means
 *              the same counter appears more than once in its link chain
 * 
 * Return: TRUE:  if no loop detected
 *         FALSW: if loop is detected
 *
 * Note: It is not necessary to perform range check in this function since the link
 *       counter index has been verified by the caller 
 ************************************************************************************/
 static int pa_verify_usr_stats_lnk_tbl(uint16_t numStats, paUsrStatsLnkEntry_t* lnktbl)
 {
    uint32_t bitMask[(pa_USR_STATS_MAX_COUNTERS + 31)>>5];
    int bitIndex, bitOffset;
    int cntIndex, index;
    
    for (index = 0; index < numStats; index++)
    {
        /* clear the  bitMask array */
        memset(bitMask, 0, sizeof(bitMask));
        
        cntIndex = index;
        
        while (lnktbl[cntIndex].lnkIndex != PA_USR_STATS_LNK_END)
        {
            /* Can not link to itself */
            if(lnktbl[cntIndex].lnkIndex == cntIndex)
                return (FALSE);
        
            /* Is the same counter used? */
            bitIndex = cntIndex >> 5; 
            bitOffset = cntIndex & 0x1F;
            
            if (bitMask[bitIndex] & (1 << bitOffset))
            {
                return FALSE;
            }
            
            bitMask[bitIndex] |= (1 << bitOffset);
            
            cntIndex = lnktbl[cntIndex].lnkIndex;
        
        }
    }
    
    return (TRUE);
 
 }

/************************************************************************************
 * FUNCTION PURPOSE: Configure User-defined Statistics
 ************************************************************************************
 * DESCRIPTION: This function is used to perform the counter configuration of 
 *              the multi-level hierarchical user-defined statistics which consists 
 *              of up to 64 64-bit counters and up to 192 32-bit counters.
 ************************************************************************************/
paReturn_t Pa_configUsrStats (Pa_Handle                iHandle,
                              paUsrStatsConfigInfo_t  *cfgInfo,
                              paCmd_t                  cmd,
                              uint16_t                *cmdSize,
                              paCmdReply_t            *reply,
                              int                     *cmdDest)
{
  paInst_t                 *paInst =  (paInst_t *)iHandle;
  paUsrStatsCounterConfig_t* pCntCfg;
  paUsrStatsCounterEntryConfig_t *cntInfo;
  pafrmCommand_t           *fcmd;
  pafrmUsrStatsCntCfg_t    *cntCfg;
  pafrmUsrStatsEntry_t     *pEntry;
  int                       i;
  uint16_t                  csize, maxUsrCounters;
  paReturn_t                errCode = pa_OK;
  paUsrStatsLnkEntry_t      usrStatsLnkTbl[pa_USR_STATS_MAX_COUNTERS];      /* local copy of the user stats link table */
  uint32_t                  mtCsKey;  
  
  /* Refresh PA Instance for read only */
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  maxUsrCounters = paInst->cfg.usrStatsConfig.numCounters;
  
  /* Sanity Check  */
  if ((cfgInfo == NULL) || ((pCntCfg = cfgInfo->pCntCfg) == NULL) || ((pCntCfg->numCnt > 0) && (pCntCfg->cntInfo == NULL)) ||(pCntCfg->numCnt > maxUsrCounters))
  {
    errCode = pa_ERR_CONFIG;
  }
  else
  {
    /* Verify that there is enough room to create the command */
    csize = sizeof(pafrmCommand_t) + sizeof(pafrmUsrStatsCntCfg_t) - sizeof(uint32_t) + sizeof(pafrmUsrStatsEntry_t) * pCntCfg->numCnt;
    if (*cmdSize < csize)
    {
        errCode = pa_INSUFFICIENT_CMD_BUFFER_SIZE;
    }
    else
    {
        *cmdSize = csize;
        /* The destination for the command must be host or discard */
        if (  (reply->dest != pa_DEST_HOST)     &&
            (reply->dest != pa_DEST_DISCARD)  )
            errCode =  pa_INVALID_CMD_REPLY_DEST;
    }

  }  
  
  if (errCode != pa_OK)
  {
    Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
    return (errCode);
  }  
  
  Pa_osalMtCsEnter(&mtCsKey);
    
  Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].size);
                         
  memcpy(usrStatsLnkTbl, paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base, paInst->nUsrStats * sizeof(paUsrStatsLnkEntry_t));                       
    
  /* Create the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_USR_STATS, 0, csize);

  /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
   * must be considered */
  if (reply->dest == pa_DEST_HOST)
    fcmd->replyDest = PAFRM_DEST_PKTDMA;
  else
    fcmd->replyDest = PAFRM_DEST_DISCARD;

  cntCfg = (pafrmUsrStatsCntCfg_t *)&(fcmd->cmd);
  
  if(pCntCfg->ctrlBitfield & pa_USR_STATS_CONFIG_RESET)
    cntCfg->ctrlFlags |= PAFRM_USR_STATS_CFG_CLR_ALL;
    
  cntCfg->ctrlFlags = SWIZ(cntCfg->ctrlFlags);
  cntCfg->nEntries  = SWIZ(pCntCfg->numCnt);
  
  if (pCntCfg->numCnt)
  {
    pEntry = (pafrmUsrStatsEntry_t *)((uint8_t *)&(fcmd->cmd) + sizeof(pafrmUsrStatsCntCfg_t));
    cntInfo = pCntCfg->cntInfo;
  
    for (i = 0; i < pCntCfg->numCnt; i++)  {
        if ((cntInfo[i].cntIndex >= maxUsrCounters)  ||
            ((cntInfo[i].cntLnk != pa_USR_STATS_LNK_END) && (cntInfo[i].cntLnk >= maxUsrCounters)))
        {
            errCode = pa_ERR_CONFIG;
            break;
        } 
        else
        {
            pEntry[i].index = SWIZ(cntInfo[i].cntIndex);
            usrStatsLnkTbl[cntInfo[i].cntIndex].lnkIndex =
            pEntry[i].lnkIndex = (cntInfo[i].cntLnk == pa_USR_STATS_LNK_END)?PAFRM_USR_STATS_LNK_END:cntInfo[i].cntLnk;
            if(cntInfo[i].cntType == pa_USR_STATS_TYPE_BYTE)
                pEntry[i].lnkIndex |= PAFRM_USR_STATS_LNK_BYTE_CNT;
            pEntry[i].lnkIndex = SWIZ(pEntry[i].lnkIndex);    
        }
    }                                      
  
    /*  Verify the updated link table  */
    if(!pa_verify_usr_stats_lnk_tbl(paInst->nUsrStats, usrStatsLnkTbl))
        errCode = pa_ERR_CONFIG;  
  }  
            
  if (errCode == pa_OK) 
  {
    memcpy(paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base, usrStatsLnkTbl, paInst->nUsrStats * sizeof(paUsrStatsLnkEntry_t)); 
    
    /* All user-defined statistics related configuration should occur at PDSP4 */
    *cmdDest = pa_CMD_TX_DEST_4;

  } 
  else
  {
    /* No need to send the command */
    *cmdSize = 0;
  }
          
  Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base,
                       paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].size);
                       
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);
  
  return (errCode);

} /* Pa_configUsrStats */                             

/************************************************************************************
 * FUNCTION PURPOSE: System-level control and configuration
 ************************************************************************************
 * DESCRIPTION: Perform system-level control configuration by updating the local
 *              instance and/or format the global configuration command packet
 *              host 
 ************************************************************************************/
paReturn_t Pa_control (Pa_Handle      iHandle, 
                       paCtrlInfo_t  *ctrl, 
                       paCmd_t        cmd,
                       uint16_t       *cmdSize,
                       paCmdReply_t   *reply,
                       int            *cmdDest)
{
  paInst_t               *paInst =  (paInst_t *)iHandle;
  paSysConfig_t          *cfg = &ctrl->params.sysCfg;
  pafrmCommand_t         *fcmd;
  uint16_t               csize;
  paReturn_t             ret = pa_OK;
  uint32_t               mtCsKey;  
  
  if((ctrl == NULL) || (ctrl->code > pa_CONTROL_IPSEC_NAT_T_CONFIG))
    return (pa_ERR_CONFIG);
    
  /* The destination for the command must be host or discard */
  if (  (reply->dest != pa_DEST_HOST)     &&
        (reply->dest != pa_DEST_DISCARD)  )
    return (pa_INVALID_CMD_REPLY_DEST);

  if (ctrl->code == pa_CONTROL_SYS_CONFIG)
  {
    pafrmCommandConfigPa_t *cpa;
  
    /* Verify that there is enough room to create the command */
    csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandConfigPa_t) - sizeof(uint32_t);
    
    if (*cmdSize < csize)
        return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

    *cmdSize = csize;

    /* Create the command */
    fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_CONFIG_PA, 0, csize);

    /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
    * must be considered */
    if (reply->dest == pa_DEST_HOST)
        fcmd->replyDest = PAFRM_DEST_PKTDMA;
    else
        fcmd->replyDest = PAFRM_DEST_DISCARD;
    
    cpa = (pafrmCommandConfigPa_t *)&(fcmd->cmd);
  
    Pa_osalMtCsEnter(&mtCsKey);
    Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
    /* Protocol Limitations */
    if (cfg->pProtoLimit)
    {
        paProtocolLimit_t* pPLcfg = cfg->pProtoLimit;
    
        /* range check */
        if ((pPLcfg->vlanMax <= pa_PROTOCOL_LIMIT_NUM_VLANS_MAX)  &&
            (pPLcfg->ipMax <=  pa_PROTOCOL_LIMIT_NUM_IP_MAX)      &&
            (pPLcfg->greMax <= pa_PROTOCOL_LIMIT_NUM_GRE_MAX))
        { 
            paInst->cfg.protoLimit = *pPLcfg;
            cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_MAX_COUNTS;
            cpa->maxCounts.vlanMaxCount = SWIZ(pPLcfg->vlanMax);
            cpa->maxCounts.ipMaxCount   = SWIZ(pPLcfg->ipMax);
            cpa->maxCounts.greMaxCount  = SWIZ(pPLcfg->greMax);
        }
        else
        {
            ret = pa_ERR_CONFIG;    
        }
  
    }
  
    /* Outer IP configuration */
    if (cfg->pOutIpReassmConfig)
    {
        paIpReassmConfig_t* pIpReassmCfg = cfg->pOutIpReassmConfig;
    
        /* range check */
        if (pIpReassmCfg->numTrafficFlow > pa_MAX_IP_REASM_TRAFFIC_FLOWS) 
        {  
            ret = pa_ERR_CONFIG;
        }
        else
        {
            paInst->cfg.outIpReassmConfig = *pIpReassmCfg;
            cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_OUT_IP_REASSEM;
            cpa->outIpReasm.numTrafficFlow = SWIZ(pIpReassmCfg->numTrafficFlow);
            cpa->outIpReasm.destFlowId     = SWIZ(pIpReassmCfg->destFlowId);
            cpa->outIpReasm.destQueue      = SWIZ(pIpReassmCfg->destQueue);
        }
    }
  
    /* Inner IP configuration */
    if (cfg->pInIpReassmConfig)
    {
        paIpReassmConfig_t* pIpReassmCfg = cfg->pInIpReassmConfig;
    
        /* range check */
        if (pIpReassmCfg->numTrafficFlow > pa_MAX_IP_REASM_TRAFFIC_FLOWS) 
        {  
            ret = pa_ERR_CONFIG;
        }
        else
        {
            paInst->cfg.inIpReassmConfig = *pIpReassmCfg;
            cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_IN_IP_REASSEM;
            cpa->inIpReasm.numTrafficFlow = SWIZ(pIpReassmCfg->numTrafficFlow);
            cpa->inIpReasm.destFlowId     = SWIZ(pIpReassmCfg->destFlowId);
            cpa->inIpReasm.destQueue      = SWIZ(pIpReassmCfg->destQueue);
        }
    }
  
    /* Command Set Configurations */
    if (cfg->pCmdSetConfig)
    {
        paCmdSetConfig_t* pCmdSetCfg = cfg->pCmdSetConfig;
    
        /* range check */
        if ((pCmdSetCfg->numCmdSets != 32) &&
            (pCmdSetCfg->numCmdSets != 64))
        {  
            ret = pa_ERR_CONFIG;
        }
        else
        {
            paInst->cfg.cmdSetConfig = *pCmdSetCfg;
            cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_CMDSET;
            if (pCmdSetCfg->numCmdSets == 32)
            {
                cpa->cmdSet.numCmdSets = 32;
                cpa->cmdSet.size = 128;
            }
            else
            {
                cpa->cmdSet.numCmdSets = 64;
                cpa->cmdSet.size = 64;
            }
            cpa->cmdSet.numCmdSets = SWIZ(cpa->cmdSet.numCmdSets);
            cpa->cmdSet.size       = SWIZ(cpa->cmdSet.size);
        }
    }
  
    /* User-defined statistics configuration*/
    if (cfg->pUsrStatsConfig)
    {
        paUsrStatsConfig_t* pUsrStatsCfg = cfg->pUsrStatsConfig;
    
        /* range check */
        if ((pUsrStatsCfg->numCounters > paInst->nUsrStats)            || 
            (pUsrStatsCfg->num64bCounters > pUsrStatsCfg->numCounters) ||
            ((pUsrStatsCfg->num64bCounters*8 + (pUsrStatsCfg->numCounters - pUsrStatsCfg->num64bCounters)*4) > pa_USR_STATS_MAX_64B_COUNTERS*8))
        {  
            ret = pa_ERR_CONFIG;
        }
        else
        {
            paInst->cfg.usrStatsConfig = *pUsrStatsCfg;
            cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_USR_STATS;
            cpa->usrStats.numCounters = SWIZ(pUsrStatsCfg->numCounters);
            cpa->usrStats.num64bCounters = SWIZ(pUsrStatsCfg->num64bCounters);
        }
    }
  
    /* Queue Divert configuration */
    if (cfg->pQueueDivertConfig)
    {
        paQueueDivertConfig_t* pQueueDevertCfg = cfg->pQueueDivertConfig;
    
        paInst->cfg.queueDivertConfig = *pQueueDevertCfg;
        cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_QUEUE_DIVERT;
        cpa->queueDivert.destFlowId     = SWIZ(pQueueDevertCfg->destFlowId);
        cpa->queueDivert.destQueue      = SWIZ(pQueueDevertCfg->destQueue);
    }
    
    /* Packet Verification configuration */
    if (cfg->pPktControl)
    {
        paPacketControlConfig_t* pPktControlCfg = cfg->pPktControl;
        
        cpa->validFlag |= PAFRM_COMMAND_CONFIG_VALID_PKT_CTRL;
        cpa->pktCtrl.ctrlBitMap = ((pPktControlCfg->ctrlBitMap & pa_PKT_CTRL_HDR_VERIFY_PPPoE)? PAFRM_PACKET_VERIFY_PPPoE:0);   
        cpa->pktCtrl.ctrlBitMap |= ((pPktControlCfg->ctrlBitMap & pa_PKT_CTRL_HDR_VERIFY_IP)? PAFRM_PACKET_VERIFY_IP:0);  
        cpa->pktCtrl.ctrlBitMap |= ((pPktControlCfg->ctrlBitMap & pa_PKT_CTRL_MAC_PADDING_CHK)? PAFRM_PACKET_CTRL_MAC_PADDING_CHK:0);  
        cpa->pktCtrl.ctrlBitMap |= ((pPktControlCfg->ctrlBitMap & pa_PKT_CTRL_IP_FRAGS_TO_EROUTE)? PAFRM_PACKET_CTRL_IP_FRAGS_TO_EROUTE:0);  
        cpa->pktCtrl.ctrlBitMap = SWIZ(cpa->pktCtrl.ctrlBitMap);
        cpa->pktCtrl.rxPaddingErrCntIndex = SWIZ(pPktControlCfg->rxPaddingErrStatsIndex);
        cpa->pktCtrl.txPaddingCntIndex = SWIZ(pPktControlCfg->txPaddingStatsIndex);
    }
  
    /* Destination can be any PDSP, but 5 is used since it is lightly loaded */
    *cmdDest = pa_CMD_TX_DEST_5;

    if (!cpa->validFlag)
    {
        /* There is no command to be sent */
        *cmdSize = 0;
        ret = pa_ERR_CONFIG;
    }
  
    cpa->validFlag = SWIZ(cpa->validFlag);
  
    Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
    Pa_osalMtCsExit(mtCsKey);
  }
  else
  {
    pafrmCommandSysConfigPa_t *ccfg;
  
    /* Verify that there is enough room to create the command */
    csize = sizeof(pafrmCommand_t) + offsetof(pafrmCommandSysConfigPa_t, u) - sizeof(uint32_t);
    csize += (ctrl->code == pa_CONTROL_802_1ag_CONFIG)?sizeof(pafrm802p1agDet_t):sizeof(pafrmIpsecNatTDet_t);   
    if (*cmdSize < csize)
        return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

    *cmdSize = csize;

    /* Create the command */
    fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_SYS_CONFIG, 0, csize);

    /* Validity of the destination was already checked (HOST, DISCARD), so no other cases 
     * must be considered */
    if (reply->dest == pa_DEST_HOST)
        fcmd->replyDest = PAFRM_DEST_PKTDMA;
    else
        fcmd->replyDest = PAFRM_DEST_DISCARD;
    
    ccfg = (pafrmCommandSysConfigPa_t *) &(fcmd->cmd);
    
    if (ctrl->code == pa_CONTROL_802_1ag_CONFIG)
    {
        pa802p1agDetConfig_t  *pa802p1agDet = &ctrl->params.pa802p1agDetCfg;
        
        ccfg->cfgCode = PAFRM_SYSTEM_CONFIG_CODE_802_1AG;
        ccfg->u.pa802p1agDet.ctrlBitMap   = (pa802p1agDet->ctrlBitMap & pa_802_1ag_DETECT_ENABLE)?PAFRM_802_1ag_DET_ENABLE:0;
        ccfg->u.pa802p1agDet.ctrlBitMap  |= (pa802p1agDet->ctrlBitMap & pa_802_1ag_DETECT_STANDARD)?PAFRM_802_1ag_DET_STANDARD:0;
        ccfg->u.pa802p1agDet.ctrlBitMap  =  SWIZ(ccfg->u.pa802p1agDet.ctrlBitMap);
        
        /* 802.1ag detector is at PDSP0 only */
        *cmdDest = pa_CMD_TX_DEST_0;
    } 
    else
    {
        paIpsecNatTConfig_t *ipsecNatTDet = &ctrl->params.ipsecNatTDetCfg;
        
        ccfg->cfgCode = PAFRM_SYSTEM_CONFIG_CODE_IPSEC_NAT_T;
        ccfg->u.ipsecNatTDet.ctrlBitMap = (ipsecNatTDet->ctrlBitMap & pa_IPSEC_NAT_T_CTRL_ENABLE)?PAFRM_IPSEC_NAT_T_DET_ENABLE:0;
        ccfg->u.ipsecNatTDet.ctrlBitMap = SWIZ(ccfg->u.ipsecNatTDet.ctrlBitMap);
        ccfg->u.ipsecNatTDet.udpPort    = SWIZ(ipsecNatTDet->udpPort);
    
        /* IPSEC NAT-T detector is at PDSP3 only */
        *cmdDest = pa_CMD_TX_DEST_3;
    }
  
  }
  
  return (ret);

} /* Pa_control */

/********************************************************************************
 * FUNCTION PURPOSE: Request Sub-System statistics
 ********************************************************************************
 * DESCRIPTION: The command to request statistics is created
 ********************************************************************************/
static paReturn_t pa_format_stats_request  (Pa_Handle      handle,
                                            uint16_t       doClear, 
                                            paCmd_t        cmd, 
                                            uint16_t      *cmdSize, 
                                            paCmdReply_t  *reply, 
                                            int           *cmdDest,
                                            uint8_t       statsType)
{

  pafrmCommand_t         *fcmd;
  pafrmCommandReqStats_t *rstat;
  uint16_t                csize;

  /* Verify that there is enough room to create the command */
  csize = sizeof(pafrmCommand_t) + sizeof(pafrmCommandReqStats_t) - sizeof(uint32_t);
  if (*cmdSize < csize)
    return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);

  *cmdSize = csize;

  /* Verify the reply destination */
  if (reply->dest != pa_DEST_HOST)
    return (pa_ERR_CONFIG);

  /* Form the command */
  fcmd = pa_format_fcmd_header (cmd, reply, PAFRM_CONFIG_COMMAND_REQ_STATS, 0, csize);

  /* Validity of the destination was already checked (HOST), so no other cases 
   * must be considered */
  fcmd->replyDest = PAFRM_DEST_PKTDMA;

  rstat = (pafrmCommandReqStats_t *)&(fcmd->cmd);

  rstat->clear = (uint8_t)doClear;
  rstat->type  = statsType;

  /* The command can go to any PDSP, but 5 is used do to light loading */
  *cmdDest = pa_CMD_TX_DEST_5;

  return (pa_OK);

}  

/********************************************************************************
 * FUNCTION PURPOSE: Request Sub-System statistics
 ********************************************************************************
 * DESCRIPTION: The command to request statistics is created
 ********************************************************************************/
paReturn_t Pa_requestStats (Pa_Handle      handle,
                            uint16_t       doClear, 
                            paCmd_t        cmd, 
                            uint16_t      *cmdSize, 
                            paCmdReply_t  *reply, 
                            int           *cmdDest)
{

  return(pa_format_stats_request(handle, doClear, cmd, cmdSize, reply, cmdDest, PAFRM_STATS_TYPE_SYS));

}  /* Pa_requestStats */

/********************************************************************************
 * FUNCTION PURPOSE: Request User-defined statistics
 ********************************************************************************
 * DESCRIPTION: The command to request User-defined statistics is created
 ********************************************************************************/
paReturn_t Pa_requestUsrStats (Pa_Handle      handle,
                               uint16_t       doClear, 
                               paCmd_t        cmd, 
                               uint16_t      *cmdSize, 
                               paCmdReply_t  *reply, 
                               int           *cmdDest,
                               paUsrStats_t  *pUsrStats)
{

  paInst_t      *paInst =  (paInst_t *)handle;
  CSL_Pa_ssRegs *passRegs;
  uint16_t       num64bCounters, num32bCounters;
  uint32_t      *pCounter;
  int            i;
  
  /* Refresh PA Instance for read only */
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  num64bCounters = paInst->cfg.usrStatsConfig.num64bCounters;
  num32bCounters = paInst->cfg.usrStatsConfig.numCounters - num64bCounters;
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
  pCounter = (uint32_t *)&passRegs->PDSP_SRAM[PAFRM_USR_STATS_SRAM_INDEX].PDSP_RAM[PAFRM_USR_STATS_OFFSET];
  
  /* Format and copy 64-bit counters */
  for (i = 0; i < num64bCounters; i++, pCounter += 2)
  {
      pUsrStats->count64[i] = (((uint64_t)pCounter[0]) << 32) + (uint64_t)pCounter[1]; 
  }
  
  /* Format and copy 32-bit counters */
  for (i = 0; i < num32bCounters; i++)
  {
      pUsrStats->count32[i] = pCounter[i]; 
  }
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  
  if (doClear)
  {
    return(pa_format_stats_request(handle, doClear, cmd, cmdSize, reply, cmdDest, PAFRM_STATS_TYPE_USR));
  }
  else
  {
    *cmdSize = 0;
    *cmdDest = pa_CMD_TX_DEST_5;

    return (pa_OK);
  }

}  /* Pa_requestUsrStats */


/****************************************************************************
 * FUNCTION PURPOSE: Format the repy generated to a stats request
 ****************************************************************************
 * DESCRIPTION: The stats request reply is swizzled and a pointer to the
 *              stats is returned
 ****************************************************************************/
paSysStats_t* Pa_formatStatsReply (Pa_Handle  handle,
                                   paCmd_t    cmd)
{
  pafrmCommand_t *fcmd;
  paSysStats_t   *statsRep;

  fcmd     = (pafrmCommand_t *)cmd;
  swizFcmd(fcmd);

  statsRep = (paSysStats_t *)((uint8_t*)&(fcmd->cmd) + sizeof(pafrmCommandReqStats_t));

  /* Verify the command was valid */
  if (fcmd->command != PAFRM_CONFIG_COMMAND_REQ_STATS)
    return (NULL);

  swizStatsRep (statsRep);

  return (statsRep);

} /* Pa_formatStatsReply */

/************************************************************************************
 * FUNCTION PURPOSE: Create a Tx checksum command
 ************************************************************************************
 * DESCRIPTION: The command is formatted
 ************************************************************************************/
static void pa_tx_chk (paTxChksum_t *chk, pasahoComChkCrc_t *ptx)
{
  PASAHO_SET_CMDID             (ptx, PASAHO_PAMOD_CMPT_CHKSUM);
  PASAHO_CHKCRC_SET_START      (ptx, chk->startOffset);
  PASAHO_CHKCRC_SET_LEN        (ptx, chk->lengthBytes);
  PASAHO_CHKCRC_SET_RESULT_OFF (ptx, chk->resultOffset);
  PASAHO_CHKCRC_SET_INITVAL    (ptx, chk->initialSum);

  if (chk->negative0)
     PASAHO_CHKCRC_SET_NEG0 (ptx, 1);
  else
     PASAHO_CHKCRC_SET_NEG0 (ptx, 0);

} 

/************************************************************************************
 * FUNCTION PURPOSE: Convert routing info to next Route command
 ************************************************************************************
 * DESCRIPTION: Convert routing info to next Route command
 ************************************************************************************/
static void pa_conv_routeInfo_nRoute (paRouteInfo_t *route, paCmdNextRoute_t *nrCmd, uint16_t nextCmd)
{
    nrCmd->ctrlBitfield = (nextCmd)?pa_NEXT_ROUTE_PROC_NEXT_CMD:0;
    nrCmd->dest = route->dest;
    nrCmd->pktType_emacCtrl = route->pktType_emacCtrl;
    nrCmd->flowId = route->flowId;
    nrCmd->queue = route->queue;
    nrCmd->swInfo0 = route->swInfo0;
    nrCmd->swInfo1 = route->swInfo1;
    
    if (route->mRouteIndex != pa_NO_MULTI_ROUTE) {
      nrCmd->ctrlBitfield |= pa_NEXT_ROUTE_PROC_MULTI_ROUTE;
      nrCmd->multiRouteIndex = (uint16_t)route->mRouteIndex;    
    }

} 

/************************************************************************************
 * FUNCTION PURPOSE: Format Tx routing / checksum information
 ************************************************************************************
 * DESCRIPTION: The command header is created for computing checksums and 
 *              routing packets through the transmit sub-system
 ************************************************************************************/
paReturn_t Pa_formatTxRoute  (paTxChksum_t  *chk0,
                              paTxChksum_t  *chk1,
                              paRouteInfo_t *route,
                              void*          cmdBuffer,
                              uint16_t      *cmdSize )
{

  paCmdInfo_t  cmdInfo[3];
  int          nCmd = 1;
  paCmdInfo_t* pCmdInfo = &cmdInfo[0];
  
  memset(cmdInfo, 0, sizeof(cmdInfo));
  
  if (chk0) {
    pCmdInfo->cmd = pa_CMD_TX_CHECKSUM;
    pCmdInfo->params.chksum = *chk0;  
    nCmd++;
    pCmdInfo++;
  }
  
  if (chk1) {
    pCmdInfo->cmd = pa_CMD_TX_CHECKSUM;
    pCmdInfo->params.chksum = *chk1;  
    nCmd++;
    pCmdInfo++;
  }
  
  pCmdInfo->cmd = pa_CMD_NEXT_ROUTE;
  pa_conv_routeInfo_nRoute(route, &pCmdInfo->params.route, FALSE);
  
  return(Pa_formatTxCmd(nCmd, &cmdInfo[0], 0, cmdBuffer, cmdSize));
 
} /* Pa_formatTxRoute */

/************************************************************************************
 * FUNCTION PURPOSE: Format blind patch / Tx routing command
 ************************************************************************************
 * DESCRIPTION: The command header is created for routing (delayed) followed
 *              by a blind patch.
 ************************************************************************************/
paReturn_t Pa_formatRoutePatch (paRouteInfo_t *route,
                                paPatchInfo_t *patch,
                                void*          cmdBuffer,
                                uint16_t      *cmdSize)
{

  paCmdInfo_t  cmdInfo[2];
  
  memset(cmdInfo, 0, sizeof(cmdInfo));
  cmdInfo[1].cmd = pa_CMD_PATCH_DATA;
  cmdInfo[1].params.patch = *patch;
  
  cmdInfo[0].cmd = pa_CMD_NEXT_ROUTE;
  pa_conv_routeInfo_nRoute(route, &cmdInfo[0].params.route, TRUE);
  
  return(Pa_formatTxCmd(2, &cmdInfo[0], 0, cmdBuffer, cmdSize));

} /* Pa_formatRoutePatch */

/************************************************************************************
 * FUNCTION PURPOSE: Format Tx command set 
 ************************************************************************************
 * DESCRIPTION: This function formats a list of commands to be executed on the packets 
 *          to be transmitted over the network
 ************************************************************************************/
paReturn_t Pa_formatTxCmd (int             nCmd,
                           paCmdInfo_t    *cmdInfo,
                           uint16_t        offset,
                           void           *cmdBuffer,
                           uint16_t       *cmdSize)
{
  pasahoComChkCrc_t *ptx;
  pasahoNextRoute_t *nr;
  pasahoComBlindPatch_t *bpCmd;
  pasahoShortInfo_t *sInfo;
  pasahoCmdInfo_t   *paCmdInfo;
  uint16_t           pdest;
  uint16_t           cmdOffset = offset;
  uint8_t            buf[128], nrCmdSize; 
  uint8_t           *pCmd = (uint8_t *)cmdBuffer;
  int                index;  
  int                fAlignDetect = FALSE;      /* Indicate whetehr we need to perform  8-byte alignmnet check for command blocl forwarding to SASS */
  int                fNRPlusPatch = FALSE;      /* Indicate that the patch Command following the next commmand, therefore, its szie should be included */
  int                cmdBlockSize;         
  
  if (((uint32_t)cmdBuffer & 0x3) || (offset & 0x3) || (offset > PAFRM_MAX_CMD_SET_SIZE)) 
    return (pa_ERR_CONFIG);
    
  if ((cmdInfo == NULL) || (cmdSize == NULL))
    return (pa_ERR_CONFIG);
    
  
  if(nCmd <= 0)return (pa_ERR_CONFIG);  
  
  memset(buf, 0, 128);
  
  for (index = 0; index < nCmd; index++)  {
    switch (cmdInfo[index].cmd)
    {
    
      case pa_CMD_NONE:
        {
          /* Insert a dummy command */
          paCmdInfo = (pasahoCmdInfo_t *)&buf[cmdOffset];
          
          PASAHO_SET_CMDID (paCmdInfo, PASAHO_PAMOD_DUMMY);
          cmdOffset += 4;
          
          if(fAlignDetect)
            cmdBlockSize+=4; 
        }
        break;         
      
      case pa_CMD_NEXT_ROUTE:
        {
          paCmdNextRoute_t* route = &cmdInfo[index].params.route;
          nr = (pasahoNextRoute_t *)&buf[cmdOffset];
          nrCmdSize = sizeof(pasahoNextRoute_t) - sizeof(nr->word1);  
          
          if ((route->ctrlBitfield & pa_NEXT_ROUTE_PROC_NEXT_CMD) && (cmdInfo[index+1].cmd != pa_CMD_PATCH_DATA)) {
            /* Only support NextRoute plus Patch command */
            return(pa_ERR_CONFIG);
          }
          
          /*
           * Detect whether we need to insert a dummy command in front of the next route command 
           * to maintain 8-byte alignment requirement for SASS.
           */
          if (fAlignDetect)
          {
            cmdBlockSize += nrCmdSize;
            
            if ((route->dest == pa_DEST_SRIO) ||
                ((route->dest != pa_DEST_SASS) && 
                 ((route->pktType_emacCtrl) || (route->ctrlBitfield & pa_NEXT_ROUTE_TX_L2_PADDING))))
            {
                cmdBlockSize += sizeof(nr->word1);
            }
          
            if (route->ctrlBitfield & pa_NEXT_ROUTE_PROC_NEXT_CMD)
            {
                paPatchInfo_t* patch = &cmdInfo[index+1].params.patch;
                
                cmdBlockSize += (patch->totalPatchSize + 4);
                fNRPlusPatch = TRUE;
            }
            
            if (cmdBlockSize % 8)
            {
                /* Insert dummy command */
                paCmdInfo = (pasahoCmdInfo_t *)&buf[cmdOffset];
                PASAHO_SET_CMDID (paCmdInfo, PASAHO_PAMOD_DUMMY);
                cmdOffset += 4;
                nr = (pasahoNextRoute_t *)&buf[cmdOffset];
            }
          
            cmdBlockSize = 0;
          } 
          
          /* Make sure the destination is valid */
          switch (route->dest)  {

            case pa_DEST_SASS:   
              pdest = PAFRM_DEST_PKTDMA;
              break;

            case pa_DEST_HOST:   
            case pa_DEST_EMAC:   
              pdest = (route->dest == pa_DEST_EMAC)?PAFRM_DEST_ETH:PAFRM_DEST_PKTDMA;
              if ((route->pktType_emacCtrl) || (route->ctrlBitfield & pa_NEXT_ROUTE_TX_L2_PADDING))
              {
                uint8_t psFlags;
                PASAHO_SET_E (nr, 1);
                psFlags = (route->pktType_emacCtrl & pa_EMAC_CTRL_CRC_DISABLE)?
                          PAFRM_ETH_PS_FLAGS_DISABLE_CRC:0;
                psFlags |= ((route->pktType_emacCtrl & pa_EMAC_CTRL_PORT_MASK) << PAFRM_ETH_PS_FLAGS_PORT_SHIFT);  
                PASAHO_SET_PKTTYPE  (nr, psFlags); 
                if (route->ctrlBitfield & pa_NEXT_ROUTE_TX_L2_PADDING)
                {
                    PASAHO_SET_TX_PADDING (nr, 1);
                }    
                
                nrCmdSize += sizeof(nr->word1);
              } 
              break;
    
            case pa_DEST_SRIO:   
              pdest = PAFRM_DEST_SRIO;
              PASAHO_SET_E (nr, 1);
              PASAHO_SET_PKTTYPE  (nr, route->pktType_emacCtrl); 
              nrCmdSize += sizeof(nr->word1);
              break;

            default:  return (pa_ERR_CONFIG);
          }

          PASAHO_SET_CMDID (nr, PASAHO_PAMOD_NROUTE);
          PASAHO_SET_DEST  (nr, pdest);
          PASAHO_SET_FLOW  (nr, route->flowId);
          PASAHO_SET_QUEUE (nr, route->queue);
          if (route->ctrlBitfield & pa_NEXT_ROUTE_PROC_NEXT_CMD) {
            PASAHO_SET_N  (nr, 1);
          } 
           
          nr->swInfo0 = route->swInfo0;
          nr->swInfo1 = route->swInfo1;
          
          cmdOffset += nrCmdSize;
        
        }
        break;
      
      case pa_CMD_CRC_OP:
        {
          paCmdCrcOp_t  *crc = &cmdInfo[index].params.crcOp;
          uint8_t ctrl;
          
          ptx    = (pasahoComChkCrc_t *)&buf[cmdOffset];
          
          if (crc->ctrlBitfield & ( pa_CRC_OP_CRC_VALIDATE | pa_CRC_OP_PAYLOAD_LENGTH_IN_HEADER ))
          {
            /* 
             * Support CRC calculation only in tx direction
             * length information should be available in the tx direction
             */
            return (pa_ERR_CONFIG); 
          
          }  
          
          ctrl = (crc->ctrlBitfield & pa_CRC_OP_CRC_RESULT_FOLLOW_PAYLOAD)?0:PAFRM_CRC_FLAG_CRC_OFFSET_VALID;
          
          PASAHO_SET_CMDID             (ptx, PASAHO_PAMOD_CMPT_CRC);
          PASAHO_CHKCRC_SET_START      (ptx, crc->startOffset);
          PASAHO_CHKCRC_SET_LEN        (ptx, crc->len);
          PASAHO_CHKCRC_SET_CTRL       (ptx, ctrl);
          PASAHO_CHKCRC_SET_RESULT_OFF (ptx, crc->crcOffset);
          
          cmdOffset += sizeof (pasahoComChkCrc_t);
          
          if(fAlignDetect)
            cmdBlockSize+=sizeof(pasahoComChkCrc_t); 
          
        }
        break;
      
      
      case pa_CMD_PATCH_DATA:
        {
          paPatchInfo_t* patch = &cmdInfo[index].params.patch;
          
          bpCmd = (pasahoComBlindPatch_t *) &buf[cmdOffset]; 
          
          /* Verify that the patch information is consisent */
          if ( (patch->nPatchBytes > pa_MAX_PATCH_BYTES)    ||
               (patch->totalPatchSize & 0x03))  
              return (pa_ERR_CONFIG);

          PASAHO_SET_CMDID                (bpCmd, PASAHO_PAMOD_PATCH);
          PASAHO_BPATCH_SET_PATCH_NBYTES  (bpCmd, patch->nPatchBytes);
          PASAHO_BPATCH_SET_PATCH_CMDSIZE (bpCmd, (patch->totalPatchSize + 4) >> 2);  /* Add the 4 byte command header */
          
          if(patch->ctrlBitfield & pa_PATCH_OP_INSERT)
            PASAHO_BPATCH_SET_OVERWRITE     (bpCmd,0);
          else
            PASAHO_BPATCH_SET_OVERWRITE     (bpCmd,1);
            
          if (patch->ctrlBitfield & pa_PATCH_OP_DELETE)
          {
            PASAHO_BPATCH_SET_OVERWRITE     (bpCmd,1);
            PASAHO_BPATCH_SET_DELETE        (bpCmd,1);
          }
            
            
          PASAHO_BPATCH_SET_OFFSET        (bpCmd, patch->offset);
          
          cmdOffset += 4;

          if (patch->patchData && patch->totalPatchSize)
          {
            int i, n;
            n = patch->nPatchBytes;
            for (i = 0; i < n; i++)
                PASAHO_BPATCH_SET_PATCH_BYTE(bpCmd, i, patch->patchData[i]);
                
            cmdOffset += patch->totalPatchSize;    
          }
          
          /* 
           * The patch command may be appended to the next route command and it is already included in the cmdBlockSize calcualtion
           */
          
          if(fAlignDetect && !fNRPlusPatch)
            cmdBlockSize+=(patch->totalPatchSize + 4); 
            
          fNRPlusPatch = FALSE;  
          
        }
        break;
      
      
      case pa_CMD_TX_CHECKSUM:
        {
          paTxChksum_t  *chk = &cmdInfo[index].params.chksum;
          
          ptx    = (pasahoComChkCrc_t *)&buf[cmdOffset];
          pa_tx_chk (chk, ptx);
          cmdOffset += sizeof (pasahoComChkCrc_t);
          if(fAlignDetect)
            cmdBlockSize+=sizeof(pasahoComChkCrc_t); 
          
        }
        break;
      
      
      case pa_CMD_REPORT_TX_TIMESTAMP:
        {
          paCmdTxTimestamp_t       *txTs = &cmdInfo[index].params.txTs;
          pasahoReportTimestamp_t  *rtInfo = (pasahoReportTimestamp_t *)&buf[cmdOffset];
          
          PASAHO_SET_CMDID (rtInfo, PASAHO_PAMOD_REPORT_TIMESTAMP);
          PASAHO_SET_REPORT_FLOW(rtInfo, (uint8_t)txTs->flowId);
          PASAHO_SET_REPORT_QUEUE(rtInfo, txTs->destQueue);
          rtInfo->swInfo0 = txTs->swInfo0;
          cmdOffset += sizeof(pasahoReportTimestamp_t);
          if(fAlignDetect)
            cmdBlockSize+=sizeof(pasahoReportTimestamp_t); 
            
        }
        break;
      
      case pa_CMD_IP_FRAGMENT:
        {
          paCmdIpFrag_t   *ipFrag = &cmdInfo[index].params.ipFrag;
          pasahoIpFrag_t  *ipFragInfo = (pasahoIpFrag_t *)&buf[cmdOffset];
          
          PASAHO_SET_CMDID (ipFragInfo, PASAHO_PAMOD_IP_FRAGMENT);
          PASAHO_SET_SUB_CODE_IP_FRAG(ipFragInfo);
          PASAHO_SET_IP_OFFSET(ipFragInfo, (uint8_t)ipFrag->ipOffset);
          PASAHO_SET_MTU_SIZE(ipFragInfo, ipFrag->mtuSize);
          cmdOffset += sizeof(pasahoIpFrag_t);
          if(fAlignDetect)
            cmdBlockSize+=sizeof(pasahoIpFrag_t); 
        }
        break;
        
      case pa_CMD_PATCH_MSG_LEN:
        {
          paPatchMsgLenInfo_t  *patchMsgLen = &cmdInfo[index].params.patchMsgLen;
          pasahoPatchMsgLen_t  *fwPatchMsgLen = (pasahoPatchMsgLen_t *)&buf[cmdOffset];
          
          PASAHO_SET_CMDID (fwPatchMsgLen, PASAHO_PAMOD_PATCH_MSG_LEN);
          PASAHO_SET_SUB_CODE_PATCH_MSG_LEN(fwPatchMsgLen);
          PASAHO_SET_MSGLEN_SIZE(fwPatchMsgLen, (patchMsgLen->msgLenSize != 2));
          PASAHO_SET_MSGLEN_OFFSET(fwPatchMsgLen, patchMsgLen->offset);
          PASAHO_SET_MSGLEN(fwPatchMsgLen, patchMsgLen->msgLen);
          cmdOffset += sizeof(pasahoPatchMsgLen_t);
          if(fAlignDetect)
            cmdBlockSize+=sizeof(pasahoPatchMsgLen_t); 
        }
        break;
        
      
      case pa_CMD_SA_PAYLOAD:
        {
          paPayloadInfo_t* pInfo = &cmdInfo[index].params.payload;
        
          sInfo = (pasahoShortInfo_t *)&buf[cmdOffset];
          PASAHO_SET_CMDID (sInfo, PASAHO_SA_SHORT_INFO);
          PASAHO_SINFO_SET_PAYLOAD_OFFSET(sInfo, pInfo->offset);
          PASAHO_SINFO_SET_PAYLOAD_LENGTH(sInfo, pInfo->len);
          sInfo->word1 = pInfo->supData;
          
          cmdOffset += sizeof(pasahoShortInfo_t);
          
          /* This is the first and only command processed by SASS, turn on alignment detector */
          fAlignDetect = TRUE;
          cmdBlockSize = 0;
           
        }
        break;
      
      default:
        return (pa_ERR_CONFIG);
    }
    
    if((cmdOffset > 128) || (cmdOffset > *cmdSize))
      return (pa_INSUFFICIENT_CMD_BUFFER_SIZE);
  }
  
  memcpy(pCmd + offset, &buf[offset], cmdOffset - offset);
  *cmdSize = cmdOffset;
  
  return (pa_OK);
  
} /* Pa_formatTxCmd */ 

/****************************************************************************************
 * FUNCTION PURPOSE: Enable a PDSP
 ****************************************************************************************
 * DESCRIPTION:  The PDSP is put into the run state if it is currently in the reset state.
 *               If taken out of reset the function waits for the PDSP to indicate 
 *               that it is ready. 
 *
 *               Return value:
 *                  PA_PDSP_ALREADY_ACTIVE - the PDSP was already out of reset
 *                  PA_PDSP_RESET_RELEASED - the PDSP was in reset and successfully taken out of reset
 *                  PA_PDSP_NO_RESTART - the PDSP was taken out of reset but timed out on the handshake
 ****************************************************************************************/

#define PA_PDSP_ALREADY_ACTIVE      0
#define PA_PDSP_RESET_RELEASED      1
#define PA_PDSP_NO_RESTART          2

static int pa_pdsp_run (volatile CSL_Pa_ssPdsp_ctlstatRegs *pdsp, volatile CSL_Pa_ssMailboxRegs *mbox)
{
  int i;

  /* Check for enabled PDSP */
  if ( (pdsp->PDSP_CONTROL & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)
    return (PA_PDSP_ALREADY_ACTIVE);

  /* Clear the mailbox */
  mbox->MBOX_SLOT[0] = 0;

  /* Enable the PDSP */
  pdsp->PDSP_CONTROL |= (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK |
                         CSL_PA_SS_PDSP_CONTROL_SOFT_RST_N_MASK);

  /* Wait for the mailbox to become non-zero */
  for (i = 0; i < PA_MAX_PDSP_ENABLE_LOOP_COUNT; i++)  {
    if (mbox->MBOX_SLOT[0] != 0)  {
      return (PA_PDSP_RESET_RELEASED);
    }
  }

  return (PA_PDSP_NO_RESTART);

} /* pa_pdsp_run */

/*****************************************************************************************
 * FUNCTION PURPOSE: Set the state of the Sub-System
 *****************************************************************************************
 * DESCRIPTION: The Sub-System state can be set or queried.
 *****************************************************************************************/
paSSstate_t Pa_resetControl (Pa_Handle iHandle, paSSstate_t newState)
{

  int    i;
  paInst_t *paInst  = (paInst_t *)iHandle;
  CSL_Pa_ssRegs *passRegs;
  paSSstate_t  ret = pa_STATE_INVALID_REQUEST;
  uint32_t     mtCsKey;  
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
  
  if (newState == pa_STATE_RESET)  {

    /* Put each of the PDSPs into reset (PC = 0)*/
    for (i = 0; i < 6; i++)  {
      passRegs->PDSP_CTLSTAT[i].PDSP_CONTROL = 0;
    }

    /* Reset packet ID */
    passRegs->PKT_ID.PKT_ID_SOFT_RESET = 1;

    /* Reset LUT2 */
    passRegs->LUT2.LUT2_SOFT_RESET = 1;

    /* Reset statistics */
    passRegs->STATS.STATS_SOFT_RESET = 1;

    /* Reset timers */
    for (i = 0; i < 6; i++)
      passRegs->PDSP_TIMER[i].TIMER_CNTRL_REG = 0;

    ret = pa_STATE_RESET;

  }

  else if (newState == pa_STATE_ENABLE)  {
    uint16_t         doGlobalReset = TRUE;
    int          res;

    ret = pa_STATE_ENABLE;
     
    /* Do nothing if a pdsp is already out of reset. If any PDSPs are out of reset
     * a global init is not performed */
    for (i = 0; i < 6; i++)  {

      res = pa_pdsp_run (&(passRegs->PDSP_CTLSTAT[i]), &(passRegs->MAILBOX[i]));
      if (res == PA_PDSP_ALREADY_ACTIVE)
        doGlobalReset = FALSE;
    
      if (res == PA_PDSP_NO_RESTART)  {
        ret = pa_STATE_ENABLE_FAILED;
        doGlobalReset = FALSE;
      }

    }

    /* If global reset is required any PDSP can do it */
    if (doGlobalReset)  {

      passRegs->MAILBOX[0].MBOX_SLOT[1] = 1;   /* Tell PDSP0 to do global init */
      passRegs->MAILBOX[0].MBOX_SLOT[0] = 0;   /* Let PDSP0 go */

      while (passRegs->MAILBOX[0].MBOX_SLOT[1] != 0);

      for (i = 1; i < 6; i++)
        passRegs->MAILBOX[i].MBOX_SLOT[0] = 0;   /* Let PDSP go */

    }  else  {

      for (i = 0; i < 6; i++)
        passRegs->MAILBOX[i].MBOX_SLOT[0] = 0;   /* Let PDSP go */

    }

  }

  else if (newState == pa_STATE_QUERY)  {

    uint16_t enable  = FALSE;
    uint16_t reset   = FALSE;

    for (i = 0; i < 6; i++)  {
      if ( (passRegs->PDSP_CTLSTAT[i].PDSP_CONTROL & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK) == 
                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK))
        enable = TRUE;
      else
        reset = TRUE;
    }

    if ( (reset == TRUE) && (enable == TRUE) )
      ret = pa_STATE_INCONSISTENT;
    else if (reset == TRUE)
      ret = pa_STATE_RESET;
    else
      ret = pa_STATE_ENABLE;

  }
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);

  return (ret);

} /* Pa_resetControl */

/*****************************************************************************************
 * FUNCTION PURPOSE: Configures the PA timer which is used to generate 32-bit timestamp
 *****************************************************************************************
 * DESCRIPTION: This function is used to configure the 16-bit timer reserved for 32-bit 
 * system timestamp.
 *****************************************************************************************/
paReturn_t Pa_configTimestamp (Pa_Handle             iHandle,
                               paTimestampConfig_t  *cfgInfo)
{
   
  paInst_t *paInst  = (paInst_t *)iHandle;
  CSL_Pa_ssRegs *passRegs;
  uint32_t      mtCsKey;  
  
  if (cfgInfo == NULL)
      return (pa_ERR_CONFIG); 
      
  if (cfgInfo->enable) {
    if ( (cfgInfo->factor < pa_TIMESTAMP_SCALER_FACTOR_2) || 
        (cfgInfo->factor > pa_TIMESTAMP_SCALER_FACTOR_8192)) {
        return (pa_ERR_CONFIG);
    }
  }      
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
      
  if (cfgInfo->enable) {
    
    passRegs->PDSP_TIMER[0].TIMER_LOAD_REG  = 0xFFFF;
    passRegs->PDSP_TIMER[0].TIMER_CNTRL_REG = CSL_PA_SS_TIMER_CNTRL_REG_GO_MASK       |  
                                              CSL_PA_SS_TIMER_CNTRL_REG_MODE_MASK     |
                                              (cfgInfo->factor < CSL_PA_SS_TIMER_CNTRL_REG_PRESCALE_SHIFT)    |
                                              CSL_PA_SS_TIMER_CNTRL_REG_PSE_MASK;
  } 
  else  {
    passRegs->PDSP_TIMER[0].TIMER_LOAD_REG = 0;
    passRegs->PDSP_TIMER[0].TIMER_CNTRL_REG = 0;
  }
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);
    
  return (pa_OK);

} /* Pa_configTimestamp */

/*****************************************************************************************
 * FUNCTION PURPOSE: Return the 48-bit system timestamp
 *****************************************************************************************
 * DESCRIPTION: This function is used to inquery the 48-bit system timestamp.
 *****************************************************************************************/
paReturn_t Pa_getTimestamp  (Pa_Handle            iHandle, 
                             paTimestamp_t        *pTimestamp)
{
   
  paInst_t *paInst  = (paInst_t *)iHandle;
  CSL_Pa_ssRegs *passRegs;
  uint32_t mtCsKey;
  
  if (pTimestamp == NULL)
      return (pa_ERR_CONFIG); 
      
  /* Refresh PA Instance */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
  
  pTimestamp->lo = 0x0000FFFF - passRegs->PDSP_TIMER[0].TIMER_VALUE_REG;
  pTimestamp->hi = passRegs->PDSP_SRAM[PAFRM_SYS_TIMESTAMP_SRAM_INDEX].PDSP_RAM[PAFRM_SYS_TIMESTAMP_OFFSET];
      
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);
  
  return (pa_OK);

} /* Pa_getTimestamp */

/****************************************************************************
 * DATA PURPOSE: PDSP Constant Registers
 ****************************************************************************
 * DESCRIPTION: Specify the user-defined PDSP constant registers (c24-c31)
 ****************************************************************************/
 const uint32_t pap_pdsp_const_reg_map[6][4] = 
 {
    /* PDSP0: C24-C31 */
    {
        0x0000007F,         /* C25-C24 */    
        0x0000006E,         /* C27-C26 */
        0x00000000,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    },
    
    /* PDSP1: C24-C31 */
    {
        0x0001007F,         /* C25-C24 */    
        0x00480040,         /* C27-C26 */
        0x00000000,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    },
    
    /* PDSP2: C24-C31 */
    {
        0x0002007F,         /* C25-C24 */    
        0x00490044,         /* C27-C26 */
        0x00000000,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    },
    
    /* PDSP3: C24-C31 */
    {
        0x0003007F,         /* C25-C24 */    
        0x0000006E,         /* C27-C26 */
        0x00000000,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    },
 
    /* PDSP4: C24-C31 */
    {
        0x0070007F,         /* C25-C24 */    
        0x00000000,         /* C27-C26 */
        0x04080404,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    },
 
    /* PDSP5: C24-C31 */
    {
        0x0071007F,         /* C25-C24 */    
        0x00000000,         /* C27-C26 */
        0x04080404,         /* C29-C28 */
        0x00000000          /* C31-C30 */
    }
};

#define PA_PDSP_CONST_REG_INDEX_C25_C24     0
#define PA_PDSP_CONST_REG_INDEX_C27_C26     1
#define PA_PDSP_CONST_REG_INDEX_C29_C28     2
#define PA_PDSP_CONST_REG_INDEX_C31_C30     3

/***********************************************************************************************
 * FUNCTION PURPOSE: Download a PDSP image
 ***********************************************************************************************
 * DESCRIPTION: A PDSP image is downloaded. The PDSP remains in reset.
 ***********************************************************************************************/
paReturn_t Pa_downloadImage (Pa_Handle iHandle, int modId, void* image, int sizeBytes)
{

  paInst_t *paInst  = (paInst_t *)iHandle;
  CSL_Pa_ssRegs *passRegs;
  paReturn_t ret = pa_OK;
  uint32_t mtCsKey;

  Rm_LldPermCallouts *rmPermCallTbl = (Rm_LldPermCallouts *) paLObj.cfg.rmHandle;
  Rm_ResourceInfo resourceData;      
  
  resourceData.resourceType = Rm_resource_PA_FIRMWARE;

  /* Verify the specified PDSP is valid */
  if ((modId < 0)  || (modId >= 6))
    return (pa_ERR_CONFIG);

  /* Verify the image size is in range */
  if ((sizeBytes < 0)  || (sizeBytes >= 8192))
    return (pa_ERR_CONFIG);
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));

  if (rmPermCallTbl)
  {
      resourceData.resourceType = Rm_resource_PA_FIRMWARE;
      
      if (rmPermCallTbl->rmUsePermissionsCheck(&resourceData) == RM_USE_PERMISSION_DENIED)
      {
          ret = pa_RESOURCE_INIT_DENIED;
          goto Pa_downloadImage_end;
      }
      else if (rmPermCallTbl->rmInitPermissionsCheck(&resourceData) == RM_INIT_PERMISSION_DENIED)
      {
          /* Use but don't init - Check firmware revision */
  
          /* if check reveals different revision - return QMSS_FIRMWARE_REVISION_DIFFERENCE; */
      }
  }
 
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;

  /* Make sure the PDSP is disabled */
  if ( (passRegs->PDSP_CTLSTAT[modId].PDSP_CONTROL & CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)  ==
                                     (CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK)   )
  {
    ret = pa_SYSTEM_STATE_INVALID;
    goto Pa_downloadImage_end;
  }  

  /* Copy the image */
  memcpy ((void *)(passRegs->PDSP_IRAM[modId].PDSP_RAM), image, sizeBytes);
  
  /* Initialize the programmable constant registers C24-31 */
  passRegs->PDSP_CTLSTAT[modId].PDSP_CONSTANT_TABLE_BLOCK_INDEX_0 = pap_pdsp_const_reg_map[modId][PA_PDSP_CONST_REG_INDEX_C25_C24];
  passRegs->PDSP_CTLSTAT[modId].PDSP_CONSTANT_TABLE_BLOCK_INDEX_1 = pap_pdsp_const_reg_map[modId][PA_PDSP_CONST_REG_INDEX_C27_C26];
  passRegs->PDSP_CTLSTAT[modId].PDSP_CONSTANT_TABLE_PROG_PTR_0    = pap_pdsp_const_reg_map[modId][PA_PDSP_CONST_REG_INDEX_C29_C28];
  passRegs->PDSP_CTLSTAT[modId].PDSP_CONSTANT_TABLE_PROG_PTR_1    = pap_pdsp_const_reg_map[modId][PA_PDSP_CONST_REG_INDEX_C31_C30];


Pa_downloadImage_end:
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  
  
  return (ret);

} /* Pa_downloadImage */

/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire PDSP Version Number
 ***********************************************************************************************
 * DESCRIPTION: Extract the PA PDSP  version number.
 ***********************************************************************************************/
paReturn_t Pa_getPDSPVersion (Pa_Handle iHandle, int modId, uint32_t *pVersion)
{
  paInst_t      *paInst =  (paInst_t *)iHandle;
  CSL_Pa_ssRegs *passRegs;
  
  /* Verify the specified PDSP is valid */
  if ((modId < 0)  || (modId >= 6))
    return (pa_ERR_CONFIG);
  
  /* Refresh PA Instance for read only */
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
  *pVersion = passRegs->PDSP_SRAM[PAFRM_PDSP_VERSION_SRAM_INDEX].PDSP_RAM[PAFRM_PDSP_VERSION_OFFSET(modId)];
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  
  return (pa_OK);

} /* Pa_getPDSPVersion */


/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire Version Number
 ***********************************************************************************************
 * DESCRIPTION: Return the PA LLD version number.
 ***********************************************************************************************/
uint32_t Pa_getVersion (void)
{
    return PA_LLD_VERSION_ID;
} /* Pa_getVersion */

/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire Version String
 ***********************************************************************************************
 * DESCRIPTION: Return the PA LLD version string.
 ***********************************************************************************************/
const char* Pa_getVersionStr (void)
{
    return paLldVersionStr;
} /* Pa_getVersionStr */
