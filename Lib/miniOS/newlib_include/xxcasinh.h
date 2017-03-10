/* xxcasinh.h -- common casinh[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

/*** TI REMOVE ***/  /* Conflicts with erroneous #define in TI's mathl.h */
/* FTYPE FFUN(asinh)(FTYPE); */

FCTYPE (FFUN(casinh))(FCTYPE x)
	{	/* compute complex hyperbolic arcsine */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	const short err_re = FNAME(Dtest)(&re);
	const short err_im = FNAME(Dtest)(&im);
	FTYPE u, v;

	if (err_re == _NANCODE)
		if (err_im == _NANCODE || im == FLIT(0.0))
			u = re, v = im;
		else if (err_im == _INFCODE)
			u = im, v = re;
		else
			{	/* (NaN, non-zero finite): raise invalid */
			_Feraise(_FE_INVALID);
			u = re, v = FCONST(Nan);
			}
	else if (err_im == _NANCODE)
		if (err_re == _INFCODE)
			u = FCONST(Inf), v = im;
		else
			{	/* (finite, NaN): raise invalid */
			_Feraise(_FE_INVALID);
			u = FCONST(Nan), v = im;
			}
	else
		{	/* re, im not NaN: compute asinh */
		const FCTYPE w = FFUN(csqrt)(FNAME(Cbuild)(FLIT(1.0) - im, re));
		const FCTYPE z = FFUN(csqrt)(FNAME(Cbuild)(FLIT(1.0) + im, -re));

		u = FFUN(asinh)(FFUN(cimag)(FNAME(Cmulcc)(w, FFUN(conj)(z))));
		v = FFUN(atan2)(im, FFUN(creal)(FNAME(Cmulcc)(w, z)));
		}
	return (FNAME(Cbuild)(u, v));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
