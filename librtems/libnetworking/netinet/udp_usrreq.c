/*
 * Copyright (c) 1982, 1986, 1988, 1990, 1993, 1995
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
 *	@(#)udp_usrreq.c	8.6 (Berkeley) 5/23/95
 * $FreeBSD: src/sys/netinet/udp_usrreq.c,v 1.170 2004/11/08 14:44:53 phk Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/param.h>
#include <sys/queue.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>
#include <sys/syslog.h>

#include <net/if.h>
#include <net/route.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/in_pcb.h>
#include <netinet/in_var.h>
#include <netinet/ip_var.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/udp_var.h>

/*
 * UDP protocol implementation.
 * Per RFC 768, August, 1980.
 */
#ifndef	COMPAT_42
static int	udpcksum = 1;
#else
static int	udpcksum = 0;		/* XXX */
#endif
SYSCTL_INT(_net_inet_udp, UDPCTL_CHECKSUM, checksum, CTLFLAG_RW,
		&udpcksum, 0, "");

static int log_in_vain = 0;
SYSCTL_INT(_net_inet_udp, OID_AUTO, log_in_vain, CTLFLAG_RW, 
	&log_in_vain, 0, "");

struct	inpcbhead udb;		/* from udp_var.h */
struct	inpcbinfo udbinfo;

#ifndef UDBHASHSIZE
#define UDBHASHSIZE 64
#endif

       struct	udpstat udpstat;	/* from udp_var.h */
SYSCTL_STRUCT(_net_inet_udp, UDPCTL_STATS, stats, CTLFLAG_RD,
	&udpstat, udpstat, "");

static struct	sockaddr_in udp_in = { sizeof(udp_in), AF_INET, 0, {0}, {0} };

static	void udp_detach(struct inpcb *);
static	int udp_output(struct inpcb *, struct mbuf *, struct mbuf *,
			    struct mbuf *);
static	void udp_notify(struct inpcb *, int);

void
udp_init(void)
{
	LIST_INIT(&udb);
	udbinfo.listhead = &udb;
	udbinfo.hashbase = hashinit(UDBHASHSIZE, M_PCB, &udbinfo.hashmask);
}

