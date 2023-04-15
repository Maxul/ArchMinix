#ifndef _DMA_H_
#define _DMA_H_

#ifdef _INLINE
#define __INLINE static inline
#else
#define __INLINE
#endif

/*
 * DMA REGISTERS
 */

/* Register Addresses */
/* DMA Channel 0 */
#define DMA0_PRIMARY_CTRL_ADDR 0x01840000 /* DMA 0 PRI CTRL REG */
#define DMA0_SECONDARY_CTRL_ADDR 0x01840008 /* DMA 0 SEC CTRL REG */
#define DMA0_SRC_ADDR_ADDR 0x01840010 /* DMA 0 SRC ADDR */
#define DMA0_DEST_ADDR_ADDR 0x01840018 /* DMA 0 DEST ADDR */
#define DMA0_XFER_COUNTER_ADDR 0x01840020 /* DMA 0 TRANS CNT */

/* DMA Channel 1 */
#define DMA1_PRIMARY_CTRL_ADDR 0x01840040 /* DMA 1 PRI CTRL REG */
#define DMA1_SECONDARY_CTRL_ADDR 0x01840048 /* DMA 1 SEC CTRL REG */
#define DMA1_SRC_ADDR_ADDR 0x01840050 /* DMA 1 SRC ADDR */
#define DMA1_DEST_ADDR_ADDR 0x01840058 /* DMA 1 DEST ADDR */
#define DMA1_XFER_COUNTER_ADDR 0x01840060 /* DMA 1 TRANS CNT */

/* DMA Channel 2 */
#define DMA2_PRIMARY_CTRL_ADDR 0x01840004 /* DMA 2 PRI CTRL REG */
#define DMA2_SECONDARY_CTRL_ADDR 0x0184000C /* DMA 2 SEC CTRL REG */
#define DMA2_SRC_ADDR_ADDR 0x01840014 /* DMA 2 SRC ADDR */
#define DMA2_DEST_ADDR_ADDR 0x0184001C /* DMA 2 DEST ADDR */
#define DMA2_XFER_COUNTER_ADDR 0x01840024 /* DMA 2 TRANS CNT */

/* DMA Channel 3 */
#define DMA3_PRIMARY_CTRL_ADDR 0x01840044 /* DMA 3 PRI CTRL REG */
#define DMA3_SECONDARY_CTRL_ADDR 0x0184004C /* DMA 3 SEC CTRL REG */
#define DMA3_SRC_ADDR_ADDR 0x01840054 /* DMA 3 SRC ADDR */
#define DMA3_DEST_ADDR_ADDR 0x0184005C /* DMA 3 DEST ADDR */
#define DMA3_XFER_COUNTER_ADDR 0x01840064 /* DMA 3 TRANS CNT */

/* DMA Auxiliary Control */
#define DMA_AUXCR_ADDR 0x01840070 /* AUX DMA CTRL REG */

/* DMA Global Registers */
#define DMA_GCR_A_ADDR 0x01840028 /* GLOBAL CNT RELOADA */
#define DMA_GCR_B_ADDR 0x0184002C /* GLOBAL CNT RELOADB */
#define DMA_GNDX_A_ADDR 0x01840030 /* GLOBAL INDEX REG A */
#define DMA_GNDX_B_ADDR 0x01840034 /* GLOBAL INDEX REG B */
#define DMA_GADDR_A_ADDR 0x01840038 /* GLOBAL ADDR REG A */
#define DMA_GADDR_B_ADDR 0x0184003C /* GLOBAL ADDR REG B */
#define DMA_GADDR_C_ADDR 0x01840068 /* GLOBAL ADDR REG C */
#define DMA_GADDR_D_ADDR 0x0184006C /* GLOBAL ADDR REG D */

/* Register Contents */
/* DMA Channel 0 */
#define DMA0_PRIMARY_CTRL *(volatile unsigned int*)DMA0_PRIMARY_CTRL_ADDR
#define DMA0_SECONDARY_CTRL *(volatile unsigned int*)DMA0_SECONDARY_CTRL_ADDR
#define DMA0_SRC_ADDR *(volatile unsigned int*)DMA0_SRC_ADDR_ADDR
#define DMA0_DEST_ADDR *(volatile unsigned int*)DMA0_DEST_ADDR_ADDR
#define DMA0_XFER_COUNTER *(volatile unsigned int*)DMA0_XFER_COUNTER_ADDR

