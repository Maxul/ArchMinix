/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/
#ifndef PLLC_H_
#define PLLC_H_

/******************************************************************************	
 *
 * File	Name:	pllc.h
 *
 * Description:	This contains PLL Control specific functions and defines
 *		prototypes.
 *
 ******************************************************************************/
#define PLLC_DIVEN_PLLDIV2    (1)
#define PLLC_DIVDS_PLLDIV2    (0)
#define PLLC_DIVEN_PLLDIV5    (1)
#define PLLC_DIVDS_PLLDIV5    (0)
#define PLLC_DIVEN_PLLDIV8    (1)
#define PLLC_DIVDS_PLLDIV8    (0)

typedef struct PllcHwSetup {
    /** \brief Divider Enable/Disable
     *  \param CSL_BitMask32
     */
    CSL_BitMask32 divEnable;
    /** \brief PLL Multiplier
     *  \param Uint32
     */
    Uint32        pllM;
    /** \brief PLL Divider 2
     *  \param Uint32
     */
    Uint32        pllDiv2;
    /** \brief PLL Divider 5
     *  \param Uint32
     */
    Uint32        pllDiv5;
    /** \brief PLL Divider 8
     *  \param Uint32
     */
    Uint32        pllDiv8;    
    /** \brief pre Divider value
     *  \param Uint32
     */
    Uint32        preDiv;
    /** \brief post Divider value
     *  \param Uint32
     */
    Uint32        postDiv;
    /** \brief Setup that can be used for future implementation
     *  \param void*
     */     
    void*         extendSetup;
} PllcHwSetup;

CSL_Status  CorePllcGetHwSetup(PllcHwSetup*);
CSL_Status  CorePllcHwSetup (PllcHwSetup*);
CSL_Status  corePllcInit(void *);
CSL_Status  SetPaPllConfig(void);
CSL_Status  SetDDR3PllConfig();
CSL_Status  DDR3Init(void);
void  		evmI2CInit(void);


/* Nothing past this point */
#endif