void
udp_input(struct mbuf *m, int iphlen)
{
	register struct ip *ip;
	register struct udphdr *uh;
	register struct inpcb *inp;
	struct mbuf *opts = 0;
	int len;
	struct ip save_ip;

	udpstat.udps_ipackets++;

	/*
	 * Strip IP options, if any; should skip this,
	 * make available to user, and use on returned packets,
	 * but we don't yet have a way to check the checksum
	 * with options still present.
	 */
	if (iphlen > sizeof (struct ip)) {
		ip_stripoptions(m, (struct mbuf *)0);
		iphlen = sizeof(struct ip);
	}

	/*
	 * Get IP and UDP header together in first mbuf.
	 */
	ip = mtod(m, struct ip *);
	if (m->m_len < iphlen + sizeof(struct udphdr)) {
		if ((m = m_pullup(m, iphlen + sizeof(struct udphdr))) == 0) {
			udpstat.udps_hdrops++;
			return;
		}
		ip = mtod(m, struct ip *);
	}
	uh = (struct udphdr *)((caddr_t)ip + iphlen);

	/*
	 * Make mbuf data length reflect UDP length.
	 * If not enough data to reflect UDP length, drop.
	 */
	len = ntohs((u_short)uh->uh_ulen);
	if (ip->ip_len != len) {
		if (len > ip->ip_len || len < sizeof(struct udphdr)) {
			udpstat.udps_badlen++;
			goto bad;
		}
		m_adj(m, len - ip->ip_len);
		/* ip->ip_len = len; */
	}
	/*
	 * Save a copy of the IP header in case we want restore it
	 * for sending an ICMP error message in response.
	 */
	save_ip = *ip;

	/*
	 * Checksum extended UDP header and data.
	 */
	if (uh->uh_sum) {
		((struct ipovly *)ip)->ih_next = 0;
		((struct ipovly *)ip)->ih_prev = 0;
		((struct ipovly *)ip)->ih_x1 = 0;
		((struct ipovly *)ip)->ih_len = uh->uh_ulen;
		uh->uh_sum = in_cksum(m, len + sizeof (struct ip));
		if (uh->uh_sum) {
			udpstat.udps_badsum++;
			m_freem(m);
			return;
		}
	}

	if (IN_MULTICAST(ntohl(ip->ip_dst.s_addr)) ||
	    in_broadcast(ip->ip_dst, m->m_pkthdr.rcvif)) {
		struct inpcb *last;
		/*
		 * Deliver a multicast or broadcast datagram to *all* sockets
		 * for which the local and remote addresses and ports match
		 * those of the incoming datagram.  This allows more than
		 * one process to receive multi/broadcasts on the same port.
		 * (This really ought to be done for unicast datagrams as
		 * well, but that would cause problems with existing
		 * applications that open both address-specific sockets and
		 * a wildcard socket listening to the same port -- they would
		 * end up receiving duplicates of every unicast datagram.
		 * Those applications open the multiple sockets to overcome an
		 * inadequacy of the UDP socket interface, but for backwards
		 * compatibility we avoid the problem here rather than
		 * fixing the interface.  Maybe 4.5BSD will remedy this?)
		 */

		/*
		 * Construct sockaddr format source address.
		 */
		udp_in.sin_port = uh->uh_sport;
		udp_in.sin_addr = ip->ip_src;
		m->m_len -= sizeof (struct udpiphdr);
		m->m_data += sizeof (struct udpiphdr);
		/*
		 * Locate pcb(s) for datagram.
		 * (Algorithm copied from raw_intr().)
		 */
		last = NULL;
		for (inp = udb.lh_first; inp != NULL; inp = inp->inp_list.le_next) {
			if (inp->inp_lport != uh->uh_dport)
				continue;
			if (inp->inp_laddr.s_addr != INADDR_ANY) {
				if (inp->inp_laddr.s_addr !=
				    ip->ip_dst.s_addr)
					continue;
			}
			if (inp->inp_faddr.s_addr != INADDR_ANY) {
				if (inp->inp_faddr.s_addr !=
				    ip->ip_src.s_addr ||
				    inp->inp_fport != uh->uh_sport)
					continue;
			}

			if (last != NULL) {
				struct mbuf *n;

				if ((n = m_copy(m, 0, M_COPYALL)) != NULL) {
					if (last->inp_flags & INP_CONTROLOPTS
					    || last->inp_socket->so_options & SO_TIMESTAMP)
						ip_savecontrol(last, &opts, ip, n);
					if (sbappendaddr(&last->inp_socket->so_rcv,
						(struct sockaddr *)&udp_in,
						n, opts) == 0) {
						m_freem(n);
						if (opts)
						    m_freem(opts);
						udpstat.udps_fullsock++;
					} else
						sorwakeup(last->inp_socket);
					opts = 0;
				}
			}
			last = inp;
			/*
			 * Don't look for additional matches if this one does
			 * not have either the SO_REUSEPORT or SO_REUSEADDR
			 * socket options set.  This heuristic avoids searching
			 * through all pcbs in the common case of a non-shared
			 * port.  It * assumes that an application will never
			 * clear these options after setting them.
			 */
			if (((last->inp_socket->so_options&(SO_REUSEPORT|SO_REUSEADDR)) == 0))
				break;
		}

		if (last == NULL) {
			/*
			 * No matching pcb found; discard datagram.
			 * (No need to send an ICMP Port Unreachable
			 * for a broadcast or multicast datgram.)
			 */
			udpstat.udps_noportbcast++;
			goto bad;
		}
		if (last->inp_flags & INP_CONTROLOPTS
		    || last->inp_socket->so_options & SO_TIMESTAMP)
			ip_savecontrol(last, &opts, ip, m);
		if (sbappendaddr(&last->inp_socket->so_rcv,
		     (struct sockaddr *)&udp_in,
		     m, opts) == 0) {
			udpstat.udps_fullsock++;
			goto bad;
		}
		sorwakeup(last->inp_socket);
		return;
	}
	/*
	 * Locate pcb for datagram.
	 */
	char buf[4*sizeof "123"];
				strcpy(buf, inet_ntoa(ip->ip_dst));
	printf("Connection attempt to UDP %s:%d"
				    " from %s:%d\n",
					buf, ntohs(uh->uh_dport),
					inet_ntoa(ip->ip_src), ntohs(uh->uh_sport));
	inp = in_pcblookuphash(&udbinfo, ip->ip_src, uh->uh_sport,
	    ip->ip_dst, uh->uh_dport, 1);
	if (inp == NULL) {
		if (log_in_vain) {
			char buf[4*sizeof "123"];

			strcpy(buf, inet_ntoa(ip->ip_dst));
			log(LOG_INFO, "Connection attempt to UDP %s:%d"
			    " from %s:%d\n",
				buf, ntohs(uh->uh_dport),
				inet_ntoa(ip->ip_src), ntohs(uh->uh_sport));
		}
		udpstat.udps_noport++;
		if (m->m_flags & (M_BCAST | M_MCAST)) {
			udpstat.udps_noportbcast++;
			goto bad;
		}
		*ip = save_ip;
		icmp_error(m, ICMP_UNREACH, ICMP_UNREACH_PORT, 0, 0);
		return;
	}

	/*
	 * Construct sockaddr format source address.
	 * Stuff source address and datagram in user buffer.
	 */
	udp_in.sin_port = uh->uh_sport;
	udp_in.sin_addr = ip->ip_src;
	if (inp->inp_flags & INP_CONTROLOPTS
	    || inp->inp_socket->so_options & SO_TIMESTAMP)
		ip_savecontrol(inp, &opts, ip, m);
	iphlen += sizeof(struct udphdr);
	m->m_len -= iphlen;
	m->m_pkthdr.len -= iphlen;
	m->m_data += iphlen;
	if (sbappendaddr(&inp->inp_socket->so_rcv, (struct sockaddr *)&udp_in,
	    m, opts) == 0) {
		udpstat.udps_fullsock++;
		goto bad;
	}
	sorwakeup(inp->inp_socket);
	return;
bad:
	m_freem(m);
	if (opts)
		m_freem(opts);
}

