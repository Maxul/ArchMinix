/* xxccosh.h -- common ccos[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FFUN(ccosh))(FCTYPE x)
	{	/* find complex hyperbolic cosine */
	return (FNAME(Cbuild)(
		FNAME(Cosh)(FFUN(creal)(x), FFUN(cos)(FFUN(cimag)(x))),
		FNAME(Sinh)(FFUN(creal)(x), FFUN(sin)(FFUN(cimag)(x)))));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
