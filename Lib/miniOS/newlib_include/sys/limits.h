/*****************************************************************************/
/* limits.h   v7.3.4                                                         */
/*                                                                           */
/* Copyright (c) 1993-2012 Texas Instruments Incorporated                    */
/* http://www.ti.com/                                                        */
/*                                                                           */
/*  Redistribution and  use in source  and binary forms, with  or without    */
/*  modification,  are permitted provided  that the  following conditions    */
/*  are met:                                                                 */
/*                                                                           */
/*     Redistributions  of source  code must  retain the  above copyright    */
/*     notice, this list of conditions and the following disclaimer.         */
/*                                                                           */
/*     Redistributions in binary form  must reproduce the above copyright    */
/*     notice, this  list of conditions  and the following  disclaimer in    */
/*     the  documentation  and/or   other  materials  provided  with  the    */
/*     distribution.                                                         */
/*                                                                           */
/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
/*     of its  contributors may  be used to  endorse or  promote products    */
/*     derived  from   this  software  without   specific  prior  written    */
/*     permission.                                                           */
/*                                                                           */
/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* TMS320C6X machine limits                                                  */
/*****************************************************************************/

#ifndef _LIMITS
#define _LIMITS

#define CHAR_BIT                 8       /* NUMBER OF BITS IN TYPE CHAR      */
#define SCHAR_MIN             -128       /* MIN VALUE FOR SIGNED CHAR        */
#define SCHAR_MAX              127       /* MAX VALUE FOR SIGNED CHAR        */
#define UCHAR_MAX              255       /* MAX VALUE FOR UNSIGNED CHAR      */
#define CHAR_MIN         SCHAR_MIN       /* MIN VALUE FOR CHAR               */
#define CHAR_MAX         SCHAR_MAX       /* MAX VALUE FOR CHAR               */
//#define MB_LEN_MAX               1       /* MAX # BYTES IN MULTI-BYTE        */

#define SHRT_MIN            -32768       /* MIN VALUE FOR SHORT              */
#define SHRT_MAX             32767       /* MAX VALUE FOR SHORT              */
#define USHRT_MAX            65535       /* MAX VALUE FOR UNSIGNED SHORT     */

#define INT_MIN         (-INT_MAX-1)     /* MIN VALUE FOR INT                */
//#define INT_MAX         2147483647       /* MAX VALUE FOR INT                */
#define UINT_MAX        4294967295U      /* MAX VALUE FOR UNSIGNED INT       */

#ifdef __TI_32BIT_LONG__
#define LONG_MIN        (-LONG_MAX-1)    /* MIN VALUE FOR LONG               */
#define LONG_MAX         2147483647      /* MAX VALUE FOR LONG               */
#define ULONG_MAX       4294967295U      /* MAX VALUE FOR UNSIGNED LONG      */
#else
#define LONG_MIN        (-LONG_MAX-1)    /* MIN VALUE FOR LONG               */
#define LONG_MAX         549755813887    /* MAX VALUE FOR LONG               */
#define ULONG_MAX      1099511627775U    /* MAX VALUE FOR UNSIGNED LONG      */
#endif

#define INT40_T_MIN    (-INT40_T_MAX-1)  /* MIN VALUE FOR __INT40_T          */
#define INT40_T_MAX     549755813887I40  /* MAX VALUE FOR __INT40_T          */
#define UINT40_T_MAX  1099511627775UI40  /* MAX VALUE FOR UNSIGNED __INT40_T */

#define LLONG_MIN         (-LLONG_MAX-1) /* MIN VALUE FOR LONG LONG          */
#define LLONG_MAX    9223372036854775807 /* MAX VALUE FOR LONG LONG          */
#define ULLONG_MAX  18446744073709551615 /* MAX VALUE FOR UNSIGNED LONG LONG */

#ifndef NAME_MAX
#define NAME_MAX   255
#endif

#endif /* #ifdef _LIMITS */

/*
 *  This file lists the minimums for the limits set by each of
 *  the POSIX features subsets.
 *
 *  XXX: Careful attention needs to be paid to section 2.8 in 1003.1b-1993
 *       to segregrate the variables below based on their "class" according
 *       to our implementation.  We also need to set the Run-Time Invariant
 *       and other related values.
 *
 *  $Id$
 */

#ifndef __POSIX_LIMITS_h
#define __POSIX_LIMITS_h

/****************************************************************************
 ****************************************************************************
 *                                                                          *
 *         P1003.1b-1993 defines the constants below this comment.          *
 *                                                                          *
 ****************************************************************************
 ****************************************************************************/

#define _POSIX_AIO_LISTIO_MAX   2
#define _POSIX_AIO_MAX          1
#define _POSIX_ARG_MAX          4096
//#define _POSIX_CHILD_MAX        6
#define _POSIX_DELAYTIMER_MAX   32
#define _POSIX_HOST_NAME_MAX    255
#define _POSIX_LINK_MAX         8
#define _POSIX_MAX_CANON        255
#define _POSIX_MAX_INPUT        255
#define _POSIX_MQ_OPEN_MAX      8
#define _POSIX_MQ_PRIO_MAX      32
//#define _POSIX_NAME_MAX         255
//#define _POSIX_NGROUPS_MAX      0
//#define _POSIX_OPEN_MAX         16
//#define _POSIX_PATH_MAX         255
#define _POSIX_PIPE_BUF         512
/* The maximum number of repeated occurrences of a regular expression
 *  *    permitted when using the interval notation `\{M,N\}'.  */