/*
 * Notify a udp user of an asynchronous error;
 * just wake up so that he can collect error status.
 */
static void
udp_notify(struct inpcb *inp, int errnum)
{
	inp->inp_socket->so_error = errnum;
	sorwakeup(inp->inp_socket);
	sowwakeup(inp->inp_socket);
}

void
udp_ctlinput(int cmd, struct sockaddr *sa, void *vip)
{
	register struct ip *ip = vip;
	register struct udphdr *uh;

	if (!PRC_IS_REDIRECT(cmd) &&
	    ((unsigned)cmd >= PRC_NCMDS || inetctlerrmap[cmd] == 0))
		return;
	if (ip) {
		uh = (struct udphdr *)((caddr_t)ip + (ip->ip_hl << 2));
		in_pcbnotify(&udb, sa, uh->uh_dport, ip->ip_src, uh->uh_sport,
			cmd, udp_notify);
	} else
		in_pcbnotify(&udb, sa, 0, zeroin_addr, 0, cmd, udp_notify);
}

static int
udp_output(struct inpcb *inp, struct mbuf *m, struct mbuf *addr,
    struct mbuf *control)
{
	register struct udpiphdr *ui;
	register int len = m->m_pkthdr.len;
	struct in_addr laddr;
	int s = 0, error = 0;

	laddr.s_addr = 0;
	if (control)
		m_freem(control);		/* XXX */

	if (len + sizeof(struct udpiphdr) > IP_MAXPACKET) {
		error = EMSGSIZE;
		goto release;
	}

	if (addr) {
		laddr = inp->inp_laddr;
		if (inp->inp_faddr.s_addr != INADDR_ANY) {
			error = EISCONN;
			goto release;
		}
		/*
		 * Must block input while temporarily connected.
		 */
		s = splnet();
		error = in_pcbconnect(inp, addr);
		if (error) {
			splx(s);
			goto release;
		}
	} else {
		if (inp->inp_faddr.s_addr == INADDR_ANY) {
			error = ENOTCONN;
			goto release;
		}
	}
	/*
	 * Calculate data length and get a mbuf
	 * for UDP and IP headers.
	 */
	M_PREPEND(m, sizeof(struct udpiphdr), M_DONTWAIT);
	if (m == 0) {
		error = ENOBUFS;
		if (addr) {
			in_pcbdisconnect(inp);
			inp->inp_laddr = laddr;
			splx(s);
		}
		goto release;
	}

	/*
	 * Fill in mbuf with extended UDP header
	 * and addresses and length put into network format.
	 */
	ui = mtod(m, struct udpiphdr *);
	ui->ui_next = ui->ui_prev = 0;
	ui->ui_x1 = 0;
	ui->ui_pr = IPPROTO_UDP;
	ui->ui_len = htons((u_short)len + sizeof (struct udphdr));
	ui->ui_src = inp->inp_laddr;
	ui->ui_dst = inp->inp_faddr;
	ui->ui_sport = inp->inp_lport;
	ui->ui_dport = inp->inp_fport;
	ui->ui_ulen = ui->ui_len;

