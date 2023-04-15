/**
 *   @file  csl_edma3GetHwStatus.c
 *
 *   @brief   
 *      The file implements the API which to get status of various entities in
 *      the EDMA module.
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
 *   @n@b CSL_edma3GetHwStatus
 *
 *   @b Description
 *   @n The function gets the status of various entities in the EDMA module. The
 *      entity whose status needs to be retreived is specified in the query and
 *      the response is populated in the 'response'.
 *
 *   @b Arguments
 *   @verbatim
            hMod            Edma module handle
            myQuery         Query to be performed
            response        Pointer to buffer to return the data requested by
                            the query passed
     @endverbatim
 *
 *   @sa
 *      CSL_Edma3HwStatusQuery
 *
 *   <b> Return Value </b>
 *   @n  Success - CSL_SOK
 *   @n  Error   - CSL_ESYS_BADHANDLE (The handle passed is invalid)
 *   @n  Error   - CSL_ESYS_INVPARAMS (The parameter passed is invalid)
 *   @n  Error   - CSL_ESYS_INVQUERY  (The query is invalid)
 *
 *   <b> Pre Condition </b>
 *   @n  Functions CSL_edma3Init(), CSL_edma3Open() must be called successfully 
 *       in that order before this API can be invoked.argument type that can be
 *       void* casted & passed with a particular command refer to 
 *       CSL_Edma3HwStatusQuery
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Reads
 *   @n TPCC_TPCC_CFG,TPCC_TPCC_PID;
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_Edma3Handle             hModule;
            CSL_Edma3Obj                edmaObj;
            CSL_Status                  status;
            CSL_Edma3QueryInfo          info;
        
            // Module Initialization
            CSL_edma3Init(NULL);
            
            // Module Level Open    
            hModule = CSL_edma3Open(&edmaObj,CSL_EDMA3,NULL,&status);
            ...
            // Query Module Info 
            CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INFO,&info);  
            ...
     @endverbatim
 * =============================================================================
 */
#pragma CODE_SECTION (CSL_edma3GetHwStatus, ".text:csl_section:edma3");
CSL_Status CSL_edma3GetHwStatus 
(
    CSL_Edma3Handle         hMod,
    CSL_Edma3HwStatusQuery  myQuery,
    void*                   response
)
{
    CSL_Status status = CSL_SOK;
    
    if (hMod == NULL)
        return CSL_ESYS_BADHANDLE;

    if (response == NULL)
        return CSL_ESYS_INVPARAMS;

    /* Process the query... */
    switch(myQuery)
    {
        case CSL_EDMA3_QUERY_MEMFAULT:  
        {
            CSL_edma3GetMemoryFaultError(hMod, (CSL_Edma3MemFaultStat*)response);
            break;
        }
        case CSL_EDMA3_QUERY_MEMPROTECT:
        {
            CSL_Edma3CmdRegion* ptrCmdRegion = (CSL_Edma3CmdRegion *)response;

            /* Get the memory protection/access attributes. */
            CSL_edma3GetMemoryProtectionAttrib(hMod, ptrCmdRegion->region, &ptrCmdRegion->regionVal);
            break;
        }
        case CSL_EDMA3_QUERY_CTRLERROR:
        {
            CSL_edma3GetControllerError(hMod,  (CSL_Edma3CtrlErrStat*)response);
            break;
        }
        case CSL_EDMA3_QUERY_INTRPEND:  
        {
            CSL_Edma3Obj*       ptrEdma3Obj = (CSL_Edma3Obj *)hMod;

            /* Get the low pending interrupts. */
            CSL_edma3GetLoPendingInterrupts(hMod,((CSL_Edma3CmdIntr*)response)->region,
                                                 &((CSL_Edma3CmdIntr*)response)->intr);

            /* Get the high pending interrupts only if they are supported */
            if (ptrEdma3Obj->cfgInfo.numINTChannel > 31)
            {
                CSL_edma3GetHiPendingInterrupts(hMod, ((CSL_Edma3CmdIntr*)response)->region,
                                                &((CSL_Edma3CmdIntr*)response)->intrh);
            }
            else
            {
                /* Reset the high pending interrupts to 0. */
                ((CSL_Edma3CmdIntr*)response)->intrh = 0;
            }
            break;
        }
        case CSL_EDMA3_QUERY_EVENTMISSED:
        {
            CSL_edma3GetEventMissed(hMod, &((CSL_BitMask32*)response)[0],
                                          &((CSL_BitMask32*)response)[1],
                                          &((CSL_BitMask32*)response)[2]);
            break;   
        }
        case CSL_EDMA3_QUERY_QUESTATUS: 
        {
            CSL_edma3GetWaterMark (hMod, ((CSL_Edma3QueStat*)response)->que, &((CSL_Edma3QueStat*)response)->waterMark);
            CSL_edma3GetNumberValidEntries (hMod, ((CSL_Edma3QueStat*)response)->que, &((CSL_Edma3QueStat*)response)->numVal);
            CSL_edma3GetStartPointer (hMod, ((CSL_Edma3QueStat*)response)->que, &((CSL_Edma3QueStat*)response)->startPtr);
            CSL_edma3GetThresholdExceeded (hMod, ((CSL_Edma3QueStat*)response)->que, &((CSL_Edma3QueStat*)response)->exceed);
            break;
        }
        case CSL_EDMA3_QUERY_ACTIVITY:
        {
            CSL_edma3GetActivityStatus(hMod, ((CSL_Edma3ActivityStat*)response));
            break;
        }
        case CSL_EDMA3_QUERY_INFO:
        {
            CSL_edma3GetInfo(hMod,((CSL_Edma3QueryInfo*)response));
            break;
        }
        default:
        {
            status = CSL_ESYS_INVQUERY;
            break; 
        }
    }
    return (status);
}

