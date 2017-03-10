/****************************************************************************/
/*  xxfftype.h           v7.4.11                                            */
/*                                                                          */
/* Copyright (c) 2010-2014 Texas Instruments Incorporated                   */
/* http://www.ti.com/                                                       */
/*                                                                          */
/*  Redistribution and  use in source  and binary forms, with  or without   */
/*  modification,  are permitted provided  that the  following conditions   */
/*  are met:                                                                */
/*                                                                          */
/*     Redistributions  of source  code must  retain the  above copyright   */
/*     notice, this list of conditions and the following disclaimer.        */
/*                                                                          */
/*     Redistributions in binary form  must reproduce the above copyright   */
/*     notice, this  list of conditions  and the following  disclaimer in   */
/*     the  documentation  and/or   other  materials  provided  with  the   */
/*     distribution.                                                        */
/*                                                                          */
/*     Neither the  name of Texas Instruments Incorporated  nor the names   */
/*     of its  contributors may  be used to  endorse or  promote products   */
/*     derived  from   this  software  without   specific  prior  written   */
/*     permission.                                                          */
/*                                                                          */
/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS   */
/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT   */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT   */
/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT   */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   */
/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT   */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                          */
/****************************************************************************/
/* xxfftype.h -- parameters for float floating-point type */
#include <float.h>

#define FTYPE	float
#define FCOMP	double
#define FCTYPE	_Fcomplex
#define FBITS	FLT_MANT_DIG
#define FMAXEXP	FLT_MAX_EXP
#define FFUN(fun)	fun##f
#define FMACRO(x)	F##x
#define FNAME(fun)	_F##fun
#define FCONST(obj)	_F##obj._Float
#define FLIT(lit)	lit##F
#define FISNEG(exp)	FSIGN(exp)

 #if _IS_EMBEDDED
#define FCPTYPE	float_complex

 #else /* _IS_EMBEDDED */
#define FCPTYPE	complex<float>
 #endif /* _IS_EMBEDDED */

#ifdef __TI_COMPILER_VERSION__
#define FBIAS _FBIAS

#include <stdint.h>

#define BIT_MASK(n) ((UINT32_C(1) << (n)) - 1)
#define __SIGN_BIT_CHSZ (UINT32_C(1) << (32-1))

 #define FTOU _ftoi
 #define UTOF _itof

#define SIGN_BIT_ZERO(f) ((FTOU(f) & __SIGN_BIT_CHSZ) == 0)

#define BIASED_EXP(f) ((FTOU(f) >> (FBITS-1)) & EXP_BITS_MASK)

#define FRAC_PART(f) (FTOU(f) & BIT_MASK(FBITS-1))
#define FRAC_PART_IS_ZERO(f) (FRAC_PART(f) == 0)

#define DISCARD_N_FRAC_BITS(v,n) UTOF(FTOU(v) & ~BIT_MASK(n))

#define DISCARD_EXP_AND_FRAC_PARTS(v) UTOF(FTOU(v) & __SIGN_BIT_CHSZ)

#define UNBIASED_EXP(f) (BIASED_EXP(f) - (FBIAS+1))
#define BIASED_EXP_PART_IS_ZERO(f) (BIASED_EXP(f) == 0)
#define EXP_BITS_MASK  ((FMAXEXP << 1) - 1)

#define HAS_BF_SQRT (defined(__TMS470__) && defined(__TI_VFP_SUPPORT__))
#define TYPED_SQRT(n)  __sqrtf(n)
#endif /* __TI_COMPILER_VERSION__ */
/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
