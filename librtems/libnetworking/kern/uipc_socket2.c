/*
 * This file has undergone several changes to reflect the
 * differences between the RTEMS and FreeBSD kernels.
 */

/*
 * Copyright (c) 1982, 1986, 1988, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)uipc_socket2.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/queue.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/signalvar.h>
#include <sys/sysctl.h>

/*
 * Primitive routines for operating on sockets and socket buffers
 */

u_long	sb_max = SB_MAX;		/* XXX should be static */
SYSCTL_INT(_kern, KIPC_MAXSOCKBUF, maxsockbuf, CTLFLAG_RW, &sb_max, 0, "");

static	u_long sb_efficiency = 8;	/* parameter for sbreserve() */
SYSCTL_INT(_kern, OID_AUTO, sockbuf_waste_factor, CTLFLAG_RW, &sb_efficiency,
	   0, "");

#if defined(__rtems__)
  void rtems_set_sb_efficiency(
    u_long efficiency
  )
  {
    sb_efficiency = (efficiency == 0) ? 2 : efficiency;
  }
#endif

/*
 * Procedures to manipulate state flags of socket
 * and do appropriate wakeups.  Normal sequence from the
 * active (originating) side is that soisconnecting() is
 * called during processing of connect() call,
 * resulting in an eventual call to soisconnected() if/when the
 * connection is established.  When the connection is torn down
 * soisdisconnecting() is called during processing of disconnect() call,
 * and soisdisconnected() is called when the connection to the peer
 * is totally severed.  The semantics of these routines are such that
 * connectionless protocols can call soisconnected() and soisdisconnected()
 * only, bypassing the in-progress calls when setting up a ``connection''
 * takes no time.
 *
 * From the passive side, a socket is created with
 * two queues of sockets: so_q0 for connections in progress
 * and so_q for connections already made and awaiting user acceptance.
 * As a protocol is preparing incoming connections, it creates a socket
 * structure queued on so_q0 by calling sonewconn().  When the connection
 * is established, soisconnected() is called, and transfers the
 * socket structure to so_q, making it available to accept().
 *
 * If a socket is closed with sockets on either
 * so_q0 or so_q, these sockets are dropped.
 *
 * If higher level protocols are implemented in
 * the kernel, the wakeups done here will sometimes
 * cause software-interrupt process scheduling.
 */

void
soisconnecting(struct socket *so)
{

	so->so_state &= ~(SS_ISCONNECTED|SS_ISDISCONNECTING);
	so->so_state |= SS_ISCONNECTING;
}

void
soisconnected(struct socket *so)
{
	register struct socket *head = so->so_head;

	so->so_state &= ~(SS_ISCONNECTING|SS_ISDISCONNECTING|SS_ISCONFIRMING);
	so->so_state |= SS_ISCONNECTED;
	if (head && (so->so_state & SS_INCOMP)) {
		TAILQ_REMOVE(&head->so_incomp, so, so_list);
		head->so_incqlen--;
		so->so_state &= ~SS_INCOMP;
		TAILQ_INSERT_TAIL(&head->so_comp, so, so_list);
		so->so_state |= SS_COMP;
		sorwakeup(head);
		soconnwakeup(head);
	} else {
		soconnwakeup(so);
		sorwakeup(so);
		sowwakeup(so);
	}
}

void
soisdisconnecting(struct socket *so)
{

	so->so_state &= ~SS_ISCONNECTING;
	so->so_state |= (SS_ISDISCONNECTING|SS_CANTRCVMORE|SS_CANTSENDMORE);
	soconnwakeup(so);
	sowwakeup(so);
	sorwakeup(so);
}

void
soisdisconnected(struct socket *so)
{

	so->so_state &= ~(SS_ISCONNECTING|SS_ISCONNECTED|SS_ISDISCONNECTING);
	so->so_state |= (SS_CANTRCVMORE|SS_CANTSENDMORE);
	soconnwakeup(so);
	sowwakeup(so);
	sorwakeup(so);
}

