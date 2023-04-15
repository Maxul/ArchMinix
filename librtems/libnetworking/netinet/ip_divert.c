/*
 * Copyright (c) 1982, 1986, 1988, 1993
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
 * $FreeBSD: src/sys/netinet/ip_divert.c,v 1.113 2005/05/13 11:44:37 glebius Exp $
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/param.h>
#include <sys/queue.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/protosw.h>
#include <sys/socketvar.h>
#include <errno.h>
#include <sys/systm.h>

#include <net/if.h>
#include <net/route.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/in_pcb.h>
#include <netinet/in_var.h>
#include <netinet/ip_var.h>

/*
 * Divert sockets
 */

/*
 * Allocate enough space to hold a full IP packet
 */
#define	DIVSNDQ		(65536 + 100)
#define	DIVRCVQ		(65536 + 100)

/* Global variables */

/*
 * ip_input() and ip_output() set this secret value before calling us to
 * let us know which divert port to divert a packet to; this is done so
 * we can use the existing prototype for struct protosw's pr_input().
 * This is stored in host order.
 */
u_short ip_divert_port;

/*
 * We set this value to a non-zero port number when we want the call to
 * ip_fw_chk() in ip_input() or ip_output() to ignore ``divert <port>''
 * chain entries. This is stored in host order.
 */
u_short ip_divert_ignore;

/* Internal variables. */
static struct inpcbhead divcb;
static struct inpcbinfo divcbinfo;

static u_long	div_sendspace = DIVSNDQ;	/* XXX sysctl ? */
static u_long	div_recvspace = DIVRCVQ;	/* XXX sysctl ? */

/* Optimization: have this preinitialized */
static struct sockaddr_in divsrc = { sizeof(divsrc), AF_INET, 0, { 0 }, { 0 } };

/* Internal functions */

static int div_output(struct socket *so,
		struct mbuf *m, struct mbuf *addr, struct mbuf *control);

/*
 * Initialize divert connection block queue.
 */
void
div_init(void)
{
	LIST_INIT(&divcb);
	divcbinfo.listhead = &divcb;
	/*
	 * XXX We don't use the hash list for divert IP, but it's easier
	 * to allocate a one entry hash list than it is to check all
	 * over the place for hashbase == NULL.
	 */
	divcbinfo.hashbase = hashinit(1, M_PCB, &divcbinfo.hashmask);
}

/*
 * Setup generic address and protocol structures
 * for div_input routine, then pass them along with
 * mbuf chain. ip->ip_len is assumed to have had
 * the header length (hlen) subtracted out already.
 * We tell whether the packet was incoming or outgoing
 * by seeing if hlen == 0, which is a hack.
 */
