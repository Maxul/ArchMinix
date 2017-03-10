/* xxcexp.h -- common cexp[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FFUN(cexp))(FCTYPE x)
	{	/* find complex exponential */
	const FTYPE im = FFUN(cimag)(x);
	FTYPE zre = FFUN(creal)(x);
	FTYPE zim = zre;

	FNAME(Exp)(&zre, FFUN(cos)(im), 0);
	FNAME(Exp)(&zim, FFUN(sin)(im), 0);
	return (FNAME(Cbuild)(zre, zim));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