/*
 * Return a random connection that hasn't been serviced yet and
 * is eligible for discard.  There is a one in qlen chance that
 * we will return a null, saying that there are no dropable
 * requests.  In this case, the protocol specific code should drop
 * the new request.  This insures fairness.
 *
 * This may be used in conjunction with protocol specific queue
 * congestion routines.
 */
struct socket *
sodropablereq(struct socket *head)
{
	register struct socket *so;
	uint32_t i, j, qlen, m;

	static int rnd;
	static long old_mono_secs;
	static unsigned int cur_cnt, old_cnt;

	if ((i = (m = rtems_bsdnet_seconds_since_boot()) - old_mono_secs) != 0) {
		old_mono_secs = m;
		old_cnt = cur_cnt / i;
		cur_cnt = 0;
	}

	so = TAILQ_FIRST(&head->so_incomp);
	if (!so)
		return (so);

	qlen = head->so_incqlen;
	if (++cur_cnt > qlen || old_cnt > qlen) {
		rnd = (314159 * rnd + 66329) & 0xffff;
		j = ((qlen + 1) * rnd) >> 16;

		while (j-- && so)
		    so = TAILQ_NEXT(so, so_list);
	}

	return (so);
}

/*
 * When an attempt at a new connection is noted on a socket
 * which accepts connections, sonewconn is called.  If the
 * connection is possible (subject to space constraints, etc.)
 * then we allocate a new structure, propoerly linked into the
 * data structure of the original socket, and return this.
 * Connstatus may be 0, or SO_ISCONFIRMING, or SO_ISCONNECTED.
 *
 * Currently, sonewconn() is defined as sonewconn1() in socketvar.h
 * to catch calls that are missing the (new) second parameter.
 */
struct socket *
sonewconn1(struct socket *head, int connstatus)
{
	register struct socket *so;

	if (head->so_qlen > 3 * head->so_qlimit / 2)
		return ((struct socket *)0);
	MALLOC(so, struct socket *, sizeof(*so), M_SOCKET, M_DONTWAIT);
	if (so == NULL)
		return ((struct socket *)0);
	bzero((caddr_t)so, sizeof(*so));
	so->so_head = head;
	so->so_type = head->so_type;
	so->so_options = head->so_options &~ SO_ACCEPTCONN;
	so->so_linger = head->so_linger;
	so->so_state = head->so_state | SS_NOFDREF;
	so->so_proto = head->so_proto;
	so->so_timeo = head->so_timeo;
	(void) soreserve(so, head->so_snd.sb_hiwat, head->so_rcv.sb_hiwat);
	if (connstatus) {
		TAILQ_INSERT_TAIL(&head->so_comp, so, so_list);
		so->so_state |= SS_COMP;
	} else {
		TAILQ_INSERT_TAIL(&head->so_incomp, so, so_list);
		so->so_state |= SS_INCOMP;
		head->so_incqlen++;
	}
	head->so_qlen++;
	if ((*so->so_proto->pr_usrreqs->pru_attach)(so, 0)) {
		if (so->so_state & SS_COMP) {
			TAILQ_REMOVE(&head->so_comp, so, so_list);
		} else {
			TAILQ_REMOVE(&head->so_incomp, so, so_list);
			head->so_incqlen--;
		}
		head->so_qlen--;
		(void) free((caddr_t)so, M_SOCKET);
		return ((struct socket *)0);
	}
	if (connstatus) {
		sorwakeup(head);
		soconnwakeup(head);
		so->so_state |= connstatus;
	}
	return (so);
}

/*
 * Socantsendmore indicates that no more data will be sent on the
 * socket; it would normally be applied to a socket when the user
 * informs the system that no more data is to be sent, by the protocol
 * code (in case PRU_SHUTDOWN).  Socantrcvmore indicates that no more data
 * will be received, and will normally be applied to the socket by a
 * protocol when it detects that the peer will send no more data.
 * Data queued for reading in the socket may yet be read.
 */