/* DMA Channel 1 */
#define DMA1_PRIMARY_CTRL *(volatile unsigned int*)DMA1_PRIMARY_CTRL_ADDR
#define DMA1_SECONDARY_CTRL *(volatile unsigned int*)DMA1_SECONDARY_CTRL_ADDR
#define DMA1_SRC_ADDR *(volatile unsigned int*)DMA1_SRC_ADDR_ADDR
#define DMA1_DEST_ADDR *(volatile unsigned int*)DMA1_DEST_ADDR_ADDR
#define DMA1_XFER_COUNTER *(volatile unsigned int*)DMA1_XFER_COUNTER_ADDR

/* DMA Channel 2 */
#define DMA2_PRIMARY_CTRL *(volatile unsigned int*)DMA2_PRIMARY_CTRL_ADDR
#define DMA2_SECONDARY_CTRL *(volatile unsigned int*)DMA2_SECONDARY_CTRL_ADDR
#define DMA2_SRC_ADDR *(volatile unsigned int*)DMA2_SRC_ADDR_ADDR
#define DMA2_DEST_ADDR *(volatile unsigned int*)DMA2_DEST_ADDR_ADDR
#define DMA2_XFER_COUNTER *(volatile unsigned int*)DMA2_XFER_COUNTER_ADDR

/* DMA Channel 3 */
#define DMA3_PRIMARY_CTRL *(volatile unsigned int*)DMA3_PRIMARY_CTRL_ADDR
#define DMA3_SECONDARY_CTRL *(volatile unsigned int*)DMA3_SECONDARY_CTRL_ADDR
#define DMA3_SRC_ADDR *(volatile unsigned int*)DMA3_SRC_ADDR_ADDR
#define DMA3_DEST_ADDR *(volatile unsigned int*)DMA3_DEST_ADDR_ADDR
#define DMA3_XFER_COUNTER *(volatile unsigned int*)DMA3_XFER_COUNTER_ADDR

/* DMA Auxiliary Control */
#define DMA_AUXCR *(volatile unsigned int*)DMA_AUXCR_ADDR

/* DMA Global Registers */
#define DMA_GCR_A *(volatile unsigned int*)DMA_GCR_A_ADDR
#define DMA_GCR_B *(volatile unsigned int*)DMA_GCR_B_ADDR
#define DMA_GNDX_A *(volatile unsigned int*)DMA_GNDX_A_ADDR
#define DMA_GNDX_B *(volatile unsigned int*)DMA_GNDX_B_ADDR
#define DMA_GADDR_A *(volatile unsigned int*)DMA_GADDR_A_ADDR
#define DMA_GADDR_B *(volatile unsigned int*)DMA_GADDR_B_ADDR
#define DMA_GADDR_C *(volatile unsigned int*)DMA_GADDR_C_ADDR
#define DMA_GADDR_D *(volatile unsigned int*)DMA_GADDR_D_ADDR

#define DMA_REG_SELECT(base, chan) \
    (((unsigned int)(base)) + (((chan)&1) * 0x40) + (((chan)&2) * 2))

#define DMA_PRIMARY_CTRL_ADDR(chan) \
    (DMA_REG_SELECT(DMA0_PRIMARY_CTRL_ADDR, chan))

#define DMA_SECONDARY_CTRL_ADDR(chan) \
    (DMA_REG_SELECT(DMA0_SECONDARY_CTRL_ADDR, chan))

#define DMA_SRC_ADDR_ADDR(chan) \
    (DMA_REG_SELECT(DMA0_SRC_ADDR_ADDR, chan))

#define DMA_DEST_ADDR_ADDR(chan) \
    (DMA_REG_SELECT(DMA0_DEST_ADDR_ADDR, chan))

#define DMA_XFER_COUNTER_ADDR(chan) \
    (DMA_REG_SELECT(DMA0_XFER_COUNTER_ADDR, chan))

