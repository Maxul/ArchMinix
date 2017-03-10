/* xxstod.h -- _[W]Sto[d f ld] common functionality */

#define D16TO7	FLIT(268435456.0)	/* 16^7 */
#define D10TO9	FLIT(1e9)			/* 10^9 */
#define NLONG	((FBITS + 27) / 28)	/* 7 * NLONG == max hexadecimal digits */

/*
FTYPE _Stodx(const CTYPE *s, CTYPE **endptr, long pten, int *perr)
 */
	{	/* convert string to FTYPE, with checking */
	FTYPE x;
	long lo[NLONG + 1];
	const CTYPE *s0 = s;
	int code = CNAME(Stopfx)(&s, endptr);
	const int neg = code & FL_NEG;

	extern FTYPE FNAME(Dtentox)(FTYPE, long, int *);

	if (perr != 0)
		*perr = 0;
	if ((code &= ~FL_NEG) == FL_DEC)
		{	/* parse decimal format */
		const int nlo = CNAME(Stoflt)(s0, s, endptr, lo, NLONG);

		if (nlo == 0)
			x = 0;
		else

 #if NLONG == 1
			x = lo[1];

 #else /* NLONG == 1 */
			{	/* combine longs, base 10^9 */
			int i;
			static const FTYPE fac[] =
				{	/* scale factors */
				D10TO9,
				D10TO9 * D10TO9,

  #if 2 < NLONG
				D10TO9 * D10TO9 * D10TO9,

   #if 3 < NLONG
				D10TO9 * D10TO9 * D10TO9 * D10TO9,

    #if 4 < NLONG
				D10TO9 * D10TO9 * D10TO9 * D10TO9 * D10TO9,

     #if 5 < NLONG
      #error NLONG TOO BIG
     #endif /* 5 < NLONG */

    #endif /* 4 < NLONG */
   #endif /* 3 < NLONG */

  #endif /* 2 < NLONG */
				};

			for (i = nlo, x = lo[nlo]; 0 < --i; )
				if (lo[i] != 0)
					x += fac[nlo - i - 1] * lo[i];
			 }
 #endif /* NLONG == 1 */

		x = FNAME(Dtentox)(x, pten + lo[0], perr);
				}
	else if (code == FL_HEX)
		{	/* parse hexadecimal format */
		const int nlo = CNAME(Stoxflt)(s0, s, endptr, lo, NLONG);

		if (nlo == 0)
			x = 0;
		else

 #if NLONG == 1
			x = lo[1];

 #else /* NLONG == 1 */
			{	/* combine longs, base 16^7 */
			int i;
			static const FTYPE fac[] =
				{	/* scale factors */
				D16TO7,
				D16TO7 * D16TO7,

  #if 2 < NLONG
				D16TO7 * D16TO7 * D16TO7,

   #if 3 < NLONG
				D16TO7 * D16TO7 * D16TO7 * D16TO7,

    #if 4 < NLONG
				D16TO7 * D16TO7 * D16TO7 * D16TO7 * D16TO7,

     #if 5 < NLONG
      #error NLONG TOO BIG
     #endif /* 5 < NLONG */

    #endif /* 4 < NLONG */
   #endif /* 3 < NLONG */

  #endif /* 2 < NLONG */
				};

			for (i = nlo, x = lo[nlo]; 0 < --i; )
				if (lo[i] != 0)
					x += fac[nlo - i - 1] * lo[i];
			 }
 #endif /* NLONG == 1 */

		FNAME(Dscale)(&x, lo[0]);
		x = FNAME(Dtentox)(x, pten, perr);
				}
	else if (code == FL_INF)
		x = FCONST(Inf);
	else if (code == FL_NAN)
		x = FCONST(Nan);
	else
		x = 0;	/* code == FL_ERR */
	return (neg != 0 ? -x : x);
			}

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
