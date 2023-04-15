/******************************************************************************
 * FILE PURPOSE:  Initialization Routines for PA LLD
 ******************************************************************************
 * FILE NAME:   pa.c  
 *
 * DESCRIPTION: PA initialization routes which are executed only during driver 
 *              initialization.  
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
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

#include "pa.h"
#include "paloc.h"
#include "pafrm.h"
#include "pa_osal.h"
#include <string.h>
#include <stddef.h>
#include <ti/csl/cslr_pa_ss.h>
#include <ti/drv/rm/rm_public_lld.h>

/** 
 * @brief Assert-type macro that checks the size of structures.
 *
 * This assertion controls the compilation process. If the postulate about
 * the structure size is not TRUE the compilation would result in error.
 * Otherwise, it would proceed.
 *
 * @param[in]  postulate A boolean expression that postulates size of a structure
 *
 * @retval `<compilation error>` - if postulate is FALSE 
 * @retval `<compilation proceeds>` - if postulate is TRUE
 *
 * <b>Typical Usage:</b><BR>
 * <code>
 * ... <arbitrary C code>....<BR>
 *
 * utlCheckSize(sizeof(xxxInst_t) <= MAX_BUF_SIZE); <BR>
 *
 * ... <code that may crash when sizeof(xxxInst_t) > MAX_BUF_SIZE> <BR>
 * </code>
 */
#define utlCheckSize(postulate)                                         \
   do {                                                                 \
       typedef struct {                                                 \
         uint16_t NegativeSizeIfPostulateFalse[((int)(postulate))*2 - 1];	\
       } PostulateCheck_t;                                              \
   }                                                                    \
   while (0)

/* Allay of PA memory buffer alignment requirements 
 *                PA_BUFFNUM_INSTANCE - the instance buffer
 *                PA_BUFFNUM_L2_TABLE - the L2 handle table
 *                PA_BUFFNUM_L3_TABLE - the L3 handle table
 *                PA_BUFFNUM_USR_STATS_LNK_TABLE - the User-defined statistics link table
 */
const int paMemAligns[pa_N_BUFS] = { 8, 8, 8, 8 };


/* Check that MAX_CMD_BUF_SIZE is correct */
#if    (pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_ADD_MAC_MIN_CMD_BUF_SIZE_BYTES                > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_ADD_MAC_MIN_CMD_BUF_SIZE_BYTES                > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_DEL_HANDLE_MIN_CMD_BUF_SIZE_BYTES             > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_DEL_HANDLE_MIN_CMD_BUF_SIZE_BYTES             > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_DEL_L4_HANDLE_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_DEL_L4_HANDLE_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_ADD_IP_MIN_CMD_BUF_SIZE_BYTES                 > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_ADD_IP_MIN_CMD_BUF_SIZE_BYTES                 > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_ADD_PORT_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_ADD_PORT_MIN_CMD_BUF_SIZE_BYTES               > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_CONFIG_EXCEPTION_ROUTE_MIN_CMD_BUF_SIZE_BYTES > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_CONFIG_EXCEPTION_ROUTE_MIN_CMD_BUF_SIZE_BYTES > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_CONFIG_CRC_ENGINE_MIN_CMD_BUF_SIZE_BYTES      > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_CONFIG_CRC_ENGINE_MIN_CMD_BUF_SIZE_BYTES      > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_CONFIG_MULTI_ROUTE_MIN_CMD_BUF_SIZE_BYTES     > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_CONFIG_MULTI_ROUTE_MIN_CMD_BUF_SIZE_BYTES     > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_SET_CUSTOM_LUT1_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_SET_CUSTOM_LUT1_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_SET_CUSTOM_LUT2_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_SET_CUSTOM_LUT2_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_802_1ag_DET_MIN_CMD_BUF_SIZE_BYTES            > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_802_1ag_DET_MIN_CMD_BUF_SIZE_BYTES            > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_IPSEC_NAT_T_DET_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_IPSEC_NAT_T_DET_MIN_CMD_BUF_SIZE_BYTES        > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_CONFIG_CMD_SET_MIN_CMD_BUF_SIZE_BYTES         > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_CONFIG_CMD_SET_MIN_CMD_BUF_SIZE_BYTES         > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_REQUEST_STATS_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_REQUEST_STATS_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_CONFIG_USR_STATS_MIN_CMD_BUF_SIZE_BYTES       > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_CONFIG_USR_STATS_MIN_CMD_BUF_SIZE_BYTES       > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

