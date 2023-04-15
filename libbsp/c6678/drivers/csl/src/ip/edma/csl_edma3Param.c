/**
 *   @file  csl_edma3Param.c
 *
 *   @brief   
 *      The file implements the API's which handle the PARAM-SET in the EDMA 
 *      Module.
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

/** ============================================================================
 *   @n@b CSL_edma3GetParamHandle
 *
 *   @b Description
 *   @n The function is used to get a specific PARAM Entry handle.
 *
 *   @b Arguments
 *   @verbatim
            hEdma           Channel Handle
            paramNum        Parameter entry number
            status          Status of the function call
      @endverbatim
 *
 *   <b> Return Value </b>  
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and CSL_edma3ChannelOpen() must be 
 *       called successfully in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle         hModule;
            CSL_Edma3Obj            edmaObj;
            CSL_Status              status;
            CSL_Edma3ChannelHandle  hChannel;
            CSL_Edma3ChannelAttr    chAttr;
            CSL_Edma3ParamHandle    hParamBasic;
            
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);

            // Open DMA Channel 1.
            chAttr.regionNum = CSL_EDMA3_REGION_GLOBAL;
            chAttr.chaNum    = 1;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            if ((hChannel == NULL) || (status != CSL_SOK)) 
            {
                printf ("Error: Unable to open EDMA Channel:%d\n", channelNum);
                return -1;
            }
            ...
            // Obtain a handle to PARAM Entry 0
            hParamBasic = CSL_edma3GetParamHandle(hChannel,0,&status);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetParamHandle, ".text:csl_section:edma3");
CSL_Edma3ParamHandle  CSL_edma3GetParamHandle 
(
    CSL_Edma3ChannelHandle    hEdma,
    Int16                     paramNum,
    CSL_Status*               status       
)
{   
    CSL_Edma3ParamHandle handle = (CSL_Edma3ParamHandle)NULL;

    if (status == NULL) {
        /* Do nothing */
    }
    else if (hEdma == NULL) {
        *status = CSL_ESYS_BADHANDLE;
    }
    else if (paramNum < 0 || (paramNum >= hEdma->cfgInfo.numParamsets)) {
        *status = CSL_ESYS_INVPARAMS;
    }
    else {
        handle = (&hEdma->regs->PARAMSET[paramNum]);
        *status = CSL_SOK;
    }

    return (handle);
}

