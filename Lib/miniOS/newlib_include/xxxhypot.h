/* xxxhypot.h -- common _[FL]Hypot functionality */
#include "xmath.h"
_STD_BEGIN

FTYPE FNAME(Hypot)(FTYPE x, FTYPE y, int *pexp)
	{	/* compute hypotenuse with scale factor */
	short errx = FNAME(Dtest)(&x);
	short erry = FNAME(Dtest)(&y);

	*pexp = 0;
	if (errx == _INFCODE || erry == _INFCODE)
		return (FCONST(Inf));
	else if (errx == _NANCODE)
		return (x);
	else if (erry == _NANCODE)
		return (y);
	else
		{	/* x and y are finite */
		FTYPE z;

		if (x < FLIT(0.0))
			x = -x;
		if (y < FLIT(0.0))
			y = -y;
		if (x < y)
			{	/* ensure that |y| < |x| */
			FTYPE tmp = x;

			x = y;
			y = tmp;
			}

		if (x == FLIT(0.0))
			return (FLIT(0.0));	/* |(0, 0)| == 0 */
		if (FLIT(1.0) <= x)
			{	/* scale down */
			*pexp = 2;
			x = FLIT(0.25) * x;
			y = FLIT(0.25) * y;
			}
		else
			{	/* scale up */
			*pexp = -2;
			x = FLIT(4.0) * x;
			y = FLIT(4.0) * y;
			}

		z = x - y;
		if (z == x)
			;	/* y unimportant */
		else if (y < z)
			{	/* use simple approximation */
			const FTYPE qv = x / y;
			z = x + y / (qv + FFUN(sqrt)(qv * qv + FLIT(1.0)));
			}
		else
			{	/* use 1 1/2 precision to preserve bits */
#if defined(__TI_COMPILER_VERSION__)
			static const _DATA_ACCESS FTYPE root2 =
#else
			static const FTYPE root2 =
#endif /* defined(__TI_COMPILER_VERSION__) */
				FLIT(1.4142135623730950488016887242096982);
#if defined(__TI_COMPILER_VERSION__)
			static const _DATA_ACCESS FTYPE oneplusroot2high = FLIT(2.4142);
			static const _DATA_ACCESS FTYPE oneplusroot2low =
#else
			static const FTYPE oneplusroot2high = FLIT(2.4142);
 			static const FTYPE oneplusroot2low =
#endif /* defined(__TI_COMPILER_VERSION__) */
				FLIT(0.0000135623730950488016887242096980785698);
			const FTYPE qv = z / y;
			const FTYPE rv = (qv + FLIT(2.0)) * qv;
			const FTYPE sv = rv / (root2 + FFUN(sqrt)(rv + FLIT(2.0)))
				+ oneplusroot2low + qv + oneplusroot2high;
			z = x + y / sv;
			}
		return (z);
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