#if    (pa_GLOBAL_CONFIG_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#error (pa_GLOBAL_CONFIG_MIN_CMD_BUF_SIZE_BYTES          > pa_MAX_CMD_BUF_SIZE_BYTES)
#endif

/*
 * If the size of pa_MAX_CMD_BUF_SIZE_BYTES changes, it is the time to check
 * whether there are new definitions.
 */
#if   (pa_MAX_CMD_BUF_SIZE_BYTES != 2068)
#error The size of pa_MAX_CMD_BUF_SIZE_BYTES changes, check whether there are other related changes.
#endif  

/* PA Local object */
Pa_LocalObj    paLObj = {0};

/**********************************************************************************
 * FUNCTION PURPOSE: Provide memory buffer allocation requirements
 **********************************************************************************
 * DESCRIPTION: Memory usage by the PA module is returned
 **********************************************************************************/
paReturn_t Pa_getBufferReq (paSizeInfo_t *sizeCfg, int sizes[], int aligns[])
{
  int i;  

  if((sizeCfg == NULL) || (sizeCfg->nUsrStats > pa_USR_STATS_MAX_COUNTERS))
      return (pa_ERR_CONFIG);
  
  sizes[PA_BUFFNUM_INSTANCE] = sizeof(paInst_t);
  
  /* L2 Table size requirements */
  sizes[PA_BUFFNUM_L2_TABLE] = sizeCfg->nMaxL2 * sizeof(paL2Entry_t);

  /* L3 Table size requirements */
  sizes[PA_BUFFNUM_L3_TABLE] = sizeCfg->nMaxL3 * sizeof(paL3Entry_t);
  
  /* User-defined statistics Link Table size requirements */
  sizes[PA_BUFFNUM_USR_STATS_LNK_TABLE] = sizeCfg->nUsrStats * sizeof(paUsrStatsLnkEntry_t);
  
  for ( i = 0; i < pa_N_BUFS; i++)
  {
      aligns[i] = paMemAligns[i];
  }

  return (pa_OK);

} /* Pa_getBufferReq */

/********************************************************************
 * FUNCTION PURPOSE: Initialize global configurations
 ********************************************************************
 * DESCRIPTION: Initialize the global configuations to default
 ********************************************************************/
static void pa_init_global_cfg (paInst_t* paInst) 
{

    paSysInfo_t* pCfg = &paInst->cfg;
    
    memset(pCfg, 0, sizeof(paSysInfo_t));
    
    pCfg->protoLimit.vlanMax         = pa_PROTOCOL_LIMIT_NUM_VLANS_DEF;
    pCfg->protoLimit.ipMax           = pa_PROTOCOL_LIMIT_NUM_IP_DEF;
    pCfg->protoLimit.greMax          = pa_PROTOCOL_LIMIT_NUM_GRE_DEF;
    
    pCfg->cmdSetConfig.numCmdSets    = pa_MAX_CMD_SETS;
    
    pCfg->usrStatsConfig.numCounters = paInst->nUsrStats;
}

/***********************************************************************************
 * FUNCTION PURPOSE: Create a PA instance
 ***********************************************************************************
 * DESCRIPTION: An instance of the PA is created and initialized
 ***********************************************************************************/
