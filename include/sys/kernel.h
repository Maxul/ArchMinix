/*-
 * Copyright (c) 1995 Terrence R. Lambert
 * All rights reserved.
 *
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)kernel.h	8.3 (Berkeley) 1/21/94
 */

#ifndef _SYS_KERNEL_H_
#define _SYS_KERNEL_H_

#include <sys/linker_set.h>

#ifdef _KERNEL

/* Global variables for the kernel. */

/* 1.1 */
extern long hostid;
extern char hostname[MAXHOSTNAMELEN];
extern char domainname[MAXHOSTNAMELEN];
extern char kernelname[MAXPATHLEN];

/* 1.2 */
extern volatile struct timeval mono_time;
extern struct timeval boottime;
extern struct timeval runtime;
/* extern volatile struct timeval time; */
extern struct timezone tz;			/* XXX */

extern int tick;			/* usec per tick (1000000 / hz) */
extern int hz;				/* system clock's frequency */
extern int psratio;			/* ratio: prof / stat */
extern int stathz;			/* statistics clock's frequency */
extern int profhz;			/* profiling clock's frequency */
extern int ticks;

#endif /* _KERNEL */

/*
 * Enumerated types for known system startup interfaces.
 *
 * Startup occurs in ascending numeric order; the list entries are
 * sorted prior to attempting startup to guarantee order.  Items
 * of the same level are arbitrated for order based on the 'order'
 * element.
 *
 * These numbers are arbitrary and are chosen ONLY for ordering; the
 * enumeration values are explicit rather than implicit to provide
 * for binary compatibility with inserted elements.
 *
 * The SI_SUB_RUN_SCHEDULER value must have the highest lexical value.
 *
 * The SI_SUB_CONSOLE and SI_SUB_SWAP values represent values used by
 * the BSD 4.4Lite but not by FreeBSD; they are maintained in dependent
 * order to support porting.
 *
 * The SI_SUB_PROTO_BEGIN and SI_SUB_PROTO_END bracket a range of
 * initializations to take place at splimp().  This is a historical
 * wart that should be removed -- probably running everything at
 * splimp() until the first init that doesn't want it is the correct
 * fix.  They are currently present to ensure historical behavior.
 */
enum sysinit_sub_id {
	SI_SUB_DUMMY		= 0x00000000,	/* not executed; for linker*/
	SI_SUB_CONSOLE		= 0x08000000,	/* console*/
	SI_SUB_COPYRIGHT	= 0x08000001,	/* first use of console*/
	SI_SUB_VM		= 0x10000000,	/* virtual memory system init*/
	SI_SUB_KMEM		= 0x18000000,	/* kernel memory*/
	SI_SUB_CPU		= 0x20000000,	/* CPU resource(s)*/
	SI_SUB_DEVFS		= 0x22000000,	/* get DEVFS ready */
	SI_SUB_DRIVERS		= 0x23000000,	/* Let Drivers initialize */
	SI_SUB_CONFIGURE	= 0x24000000,	/* Configure devices */
	SI_SUB_INTRINSIC	= 0x28000000,	/* proc 0*/
	SI_SUB_RUN_QUEUE	= 0x30000000,	/* the run queue*/
	SI_SUB_VM_CONF		= 0x38000000,	/* config VM, set limits*/
	SI_SUB_VFS		= 0x40000000,	/* virtual file system*/
	SI_SUB_CLOCKS		= 0x48000000,	/* real time and stat clocks*/
	SI_SUB_MBUF		= 0x50000000,	/* mbufs*/
	SI_SUB_CLIST		= 0x58000000,	/* clists*/
	SI_SUB_SYSV_SHM		= 0x64000000,	/* System V shared memory*/
	SI_SUB_SYSV_SEM		= 0x68000000,	/* System V semaphores*/
	SI_SUB_SYSV_MSG		= 0x6C000000,	/* System V message queues*/
	SI_SUB_PSEUDO		= 0x70000000,	/* pseudo devices*/
	SI_SUB_PROTO_BEGIN	= 0x80000000,	/* XXX: set splimp (kludge)*/
	SI_SUB_PROTO_IF		= 0x84000000,	/* interfaces*/
	SI_SUB_PROTO_DOMAIN	= 0x88000000,	/* domains (address families?)*/
	SI_SUB_PROTO_END	= 0x8fffffff,	/* XXX: set splx (kludge)*/
	SI_SUB_KPROF		= 0x90000000,	/* kernel profiling*/
	SI_SUB_KICK_SCHEDULER	= 0xa0000000,	/* start the timeout events*/
	SI_SUB_ROOT		= 0xb0000000,	/* root mount*/
	SI_SUB_ROOT_FDTAB	= 0xb8000000,	/* root vnode in fd table...*/
	SI_SUB_SWAP		= 0xc0000000,	/* swap*/
	SI_SUB_INTRINSIC_POST	= 0xd0000000,	/* proc 0 cleanup*/
	SI_SUB_KTHREAD_INIT	= 0xe0000000,	/* init process*/
	SI_SUB_KTHREAD_PAGE	= 0xe4000000,	/* pageout daemon*/
	SI_SUB_KTHREAD_VM	= 0xe8000000,	/* vm daemon*/
	SI_SUB_KTHREAD_UPDATE	= 0xec000000,	/* update daemon*/
	SI_SUB_RUN_SCHEDULER	= 0xffffffff	/* scheduler: no return*/
};


/*
 * Some enumerated orders; "ANY" sorts last.
 */
enum sysinit_elem_order {
	SI_ORDER_FIRST		= 0x00000000,	/* first*/
	SI_ORDER_SECOND		= 0x00000001,	/* second*/
	SI_ORDER_THIRD		= 0x00000002,	/* third*/
	SI_ORDER_MIDDLE		= 0x10000000,	/* somewhere in the middle */
	SI_ORDER_ANY		= 0xffffffff	/* last*/
};


/*
 * A system initialization call instance
 *
 * The subsystem
 */
struct sysinit {
	unsigned int	subsystem;		/* subsystem identifier*/
	unsigned int	order;			/* init order within subsystem*/
	void		(*func)(void *);	/* init function*/
	void		*udata;			/* multiplexer/argument */
};


/*
 * Default: no special processing
 */
#define	SYSINIT(uniquifier, subsystem, order, func, ident)	

/*
 * Call 'fork()' before calling '(*func)(ident)';
 * for making a kernel 'thread' (or builtin process.)
 */
#define	SYSINIT_KT(uniquifier, subsystem, order, func, ident)


/*
 * A kernel process descriptor; used to start "internal" daemons
 *
 * Note: global_procpp may be NULL for no global save area
 */
struct kproc_desc {
	char		*arg0;			/* arg 0 (for 'ps' listing)*/
	void		(*func)(void);	/* "main" for kernel process*/
	struct proc	**global_procpp;	/* ptr to proc ptr save area*/
};

void	kproc_start(void *udata);

#endif /* !_SYS_KERNEL_H_*/