void
socantsendmore(struct socket *so)
{

	so->so_state |= SS_CANTSENDMORE;
	sowwakeup(so);
}

void
socantrcvmore(struct socket *so)
{

	so->so_state |= SS_CANTRCVMORE;
	sorwakeup(so);
}

/*
 * Socket buffer (struct sockbuf) utility routines.
 *
 * Each socket contains two socket buffers: one for sending data and
 * one for receiving data.  Each buffer contains a queue of mbufs,
 * information about the number of mbufs and amount of data in the
 * queue, and other fields allowing select() statements and notification
 * on data availability to be implemented.
 *
 * Data stored in a socket buffer is maintained as a list of records.
 * Each record is a list of mbufs chained together with the m_next
 * field.  Records are chained together with the m_nextpkt field. The upper
 * level routine soreceive() expects the following conventions to be
 * observed when placing information in the receive buffer:
 *
 * 1. If the protocol requires each message be preceded by the sender's
 *    name, then a record containing that name must be present before
 *    any associated data (mbuf's must be of type MT_SONAME).
 * 2. If the protocol supports the exchange of ``access rights'' (really
 *    just additional data associated with the message), and there are
 *    ``rights'' to be received, then a record containing this data
 *    should be present (mbuf's must be of type MT_RIGHTS).
 * 3. If a name or rights record exists, then it must be followed by
 *    a data record, perhaps of zero length.
 *
 * Before using a new socket structure it is first necessary to reserve
 * buffer space to the socket, by calling sbreserve().  This should commit
 * some of the available buffer space in the system buffer pool for the
 * socket (currently, it does nothing but enforce limits).  The space
 * should be released by calling sbrelease() when the socket is destroyed.
 */

int
soreserve(struct socket *so, u_long sndcc, u_long rcvcc)
{

	if (sbreserve(&so->so_snd, sndcc) == 0)
		goto bad;
	if (sbreserve(&so->so_rcv, rcvcc) == 0)
		goto bad2;
	if (so->so_rcv.sb_lowat == 0)
		so->so_rcv.sb_lowat = 1;
	if (so->so_snd.sb_lowat == 0)
		so->so_snd.sb_lowat = MCLBYTES;
	if (so->so_snd.sb_lowat > so->so_snd.sb_hiwat)
		so->so_snd.sb_lowat = so->so_snd.sb_hiwat;
	return (0);
bad2:
	sbrelease(&so->so_snd);
bad:
	return (ENOBUFS);
}

/*
 * Allot mbufs to a sockbuf.
 * Attempt to scale mbmax so that mbcnt doesn't become limiting
 * if buffering efficiency is near the normal case.
 */
int
sbreserve(struct sockbuf *sb, u_long cc)
{

	if (cc > sb_max * MCLBYTES / (MSIZE + MCLBYTES))
		return (0);
	sb->sb_hiwat = cc;
	sb->sb_mbmax = min(cc * sb_efficiency, sb_max);
	if (sb->sb_lowat > sb->sb_hiwat)
		sb->sb_lowat = sb->sb_hiwat;
	return (1);
}

/*
 * Free mbufs held by a socket, and reserved mbuf space.
 */
void
sbrelease(struct sockbuf *sb)
{

	sbflush(sb);
	sb->sb_hiwat = sb->sb_mbmax = 0;
}

/*
 * Routines to add and remove
 * data from an mbuf queue.
 *
 * The routines sbappend() or sbappendrecord() are normally called to
 * append new mbufs to a socket buffer, after checking that adequate
 * space is available, comparing the function sbspace() with the amount
 * of data to be added.  sbappendrecord() differs from sbappend() in
 * that data supplied is treated as the beginning of a new record.
 * To place a sender's address, optional access rights, and data in a
 * socket receive buffer, sbappendaddr() should be used.  To place
 * access rights and data in a socket receive buffer, sbappendrights()
 * should be used.  In either case, the new data begins a new record.
 * Note that unlike sbappend() and sbappendrecord(), these routines check
 * for the caller that there will be enough space to store the data.
 * Each fails if there is not enough space, or if it cannot find mbufs
 * to store additional information in.
 *
 * Reliable protocols may use the socket send buffer to hold data
 * awaiting acknowledgement.  Data is normally copied from a socket
 * send buffer in a protocol with m_copy for output to a peer,
 * and then removing the data from the socket buffer with sbdrop()
 * or sbdroprecord() when the data is acknowledged by the peer.
 */