paReturn_t Pa_create (paConfig_t *config, void* bases[], Pa_Handle *pHandle)
{
  paInst_t     *paInst = (paInst_t *)bases[0];
  paSizeInfo_t *sizeCfg;
  paL2Entry_t  *l2Table;
  paL3Entry_t  *l3Table;
  paUsrStatsLnkEntry_t *lnkTable;
  int          i;
  uint32_t     mtCsKey;
  
  if ((paInst == NULL) || (config == NULL) || (pHandle == NULL) || (config->sizeCfg == NULL) || (config->sizeCfg->nUsrStats > pa_USR_STATS_MAX_COUNTERS))
    return (pa_ERR_CONFIG);
    
  *pHandle = paInst;  
  sizeCfg = config->sizeCfg; 
  
  /* Refresh PA Instance */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));

  /* Verify buffer base addresses */
  for (i = 0; i < pa_N_BUFS; i++)  {
    if (bases[i] == NULL)
    {
        if ((i != PA_BUFFNUM_USR_STATS_LNK_TABLE) || (config->sizeCfg->nUsrStats))
        {
            Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
            return (pa_ERR_CONFIG);
        }    
    }  
    paInst->paBufs[i].base = bases[i];
  }

  /* Determine the number of table entries. The result will always be
   * >= 0, so no check is required (0 is a valid number of entries). */
  paInst->l2n = sizeCfg->nMaxL2;
  paInst->l3n = sizeCfg->nMaxL3;
  paInst->nUsrStats = sizeCfg->nUsrStats;
  paInst->paBufs[PA_BUFFNUM_L2_TABLE].size = paInst->l2n * sizeof(paL2Entry_t);
  paInst->paBufs[PA_BUFFNUM_L3_TABLE].size = paInst->l3n * sizeof(paL3Entry_t);
  paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].size = paInst->nUsrStats * sizeof(paUsrStatsLnkEntry_t);
  
  paInst->baseAddr = config->baseAddr; 

  if (config->initTable)  {

    l2Table = (paL2Entry_t *)paInst->paBufs[PA_BUFFNUM_L2_TABLE].base;

    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base, 
                           paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);

    memset (l2Table, 0, paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);

    /* The status value PA_TBL_STAT_INACTIVE should have a value of 0, but take the safe approach */
    for (i = 0; i < paInst->l2n; i++) {
      l2Table[i].hdr.status    = PA_TBL_STAT_INACTIVE;
      l2Table[i].hdr.tableIdx  = i;
      l2Table[i].hdr.type      = PA_TABLE_ENTRY_TYPE_L2;
    }

    Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L2_TABLE].base, 
                         paInst->paBufs[PA_BUFFNUM_L2_TABLE].size);



    l3Table = (paL3Entry_t *)paInst->paBufs[PA_BUFFNUM_L3_TABLE].base;

    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

    memset (l3Table, 0, paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

    for (i = 0; i < paInst->l3n; i++)  {
      l3Table[i].hdr.status   = PA_TBL_STAT_INACTIVE;
      l3Table[i].hdr.tableIdx = i;
      l3Table[i].hdr.type     = PA_TABLE_ENTRY_TYPE_L3;
    }

    Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_L3_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_L3_TABLE].size);

  }
  
  /* Initialize the user-defined statistics link table */
  if (paInst->nUsrStats)
  {
  
    lnkTable = (paUsrStatsLnkEntry_t *)paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base;

    Pa_osalBeginMemAccess (paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base,
                           paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].size);

    for (i = 0; i < paInst->nUsrStats; i++)  {
      lnkTable[i].lnkIndex = PA_USR_STATS_LNK_END;
    }

    Pa_osalEndMemAccess (paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].base,
                         paInst->paBufs[PA_BUFFNUM_USR_STATS_LNK_TABLE].size);
  }
  
  pa_init_global_cfg(paInst);
  
  if (config->initDefaultRoute)
  {
    CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)paInst->baseAddr;
    passRegs->STREAMSWITCH.STREAMSWITCH_SOFT_RESET = (PAFRM_DEST_PDSP0 << CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTA_SHIFT) |
                                                     (PAFRM_DEST_PDSP0 << CSL_PA_SS_STREAMSWITCH_SOFT_RESET_TXSTB_SHIFT);
  }
  
  paInst->ready = TRUE;
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  /* Sanity check the defined sizes. This is effectively a compile time check */
  utlCheckSize(pa_ADD_MAC_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandAddLut1_t))); 
  utlCheckSize(pa_ADD_IP_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandAddLut1_t)));
  utlCheckSize(pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandAddLut1_t))); 
  utlCheckSize(pa_DEL_HANDLE_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandDelLut1_t)));
  utlCheckSize(pa_DEL_L4_HANDLE_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof (pafrmCommandDelLut2_t)));
  utlCheckSize(pa_ADD_PORT_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandAddLut2_t)));
  utlCheckSize(pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandAddLut2_t)));
  utlCheckSize(pa_CONFIG_CMD_SET_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandCmdSet_t)));
  utlCheckSize(pa_CONFIG_CRC_ENGINE_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandConfigCRC_t)));
  utlCheckSize(pa_CONFIG_MULTI_ROUTE_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandMultiRoute_t)));
  utlCheckSize(pa_CONFIG_EXCEPTION_ROUTE_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandSysConfigPa_t)));
  utlCheckSize(pa_SET_CUSTOM_LUT1_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandSysConfigPa_t)  - sizeof(pafrmComEroute_t) + sizeof(pafrmC1Custom_t)));
  utlCheckSize(pa_SET_CUSTOM_LUT2_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandSysConfigPa_t)  - sizeof(pafrmComEroute_t) + sizeof(pafrmC2Custom_t)));
  utlCheckSize(pa_802_1ag_DET_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandSysConfigPa_t)  - sizeof(pafrmComEroute_t) + sizeof(pafrm802p1agDet_t)));
  utlCheckSize(pa_IPSEC_NAT_T_DET_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandSysConfigPa_t)  - sizeof(pafrmComEroute_t) + sizeof(pafrmIpsecNatTDet_t)));
  utlCheckSize(pa_REQUEST_STATS_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandReqStats_t)));
  utlCheckSize(pa_GLOBAL_CONFIG_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmCommandConfigPa_t)));
  utlCheckSize(pa_CONFIG_USR_STATS_MIN_CMD_BUF_SIZE_BYTES >= (sizeof(pafrmCommand_t) - sizeof(uint32_t) + sizeof(pafrmUsrStatsCntCfg_t) + sizeof(pafrmUsrStatsEntry_t)*pa_USR_STATS_MAX_COUNTERS));
                 
  return (pa_OK);

} /* Pa_create */

