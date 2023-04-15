/******************************************************************************	
 * Copyright (c) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary	forms, with or without	
 *  modification, are permitted	provided that the following conditions	
 *  are	met:
 *
 *    Redistributions of source	code must retain the above copyright 
 *    notice, this list	of conditions and the following	disclaimer.
 *
 *    Redistributions in binary	form must reproduce the	above copyright	
 *    notice, this list	of conditions and the following	disclaimer in the 
 *    documentation and/or other materials provided with the	
 *    distribution.
 *
 *    Neither the name of Texas	Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this	software without specific prior	written	permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	
 *  "AS	IS" AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO,	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR	ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY,	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO,	PROCUREMENT OF SUBSTITUTE GOODS	OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION)	HOWEVER	CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,	OR TORT	
 *  (INCLUDING NEGLIGENCE OR OTHERWISE)	ARISING	IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/	

/******************************************************************************	
 *
 * File	Name:	types.h	
 *
 * Description:	This file contains the standard	data types used	
 * 
 ******************************************************************************/
#ifndef	_TYPES_H_
#define	_TYPES_H_

/*********************************
 * Defines and Macros and globals
 *********************************/

// Generic masks.
#define	BIT0	(0x00000001)
#define	BIT1	(0x00000002)
#define	BIT2	(0x00000004)
#define	BIT3	(0x00000008)
#define	BIT4	(0x00000010)
#define	BIT5	(0x00000020)
#define	BIT6	(0x00000040)
#define	BIT7	(0x00000080)
#define	BIT8	(0x00000100)
#define	BIT9	(0x00000200)
#define	BIT10	(0x00000400)
#define	BIT11	(0x00000800)
#define	BIT12	(0x00001000)
#define	BIT13	(0x00002000)
#define	BIT14	(0x00004000)
#define	BIT15	(0x00008000)
#define	BIT16	(0x00010000)
#define	BIT17	(0x00020000)
#define	BIT18	(0x00040000)
#define	BIT19	(0x00080000)
#define	BIT20	(0x00100000)
#define	BIT21	(0x00200000)
#define	BIT22	(0x00400000)
#define	BIT23	(0x00800000)
#define	BIT24	(0x01000000)
#define	BIT25	(0x02000000)
#define	BIT26	(0x04000000)
#define	BIT27	(0x08000000)
#define	BIT28	(0x10000000)
#define	BIT29	(0x20000000)
#define	BIT30	(0x40000000)
#define	BIT31	(0x80000000)


#define	NULL 0

#define	SUCCESS					(0)
#define	FAIL 					(1)

// Macros for error
#define	NULL_POINTER_ERROR		(2)
#define	INVALID_ARGUMENTS		(3)


#endif // TYPES_H_
