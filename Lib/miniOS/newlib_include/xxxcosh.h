/* xxcosh.h -- common _[FL]Cosh functionality */
#include "xmath.h"
_STD_BEGIN

FTYPE FNAME(Cosh)(FTYPE x, FTYPE y)
	{	/* compute y * cosh(x), |y| <= 1 */
	switch (FNAME(Dtest)(&x))
		{	/* test for special codes */
	case _NANCODE:
	case _INFCODE:
		return (x);
	case 0:
		return (y);
	default:	/* finite */
		if (y == FLIT(0.0))
			return (y);
		if (x < FLIT(0.0))
			x = -x;

		if (x < FNAME(Xbig))
			{	/* worth adding in exp(-x) */
			FNAME(Exp)(&x, FLIT(1.0), -1);
			return (y * (x + FLIT(0.25) / x));
			}
		else
			{	/* x large, compute y*exp(x)/2 */
			switch (FNAME(Exp)(&x, y, -1))
				{	/* report over/underflow */
			case 0:
				_Feraise(_FE_UNDERFLOW);
				break;
			case _INFCODE:
				_Feraise(_FE_OVERFLOW);
				}
			return (x);
			}
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