	/*
	 * Stuff checksum and output datagram.
	 */
	ui->ui_sum = 0;
	if (udpcksum) {
	    if ((ui->ui_sum = in_cksum(m, sizeof (struct udpiphdr) + len)) == 0)
		ui->ui_sum = 0xffff;
	}
	((struct ip *)ui)->ip_len = sizeof (struct udpiphdr) + len;
	((struct ip *)ui)->ip_ttl = inp->inp_ip_ttl;	/* XXX */
	((struct ip *)ui)->ip_tos = inp->inp_ip_tos;	/* XXX */
	udpstat.udps_opackets++;
	error = ip_output(m, inp->inp_options, &inp->inp_route,
	    inp->inp_socket->so_options & (SO_DONTROUTE | SO_BROADCAST),
	    inp->inp_moptions);

	if (addr) {
		in_pcbdisconnect(inp);
		inp->inp_laddr = laddr;
		splx(s);
	}
	return (error);

release:
	m_freem(m);
	return (error);
}

#ifdef __rtems__
#define INP_INFO_RLOCK(a)
#define INP_INFO_RUNLOCK(a)
#define INP_LOCK(a)
#define INP_UNLOCK(a)
#endif

static int
udp_pcblist(SYSCTL_HANDLER_ARGS)
{
	int error, i, n, s;
	struct inpcb *inp, **inp_list;
	inp_gen_t gencnt;
	struct xinpgen xig;

	/*
	 * The process of preparing the TCB list is too time-consuming and
	 * resource-intensive to repeat twice on every request.
	 */
	if (req->oldptr == 0) {
		n = udbinfo.ipi_count;
		req->oldidx = 2 * (sizeof xig)
			+ (n + n/8) * sizeof(struct xinpcb);
		return 0;
	}

	if (req->newptr != 0)
		return EPERM;

	/*
	 * OK, now we're committed to doing something.
	 */
	s = splnet();
	gencnt = udbinfo.ipi_gencnt;
	n = udbinfo.ipi_count;
	splx(s);

	sysctl_wire_old_buffer(req, 2 * (sizeof xig)
		+ n * sizeof(struct xinpcb));

	xig.xig_len = sizeof xig;
	xig.xig_count = n;
	xig.xig_gen = gencnt;
#if 0
	xig.xig_sogen = so_gencnt;
#endif
	error = SYSCTL_OUT(req, &xig, sizeof xig);
	if (error)
		return error;

  /* ccj add the exit if count is 0 */
  if (!n)
    return error;
  
	inp_list = malloc(n * sizeof *inp_list, M_TEMP, M_WAITOK);
	if (inp_list == 0)
		return ENOMEM;
	
	s = splnet();
	INP_INFO_RLOCK(&udbinfo);
	for (inp = LIST_FIRST(udbinfo.listhead), i = 0; inp && i < n;
	     inp = LIST_NEXT(inp, inp_list)) {
		INP_LOCK(inp);
		if (inp->inp_gencnt <= gencnt)
#if 0
      &&
		    cr_canseesocket(req->td->td_ucred, inp->inp_socket) == 0)
#endif
			inp_list[i++] = inp;
		INP_UNLOCK(inp);
	}
	INP_INFO_RUNLOCK(&udbinfo);
	splx(s);
	n = i;

	error = 0;
	for (i = 0; i < n; i++) {
		inp = inp_list[i];
		INP_LOCK(inp);
		if (inp->inp_gencnt <= gencnt) {
			struct xinpcb xi;
			xi.xi_len = sizeof xi;
			/* XXX should avoid extra copy */
			bcopy(inp, &xi.xi_inp, sizeof *inp);
#if 0
			if (inp->inp_socket)
				sotoxsocket(inp->inp_socket, &xi.xi_socket);
#endif
			error = SYSCTL_OUT(req, &xi, sizeof xi);
		}
		INP_UNLOCK(inp);
	}
	if (!error) {
		/*
		 * Give the user an updated idea of our state.
		 * If the generation differs from what we told
		 * her before, she knows that something happened
		 * while we were processing this request, and it
		 * might be necessary to retry.
		 */
		s = splnet();
		INP_INFO_RLOCK(&udbinfo);
		xig.xig_gen = udbinfo.ipi_gencnt;
#if 0
		xig.xig_sogen = so_gencnt;
#endif
		xig.xig_count = udbinfo.ipi_count;
		INP_INFO_RUNLOCK(&udbinfo);
		splx(s);
		error = SYSCTL_OUT(req, &xig, sizeof xig);
	}
	free(inp_list, M_TEMP);
	return error;
}

