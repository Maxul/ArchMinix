
#include "dma.h"

/*
 *  dma_init
 *
 *  Initialize channel specific control registers.
 *  This function is responsible for setting the primary control register,
 *  secondary control register, source address, destination address and
 *  transfer count for the specified DMA channel.
 */

void dma_init(
    unsigned short chan, 	/*IN: DMA channel number */
    unsigned int pri_ctrl, 	/*IN: Value to set primary cntl reg */
    unsigned int sec_ctrl, 	/*IN: Value to set sec cntl reg */
    unsigned int src_addr, 	/*IN: Value to set source addr reg */
    unsigned int dst_addr, 	/*IN: Value to set dest addr reg */
    unsigned int trans_ctr) /*IN: Value to set transfer counter */
{
    *((unsigned int *)(DMA_PRIMARY_CTRL_ADDR(chan)))   = pri_ctrl;
    *((unsigned int *)(DMA_SECONDARY_CTRL_ADDR(chan))) = sec_ctrl;
    *((unsigned int *)(DMA_SRC_ADDR_ADDR(chan)))       = src_addr;
    *((unsigned int *)(DMA_DEST_ADDR_ADDR(chan)))      = dst_addr;
    *((unsigned int *)(DMA_XFER_COUNTER_ADDR(chan)))   = trans_ctr;
}

/*
 *  dma_global_init
 *
 *  Initialize global control registers.
 *  This function is responsible for setting the DMA auxiliary control
 *  register, global count reload registers A & B, global index registers
 *  A & B, and global address registers A¨CD.
 */

void dma_global_init(
    unsigned int auxcr, 	/*IN:Value for Aux Control Register */
    unsigned int gcra, 	/*IN:Value for Global Cnt Reload Reg A*/
    unsigned int gcrb, 	/*IN:Value for Global Cnt Reload Reg B*/
    unsigned int gndxa, 	/*IN:Value for Global Idx Reg A */
    unsigned int gndxb, 	/*IN:Value for Global Idx Reg B */
    unsigned int gaddra, 	/*IN:Value for Global Addr Reg A */
    unsigned int gaddrb, 	/*IN:Value for Global Addr Reg B */
    unsigned int gaddrc, 	/*IN:Value for Global Addr Reg C */
    unsigned int gaddrd) 	/*IN:Value for Global Addr Reg D */
{
    DMA_AUXCR   = auxcr;
    DMA_GCR_A   = gcra;
    DMA_GCR_B   = gcrb;
    DMA_GNDX_A  = gndxa;
    DMA_GNDX_B  = gndxb;
    DMA_GADDR_A = gaddra;
    DMA_GADDR_B = gaddrb;
    DMA_GADDR_C = gaddrc;
    DMA_GADDR_D = gaddrd;
}


/*
 *  dma_reset
 *
 *  Reset all four DMA channels.
 *  This function resets all four DMA channels by initializing
 *  channel control registers to their default values
 */

void dma_reset( void )
{
    int chan;

    for (chan = 0; chan <= DMA_CH3; chan++)
    {
        *(unsigned int *)(DMA_PRIMARY_CTRL_ADDR(chan))   = 0x00;
        *(unsigned int *)(DMA_SECONDARY_CTRL_ADDR(chan)) = 0x80;
        *(unsigned int *)(DMA_SRC_ADDR_ADDR(chan))       = 0x00;
        *(unsigned int *)(DMA_DEST_ADDR_ADDR(chan))      = 0x00;
        *(unsigned int *)(DMA_XFER_COUNTER_ADDR(chan))   = 0x00;
    }

    DMA_AUXCR   = 0x00;
    DMA_GCR_A   = 0x00;
    DMA_GCR_B   = 0x00;
    DMA_GNDX_A  = 0x00;
    DMA_GNDX_B  = 0x00;
    DMA_GADDR_A = 0x00;
    DMA_GADDR_B = 0x00;
    DMA_GADDR_C = 0x00;
    DMA_GADDR_D = 0x00;
}
