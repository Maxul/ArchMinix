/* xxcsin.h -- common csin[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FFUN(csin))(FCTYPE x)
	{	/* find complex sine */
	return (FNAME(Cbuild)(
		FNAME(Cosh)(FFUN(cimag)(x), FFUN(sin)(FFUN(creal)(x))),
		FNAME(Sinh)(FFUN(cimag)(x), FFUN(cos)(FFUN(creal)(x)))));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