/***********************************************************************************
 * FUNCTION PURPOSE: Adds PA configuration
 ***********************************************************************************
 * DESCRIPTION: This function needs to be called from all cores to initialize PA with 
 *           per core configurations
 ***********************************************************************************/
void Pa_startCfg (Pa_Handle pHandle, paStartCfg_t *startCfg)
{
    paLObj.cfg = *startCfg;
}

/**********************************************************************************************
 * FUNCTION PURPOSE: Return memory buffer usage for application to free
 **********************************************************************************************
 * DESCRIPTION: Returns the base address, size, alignment, type and memory space
 *              of all buffers previously allocated for the PA drivers instance.
 **********************************************************************************************/
paReturn_t Pa_close (Pa_Handle handle, void* bases[])
{
  paInst_t *paInst = (paInst_t *)handle;
  int i;
  uint32_t mtCsKey;
  
  /* Refresh PA Instance for read only */
  Pa_osalMtCsEnter(&mtCsKey);
  Pa_osalBeginMemAccess (paInst, sizeof(paInst_t));
  
  for ( i = 0; i < pa_N_BUFS; i++)
  {
    bases[i] = paInst->paBufs[i].base;
  }
  
  Pa_osalEndMemAccess (paInst, sizeof(paInst_t));
  Pa_osalMtCsExit(mtCsKey);  

  return (pa_OK);

} /* Pa_close */