/*
 * Append mbuf chain m to the last record in the
 * socket buffer sb.  The additional space associated
 * the mbuf chain is recorded in sb.  Empty mbufs are
 * discarded and mbufs are compacted where possible.
 */
void
sbappend(struct sockbuf *sb, struct mbuf *m)
{
	register struct mbuf *n;

	if (m == 0)
		return;
	n = sb->sb_mb;
	if (n) {
		while (n->m_nextpkt)
			n = n->m_nextpkt;
		do {
			if (n->m_flags & M_EOR) {
				sbappendrecord(sb, m); /* XXXXXX!!!! */
				return;
			}
		} while (n->m_next && (n = n->m_next));
	}
	sbcompress(sb, m, n);
}

#ifdef SOCKBUF_DEBUG
void
sbcheck(struct sockbuf *sb)
{
	register struct mbuf *m;
	register int len = 0, mbcnt = 0;

	for (m = sb->sb_mb; m; m = m->m_next) {
		len += m->m_len;
		mbcnt += MSIZE;
		if (m->m_flags & M_EXT) /*XXX*/ /* pretty sure this is bogus */
			mbcnt += m->m_ext.ext_size;
		if (m->m_nextpkt)
			panic("sbcheck nextpkt");
	}
	if (len != sb->sb_cc || mbcnt != sb->sb_mbcnt) {
		printf("cc %d != %d || mbcnt %d != %d\n", len, sb->sb_cc,
		    mbcnt, sb->sb_mbcnt);
		panic("sbcheck");
	}
}
#endif

/*
 * As above, except the mbuf chain
 * begins a new record.
 */
void
sbappendrecord(struct sockbuf *sb, struct mbuf *m0)
{
	register struct mbuf *m;

	if (m0 == 0)
		return;
	m = sb->sb_mb;
	if (m)
		while (m->m_nextpkt)
			m = m->m_nextpkt;
	/*
	 * Put the first mbuf on the queue.
	 * Note this permits zero length records.
	 */
	sballoc(sb, m0);
	if (m)
		m->m_nextpkt = m0;
	else
		sb->sb_mb = m0;
	m = m0->m_next;
	m0->m_next = 0;
	if (m && (m0->m_flags & M_EOR)) {
		m0->m_flags &= ~M_EOR;
		m->m_flags |= M_EOR;
	}
	sbcompress(sb, m, m0);
}

/*
 * As above except that OOB data
 * is inserted at the beginning of the sockbuf,
 * but after any other OOB data.
 */
void
sbinsertoob(struct sockbuf *sb, struct mbuf *m0)
{
	register struct mbuf *m;
	register struct mbuf **mp;

	if (m0 == 0)
		return;
	for (mp = &sb->sb_mb; *mp ; mp = &((*mp)->m_nextpkt)) {
	    m = *mp;
	    again:
		switch (m->m_type) {

		case MT_OOBDATA:
			continue;		/* WANT next train */

		case MT_CONTROL:
			m = m->m_next;
			if (m)
				goto again;	/* inspect THIS train further */
		}
		break;
	}
	/*
	 * Put the first mbuf on the queue.
	 * Note this permits zero length records.
	 */
	sballoc(sb, m0);
	m0->m_nextpkt = *mp;
	*mp = m0;
	m = m0->m_next;
	m0->m_next = 0;
	if (m && (m0->m_flags & M_EOR)) {
		m0->m_flags &= ~M_EOR;
		m->m_flags |= M_EOR;
	}
	sbcompress(sb, m, m0);
}