#define _POSIX2_RE_DUP_MAX              255
#define _POSIX_RTSIG_MAX        8
#define _POSIX_SEM_NSEMS_MAX    256
#define _POSIX_SEM_VALUE_MAX    32767
#define _POSIX_SIGQUEUE_MAX     32
#define _POSIX_SSIZE_MAX        32767
#define _POSIX_STREAM_MAX       8
#define _POSIX_TIMER_MAX        32
//#define _POSIX_TZNAME_MAX       3

/*
 *  Definitions of the following may be omitted if the value is >= stated
 *  minimum but is indeterminate.
 */

#define AIO_LISTIO_MAX          2
#define AIO_MAX                 1
#define AIO_PRIO_DELTA_MAX      0
#define DELAYTIMER_MAX          32
#define MQ_OPEN_MAX             8
#define MQ_PRIO_MAX             32
#define PAGESIZE                (1<<12)
#define RTSIG_MAX               8
#define SEM_NSEMS_MAX           256
#define SEM_VALUE_MAX           32767
#define SIGQUEUE_MAX            32
#define STREAM_MAX              8
#define TIMER_MAX               32
#define TZNAME_MAX              3

/*
 *  Invariant values
 */

#ifdef __SIZE_MAX__
#define SSIZE_MAX		(__SIZE_MAX__ >> 1)
#elif defined(__SIZEOF_SIZE_T__) && defined(__CHAR_BIT__)
#define SSIZE_MAX               ((1UL << (__SIZEOF_SIZE_T__ * __CHAR_BIT__ - 1)) - 1)
#else /* historic fallback, wrong in most cases */
#define SSIZE_MAX               32767
#endif

/*
 *  Maximum Values
 */

//#define _POSIX_CLOCKRES_MIN      0   /* in nanoseconds */

/****************************************************************************
 ****************************************************************************
 *                                                                          *
 *         P1003.1c/D10 defines the constants below this comment.           *
 *
 *  XXX: doc seems to have printing problems in this table :(
 *                                                                          *
 ****************************************************************************
 ****************************************************************************/

#define _POSIX_LOGIN_NAME_MAX                9
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS  4
//#define _POSIX_THREAD_KEYS_MAX               28
#define _POSIX_THREAD_THREADS_MAX            64
#define _POSIX_TTY_NAME_MAX                  9

/*
 *  Definitions of the following may be omitted if the value is >= stated
 *  minimum but is indeterminate.
 *
 *  NOTE:  LOGIN_NAME_MAX is named LOGNAME_MAX under Solaris 2.x.  Perhaps
 *         the draft specification will be changing.  jrs 05/24/96
 */

#define LOGIN_NAME_MAX                      _POSIX_LOGIN_NAME_MAX
#define TTY_NAME_MAX                        _POSIX_TTY_NAME_MAX
#define PTHREAD_DESTRUCTOR_ITERATIONS       _POSIX_THREAD_DESTRUCTOR_ITERATIONS

/*
 *  RTEMS is smart enough to give us the minimum stack size if we ask
 *  for too little.  Because the real RTEMS limit for this is cpu dependent
 *  AND rtems header files are not installed yet, we cannot use the cpu
 *  dependent constant CPU_STACK_MINIMUM_SIZE.  Moreover, we do not want
 *  to duplicate that information here so we will just let RTEMS magically
 *  give us its minimum stack size.
 *
 *  NOTE:  The other alternative is to have this be a macro for a
 *         routine in RTEMS which returns the constant.
 */

#define PTHREAD_STACK_MIN                   0

/*
 *  The maximum number of keys (PTHREAD_KEYS_MAX) and threads
 *  (PTHREAD_THREADS_MAX) are configurable and may exceed the minimum.
 *
#define PTHREAD_KEYS_MAX                    _POSIX_THREAD_KEYS_MAX
#define PTHREAD_THREADS_MAX                 _POSIX_THREAD_THREADS_MAX
*/


/****************************************************************************
 ****************************************************************************
 *                                                                          *
 *         P1003.4b/D8 defines the constants below this comment.            *
 *                                                                          *
 ****************************************************************************
 ****************************************************************************/

#define _POSIX_INTERRUPT_OVERRUN_MAX        32

/*
 *  Definitions of the following may be omitted if the value is >= stated
 *  minimum but is indeterminate.
 */

#define INTERRUPT_OVERRUN_MAX               32

/*
 *  Pathname Variables
 */

#define MIN_ALLOC_SIZE
#define REC_MIN_XFER_SIZE
#define REC_MAX_XFER_SIZE
#define REC_INCR_XFER_SIZE
#define REC_XFER_ALIGN
#define MAX_ATOMIC_SIZE

#endif
/* end of include file */

