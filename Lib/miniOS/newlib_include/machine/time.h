#ifndef	_MACHTIME_H_
#define	_MACHTIME_H_

#if defined(__rtos__)
#define _CLOCKS_PER_SEC_  sysconf(_SC_CLK_TCK)
#else  /* !__rtos__ */
#if defined(__arm__) || defined(__thumb__)
#define _CLOCKS_PER_SEC_ 100
#endif
#endif /* !__rtos__ */

#endif	/* _MACHTIME_H_ */