/*
 * Append address and data, and optionally, control (ancillary) data
 * to the receive queue of a socket.  If present,
 * m0 must include a packet header with total length.
 * Returns 0 if no space in sockbuf or insufficient mbufs.
 */
int
sbappendaddr(struct sockbuf *sb, struct sockaddr *asa,
    struct mbuf *m0, struct mbuf *control)
{
	register struct mbuf *m, *n;
	int space = asa->sa_len;

if (m0 && (m0->m_flags & M_PKTHDR) == 0)
panic("sbappendaddr");
	if (m0)
		space += m0->m_pkthdr.len;
	for (n = control; n; n = n->m_next) {
		space += n->m_len;
		if (n->m_next == 0)	/* keep pointer to last control buf */
			break;
	}
	if (space > sbspace(sb))
		return (0);
	if (asa->sa_len > MLEN)
		return (0);
	MGET(m, M_DONTWAIT, MT_SONAME);
	if (m == 0)
		return (0);
	m->m_len = asa->sa_len;
	bcopy((caddr_t)asa, mtod(m, caddr_t), asa->sa_len);
	if (n)
		n->m_next = m0;		/* concatenate data to control */
	else
		control = m0;
	m->m_next = control;
	for (n = m; n; n = n->m_next)
		sballoc(sb, n);
	n = sb->sb_mb;
	if (n) {
		while (n->m_nextpkt)
			n = n->m_nextpkt;
		n->m_nextpkt = m;
	} else
		sb->sb_mb = m;
	return (1);
}

int
sbappendcontrol(struct sockbuf *sb, struct mbuf *m0,
    struct mbuf *control)
{
	register struct mbuf *m, *n;
	int space = 0;

	if (control == 0)
		panic("sbappendcontrol");
	for (m = control; ; m = m->m_next) {
		space += m->m_len;
		if (m->m_next == 0)
			break;
	}
	n = m;			/* save pointer to last control buffer */
	for (m = m0; m; m = m->m_next)
		space += m->m_len;
	if (space > sbspace(sb))
		return (0);
	n->m_next = m0;			/* concatenate data to control */
	for (m = control; m; m = m->m_next)
		sballoc(sb, m);
	n = sb->sb_mb;
	if (n) {
		while (n->m_nextpkt)
			n = n->m_nextpkt;
		n->m_nextpkt = control;
	} else
		sb->sb_mb = control;
	return (1);
}

/*
 * Compress mbuf chain m into the socket
 * buffer sb following mbuf n.  If n
 * is null, the buffer is presumed empty.
 */
void
sbcompress(struct sockbuf *sb, struct mbuf *m, struct mbuf *n)
{
	register int eor = 0;
	register struct mbuf *o;

	while (m) {
		eor |= m->m_flags & M_EOR;
		if (m->m_len == 0 &&
		    (eor == 0 ||
		     (((o = m->m_next) || (o = n)) &&
		      o->m_type == m->m_type))) {
			m = m_free(m);
			continue;
		}
		if (n && (n->m_flags & (M_EXT | M_EOR)) == 0 &&
		    (n->m_data + n->m_len + m->m_len) < &n->m_dat[MLEN] &&
		    n->m_type == m->m_type) {
			bcopy(mtod(m, caddr_t), mtod(n, caddr_t) + n->m_len,
			    (unsigned)m->m_len);
			n->m_len += m->m_len;
			sb->sb_cc += m->m_len;
			m = m_free(m);
			continue;
		}
		if (n)
			n->m_next = m;
		else
			sb->sb_mb = m;
		sballoc(sb, m);
		n = m;
		m->m_flags &= ~M_EOR;
		m = m->m_next;
		n->m_next = 0;
	}
	if (eor) {
		if (n)
			n->m_flags |= eor;
		else
			printf("semi-panic: sbcompress\n");
	}
}

/*
 * Free all mbufs in a sockbuf.
 * Check that all resources are reclaimed.
 */