SYSCTL_PROC(_net_inet_udp, UDPCTL_PCBLIST, pcblist, CTLFLAG_RD, 0, 0,
	    udp_pcblist, "S,xinpcb", "List of active UDP sockets");

static u_long	udp_sendspace = 9216;		/* really max datagram size */
					/* 40 1K datagrams */
SYSCTL_INT(_net_inet_udp, UDPCTL_MAXDGRAM, maxdgram, CTLFLAG_RW,
	&udp_sendspace, 0, "");

static u_long	udp_recvspace = 40 * (1024 + sizeof(struct sockaddr_in));
SYSCTL_INT(_net_inet_udp, UDPCTL_RECVSPACE, recvspace, CTLFLAG_RW,
	&udp_recvspace, 0, "");

#if defined(__rtems__)
void rtems_set_udp_buffer_sizes(u_long sendspace, u_long recvspace)
{
    if ( sendspace != 0 )
      udp_sendspace = sendspace;
    if ( recvspace != 0 )
      udp_recvspace = recvspace;
}
#endif

/*ARGSUSED*/
int
udp_usrreq(struct socket *so, int req, struct mbuf *m, struct mbuf *addr,
    struct mbuf *control)
{
	struct inpcb *inp = sotoinpcb(so);
	int error = 0;
	int s;

	if (req == PRU_CONTROL)
		return (in_control(so, (uintptr_t)m, (caddr_t)addr,
			(struct ifnet *)control));
	if (inp == NULL && req != PRU_ATTACH) {
		error = EINVAL;
		goto release;
	}
	/*
	 * Note: need to block udp_input while changing
	 * the udp pcb queue and/or pcb addresses.
	 */
	switch (req) {

	case PRU_ATTACH:
		if (inp != NULL) {
			error = EINVAL;
			break;
		}
		s = splnet();
		error = in_pcballoc(so, &udbinfo);
		splx(s);
		if (error)
			break;
		error = soreserve(so, udp_sendspace, udp_recvspace);
		if (error)
			break;
		((struct inpcb *) so->so_pcb)->inp_ip_ttl = ip_defttl;
		break;

	case PRU_DETACH:
		udp_detach(inp);
		break;

	case PRU_BIND:
		s = splnet();
		error = in_pcbbind(inp, addr);
		splx(s);
		break;

	case PRU_LISTEN:
		error = EOPNOTSUPP;
		break;

	case PRU_CONNECT:
		if (inp->inp_faddr.s_addr != INADDR_ANY) {
			error = EISCONN;
			break;
		}
		s = splnet();
		error = in_pcbconnect(inp, addr);
		splx(s);
		if (error == 0)
			soisconnected(so);
		break;

	case PRU_CONNECT2:
		error = EOPNOTSUPP;
		break;

	case PRU_ACCEPT:
		error = EOPNOTSUPP;
		break;

	case PRU_DISCONNECT:
		if (inp->inp_faddr.s_addr == INADDR_ANY) {
			error = ENOTCONN;
			break;
		}
		s = splnet();
		in_pcbdisconnect(inp);
		inp->inp_laddr.s_addr = INADDR_ANY;
		splx(s);
		so->so_state &= ~SS_ISCONNECTED;		/* XXX */
		break;

	case PRU_SHUTDOWN:
		socantsendmore(so);
		break;

	case PRU_SEND:
		return (udp_output(inp, m, addr, control));

	case PRU_ABORT:
		soisdisconnected(so);
		udp_detach(inp);
		break;

	case PRU_SOCKADDR:
		in_setsockaddr(inp, addr);
		break;

	case PRU_PEERADDR:
		in_setpeeraddr(inp, addr);
		break;

	case PRU_SENSE:
		/*
		 * stat: don't bother with a blocksize.
		 */
		return (0);

	case PRU_SENDOOB:
	case PRU_FASTTIMO:
	case PRU_SLOWTIMO:
	case PRU_PROTORCV:
	case PRU_PROTOSEND:
		error =  EOPNOTSUPP;
		break;

	case PRU_RCVD:
	case PRU_RCVOOB:
		return (EOPNOTSUPP);	/* do not free mbuf's */

	default:
		panic("udp_usrreq");
	}

release:
	if (control) {
		printf("udp control data unexpectedly retained\n");
		m_freem(control);
	}
	if (m)
		m_freem(m);
	return (error);
}

static void
udp_detach(struct inpcb *inp)
{
	int s = splnet();

	in_pcbdetach(inp);
	splx(s);
}
