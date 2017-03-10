/*
 * Copyright (c) 1995, David Greenman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/dev/fxp/if_fxpvar.h,v 1.17.2.3 2001/06/08 20:36:58 jlemon Exp $
 */

/*
 * Misc. defintions for the Intel EtherExpress Pro/100B PCI Fast
 * Ethernet driver
 */

/*
 * Number of transmit control blocks. This determines the number
 * of transmit buffers that can be chained in the CB list.
 * This must be a power of two.
 */
#define FXP_NTXCB       128

/*
 * Number of completed TX commands at which point an interrupt
 * will be generated to garbage collect the attached buffers.
 * Must be at least one less than FXP_NTXCB, and should be
 * enough less so that the transmitter doesn't becomes idle
 * during the buffer rundown (which would reduce performance).
 */
#define FXP_CXINT_THRESH 120

/*
 * TxCB list index mask. This is used to do list wrap-around.
 */
#define FXP_TXCB_MASK   (FXP_NTXCB - 1)

/*
 * Number of receive frame area buffers. These are large so chose
 * wisely.
 */
#if 0
#define FXP_NRFABUFS    64
#else
#define FXP_NRFABUFS    16
#endif
/*
 * Maximum number of seconds that the receiver can be idle before we
 * assume it's dead and attempt to reset it by reprogramming the
 * multicast filter. This is part of a work-around for a bug in the
 * NIC. See fxp_stats_update().
 */
#define FXP_MAX_RX_IDLE 15

#if __FreeBSD_version < 500000
#define	FXP_LOCK(_sc)
#define	FXP_UNLOCK(_sc)
#define mtx_init(a, b, c)
#define mtx_destroy(a)
struct mtx { int dummy; };
#else
#define	FXP_LOCK(_sc)		mtx_lock(&(_sc)->sc_mtx)
#define	FXP_UNLOCK(_sc)		mtx_unlock(&(_sc)->sc_mtx)
#endif

#ifdef __alpha__
#undef vtophys
#define vtophys(va)	alpha_XXX_dmamap((vm_offset_t)(va))
#endif /* __alpha__ */

/*
 * NOTE: Elements are ordered for optimal cacheline behavior, and NOT
 *	 for functional grouping.
 */
struct fxp_softc {
	struct arpcom arpcom;		/* per-interface network data */
#ifdef NOTUSED
	struct resource *mem;		/* resource descriptor for registers */
	int rtp;			/* register resource type */
	int rgd;			/* register descriptor in use */
	struct resource *irq;		/* resource descriptor for interrupt */
#endif
	void *ih;			/* interrupt handler cookie */
	struct mtx sc_mtx;
#ifdef NOTUSED /* change for RTEMS */
	bus_space_tag_t sc_st;		/* bus space tag */
	bus_space_handle_t sc_sh;	/* bus space handle */
#else
        int pci_signature;              /* RTEMS i386 PCI signature */
        bool pci_regs_are_io;           /* RTEMS dev regs are I/O mapped */
        u_int32_t pci_regs_base;        /* RTEMS i386 register base */
        rtems_id daemonTid;             /* Task ID of deamon        */
        rtems_irq_connect_data	irqInfo;

#endif
	struct mbuf *rfa_headm;		/* first mbuf in receive frame area */
	struct mbuf *rfa_tailm;		/* last mbuf in receive frame area */
	struct fxp_cb_tx *cbl_first;	/* first active TxCB in list */
	int tx_queued;			/* # of active TxCB's */
	int need_mcsetup;		/* multicast filter needs programming */
	struct fxp_cb_tx *cbl_last;	/* last active TxCB in list */
	struct fxp_stats *fxp_stats;	/* Pointer to interface stats */
	int rx_idle_secs;		/* # of seconds RX has been idle */
        enum {fxp_timeout_stopped,fxp_timeout_running,fxp_timeout_stop_rq}
          stat_ch;                     /* status of status updater */
	struct fxp_cb_tx *cbl_base;	/* base of TxCB list */
	struct fxp_cb_mcs *mcsp;	/* Pointer to mcast setup descriptor */
#ifdef NOTUSED
	struct ifmedia sc_media;	/* media information */
	device_t miibus;
	device_t dev;
#endif
	int eeprom_size;		/* size of serial EEPROM */
	int suspended;			/* 0 = normal  1 = suspended (APM) */
	int cu_resume_bug;
	int chip;
	int flags;
	u_int32_t saved_maps[5];	/* pci data */
	u_int32_t saved_biosaddr;
	u_int8_t saved_intline;
	u_int8_t saved_cachelnsz;
	u_int8_t saved_lattimer;
};