void
sbflush(struct sockbuf *sb)
{

	if (sb->sb_flags & SB_LOCK)
		panic("sbflush");
	while (sb->sb_mbcnt)
		sbdrop(sb, (int)sb->sb_cc);
	if (sb->sb_cc || sb->sb_mb)
		panic("sbflush 2");
}

/*
 * Drop data from (the front of) a sockbuf.
 */
void
sbdrop(struct sockbuf *sb, int len)
{
	register struct mbuf *m, *mn;
	struct mbuf *next;

	next = (m = sb->sb_mb) ? m->m_nextpkt : 0;
	while (len > 0) {
		if (m == 0) {
			if (next == 0)
				panic("sbdrop");
			m = next;
			next = m->m_nextpkt;
			continue;
		}
		if (m->m_len > len) {
			m->m_len -= len;
			m->m_data += len;
			sb->sb_cc -= len;
			break;
		}
		len -= m->m_len;
		sbfree(sb, m);
		MFREE(m, mn);
		m = mn;
	}
	while (m && m->m_len == 0) {
		sbfree(sb, m);
		MFREE(m, mn);
		m = mn;
	}
	if (m) {
		sb->sb_mb = m;
		m->m_nextpkt = next;
	} else
		sb->sb_mb = next;
}

/*
 * Drop a record off the front of a sockbuf
 * and move the next record to the front.
 */
void
sbdroprecord(struct sockbuf *sb)
{
	register struct mbuf *m, *mn;

	m = sb->sb_mb;
	if (m) {
		sb->sb_mb = m->m_nextpkt;
		do {
			sbfree(sb, m);
			MFREE(m, mn);
			m = mn;
		} while (m);
	}
}

/*
 * Create a "control" mbuf containing the specified data
 * with the specified type for presentation on a socket buffer.
 */
struct mbuf *
sbcreatecontrol(caddr_t p, int size, int type, int level)
{
	register struct cmsghdr *cp;
	struct mbuf *m;

	if ((m = m_get(M_DONTWAIT, MT_CONTROL)) == NULL)
		return ((struct mbuf *) NULL);
	cp = mtod(m, struct cmsghdr *);
	/* XXX check size? */
	(void)memcpy(CMSG_DATA(cp), p, size);
	size += sizeof(*cp);
	m->m_len = size;
	cp->cmsg_len = size;
	cp->cmsg_level = level;
	cp->cmsg_type = type;
	return (m);
}

#ifdef PRU_OLDSTYLE
/*
 * The following routines mediate between the old-style `pr_usrreq'
 * protocol implementations and the new-style `struct pr_usrreqs'
 * calling convention.
 */

/* syntactic sugar */
#define	nomb	(struct mbuf *)0

static int
old_abort(struct socket *so)
{
	return so->so_proto->pr_ousrreq(so, PRU_ABORT, nomb, nomb, nomb);
}

static int
old_accept(struct socket *so, struct mbuf *nam)
{
	return so->so_proto->pr_ousrreq(so, PRU_ACCEPT, nomb,  nam, nomb);
}

static int
old_attach(struct socket *so, intptr_t proto)
{
	return so->so_proto->pr_ousrreq(so, PRU_ATTACH, nomb,
				       (struct mbuf *)proto, /* XXX */
				       nomb);
}

static int
old_bind(struct socket *so, struct mbuf *nam)
{
	return so->so_proto->pr_ousrreq(so, PRU_BIND, nomb, nam, nomb);
}

static int
old_connect(struct socket *so, struct mbuf *nam)
{
	return so->so_proto->pr_ousrreq(so, PRU_CONNECT, nomb, nam, nomb);
}

static int
old_connect2(struct socket *so1, struct socket *so2)
{
	return so1->so_proto->pr_ousrreq(so1, PRU_CONNECT2, nomb, 
				       (struct mbuf *)so2, nomb);
}

