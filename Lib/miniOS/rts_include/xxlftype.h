/****************************************************************************/
/*  xxlftype.h           v7.4.11                                            */
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
/* xxlftype.h -- parameters for long double floating-point type */
#include <float.h>

#define FTYPE	long double
#define FCOMP	long double
#define FCTYPE	_Lcomplex
#define FBITS	LDBL_MANT_DIG
#define FMAXEXP	LDBL_MAX_EXP
#define FFUN(fun)	fun##l
#define FMACRO(x)	L##x
#define FNAME(fun)	_L##fun
#define FCONST(obj)	_L##obj._Long_double
#define FLIT(lit)	lit##L
#define FISNEG(exp)	LSIGN(exp)

 #if _IS_EMBEDDED
#define FCPTYPE	float_complex	/* place holder */

 #else /* _IS_EMBEDDED */
#define FCPTYPE	complex<long double>
 #endif /* _IS_EMBEDDED */

#ifdef __TI_COMPILER_VERSION__
#define FBIAS _LBIAS

#include <stdint.h>

/*---------------------------------------------------------------------------*/
/* This code requires the target to support type-punning intrinsics.  These  */
/* intrinsics treat the bits of a floating-point value as if it were an      */
/* integer, to allow fast access to the sign, exponent, and fractional bits. */
/* We need to know whether the target has an intrinsic that can represent    */
/* the entire floating-point type with one integral type.                    */
/*---------------------------------------------------------------------------*/
#if LDBL_MANT_DIG == 24 /* LDBL_MANT_DIG == 24 implies _32_BIT_DOUBLE */
 #define __LONG_DOUBLE_SIZE 32
#else
 #define __LONG_DOUBLE_SIZE 64
#endif

/*---------------------------------------------------------------------------*/
/* ARP32, C6000, and TMS470 only have intrinsics that produce 32-bit chunks. */
/*---------------------------------------------------------------------------*/

 #define __CHUNK_SIZE 32

 #define BIT_MASK(n) ((UINT32_C(1) << (n)) - 1)
 #define __SIGN_BIT_CHSZ (UINT32_C(1) << (__CHUNK_SIZE-1))

  #define FTOU _ftoi
  #define UTOF _itof

/*---------------------------------------------------------------------------*/
/* MSP430 can handle 64-chunks without splitting.                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* If the target has intrinsic which can handle the entire float with one    */
/* integral type, operate on the entire integer.                             */
/*---------------------------------------------------------------------------*/
#if __CHUNK_SIZE == __LONG_DOUBLE_SIZE

 #define SIGN_BIT_ZERO(f) ((FTOU(f) & __SIGN_BIT_CHSZ) == 0)

 #define BIASED_EXP(f) ((FTOU(f) >> (FBITS-1)) & EXP_BITS_MASK)

 #define FRAC_PART(f) (FTOU(f) & BIT_MASK(FBITS-1))
 #define FRAC_PART_IS_ZERO(f) (FRAC_PART(f) == 0)

 #define DISCARD_N_FRAC_BITS(v,n) UTOF(FTOU(v) & ~BIT_MASK(n))

 #define DISCARD_EXP_AND_FRAC_PARTS(v) UTOF(FTOU(v) & __SIGN_BIT_CHSZ)

/*---------------------------------------------------------------------------*/
/* If not, split it into two parts and operate on those.                     */
/*---------------------------------------------------------------------------*/
#else /* __CHUNK_SIZE == __LONG_DOUBLE_SIZE */

 /* We know __CHUNK_SIZE == 32, so use magic number 32 for brevity */

 #define SIGN_BIT_ZERO(f) ((_hi(f) & __SIGN_BIT_CHSZ) == 0)

 #define BIASED_EXP(f) ((_hi(f) >> (FBITS-1-32)) & EXP_BITS_MASK)

 #define FRAC_PART_HI(f) (_hi(f) & BIT_MASK(FBITS-1-32))
 #define FRAC_PART_LO(f) (_lo(f))

 #define FRAC_PART_IS_ZERO(f) \
     ((FRAC_PART_HI(f) & BIT_MASK(FBITS-1-32)) == 0 && FRAC_PART_LO(f) == 0)

 #define DISCARD_N_FRAC_BITS(v,n) \
     (n >= 32 ? (_itod(_hi(v) & ~BIT_MASK(n-32), 0)) : \
                (_itod(_hi(v), _lo(v) & ~BIT_MASK(n))))

 #define DISCARD_EXP_AND_FRAC_PARTS(v) _itod(_hi(v) & __SIGN_BIT_CHSZ, 0)

#endif /* __CHUNK_SIZE == __LONG_DOUBLE_SIZE */

#define UNBIASED_EXP(f) (BIASED_EXP(f) - (FBIAS+1))
#define BIASED_EXP_PART_IS_ZERO(f) (BIASED_EXP(f) == 0)
#define EXP_BITS_MASK  ((FMAXEXP << 1) - 1)

/*---------------------------------------------------------------------------*/
/* Some TMS470 hardware has hardware for sqrt                                */
/*---------------------------------------------------------------------------*/
#define HAS_BF_SQRT ( defined(__TMS470__) && \
                      defined(__TI_VFP_SUPPORT__) && \
                     !defined(__TI_FPv4SPD16_SUPPORT__))
#define TYPED_SQRT(n)  __sqrt(n)
#endif /* __TI_COMPILER_VERSION__ */
/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
