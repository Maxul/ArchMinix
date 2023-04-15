;/*****************************************************************************
; *  Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008   
; *  
; *  Use of this software is controlled by the terms and conditions found in the
; *  license agreement under which this software has been supplied.             
; *****************************************************************************/

;/** @file _csl_intcIsrDispatch.asm
; *
; *  @date 12th June, 2004
; *  @author Ruchika Kharwar
; */
    .global __CSL_intcDispatcher
    .def __CSL_intcVectorTable
    .ref __CSL_intcCpuIntrTable
    
RESV    .macro num
    .loop num
    mvkh __CSL_intcCpuIntrTable,a4
    .endloop
    .endm
_CSL_intcpush .macro reg
    stw reg,*b15--[1]
    .endm
_CSL_intcpop .macro reg
    ldw *++b15[1],reg
    .endm
CALLDISP .macro intr
    _CSL_intcpush a0
    mvkl __CSL_intcCpuIntrTable, a0
    mvkh __CSL_intcCpuIntrTable, a0
    ldw *++a0[intr + 1], a0
    nop 2
    stw a0, *-a0[intr + 1]  
    _CSL_intcpop a0
    bnop a0, 5  
    .endm   
    .sect ".csl_vect"
    .align 1024          
__CSL_intcVectorTable:    
__CSL_intcRsv0:
    RESV 8

__CSL_intcIsrNMI:   
    CALLDISP 1
     
__CSL_intcRsv2:
    RESV 8
    
__CSL_intcRsv3:
    RESV 8
    
__CSL_intcIsr4:
    CALLDISP 4 

__CSL_intcIsr5:
    CALLDISP 5

__CSL_intcIsr6:
    CALLDISP 6

__CSL_intcIsr7:
    CALLDISP 7

__CSL_intcIsr8:
    CALLDISP 8

__CSL_intcIsr9:
    CALLDISP 9

__CSL_intcIsr10:
    CALLDISP 10

__CSL_intcIsr11:
    CALLDISP 11

__CSL_intcIsr12:
    CALLDISP 12

__CSL_intcIsr13:
    CALLDISP 13

__CSL_intcIsr14:
    CALLDISP 14

__CSL_intcIsr15:
    CALLDISP 15 
    

    
    
        
