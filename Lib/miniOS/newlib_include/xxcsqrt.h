/* xxcsqrt.h -- common csqrt[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FFUN(csqrt))(FCTYPE x)
	{	/* find complex square root */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	int xexp;
	FTYPE rho =
		FNAME(Hypot)(re, im, &xexp);	/* get magnitude and scale factor */

	if (xexp == 0)
		return (FNAME(Cbuild)(rho, rho));	/* x is 0, INF, or NaN */
	else
		{	/* compute in safest quadrant */
		FTYPE remag = FFUN(ldexp)(re < FLIT(0.0) ? -re : re, -xexp);

		rho = FFUN(ldexp)(FFUN(sqrt)(FLIT(2.0) * (remag + rho)),
			xexp / 2 - 1);
		if (FLIT(0.0) <= re)
			return (FNAME(Cbuild)(rho, im / (FLIT(2.0) * rho)));
		else if (im < FLIT(0.0))
			return (FNAME(Cbuild)(-im / (FLIT(2.0) * rho), -rho));
		else
			return (FNAME(Cbuild)(im / (FLIT(2.0) * rho), rho));
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
