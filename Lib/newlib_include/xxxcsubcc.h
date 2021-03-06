/* xxxcsubcc.h -- common _[FL]Csubcc functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FNAME(Csubcc))(FCTYPE x, FCTYPE y)
	{	/* find complex difference */
	FTYPE xre = FFUN(creal)(x);
	FTYPE xim = FFUN(cimag)(x);
	FTYPE yre = FFUN(creal)(y);
	FTYPE yim = FFUN(cimag)(y);

	return (FNAME(Cbuild)(xre - xim, yre - yim));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
