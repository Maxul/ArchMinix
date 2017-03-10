/* xxcacos.h -- common cacos[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

/*** TI REMOVE ***/  /* Conflicts with erroneous #define in TI's mathl.h */
/* FTYPE FFUN(asinh)(FTYPE); */

FCTYPE (FFUN(cacos))(FCTYPE x)
	{	/* compute complex arccosine */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	const short err_re = FNAME(Dtest)(&re);
	const short err_im = FNAME(Dtest)(&im);
#if defined(__TI_COMPILER_VERSION__)
	static const _DATA_ACCESS FTYPE piby2 = FLIT(1.5707963267948966192313216916397515);
#else
	static const FTYPE piby2 = FLIT(1.5707963267948966192313216916397515);
#endif /* defined(__TI_COMPILER_VERSION__) */

	if (err_re == _NANCODE)
		if (err_im == _NANCODE)
			return (x);
		else if (err_im == _INFCODE)
			return (FNAME(Cbuild)(re,
				FISNEG(im) ? FCONST(Inf) : -FCONST(Inf)));
		else
			{	/* (NaN, finite): raise invalid */
			_Feraise(_FE_INVALID);
			return (FNAME(Cbuild)(re, FCONST(Nan)));
			}
	else if (err_im == _NANCODE)
		if (err_im == _INFCODE)
			return (FNAME(Cbuild)(im, re));
		else if (re == FLIT(0.0))
			return (FNAME(Cbuild)(piby2, im));
		else
			{	/* (non-zero finite, NaN): raise invalid */
			_Feraise(_FE_INVALID);
			return (FNAME(Cbuild)(FCONST(Nan), im));
			}
	else
		{	/* re, im not NaN: compute acos */
		const FCTYPE z = FFUN(csqrt)(FNAME(Cbuild)(FLIT(1.0) - re, -im));
		const FTYPE u = FLIT(2.0) * FFUN(atan2)(FFUN(creal)(z),
			FFUN(creal)(FFUN(csqrt)(FNAME(Cbuild)(FLIT(1.0) + re, im))));
		const FTYPE v = FFUN(asinh)(FFUN(cimag)(FNAME(Cmulcc)(z,
			FFUN(csqrt)(FNAME(Cbuild)(FLIT(1.0) + re, -im)))));

		return (FNAME(Cbuild)(u, v));
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
