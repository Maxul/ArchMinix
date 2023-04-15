/******************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

/******************************************************************************	
 *
 * File Name:   evmc66x_spi.h
 *
 * Description: This is the header file for SPI API's.
 *
 ******************************************************************************/
#ifndef SPI_H_
#define SPI_H_

/* ------------------------------------------------------------------------ *
 *  Include Files                                                           *
 * ------------------------------------------------------------------------ */
//#include "types.h"
#include "ti/csl/cslr_device.h"
#include "ti/csl/cslr_spi.h"

/* ------------------------------------------------------------------------ *
 *  SPI Controller                                                          *
 * ------------------------------------------------------------------------ */
#define SPI_BASE                CSL_SPI_REGS
#define SPI_SPIGCR0             *( volatile Uint32* )( SPI_BASE + 0x0 )
#define SPI_SPIGCR1             *( volatile Uint32* )( SPI_BASE + 0x4 )
#define SPI_SPIINT0             *( volatile Uint32* )( SPI_BASE + 0x8 )
#define SPI_SPILVL              *( volatile Uint32* )( SPI_BASE + 0xc )
#define SPI_SPIFLG              *( volatile Uint32* )( SPI_BASE + 0x10 )
#define SPI_SPIPC0              *( volatile Uint32* )( SPI_BASE + 0x14 )
#define SPI_SPIDAT0             *( volatile Uint32* )( SPI_BASE + 0x38 )
#define SPI_SPIDAT1             *( volatile Uint32* )( SPI_BASE + 0x3c )
#define SPI_SPIBUF              *( volatile Uint32* )( SPI_BASE + 0x40 )
#define SPI_SPIEMU              *( volatile Uint32* )( SPI_BASE + 0x44 )
#define SPI_SPIDELAY            *( volatile Uint32* )( SPI_BASE + 0x48 )
#define SPI_SPIDEF              *( volatile Uint32* )( SPI_BASE + 0x4c )
#define SPI_SPIFMT0             *( volatile Uint32* )( SPI_BASE + 0x50 )
#define SPI_SPIFMT1             *( volatile Uint32* )( SPI_BASE + 0x54 )
#define SPI_SPIFMT2             *( volatile Uint32* )( SPI_BASE + 0x58 )
#define SPI_SPIFMT3             *( volatile Uint32* )( SPI_BASE + 0x5c )
#define SPI_INTVEC0             *( volatile Uint32* )( SPI_BASE + 0x60 )
#define SPI_INTVEC1             *( volatile Uint32* )( SPI_BASE + 0x64 )

#define SPI_NOR_CS              0           /* SPI Chip Select number for NOR*/
#define SPI_FPGA_CS             1           /* SPI Chip Select number for FPGA*/
#define SPI_MODULE_CLK          200000000   /* SYSCLK7  = CPU_Clk/6 in HZ */
#define SPI_MAX_FREQ            25000000    /* SPI Max frequency in Hz */
#define SPI_NOR_CHAR_LENTH      8           /* Number of bits per SPI trasfered data element for NOR flash */
#define SPI_FPGA_CHAR_LENTH     16          /* Number of bits per SPI trasfered data element for FPGA */


/* SPI error status */
#define SPI_STATUS        Uint32           /* SPI error status type */
#define SPI_EFAIL         (SPI_STATUS)-1   /* General failure code */
#define SPI_EOK           0                /* General success code */


SPI_STATUS 
spi_claim
(
    Uint32      cs,
    Uint32      freq
);

void 
spi_release
(
    void
);

SPI_STATUS 
spi_xfer
(
    Uint32              bitlen,
    Uint8*		        dout, 
    Uint8*              din, 
    Bool                flags
);

SPI_STATUS 
spi_cmd
(
    Uint8               cmd, 
    Uint8*              response, 
    Uint32              len
);

SPI_STATUS 
spi_cmd_read
(
    Uint8*              cmd,
    Uint32              cmd_len, 
    Uint8*              data, 
    Uint32              data_len
);

SPI_STATUS 
spi_cmd_write
(
    Uint8*              cmd, 
    Uint32              cmd_len,
    Uint8*              data, 
    Uint32              data_len
);

SPI_STATUS 
spi_read_word
(
    Uint16*             cmd_buf,
    Uint32              cmd_len, 
    Uint16*             data_buf, 
    Uint32              data_len
);

SPI_STATUS 
spi_write_word
(
    Uint16*             cmd_buf, 
    Uint32              cmd_len,
    Uint16*             data_buf, 
    Uint32              data_len
);

#endif /* SPI_H_ */
