/******************************************************************************

  Copyright (C), 2014, Texas Instrument.

 ******************************************************************************
  File Name     : common_test.h
  Version       : Initial Draft
  Author        : Brighton Feng
  Created       : June 5, 2014
  Last Modified : 
  Description   : KeyStone common test functions and definitions
  History       :
  1.Date        : June 5, 2014
    Author      : Brighton Feng
    Modification: Created file

******************************************************************************/
#ifndef _COMMON_TEST_H_
#define _COMMON_TEST_H_

#include <ti/csl/tistdtypes.h>

/*****************************************************************************
 Description  : this function write the address to corresponding memory
                unit and readback for verification
 Input        : unsigned int uiStartAddress  
                unsigned int uiByteCount     
                unsigned int iStep: step (index offset) of next access 
                                    from the privous one
                uiByteCount and iStep should be multiple of 8 bytes
                unsigned int uiMaxFails: return if uiMaxFails happen
 Return Value : number of failed units
*****************************************************************************/
extern unsigned int Memory_Address_Test(unsigned int uiStartAddress, 
	unsigned int uiByteCount, unsigned int uiMaxFails, int iStep);

/*****************************************************************************
 Description  : this function fill memory with a pattern
                and readback for verification
 Input        : unsigned int uiStartAddress  
                unsigned int uiByteCount     
                unsigned int uiPattern       
                unsigned int iStep: step (index offset) of next access 
                                    from the privous one
                uiByteCount and iStep should be multiple of 8 bytes
                unsigned int uiMaxFails: return if uiMaxFails happen
 Return Value : number of failed units
*****************************************************************************/
extern unsigned int Memory_Fill_Test(unsigned int uiStartAddress, 
	unsigned int uiByteCount, unsigned int uiPattern, 
	unsigned int uiMaxFails, int iStep);

/*****************************************************************************
 Description  : this function test the data bus to detect bit stuck, and
                interference between bits.
 Return Value : fail count
*****************************************************************************/
extern unsigned int Memory_Data_Bus_Test(unsigned int uiBaseAddress,
	unsigned int uiBusWidth);

/*****************************************************************************
 Description  : this function test the address bus to detect bit stuck, and
                interference between bits.
 Input        : unsigned int uiBaseAddress, must align to uiNumBytes boundary  
                unsigned int uiNumBytes, number of bytes of the memory under test,
                  this is used to determine number of address bits
                unsigned int uiMAU_bytes, Min Access Unit in bytes
 Return Value : fail count
*****************************************************************************/
extern unsigned int Memory_Address_Bus_Test(unsigned int uiBaseAddress,
	unsigned int uiNumBytes, unsigned int uiMAU_bytes);

/*quick test to verify basic functions of memory from "uiStartAddress".
"uiTotalByteCount" determines number of address bits for address bus test.
"uiFillByteCount" determines how many bytes are filled into the memory, 
the bigger the "uiFillByteCount", the longer the test time.
this function returns fail count*/
extern int Memory_quick_test(unsigned int uiStartAddress,
	unsigned int uiTotalByteCount, unsigned int uiFillByteCount,
	unsigned int uiDataBusWidth);

/*double check error at a specific address to see if the error is consistent 
and hlep judge it is read or write error*/
extern void Memory_error_double_check(volatile unsigned long long * ullpAddress);

/*Test throughput of EDMA "TC_channel" for data transfer from "src" to "dst"*/
extern void edma_Throughput_Test (Uint32 src, Uint32 dst, Uint32 uiACount, 
	Uint32 uiBCount, Uint32 uiIndex, CSL_TpccRegs*  EDMACCRegs, Uint32 TC_channel);


//Copy multiple of 8 bytes data to show the max bandwith of data transfer by CPU
extern void MemCopy8(unsigned long long * restrict dst, unsigned long long * restrict src, Uint32 uiCount);
extern void MemCopy8Test(unsigned long long * srcBuff, unsigned long long * dstBuff, Uint32 uiByteCnt);

#endif