void
div_input(struct mbuf *m, int hlen)
{
	struct ip *ip;
	struct inpcb *inp;
	struct socket *sa;

	/* Sanity check */
	if (ip_divert_port == 0)
		panic("div_input: port is 0");

	/* Assure header */
	if (m->m_len < sizeof(struct ip) &&
	    (m = m_pullup(m, sizeof(struct ip))) == 0) {
		return;
	}
	ip = mtod(m, struct ip *);

	/* Record divert port */
	divsrc.sin_port = htons(ip_divert_port);

	/* Restore packet header fields */
	ip->ip_len += hlen;
	HTONS(ip->ip_len);
	HTONS(ip->ip_off);

	/* Record receive interface address, if any */
	divsrc.sin_addr.s_addr = 0;
	if (hlen) {
		struct ifaddr *ifa;

#ifdef DIAGNOSTIC
		/* Sanity check */
		if (!(m->m_flags & M_PKTHDR))
			panic("div_input: no pkt hdr");
#endif

		/* More fields affected by ip_input() */
		HTONS(ip->ip_id);

		/* Find IP address for recieve interface */
		for (ifa = m->m_pkthdr.rcvif->if_addrlist;
		    ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;
			if (ifa->ifa_addr->sa_family != AF_INET)
				continue;
			divsrc.sin_addr =
			    ((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
			break;
		}
	}

	/* Put packet on socket queue, if any */
	sa = NULL;
	for (inp = divcb.lh_first; inp != NULL; inp = inp->inp_list.le_next) {
		if (inp->inp_lport == htons(ip_divert_port))
			sa = inp->inp_socket;
	}
	if (sa) {
		if (sbappendaddr(&sa->so_rcv, (struct sockaddr *)&divsrc,
				m, (struct mbuf *)0) == 0)
			m_freem(m);
		else
			sorwakeup(sa);
	} else {
		m_freem(m);
		ipstat.ips_noproto++;
		ipstat.ips_delivered--;
        }
}

/*
 * Deliver packet back into the IP processing machinery.
 *
 * If no address specified, or address is 0.0.0.0, send to ip_output();
 * otherwise, send to ip_input() and mark as having been received on
 * the interface with that address.
 *
 * If no address specified, or dest port is 0, allow packet to divert
 * back to this socket; otherwise, don't.
 */
static int
div_output(struct socket *so, struct mbuf *m,
	struct mbuf *addr, struct mbuf *control)
{
	register struct inpcb *const inp = sotoinpcb(so);
	register struct ip *const ip = mtod(m, struct ip *);
	struct sockaddr_in *sin = NULL;
	int error = 0;

	if (control)
		m_freem(control);		/* XXX */
	if (addr)
		sin = mtod(addr, struct sockaddr_in *);

	/* Loopback avoidance option */
	ip_divert_ignore = ntohs(inp->inp_lport);

	/* Reinject packet into the system as incoming or outgoing */
	if (!sin || sin->sin_addr.s_addr == 0) {
		/* Don't allow both user specified and setsockopt options,
		   and don't allow packet length sizes that will crash */
		if (((ip->ip_hl != (sizeof (*ip) >> 2)) && inp->inp_options) ||
		     ((u_short)ntohs(ip->ip_len) > m->m_pkthdr.len)) {
			error = EINVAL;
			goto cantsend;
		}

		/* Convert fields to host order for ip_output() */
		NTOHS(ip->ip_len);
		NTOHS(ip->ip_off);

		/* Send packet to output processing */
		ipstat.ips_rawout++;			/* XXX */
		error = ip_output(m, inp->inp_options, &inp->inp_route,
			(so->so_options & SO_DONTROUTE) |
			IP_ALLOWBROADCAST | IP_RAWOUTPUT, inp->inp_moptions);
	} else {
		struct ifaddr *ifa;

		/* Find receive interface with the given IP address */
		sin->sin_port = 0;
		if ((ifa = ifa_ifwithaddr((struct sockaddr *) sin)) == 0) {
			error = EADDRNOTAVAIL;
			goto cantsend;
		}
		m->m_pkthdr.rcvif = ifa->ifa_ifp;

		/* Send packet to input processing */
		ip_input(m);
	}

	/* Reset for next time (and other packets) */
	ip_divert_ignore = 0;
	return error;

cantsend:
	ip_divert_ignore = 0;
	m_freem(m);
	return error;
}

/*ARGSUSED*/
int
div_usrreq(
	struct socket *so,
	int req,
	struct mbuf *m,
	struct mbuf *nam, 
	struct mbuf *control )
{
	register int error = 0;
	register struct inpcb *inp = sotoinpcb(so);
	int s;

	if (inp == NULL && req != PRU_ATTACH) {
		error = EINVAL;
		goto release;
	}
	switch (req) {

	case PRU_ATTACH:
		if (inp)
			panic("div_attach");
		if ((so->so_state & SS_PRIV) == 0) {
			error = EACCES;
			break;
		}
		s = splnet();
		error = in_pcballoc(so, &divcbinfo);
		splx(s);
		if (error)
			break;
		error = soreserve(so, div_sendspace, div_recvspace);
		if (error)
			break;
		inp = (struct inpcb *)so->so_pcb;
		inp->inp_ip_p = (intptr_t)nam;	/* XXX */
		inp->inp_flags |= INP_HDRINCL;
		/* The socket is always "connected" because
		   we always know "where" to send the packet */
		so->so_state |= SS_ISCONNECTED;
		break;

	case PRU_DISCONNECT:
		if ((so->so_state & SS_ISCONNECTED) == 0) {
			error = ENOTCONN;
			break;
		}
		/* FALLTHROUGH */
	case PRU_ABORT:
		soisdisconnected(so);
		/* FALLTHROUGH */
	case PRU_DETACH:
		if (inp == 0)
			panic("div_detach");
		in_pcbdetach(inp);
		break;

	case PRU_BIND:
		s = splnet();
		error = in_pcbbind(inp, nam);
		splx(s);
		break;

	/*
	 * Mark the connection as being incapable of further input.
	 */
	case PRU_SHUTDOWN:
		socantsendmore(so);
		break;

	case PRU_SEND:
		/* Packet must have a header (but that's about it) */
		if (m->m_len < sizeof (struct ip) ||
		    (m = m_pullup(m, sizeof (struct ip))) == 0) {
			ipstat.ips_toosmall++;
			error = EINVAL;
			break;
		}

		/* Send packet */
		error = div_output(so, m, nam, control); 
		m = NULL;
		break;

	case PRU_SOCKADDR:
		in_setsockaddr(inp, nam);
		break;

	case PRU_SENSE:
		/*
		 * stat: don't bother with a blocksize.
		 */
		return (0);

	/*
	 * Not supported.
	 */
	case PRU_CONNECT:
	case PRU_CONNECT2:
	case PRU_CONTROL:
	case PRU_RCVOOB:
	case PRU_RCVD:
	case PRU_LISTEN:
	case PRU_ACCEPT:
	case PRU_SENDOOB:
	case PRU_PEERADDR:
		error = EOPNOTSUPP;
		break;

	default:
		panic("div_usrreq");
	}
release:
	if (m)
		m_freem(m);
	return (error);
}
