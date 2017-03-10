/* xxcatanh.h -- common catanh[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FTYPE FFUN(log1p)(FTYPE);

 #if FMAXEXP <= 128
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE big = FLIT(4.61e18);	/* ~sqrt(FLT_MAX)/4 */
static const _DATA_ACCESS FTYPE invbig = FLIT(0.217e-18);	/* 1/big */
#else
static const FTYPE big = FLIT(4.61e18);	/* ~sqrt(FLT_MAX)/4 */
static const FTYPE invbig = FLIT(0.217e-18);	/* 1/big */
#endif /* defined(__TI_COMPILER_VERSION__) */
 #elif FMAXEXP <= 1024
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE big = FLIT(3.35e153);	/* ~sqrt(FLT_MAX)/4 */
static const _DATA_ACCESS FTYPE invbig = FLIT(0.298e-153);	/* 1/big */
#else
static const FTYPE big = FLIT(3.35e153);	/* ~sqrt(FLT_MAX)/4 */
static const FTYPE invbig = FLIT(0.298e-153);	/* 1/big */
#endif /* defined(__TI_COMPILER_VERSION__) */
 #elif FMAXEXP <= 16384
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE big = FLIT(3.50e246);	/* ~sqrt(FLT_MAX)/4 */
static const _DATA_ACCESS FTYPE invbig = FLIT(0.286e-246);	/* 1/big */
#else
static const FTYPE big = FLIT(3.50e246);	/* ~sqrt(FLT_MAX)/4 */
static const FTYPE invbig = FLIT(0.286e-246);	/* 1/big */
#endif /* defined(__TI_COMPILER_VERSION__) */

 #else /* FMAXEXP */
  #error catanh has insufficient range
 #endif /* FMAXEXP */

FCTYPE (FFUN(catanh))(FCTYPE x)
	{	/* compute complex arctangent */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	const short err_re = FNAME(Dtest)(&re);
	const short err_im = FNAME(Dtest)(&im);
	FTYPE u, v;
#if defined(__TI_COMPILER_VERSION__)
	static const _DATA_ACCESS FTYPE piby2 = FLIT(1.5707963267948966192313216916397515);
#else
	static const FTYPE piby2 = FLIT(1.5707963267948966192313216916397515);
#endif /* defined(__TI_COMPILER_VERSION__) */
	if (err_re == _NANCODE)
		if (err_im == _NANCODE)
			u = re, v = im;
		else if (err_im == _INFCODE)
			u = FLIT(0.0), v = piby2;
		else
			{	/* (NaN finite): raise invalid */
			_Feraise(_FE_INVALID);
			u = re, v = FCONST(Nan);
			}
	else if (err_im == _NANCODE)
		if (err_re == _INFCODE || re == FLIT(0.0))
			u = FLIT(0.0), v = im;
		else
			{	/* (finite, NaN): raise invalid */
			_Feraise(_FE_INVALID);
			u = re, v = FCONST(Nan);
			}
	else
		{	/* re, im not NaN: compute atanh */
		int neg;
		FTYPE magim = FISNEG(im) ? -im : im;

		if (FISNEG(re))
			{	/* creal(x) < 0 */
			neg = 1;
			re = -re;
			}
		else
			{	/* 0 <= creal(x) */
			neg = 0;
			im = -im;
			}
		if (big < re)
			{	/* |re| is large */
			FTYPE f = im / re;

			u = FLIT(1.0) / re / (FLIT(1.0) + f * f);	/* creal(1 / x) */
			v = FISNEG(im) ? -piby2 : piby2;
			}
		else if (big < magim)
			{	/* |im| is large */
			double f = re / im;	/* creal(1 / x) */

			u = f / im / (FLIT(1.0) + f * f);
			v = FISNEG(im) ? -piby2 : piby2;
			}
		else if (re == FLIT(1.0))
			{	/* |x| == 1, singular */
			u = FFUN(log)(FFUN(sqrt)(FFUN(sqrt)(FLIT(4.0) + im * im))
				/ FFUN(sqrt)(magim + invbig));
			v = FLIT(0.5) * (piby2 + FFUN(atan2)(magim + invbig, FLIT(2.0)));
			if (FISNEG(im))
				v = -v;
			}
		else
			{	/* |x| is small */
			FTYPE refrom1 = FLIT(1.0) - re;
			FTYPE imeps2 = (magim + invbig) * (magim + invbig);

			u = FLIT(0.25) * FFUN(log1p)(FLIT(4.0) * re
				/ (refrom1 * refrom1 + imeps2));
			v = FLIT(0.5) * FFUN(atan2)(FLIT(2.0) * im,
				refrom1 * (FLIT(1.0) + re) - imeps2);
			}
		if (neg)
			u = -u;
		else
			v = -v;
		}
	return (FNAME(Cbuild)(u, v));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
