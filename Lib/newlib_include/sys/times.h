#ifndef	_SYS_TIMES_H
#ifdef __cplusplus
extern "C" {
#endif
#define	_SYS_TIMES_H

#include <_ansi.h>
#include <machine/types.h>

/*  Get Process Times, P1003.1b-1993, p. 92 */
struct tms
{
    clock_t	tms_utime;		/* user time */
    clock_t	tms_stime;		/* system time */
    clock_t	tms_cutime;		/* user time, children */
    clock_t	tms_cstime;		/* system time, children */
};

clock_t _EXFUN(times, (struct tms *));

#ifdef __cplusplus
}
#endif
#endif	/* !_SYS_TIMES_H */
