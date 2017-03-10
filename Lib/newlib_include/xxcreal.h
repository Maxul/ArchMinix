/* xxcreal.h -- common creal[fl] functionality */
#include <complex.h>
_STD_BEGIN

FTYPE (FFUN(creal))(FCTYPE x)
	{	/* extract real component of complex */
	return (((FTYPE *)&x)[0]);
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
