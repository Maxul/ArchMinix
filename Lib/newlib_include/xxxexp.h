/* xxxexp.h -- common _[FL]Exp functionality */
#include "xmath.h"
_STD_BEGIN

	/* coefficients */

 #if   FBITS <= 33
  #define CPOLY(x)	(c[0])
  #define SPOLY(x)	(s[0] * x + s[1])
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE c[] = {	/* 3rd-order, |x| < ln(2)/2 (minimax) */
#else
static const FTYPE c[] = {	/* 3rd-order, |x| < ln(2)/2 (minimax) */
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0999748594),
	};
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE s[] = {
#else
static const FTYPE s[] = {
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0083208258),
	FLIT(0.4999999992),
	};

 #elif FBITS <= 60
  #define CPOLY(x)	(c[0] * x + c[1])
  #define SPOLY(x)	((s[0] * x + s[1]) * x + s[2])
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE c[] = {	/* 5th-order, |x| < ln(2)/2 (minimax) */
#else
static const FTYPE c[] = {	/* 5th-order, |x| < ln(2)/2 (minimax) */
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.00099173235263350450),
	FLIT(0.11110779924116564678),
	};
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE s[] = {
#else
static const FTYPE s[] = {
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.00003304120783105597),
	FLIT(0.01388723295391837963),
	FLIT(0.49999999999999998664),
	};

 #elif FBITS <= 74
  #define CPOLY(x)	((c[0] * x + c[1]) * x + c[2])
  #define SPOLY(x)	((s[0] * x + s[1]) * x + s[2])
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE c[] = {	/* 6th-order, |x| < ln(2)/2 (minimax) */
#else
static const FTYPE c[] = {	/* 6th-order, |x| < ln(2)/2 (minimax) */
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0000015021009413506901594),
	FLIT(0.0012624415588135726592198),
	FLIT(0.1136346396655614546681762),
	};
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE s[] = {
#else
static const FTYPE s[] = {
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0000631107933417128455181),
	FLIT(0.0151506531661140608604973),
	FLIT(0.4999999999999999999991708),
	};

 #elif FBITS <= 118
  #define CPOLY(x)	(((c[0] * x + c[1]) * x + c[2]) * x + c[3])
  #define SPOLY(x)	((((s[0] * x + s[1]) * x + s[2]) * x + s[3]) * x + s[4])
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE c[] = {	/* 9th-order, |x| < ln(2)/2 (102.5%) */
#else
static const FTYPE c[] = {	/* 9th-order, |x| < ln(2)/2 (102.5%) */
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0000000050996601178111255913989690351278),
	FLIT(0.0000062839134587711380261342848362075402),
	FLIT(0.0017156346619451467305600244926131415489),
	FLIT(0.1176466115124820805029461151975277929451),
	};
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE s[] = {
#else
static const FTYPE s[] = {
#endif /* defined(__TI_COMPILER_VERSION__) */
	FLIT(0.0000000000566531262664884418901404512823),
	FLIT(0.0000002244096566956636845481353790434277),
	FLIT(0.0001225418512858200109905907769221302052),
	FLIT(0.0171566390895743735848063909321167993553),
	FLIT(0.4999999999999999999999999999999999603141),
	};

 #else /* FBITS */
  #error exp has insufficient precision
 #endif /* FBITS */

 #if FBITS < 24
  #error exp has too much precision
 #endif /* FBITS < 24 */
#if defined(__TI_COMPILER_VERSION__)
static const _DATA_ACCESS FTYPE c1 = (FTYPE)(5814539.0 / 8388608.0);
static const _DATA_ACCESS FTYPE c2 = FLIT(1.1730463525082348212145817656807550e-7);
static const _DATA_ACCESS FTYPE hugexp = FMACRO(HUGE_EXP);
static const _DATA_ACCESS FTYPE invln2 = FLIT(1.4426950408889634073599246810018922);
#else
static const FTYPE c1 = (FTYPE)(5814539.0 / 8388608.0);
static const FTYPE c2 = FLIT(1.1730463525082348212145817656807550e-7);
static const FTYPE hugexp = FMACRO(HUGE_EXP);
static const FTYPE invln2 = FLIT(1.4426950408889634073599246810018922);
#endif /* defined(__TI_COMPILER_VERSION__) */
short FNAME(Exp)(FTYPE *px, FTYPE y, short eoff)
	{	/* compute y*e^(*px)*2^eoff, (*px) finite, |y| not huge */
	if (*px < -hugexp || y == FLIT(0.0))
		{	/* certain underflow */
		*px = FLIT(0.0);
		return (0);
		}
	else if (hugexp < *px)
		{	/* certain overflow */
		*px = FCONST(Inf);
		return (_INFCODE);
		}
	else
		{	/* xexp won't overflow */
		FTYPE g = *px * invln2;
		short xexp = (short)(g + (g < FLIT(0.0) ? - FLIT(0.5) : FLIT(0.5)));

		g = xexp;
		g = (FTYPE)((*px - g * c1) - g * c2);
		if (-FCONST(Eps) < g && g < FCONST(Eps))
			*px = y;
		else
			{	/* g*g worth computing */
			const FTYPE z = g * g;
			const FCOMP ch = z * CPOLY(z);
			const FCOMP sh = g * SPOLY(z);

			*px = (FLIT(1.0) + (ch + sh)) / (FLIT(1.0) + (ch - sh)) * y;
			}
		return (FNAME(Dscale)(px, (long)xexp + eoff));
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