static int
old_control(struct socket *so, intptr_t cmd, caddr_t data, struct ifnet *ifp)
{
	return so->so_proto->pr_ousrreq(so, PRU_CONTROL, (struct mbuf *)cmd, 
				       (struct mbuf *)data, 
				       (struct mbuf *)ifp);
}

static int
old_detach(struct socket *so)
{
	return so->so_proto->pr_ousrreq(so, PRU_DETACH, nomb, nomb, nomb);
}

static int
old_disconnect(struct socket *so)
{
	return so->so_proto->pr_ousrreq(so, PRU_DISCONNECT, nomb, nomb, nomb);
}

static int
old_listen(struct socket *so)
{
	return so->so_proto->pr_ousrreq(so, PRU_LISTEN, nomb, nomb, nomb);
}

static int
old_peeraddr(struct socket *so, struct mbuf *nam)
{
	return so->so_proto->pr_ousrreq(so, PRU_PEERADDR, nomb, nam, nomb);
}

static int
old_rcvd(struct socket *so, intptr_t flags)
{
	return so->so_proto->pr_ousrreq(so, PRU_RCVD, nomb,
				       (struct mbuf *)flags, /* XXX */
				       nomb);
}

static int
old_rcvoob(struct socket *so, struct mbuf *m, intptr_t flags)
{
	return so->so_proto->pr_ousrreq(so, PRU_RCVOOB, m,
				       (struct mbuf *)flags, /* XXX */
				       nomb);
}

static int
old_send(struct socket *so, int flags, struct mbuf *m, struct mbuf *addr,
	 struct mbuf *control)
{
	int req;

	if (flags & PRUS_OOB) {
		req = PRU_SENDOOB;
	} else if(flags & PRUS_EOF) {
		req = PRU_SEND_EOF;
	} else {
		req = PRU_SEND;
	}
	return so->so_proto->pr_ousrreq(so, req, m, addr, control);
}

static int
old_sense(struct socket *so, struct stat *sb)
{
	return so->so_proto->pr_ousrreq(so, PRU_SENSE, (struct mbuf *)sb,
				       nomb, nomb);
}

static int
old_shutdown(struct socket *so)
{
	return so->so_proto->pr_ousrreq(so, PRU_SHUTDOWN, nomb, nomb, nomb);
}

static int
old_sockaddr(struct socket *so, struct mbuf *nam)
{
	return so->so_proto->pr_ousrreq(so, PRU_SOCKADDR, nomb, nam, nomb);
}

struct pr_usrreqs pru_oldstyle = {
	old_abort, old_accept, old_attach, old_bind, old_connect,
	old_connect2, old_control, old_detach, old_disconnect,
	old_listen, old_peeraddr, old_rcvd, old_rcvoob, old_send,
	old_sense, old_shutdown, old_sockaddr
};

#endif /* PRU_OLDSTYLE */

/*
 * Some routines that return EOPNOTSUPP for entry points that are not
 * supported by a protocol.  Fill in as needed.
 */
int
pru_accept_notsupp(struct socket *so, struct mbuf *nam)
{
	return EOPNOTSUPP;
}

int
pru_connect2_notsupp(struct socket *so1, struct socket *so2)
{
	return EOPNOTSUPP;
}

int
pru_control_notsupp(struct socket *so, int cmd, caddr_t data,
		    struct ifnet *ifp)
{
	return EOPNOTSUPP;
}

int
pru_listen_notsupp(struct socket *so)
{
	return EOPNOTSUPP;
}

int
pru_rcvd_notsupp(struct socket *so, int flags)
{
	return EOPNOTSUPP;
}

int
pru_rcvoob_notsupp(struct socket *so, struct mbuf *m, int flags)
{
	return EOPNOTSUPP;
}

/*
 * This isn't really a ``null'' operation, but it's the default one
 * and doesn't do anything destructive.
 */
int
pru_sense_null(struct socket *so, struct stat *sb)
{
	sb->st_blksize = so->so_snd.sb_hiwat;
	return 0;
}
