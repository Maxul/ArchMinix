/* xxxcdivcc.h -- common _[FL]Cdivcc functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FTYPE FFUN(nan)(const char *);

FCTYPE (FNAME(Cdivcc))(FCTYPE x, FCTYPE y)
	{	/* find complex quotient */
	FTYPE xre = FFUN(creal)(x);
	FTYPE xim = FFUN(cimag)(x);
	FTYPE yre = FFUN(creal)(y);
	FTYPE yim = FFUN(cimag)(y);

	if (FNAME(Dtest)(&yre) || FNAME(Dtest)(&yim))
		xre = FFUN(nan)(0), xim = xre;
	else if ((yim < 0 ? -yim : +yim)
		< (yre < 0 ? -yre : +yre))
		{	/* |yim| < |yre| */
		FTYPE wr = yim / yre;
		FTYPE wd = yre + wr * yim;
		if (FNAME(Dtest)(&wd) || wd == 0)
			xre = FFUN(nan)(0), xim = xre;
		else
			{	/* compute representable result */
			FTYPE temp = (xre + xim * wr) / wd;
			xim = (xim - xre * wr) / wd;
			xre = temp;
			}
		}
	else if (yim == 0)
		xre = FFUN(nan)(0), xim = xre;
	else
		{	/* 0 < |yre| <= |yim| */
		FTYPE wr = yre / yim;
		FTYPE wd = yim + wr * yre;
		if (FNAME(Dtest)(&wd) || wd == 0)
			xre = FFUN(nan)(0), xim = xre;
		else
			{	/* compute representable result */
			FTYPE temp = (xre * wr + xim) / wd;
			xim = (xim * wr - xre) / wd;
			xre = temp;
			}
		}

	return (FNAME(Cbuild)(xre, xim));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
