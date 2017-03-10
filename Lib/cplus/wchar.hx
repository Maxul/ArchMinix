/* wchar.h minimal header for C++ */
#ifndef _WCHAR
#define _WCHAR
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <stddef.h>
#include <stdio.h>

_C_STD_BEGIN


		/* MACROS */
#ifndef WEOF
 #define WEOF	((wint_t)(-1))
#endif /* WEOF */

#ifndef WCHAR_MIN
 #define WCHAR_MIN 0
#endif

#ifndef WCHAR_MAX
#if !defined(__TI_WCHAR_T_BITS__) || __TI_WCHAR_T_BITS__ == 16
#define WCHAR_MAX 0xffffu
#else 
#define WCHAR_MAX 0xffffffffu
#endif
#endif

		/* TYPE DEFINITIONS */

 #ifndef _MBSTATET
  #define _MBSTATET
typedef int _Mbstatet;
 #endif /* _MBSTATET */

typedef _Mbstatet mbstate_t;
struct tm;

#ifndef _WINTT
#define _WINTT
#if defined (__TMS320C6X__) && defined(__TI_EABI__)
typedef unsigned int wint_t;
#else
typedef int wint_t;
#endif
#endif

_C_LIB_DECL
		/* FUNCTIONS */
wint_t btowc(int);
wint_t fgetwc(FILE *);
wint_t fputwc(wchar_t, FILE *);
size_t mbrtowc(wchar_t *, const char *, size_t, mbstate_t *);
wint_t ungetwc(wint_t, FILE *);
size_t wcslen(const wchar_t *);
size_t wcrtomb(char *, wchar_t, mbstate_t *);
int wctob(wint_t);

int wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);

int fwide(FILE *str, int mode);

wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);

_END_C_LIB_DECL

	/* inlines/macros */

_C_STD_END
#endif /* _WCHAR */

 #if defined(_STD_USING)
using _CSTD mbstate_t; using _CSTD tm; using _CSTD wint_t;

using _CSTD btowc; 
using _CSTD fgetwc; 
using _CSTD fputwc;
using _CSTD mbrtowc;
using _CSTD ungetwc;
using _CSTD wcslen;
using _CSTD wcrtomb; 
using _CSTD wctob;
using _CSTD wmemcmp; 
using _CSTD wmemcpy;
using _CSTD wmemmove; 
using _CSTD wmemset; 
using _CSTD wmemchr; 
using _CSTD fwide; 

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:1476 */
