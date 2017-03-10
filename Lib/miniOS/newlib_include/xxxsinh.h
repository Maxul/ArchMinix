/* xxxsinh.h -- common _[FL]Sinh functionality */
#include "xmath.h"
_STD_BEGIN

	/* coefficients */

 #if   FBITS <= 30
  #define CPOLY(x)	(c[0] * x + c[1])
  #define SPOLY(x)	(s[0])
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE c[] = {	/* 3rd-order, |x| < ln(3)/2 (100.0%) */
/*** END TI REPLACE ***/
	FLIT( 0.0018399770),
	FLIT(-0.0610359754),
	};
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE s[] = {
/*** END TI REPLACE ***/
	FLIT( 0.1056307265),
	};

 #elif FBITS <= 53
  #define CPOLY(x)	(c[0] * x + c[1])
  #define SPOLY(x)	((s[0] * x + s[1]) * x + s[2])
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE c[] = {	/* 5th-order, |x| < ln(3)/2 (100.0%) */
/*** END TI REPLACE ***/
	FLIT( 0.00015900461490295664),
	FLIT(-0.02056695099386675945),
	};
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE s[] = {
/*** END TI REPLACE ***/
	FLIT( 0.00005352221341916260),
	FLIT( 0.00506451278229574483),
	FLIT( 0.14609971567280727143),
	};

 #elif FBITS <= 64
  #define CPOLY(x)	((c[0] * x + c[1]) * x + c[2])
  #define SPOLY(x)	((s[0] * x + s[1]) * x + s[2])
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE c[] = {	/* 6th-order, |x| < ln(3)/2 (100.0%) */
/*** END TI REPLACE ***/
	FLIT(-0.00000158330729052950769915),
	FLIT( 0.00027519426342349878805713),
	FLIT( -0.0241170695527346096673505),
	};
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE s[] = {
/*** END TI REPLACE ***/
	FLIT( 0.0000417195220925635337445),
	FLIT( 0.0045890160046340899454803),
	FLIT( 0.1425495971139320626966453),
	};

 #elif FBITS <= 114
  #define CPOLY(x)	((((c[0] * x + c[1]) * x + c[2]) * x + c[3]) * x + c[4])
  #define SPOLY(x)	((((s[0] * x + s[1]) * x + s[2]) * x + s[3]) * x + s[4])
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE c[] = {	/* 9th-order, |x| < ln(3)/2 (100.0%) */
/*** END TI REPLACE ***/
	FLIT(-0.00000000000336993857130038006925178471496),
	FLIT( 0.00000000194074062258199190461301221438596),
	FLIT(-0.00000060338126386989611923562546176683121),
	FLIT( 0.00012225633519572222239274990096628782027),
	FLIT(-0.01570282210530478910525045132285980643671),
	};
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE s[] = {
/*** END TI REPLACE ***/
	FLIT( 0.0000000013284592352809230453478855863105),
	FLIT( 0.0000005602726057324471474526599821935838),
	FLIT( 0.0000873285221372423111008817391507917175),
	FLIT( 0.0058384526509782573715176746781914259057),
	FLIT( 0.1509638445613618775614162153438248456945),
	};

 #else /* FBITS */
  #error sinh has insufficient precision
 #endif /* FBITS */
/*** START TI REPLACE ***/
static const _DATA_ACCESS FTYPE ln3by2 = FLIT(0.54930614433405484569762261846126284);
/*** END TI REPLACE ***/
FTYPE FNAME(Sinh)(FTYPE x, FTYPE y)
	{	/* compute tanh(x) */
	short neg;

	switch (FNAME(Dtest)(&x))
		{	/* test for special codes */
	case _NANCODE:
		return (x);
	case _INFCODE:
		return (y != FLIT(0.0) ? x : FISNEG(x) ? -y : y);
	case 0:
		return (x * y);
	default:	/* finite */
		if (y == FLIT(0.0))
			return (x < FLIT(0.0) ? -y : y);
		if (x < FLIT(0.0))
			x = -x, neg = 1;
		else
			neg = 0;

		if (x < FCONST(Rteps))
			x *= y;	/* x tiny */
		else if (x < ln3by2)
			{	/* |x| < ln(3)/2 */
			const FTYPE z = x * x;
			const FCOMP ch = FLIT(1.0) + z * CPOLY(z);

			x += x * z * SPOLY(z);
			x /= ch;
			}
		else if (x < FNAME(Xbig))
			{	/* worth subtracting exp(-x)/2 */
			FNAME(Exp)(&x, FLIT(1.0), -1);
			x = y * (x - FLIT(0.25) / x);
			}
		else
			switch (FNAME(Exp)(&x, y, -1))
				{	/* report over/underflow */
			case 0:
				_Feraise(_FE_UNDERFLOW);
				break;
			case _INFCODE:
				_Feraise(_FE_OVERFLOW);
				}
		return (neg ? -x : x);
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