/** ============================================================================
 *   @n@b CSL_edma3ParamSetup
 *
 *   @b Description
 *   @n The function is used to configure the PARAM Entry block with the specific
 *      parameter information. 
 *
 *   @b Arguments
 *   @verbatim
            hParamHndl      Handle to the param entry
            setup           Pointer to param setup structure
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK
 *   @n Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *                                                
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and  CSL_edma3ChannelOpen() must be 
 *       called successfully in that order before this API can be invoked
 *
 *   <b> Post Condition </b>
 *   @n  Configures the EDMA parameter Entry
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle         hModule;
            CSL_Edma3Obj            edmaObj;
            CSL_Status              status;
            CSL_Edma3ChannelHandle  hChannel;
            CSL_Edma3ChannelAttr    chAttr;
            CSL_Edma3ParamHandle    hParamBasic;
            CSL_Edma3ParamSetup     myParamSetup;
            
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);

            // Open DMA Channel 1.
            chAttr.regionNum = CSL_EDMA3_REGION_GLOBAL;
            chAttr.chaNum    = 1;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            if ((hChannel == NULL) || (status != CSL_SOK)) 
            {
                printf ("Error: Unable to open EDMA Channel:%d\n", channelNum);
                return -1;
            }
            ...
            // Obtain a handle to PARAM Entry 0
            hParamBasic = CSL_edma3GetParamHandle(hChannel,0,&status);
            ...
            // Setup the first param Entry (Ping buffer) 
            myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                                     CSL_EDMA3_TCCH_DIS, \
                                                     CSL_EDMA3_ITCINT_DIS, \
                                                     CSL_EDMA3_TCINT_EN,\
                                                     0,CSL_EDMA3_TCC_NORMAL,\
                                                     CSL_EDMA3_FIFOWIDTH_NONE, \
                                                     CSL_EDMA3_STATIC_DIS, \
                                                     CSL_EDMA3_SYNC_A, \
                                                     CSL_EDMA3_ADDRMODE_INCR, \
                                                     CSL_EDMA3_ADDRMODE_INCR);           
            myParamSetup.srcAddr     = (Uint32)srcBuff1;         
            myParamSetup.aCntbCnt    = CSL_EDMA3_CNT_MAKE(256,1);       
            myParamSetup.dstAddr     = (Uint32)dstBuff1;        
            myParamSetup.srcDstBidx  = CSL_EDMA3_BIDX_MAKE(1,1);     
            myParamSetup.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(CSL_EDMA3_LINK_NULL,0);     
            myParamSetup.srcDstCidx  = CSL_EDMA3_CIDX_MAKE(0,1);     
            myParamSetup.cCnt        = 1;

            // Configure the PARAM Entry with the setup information. 
            CSL_edma3ParamSetup(hParamBasic,&myParamSetup);     
            ... 
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3ParamSetup, ".text:csl_section:edma3");
CSL_Status  CSL_edma3ParamSetup 
(
    CSL_Edma3ParamHandle    hParamHndl,
    CSL_Edma3ParamSetup*    setup
)
{
    Uint32        *paramHndl = (Uint32*)hParamHndl;
    Uint32        i;
    CSL_Status    status = CSL_SOK;
    
    if (hParamHndl == NULL) {
        status = CSL_ESYS_BADHANDLE;
    }
    else if (setup == NULL) {
        status = CSL_ESYS_INVPARAMS;
    }
    else {
        for (i = 0; i < 8 ; i++)
            paramHndl[i] = ((Uint32*)setup)[i];  
    }

    return (status);
}

/** ============================================================================
 *   @n@b CSL_edma3ParamWriteWord
 *
 *   @b Description
 *   @n This is for the ease of QDMA channels. Once the QDMA channel transfer is 
 *      triggered, subsequent triggers may be done with only writing the modified
 *      words in the parameter entry along with the trigger word. This API is 
 *      expected to achieve this purpose. Most usage scenarios, the user 
 *      should not be writing more than the trigger word entry.
 *
 *   @b Arguments
 *   @verbatim
            hParamHndl      Handle to the param entry
            wordOffset      word offset in the 8 word paramater entry
            word            word to be written
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Success - CSL_SOK (Param Write Word successful)
 *   @n Error   - CSL_ESYS_BADHANDLE (Invalid handle)
 *
 *   <b> Pre Condition </b>
 *   @n  CSL_edma3Init(), CSL_edma3Open() and  CSL_edma3ChannelOpen() and 
 *       must be CSL_edma3GetParamHandle(), CSL_edma3ParamSetup()
 *       called successfully in that order before this API can be invoked.
 *       The main setup structure consists of pointers to sub-structures.The  
 *       user has to allocate space for & fill in the parameter setup structure.
 *
 *   <b> Post Condition </b>
 *   @n  Configure trigger word
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle         hModule;
            CSL_Edma3Obj            edmaObj;
            CSL_Status              status;
            CSL_Edma3ChannelHandle  hChannel;
            CSL_Edma3ChannelAttr    chAttr;
            CSL_Edma3ParamHandle    hParamBasic;

            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);

            // Open DMA Channel 1.
            chAttr.regionNum = CSL_EDMA3_REGION_GLOBAL;
            chAttr.chaNum    = 1;
            hChannel = CSL_edma3ChannelOpen(&chObj, 0, &chAttr, &status);
            if ((hChannel == NULL) || (status != CSL_SOK)) 
            {
                printf ("Error: Unable to open EDMA Channel:%d\n", channelNum);
                return -1;
            }
            ...
            // Obtain a handle to PARAM Entry 0
            hParamBasic = CSL_edma3GetParamHandle(hChannel,0,&status);
            ...
            // Write trigger word
            CSL_edma3ParamWriteWord(hParamBasic, 7, myParamSetup.cCnt);
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3ParamWriteWord, ".text:csl_section:edma3");
CSL_Status  CSL_edma3ParamWriteWord
(
    CSL_Edma3ParamHandle       hParamHndl,
    Uint16                     wordOffset,
    Uint32                     word 
)
{
    Uint32*     hParam = (Uint32*)(hParamHndl);
    CSL_Status  status = CSL_SOK;
    
    if (hParamHndl == NULL)
        status = CSL_ESYS_BADHANDLE;
    else    
        hParam[wordOffset] = word;  
    
    return (status);
}