/* DMA Register Bitfields */
/* Primary Control Register */
#define START 0
#define START_SZ 2
#define STATUS 2
#define STATUS_SZ 2
#define SRC_DIR 4
#define SRC_DIR_SZ 2
#define DST_DIR 6
#define DST_DIR_SZ 2
#define ESIZE 8
#define ESIZE_SZ 2
#define SPLIT 10
#define SPLIT_SZ 2
#define CNT_RELOAD 12
#define INDEX 13
#define RSYNC 14
#define RSYNC_SZ 5
#define WSYNC 19
#define WSYNC_SZ 5
#define PRI 24
#define TCINT 25
#define FS 26
#define EMOD 27
#define SRC_RELOAD 28
#define SRC_RELOAD_SZ 2
#define DST_RELOAD 30
#define DST_RELOAD_SZ 2

/* Secondary Control Register */
#define SX_COND 0
#define SX_IE 1
#define FRAME_COND 2
#define FRAME_IE 3
#define LAST_COND 4
#define LAST_IE 5
#define BLOCK_COND 6
#define BLOCK_IE 7
#define RDROP_COND 8
#define RDROP_IE 9
#define WDROP_COND 10
#define WDROP_IE 11
#define RSYNC_STAT 12
#define RSYNC_CLR 13
#define WSYNC_STAT 14
#define WSYNC_CLR 15
#define DMAC_EN 16
#define DMAC_EN_SZ 3

/* DMA Channel Transfer Counter Register */
#define ELEMENT_COUNT 0
#define ELEMENT_COUNT_SZ 16
#define FRAME_COUNT 16
#define FRAME_COUNT_SZ 16

/* DMA Global Count Reload Register Bits */
#define ELEMENT_COUNT_RELOAD 0
#define ELEMENT_COUNT_RELOAD_SZ 16
#define FRAME_COUNT_RELOAD 16
#define FRAME_COUNT_RELOAD_SZ 16

/* DMA Global Index Register Bits */
#define ELEMENT_INDEX 0
#define ELEMENT_INDEX_SZ 16
#define FRAME_INDEX 16
#define FRAME_INDEX_SZ 16

/* DMA Global Address Register Bits */
#define SPLIT_ADDRESS 3
#define SPLIT_ADDRESS_SZ 29

/* DMA Auxiliary Control Register Bits */
#define CH_PRI 0
#define CH_PRI_SZ 4
#define AUXPRI 4

/*
 * DEFINES
 */
#define DMA_CH0 0x00
#define DMA_CH1 0x01
#define DMA_CH2 0x02
#define DMA_CH3 0x03

/* BITFIELD VALUES */
/* DMA Channel Primary Control Register bitfield values */

/* START */
#define DMA_STOP_VAL 0x00
#define DMA_START_VAL 0x01
#define DMA_PAUSE_VAL 0x02
#define DMA_AUTO_START_VAL 0x03

/* SRC DIR, DST DIR */
#define DMA_ADDR_NO_MOD 0x00
#define DMA_ADDR_INC 0x01
#define DMA_ADDR_DEC 0x02
#define DMA_ADDR_INDX 0x03

/* Synchronization Event Numbers */
#define SEN_NONE 0x00
#define SEN_TINT0 0x01
#define SEN_TINT1 0x02
#define SEN_SD_INT 0x03
#define SEN_EXT_INT4 0x04
#define SEN_EXT_INT5 0x05
#define SEN_EXT_INT6 0x06
#define SEN_EXT_INT7 0x07
#define SEN_DMA_INT0 0x08
#define SEN_DMA_INT1 0x09
#define SEN_DMA_INT2 0x0A
#define SEN_DMA_INT3 0x0B
#define SEN_XEVT0 0x0C
#define SEN_REVT0 0x0D
#define SEN_XEVT1 0x0E
#define SEN_REVT1 0x0F
#define SEN_DSPINT 0x10

/* ESIZE defines */
#define DMA_ESIZE32 0x00
#define DMA_ESIZE16 0x01
#define DMA_ESIZE8 0x02

/* PRI defines */
#define DMA_CPU_PRI 0x00
#define DMA_DMA_PRI 0x01

/* SPLIT mode operation defines */
#define DMA_SPLIT_DIS 0x00
#define DMA_SPLIT_GARA 0x01
#define DMA_SPLIT_GARB 0x02
#define DMA_SPLIT_GARC 0x03

/* CNT RELOAD defines */
#define DMA_CNT_RELOADA 0x00
#define DMA_CNT_RELOADB 0x01

/* INDEX defines */
#define DMA_INDXA 0x00
#define DMA_INDXB 0x01

