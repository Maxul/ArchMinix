/* xxxcdivcr.h -- common _[FL]Cdivcr functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FNAME(Cdivcr))(FCTYPE x, FTYPE y)
	{	/* find complex and real quotient */
	FTYPE xre = FFUN(creal)(x);
	FTYPE xim = FFUN(cimag)(x);

	return (FNAME(Cbuild)(xre / y, xim / y));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