#define FXP_CHIP_82557		1	/* 82557 chip type */

#define FXP_FLAG_MWI_ENABLE	0x0001	/* MWI enable */
#define FXP_FLAG_READ_ALIGN	0x0002	/* align read access with cacheline */
#define FXP_FLAG_WRITE_ALIGN	0x0004	/* end write on cacheline */
#define FXP_FLAG_EXT_TXCB	0x0008	/* enable use of extended TXCB */
#define FXP_FLAG_SERIAL_MEDIA	0x0010	/* 10Mbps serial interface */
#define FXP_FLAG_LONG_PKT_EN	0x0020	/* enable long packet reception */
#define FXP_FLAG_ALL_MCAST	0x0040	/* accept all multicast frames */
#define FXP_FLAG_CU_RESUME_BUG	0x0080	/* requires workaround for CU_RESUME */

/* Macros to ease CSR access. */
#if 0
#define	CSR_READ_1(sc, reg)						\
	bus_space_read_1((sc)->sc_st, (sc)->sc_sh, (reg))
#define	CSR_READ_2(sc, reg)						\
	bus_space_read_2((sc)->sc_st, (sc)->sc_sh, (reg))
#define	CSR_READ_4(sc, reg)						\
	bus_space_read_4((sc)->sc_st, (sc)->sc_sh, (reg))
#define	CSR_WRITE_1(sc, reg, val)					\
	bus_space_write_1((sc)->sc_st, (sc)->sc_sh, (reg), (val))
#define	CSR_WRITE_2(sc, reg, val)					\
	bus_space_write_2((sc)->sc_st, (sc)->sc_sh, (reg), (val))
#define	CSR_WRITE_4(sc, reg, val)					\
	bus_space_write_4((sc)->sc_st, (sc)->sc_sh, (reg), (val))
#else
#define	CSR_READ_1(sc, reg) fxp_csr_read_1(sc,reg)
#define	CSR_READ_2(sc, reg) fxp_csr_read_2(sc,reg)
#define	CSR_READ_4(sc, reg) fxp_csr_read_4(sc,reg)

#define	CSR_WRITE_1(sc, reg, val)					\
  do {                                                                  \
     if ((sc)->pci_regs_are_io)                                         \
       outport_byte((sc)->pci_regs_base+(reg),val);                     \
     else                                                               \
       *((u_int8_t*)((sc)->pci_regs_base)+(reg)) = val;                \
  }while (0)

#define	CSR_WRITE_2(sc, reg, val)					\
  do {                                                                  \
     if ((sc)->pci_regs_are_io)                                         \
       outport_word((sc)->pci_regs_base+(reg),val);                     \
     else                                                               \
       *((u_int16_t*)((u_int8_t*)((sc)->pci_regs_base)+(reg))) = val; \
  }while (0)

#define	CSR_WRITE_4(sc, reg, val)					\
  do {                                                                  \
     if ((sc)->pci_regs_are_io)                                         \
       outport_long((sc)->pci_regs_base+(reg),val);                     \
     else                                                               \
       *((u_int32_t*)((u_int8_t*)((sc)->pci_regs_base)+(reg))) = val; \
  }while (0)

#endif

#define	sc_if			arpcom.ac_if

#define	FXP_UNIT(_sc)		(_sc)->arpcom.ac_if.if_unit