/* EMULATION MODE response defines */
#define DMA_NO_EM_HALT 0x00
#define DMA_EM_HALT 0x01

/* SRC/DST RELOAD defines */
#define DMA_RELOAD_NONE 0x00
#define DMA_RELOAD_GARB 0x01
#define DMA_RELOAD_GARC 0x02
#define DMA_RELOAD_GARD 0x03

/* DMA Channel Primary Control Register bitfield values */
/* DMAC EN Control */
#define DMAC_LO 0x00
#define DMAC_HI 0x01
#define DMAC_RSYNC_STAT 0x02
#define DMAC_WSYNC_STAT 0x03
#define DMAC_FRAME_COND 0x04
#define DMAC_BLOCK_COND 0x05

/*
 * MACRO DEFINITIONS
 */
#define DMA_START(chan) \
    LOAD_FIELD(DMA_PRIMARY_CTRL_ADDR(chan), DMA_START_VAL, START, START_SZ)
#define DMA_AUTO_START(chan) \
    LOAD_FIELD(DMA_PRIMARY_CTRL_ADDR(chan), DMA_AUTO_START_VAL, START, START_SZ)
#define DMA_STOP(chan) \
    LOAD_FIELD(DMA_PRIMARY_CTRL_ADDR(chan), DMA_STOP_VAL, START, START_SZ)
#define DMA_PAUSE(chan) \
    LOAD_FIELD(DMA_PRIMARY_CTRL_ADDR(chan), DMA_PAUSE_VAL, START, START_SZ)
#define DMA_RSYNC_CLR(chan) \
    LOAD_FIELD(DMA_SECONDARY_CTRL_ADDR(chan), 2, RSYNC_STAT, 2)
#define DMA_WSYNC_CLR(chan) \
    LOAD_FIELD(DMA_SECONDARY_CTRL_ADDR(chan), 2, WSYNC_STAT, 2)
#define DMA_RSYNC_SET(chan) \
    LOAD_FIELD(DMA_SECONDARY_CTRL_ADDR(chan), 1, RSYNC_STAT, 2)
#define DMA_WSYNC_SET(chan) \
    LOAD_FIELD(DMA_SECONDARY_CTRL_ADDR(chan), 1, WSYNC_STAT, 2)

/*
 * FUNCTION DEFINITIONS
 */

/*
 * dma_init 每 Initialize channel specific control registers.
 *
 * This function is responsible for setting the primary control register,
 * secondary control register, source address, destination address and
 * transfer count for the specified DMA channel.
 */
__inline void dma_init(/*RET: OK or ERROR (invalid channel) */
    unsigned short chan, /*IN: DMA channel number */
    unsigned int pri_ctrl, /*IN: Value to set primary cntl reg */
    unsigned int sec_ctrl, /*IN: Value to set sec cntl reg */
    unsigned int src_addr, /*IN: Value to set source addr reg */
    unsigned int dst_addr, /*IN: Value to set dest addr reg */
    unsigned int trans_ctr /*IN: Value to set transfer counter */
);

/*
 * dma_global_init 每 Initialize global control registers.
 *
 * This function is responsible for setting the DMA auxiliary control
 * register, global count reload registers A & B, global index registers
 * A & B, and global address registers A每D.
 */
__inline void dma_global_init(/*RET: VOID function */
    unsigned int auxcr, /*IN:Value for Aux Control Register */
    unsigned int gcra, /*IN:Value for Global Cnt Reload Reg A*/
    unsigned int gcrb, /*IN:Value for Global Cnt Reload Reg B*/
    unsigned int gndxa, /*IN:Value for Global Idx Reg A */
    unsigned int gndxb, /*IN:Value for Global Idx Reg B */
    unsigned int gaddra, /*IN:Value for Global Addr Reg A */
    unsigned int gaddrb, /*IN:Value for Global Addr Reg B */
    unsigned int gaddrc, /*IN:Value for Global Addr Reg C */
    unsigned int gaddrd /*IN:Value for Global Addr Reg D */
);

/*
 * dma_reset 每 Reset all four DMA channels.
 *
 * This function resets the specified DMA channel by initializing
 * channel control registers to their default values
 */

#ifdef __INLINE
#undef __INLINE
#endif

#endif /* _DMA_H_ */
