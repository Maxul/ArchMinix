/* xxcacosh.h -- common cacosh[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

/*** TI REMOVE ***/  /* Conflicts with erroneous #define in TI's mathl.h */
/* FTYPE FFUN(asinh)(FTYPE); */

FCTYPE (FFUN(cacosh))(FCTYPE x)
	{	/* compute complex hyperbolic arccosine */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	const short err_re = FNAME(Dtest)(&re);
	const short err_im = FNAME(Dtest)(&im);

	if (err_re == _NANCODE)
		if (err_im == _NANCODE)
			return (x);
		else if (err_im == _INFCODE)
			return (FNAME(Cbuild)(FCONST(Inf), re));
		else
			{	/* (NaN, finite): raise invalid */
			_Feraise(_FE_INVALID);
			return (FNAME(Cbuild)(re, FCONST(Nan)));
			}
	else if (err_im == _NANCODE)
		if (err_im == _INFCODE)
			return (FNAME(Cbuild)(FCONST(Inf), im));
		else
			{	/* (finite, NaN): raise invalid */
			_Feraise(_FE_INVALID);
			return (FNAME(Cbuild)(FCONST(Nan), im));
			}
	else
		{	/* re, im not NaN: compute acosh */
		const FCTYPE z = FFUN(csqrt)(FNAME(Cbuild)(re + FLIT(1.0), im));
		const FTYPE u = FFUN(asinh)(FFUN(creal)(FNAME(Cmulcc)(z,
			FFUN(csqrt)(FNAME(Cbuild)(re - FLIT(1.0), -im)))));
		const FTYPE v = FLIT(2.0) * FFUN(atan2)(
			FFUN(cimag)(FFUN(csqrt)(FNAME(Cbuild)(re - FLIT(1.0), im))),
			FFUN(creal)(z));

		return (FNAME(Cbuild)(u, v));
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
