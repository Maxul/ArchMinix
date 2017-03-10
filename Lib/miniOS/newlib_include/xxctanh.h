/* xxctanh.h -- common ctanh[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

FCTYPE (FFUN(ctanh))(FCTYPE x)
	{	/* find hyperbolic complex tangent */
	const FTYPE tv = FFUN(tan)(FFUN(cimag)(x));
	const FTYPE sv = FNAME(Sinh)(FFUN(creal)(x), FLIT(1.0));
	const FTYPE bv = sv * (FLIT(1.0) + tv * tv);
	const FTYPE dv = FLIT(1.0) + bv * sv;

	return (FNAME(Cbuild)(FFUN(sqrt)(FLIT(1.0) + sv * sv) * bv / dv,
		tv / dv));
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
