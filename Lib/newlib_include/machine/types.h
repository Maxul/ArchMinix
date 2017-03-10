#ifndef	_MACHTYPES_H_
#define	_MACHTYPES_H_

/*
 *  The following section is RTOS specific and is needed to more
 *  closely match the types defined in the BSD machine/types.h.
 *  This is needed to let the RTOS/BSD TCP/IP stack compile.
 */
#if defined(__rtos__)
#include <machine/_types.h>
#endif

#define	_CLOCK_T_	unsigned int		/* clock() */
#define	_TIME_T_	unsigned int		/* time() */
#define _CLOCKID_T_ 	unsigned int
#define _TIMER_T_   	unsigned int

#ifndef __SIZE_T_TYPE__
#define __SIZE_T_TYPE__	unsigned int
#endif

#ifndef __clock_t_defined
typedef _CLOCK_T_ clock_t;
#define __clock_t_defined
#endif

#ifndef __time_t_defined
typedef _TIME_T_ time_t;
#define __time_t_defined
#endif

#ifndef __clockid_t_defined
typedef _CLOCKID_T_  clockid_t;
#define __clockid_t_defined
#endif

#ifndef __timer_t_defined
typedef _TIMER_T_ timer_t;
#define __timer_t_defined
#endif

#ifndef _SIZE_T
#define __size_t_defined
#define _SIZE_T
typedef __SIZE_T_TYPE__ size_t;
#endif
/*
typedef unsigned int __uint32_t;
typedef unsigned long int  __uint64_t;
typedef unsigned long int __int64_t;
typedef unsigned long int  u_int64_t;
*/
typedef unsigned long useconds_t;
typedef long suseconds_t;
typedef	__int64_t	sbintime_t;

#ifndef _HAVE_SYSTYPES
typedef long int __off_t;
typedef int __pid_t;
#ifdef __GNUC__
__extension__ typedef long long int __loff_t;
#else
typedef long int __loff_t;
#endif
#endif

#endif	/* _MACHTYPES_H_ */


