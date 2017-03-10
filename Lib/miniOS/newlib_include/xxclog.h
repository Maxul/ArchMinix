/* xxclog.h -- common clog[fl] functionality */
#include <complex.h>
#include "xmath.h"
_STD_BEGIN

 #if FBITS < 24
  #error clog has too much precision
 #endif /* FBITS < 24 */

FCTYPE (FFUN(clog))(FCTYPE x)
	{	/* compute complex logarithm */
	FTYPE re = FFUN(creal)(x);
	FTYPE im = FFUN(cimag)(x);
	FTYPE theta = FFUN(atan2)(im, re);	/* get phase */

	if (FNAME(Dtest)(&theta) == _NANCODE)
		return (FNAME(Cbuild)(theta, theta));	/* real or imag is NaN */
	else
		{	/* use 1 1/2 precision to preserve bits */
		int xexp;
		FTYPE rho =
			FNAME(Hypot)(re, im, &xexp);	/* get magnitude, scale factor */
		const short err_rho = FNAME(Dtest)(&rho);

		if (err_rho == 0)
			rho = -FCONST(Inf);	/* log(0) == -INF */
		else if (err_rho == _INFCODE)
			;	/* log(INF) == INF */
		else
			{	/* add ln2 * scale */
			FTYPE xn = (FTYPE)xexp;
#if defined(__TI_COMPILER_VERSION__)
			static const _DATA_ACCESS FTYPE cm = (FTYPE)(5814539.0 / 8388608.0);
			static const _DATA_ACCESS FTYPE cl =
#else
			static const FTYPE cm = (FTYPE)(5814539.0 / 8388608.0);
 			static const FTYPE cl =
#endif /* defined(__TI_COMPILER_VERSION__) */
				FLIT(1.1730463525082348212145817656807550e-7);

			rho = FFUN(log)(rho) + xn * cl + xn * cm;
			}
		return (FNAME(Cbuild)(rho, theta));
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
