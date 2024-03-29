/*
 * elf32.h
 *
 * Basic Data Structures for 32-bit ELF Object Format Files
 *
 * The data structures in this file come primarily from this specification:
 *
 *    Tool Interface Standard (TIS)
 *    Executable and Linking Format (ELF) Specification
 *    Version 1.2
 *
 *    TIS Committee
 *    May 1995
 *
 * Additions and enhancements from this specification are also included:
 *
 *    System V Application Binary Interface
 *    DRAFT 17
 *    December 2003
 *
 *    http://sco.com/developers/gabi/2003-12-17/contents.html
 *
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ELF32_H
#define ELF32_H

#include <inttypes.h>

/*---------------------------------------------------------------------------*/
/* 32-Bit Data Types (Figure 1-2, page 1-2)                                  */
/*---------------------------------------------------------------------------*/
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

/*****************************************************************************/
/* ELF Header                                                                */
/* PP. 1-4                                                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* ELF Identification Indexes (indexes into Elf32_Ehdr.e_ident[] below)      */
/*---------------------------------------------------------------------------*/
enum {
	EI_MAG0 = 0, /* File identification          */
	EI_MAG1 = 1, /* File identification          */
	EI_MAG2 = 2, /* File identification          */
	EI_MAG3 = 3, /* File identification          */
	EI_CLASS = 4, /* File class                   */
	EI_DATA = 5, /* Data encoding                */
	EI_VERSION = 6, /* File version                 */
	EI_OSABI = 7, /* Operating system / ABI       */
	EI_ABIVERSION = 8, /* ABI version                  */
	EI_PAD = 9, /* Start of padding bytes       */
	EI_NIDENT = 16 /* Size of Elf32_Ehdr.e_ident[] */
};

/*---------------------------------------------------------------------------*/
/* ELF Header Data Structure                                                 */
/*---------------------------------------------------------------------------*/
struct Elf32_Ehdr {
	uint8_t e_ident[EI_NIDENT]; /* ELF Magic Number                     */
	Elf32_Half e_type; /* Object File Type                     */
	Elf32_Half e_machine; /* Target Processor                     */
	Elf32_Word e_version; /* Object File Version                  */
	Elf32_Addr e_entry; /* Entry Point                          */
	Elf32_Off e_phoff; /* Program Header Table Offset          */
	Elf32_Off e_shoff; /* Section Header Table Offset          */
	Elf32_Word e_flags; /* Processor-Specific Flags             */
	Elf32_Half e_ehsize; /* Size of ELF header                   */
	Elf32_Half e_phentsize; /* Size of a Program Header             */
	Elf32_Half e_phnum; /* # Entries in Program Header Table    */
	Elf32_Half e_shentsize; /* Size of a Section Header             */
	Elf32_Half e_shnum; /* # Entries in Section Header Table    */
	Elf32_Half e_shstrndx; /* Section Name String Table Section    */
};

/*---------------------------------------------------------------------------*/
/* Object File Types (value of "e_type")                                     */
/*---------------------------------------------------------------------------*/
enum {
	ET_NONE = 0, /* No file type                   */
	ET_REL = 1, /* Relocatable file               */
	ET_EXEC = 2, /* Executable file                */
	ET_DYN = 3, /* Shared object file             */
	ET_CORE = 4, /* Core file                      */
	ET_LOOS = 0xfe00, /* First OS-specific value        */
	ET_HIPS = 0xfeff, /* Last  OS-specific value        */
	ET_LOPROC = 0xff00, /* First processor-specific value */
	ET_HIPROC = 0xffff /* Last  processor-specific value */
};

/*---------------------------------------------------------------------------*/
/* Target Processors (value of "e_machine")                                  */
/*---------------------------------------------------------------------------*/
enum {
	EM_NONE = 0, /* No machine                                      */
	EM_M32 = 1, /* AT&T WE 32100                                   */
	EM_SPARC = 2, /* SPARC                                           */
	EM_386 = 3, /* Intel 80386                                     */
	EM_68K = 4, /* Motorola 68000                                  */
	EM_88K = 5, /* Motorola 88000                                  */
	EM_860 = 7, /* Intel 80860                                     */
	EM_MIPS = 8, /* MIPS I Architecture                             */
	EM_S370 = 9, /* IBM System/370 Processor                        */
	EM_MIPS_RS3_LE = 10, /* MIPS RS3000 Little-endian                       */
	EM_PARISC = 15, /* Hewlett-Packard PA-RISC                         */
	EM_VPP500 = 17, /* Fujitsu VPP500                                  */
	EM_SPARC32PLUS = 18, /* Enhanced instruction set SPARC                  */
	EM_960 = 19, /* Intel 80960                                     */
	EM_PPC = 20, /* PowerPC                                         */
	EM_PPC64 = 21, /* 64-bit PowerPC                                  */
	EM_S390 = 22, /* IBM System/390 Processor                        */
	EM_V800 = 36, /* NEC V800                                        */
	EM_FR20 = 37, /* Fujitsu FR20                                    */
	EM_RH32 = 38, /* TRW RH-32                                       */
	EM_RCE = 39, /* Motorola RCE                                    */
	EM_ARM = 40, /* Advanced RISC Machines ARM                      */
	EM_ALPHA = 41, /* Digital Alpha                                   */
	EM_SH = 42, /* Hitachi SH                                      */
	EM_SPARCV9 = 43, /* SPARC Version 9                                 */
	EM_TRICORE = 44, /* Siemens TriCore embedded processor              */
	EM_ARC = 45, /* "Argonaut RISC Core, Argonaut Technologies Inc. */
	EM_H8_300 = 46, /* Hitachi H8/300                                  */
	EM_H8_300H = 47, /* Hitachi H8/300H                                 */
	EM_H8S = 48, /* Hitachi H8S                                     */
	EM_H8_500 = 49, /* Hitachi H8/500                                  */
	EM_IA_64 = 50, /* Intel IA-64 processor architecture              */
	EM_MIPS_X = 51, /* Stanford MIPS-X                                 */
	EM_COLDFIRE = 52, /* Motorola ColdFire                               */
	EM_68HC12 = 53, /* Motorola M68HC12                                */
	EM_MMA = 54, /* Fujitsu MMA Multimedia Accelerator              */
	EM_PCP = 55, /* Siemens PCP                                     */
	EM_NCPU = 56, /* Sony nCPU embedded RISC processor               */
	EM_NDR1 = 57, /* Denso NDR1 microprocessor                       */
	EM_STARCORE = 58, /* Motorola Star*Core processor                    */
	EM_ME16 = 59, /* Toyota ME16 processor                           */
	EM_ST100 = 60, /* STMicroelectronics ST100 processor              */
	EM_TINYJ = 61, /* Advanced Logic Corp. TinyJ embedded processor f */
	EM_X86_64 = 62, /* AMD x86-64 architecture                         */
	EM_PDSP = 63, /* Sony DSP Processor                              */
	EM_PDP10 = 64, /* Digital Equipment Corp. PDP-10                  */
	EM_PDP11 = 65, /* Digital Equipment Corp. PDP-11                  */
	EM_FX66 = 66, /* Siemens FX66 microcontroller                    */
	EM_ST9PLUS = 67, /* STMicroelectronics ST9+ 8/16 bit microcontrolle */
	EM_ST7 = 68, /* STMicroelectronics ST7 8-bit microcontroller    */
	EM_68HC16 = 69, /* Motorola MC68HC16 Microcontroller               */
	EM_68HC11 = 70, /* Motorola MC68HC11 Microcontroller               */
	EM_68HC08 = 71, /* Motorola MC68HC08 Microcontroller               */
	EM_68HC05 = 72, /* Motorola MC68HC05 Microcontroller               */
	EM_SVX = 73, /* Silicon Graphics SVx                            */
	EM_ST19 = 74, /* STMicroelectronics ST19 8-bit microcontroller   */
	EM_VAX = 75, /* Digital VAX                                     */
	EM_CRIS = 76, /* Axis Communications 32-bit embedded processor   */
	EM_JAVELIN = 77, /* Infineon Technologies 32-bit embedded processor */
	EM_FIREPATH = 78, /* Element 14 64-bit DSP Processor                 */
	EM_ZSP = 79, /* LSI Logic 16-bit DSP Processor                  */
	EM_MMIX = 80, /* Donald Knuth's educational 64-bit processor     */
	EM_HUANY = 81, /* Harvard University machine-independent object f */
	EM_PRISM = 82, /* SiTera Prism                                    */
	EM_AVR = 83, /* Atmel AVR 8-bit microcontroller                 */
	EM_FR30 = 84, /* Fujitsu FR30                                    */
	EM_D10V = 85, /* Mitsubishi D10V                                 */
	EM_D30V = 86, /* Mitsubishi D30V                                 */
	EM_V850 = 87, /* NEC v850                                        */
	EM_M32R = 88, /* Mitsubishi M32R                                 */
	EM_MN10300 = 89, /* Matsushita MN10300                              */
	EM_MN10200 = 90, /* Matsushita MN10200                              */
	EM_PJ = 91, /* picoJava                                        */
	EM_OPENRISC = 92, /* OpenRISC 32-bit embedded processor              */
	EM_ARC_A5 = 93, /* ARC Cores Tangent-A5                            */
	EM_XTENSA = 94, /* Tensilica Xtensa Architecture                   */
	EM_VIDEOCORE = 95, /* Alphamosaic VideoCore processor                 */
	EM_TMM_GPP = 96, /* Thompson Multimedia General Purpose Processor   */
	EM_NS32K = 97, /* National Semiconductor 32000 series             */
	EM_TPC = 98, /* Tenor Network TPC processor                     */
	EM_SNP1K = 99, /* Trebia SNP 1000 processor                       */
	EM_ST200 = 100, /* STMicroelectronics (www.st.com) ST200 microcont */
	EM_IP2K = 101, /* Ubicom IP2xxx microcontroller family            */
	EM_MAX = 102, /* MAX Processor                                   */
	EM_CR = 103, /* National Semiconductor CompactRISC microprocess */
	EM_F2MC16 = 104, /* Fujitsu F2MC16                                  */
	EM_MSP430 = 105, /* Texas Instruments embedded microcontroller msp4 */
	EM_BLACKFIN = 106, /* Analog Devices Blackfin (DSP) processor         */
	EM_SE_C33 = 107, /* S1C33 Family of Seiko Epson processors          */
	EM_SEP = 108, /* Sharp embedded microprocessor                   */
	EM_ARCA = 109, /* Arca RISC Microprocessor                        */
	EM_UNICORE = 110, /* Microprocessor series from PKU-Unity Ltd. and M */

	/*------------------------------------------------------------------------*/
	/* ELF Magic Numbers Reserved For Texas Instruments                       */
	/*                                                                        */
	/*   The magic numbers 140-159 were reserved through SCO to be included   */
	/*   in the official ELF specification.  Please see Don Darling           */
	/*   regarding any changes or allocation of the numbers below.            */
	/*                                                                        */
	/*   When we allocate a number for use, SCO needs to be notified so they  */
	/*   can update the ELF specification accordingly.                        */
	/*------------------------------------------------------------------------*/
	EM_TI_C6000 = 140, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED02 = 141, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED03 = 142, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED04 = 143, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED05 = 144, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED06 = 145, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED07 = 146, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED08 = 147, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED09 = 148, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED10 = 149, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED11 = 150, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED12 = 151, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED13 = 152, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED14 = 153, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED15 = 154, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED16 = 155, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED17 = 156, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED18 = 157, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED19 = 158, /* Reserved for Texas Instruments; unused          */
	EM_TI_UNUSED20 = 159 /* Reserved for Texas Instruments; unused          */
};

/*---------------------------------------------------------------------------*/
/* Object File Version (value of "e_version")                                */
/*---------------------------------------------------------------------------*/
enum {
	EV_NONE = 0, /* Invalid version */
	EV_CURRENT = 1 /* Current version */
};

/*****************************************************************************/
/* ELF Identification                                                        */
/* PP. 1-6                                                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Identification Values for ELF Files                                       */
/*---------------------------------------------------------------------------*/

/* EI_MAG0 to EI_MAG3 */
enum {
	ELFMAG0 = 0x7f, /* e_ident[EI_MAG0] */
	ELFMAG1 = 'E', /* e_ident[EI_MAG1] */
	ELFMAG2 = 'L', /* e_ident[EI_MAG2] */
	ELFMAG3 = 'F' /* e_ident[EI_MAG3] */
};

/* EI_CLASS */
enum {
	ELFCLASSNONE = 0, /* Invalid class  */
	ELFCLASS32 = 1, /* 32-bit objects */
	ELFCLASS64 = 2 /* 64-bit objects */
};

/* EI_DATA */
enum {
	ELFDATANONE = 0, /* Invalid data encoding */
	ELFDATA2LSB = 1, /* Little-endian data    */
	ELFDATA2MSB = 2 /* Big-endian data       */
};

/* EI_OSABI */
enum {
	ELFOSABI_NONE = 0, /* No extensions or unspecified       */
	ELFOSABI_HPUX = 1, /* Hewlett-Packard HP-UX              */
	ELFOSABI_NETBSD = 2, /* NetBSD                             */
	ELFOSABI_LINUX = 3, /* Linux                              */
	ELFOSABI_SOLARIS = 6, /* Sun Solaris                        */
	ELFOSABI_AIX = 7, /* AIX                                */
	ELFOSABI_IRIX = 8, /* IRIX                               */
	ELFOSABI_FREEBSD = 9, /* FreeBSD                            */
	ELFOSABI_TRU64 = 10, /* Compaq TRU64 UNIX                  */
	ELFOSABI_MODESTO = 11, /* Novell Modesto                     */
	ELFOSABI_OPENBSD = 12, /* Open BSD                           */
	ELFOSABI_OPENVMS = 13, /* Open VMS                           */
	ELFOSABI_NSK = 14, /* Hewlett-Packard Non-Stop Kernel    */
	ELFOSABI_AROS = 15 /* Amiga Research OS                  */
};

/*****************************************************************************/
/* Program Header                                                            */
/* PP. 2-2                                                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Program Header Data Structure                                             */
/*---------------------------------------------------------------------------*/
struct Elf32_Phdr {
	Elf32_Word p_type; /* Segment type              */
	Elf32_Off p_offset; /* Segment file offset       */
	Elf32_Addr p_vaddr; /* Segment virtual address   */
	Elf32_Addr p_paddr; /* Segment physical address  */
	Elf32_Word p_filesz; /* Segment file image size   */
	Elf32_Word p_memsz; /* Segment memory image size */
	Elf32_Word p_flags; /* Segment flags             */
	Elf32_Word p_align; /* Segment alignment         */
};

/*---------------------------------------------------------------------------*/
/* Segment Types (value of "p_type")                                         */
/*---------------------------------------------------------------------------*/
enum {
	PT_NULL = 0, /* Unused table entry                           */
	PT_LOAD = 1, /* Loadable segment                             */
	PT_DYNAMIC = 2, /* Dynamic linking information                  */
	PT_INTERP = 3, /* Interpreter path string location             */
	PT_NOTE = 4, /* Location and size of auxiliary information   */
	PT_SHLIB = 5, /* Shared library information                   */
	PT_PHDR = 6, /* Location and size of program header table    */
	PT_TLS = 7, /* Specifies the Thread-Local Storage template  */
	PT_LOOS = 0x60000000, /* First OS-specific value                      */
	PT_HIOS = 0x6fffffff, /* Last  OS-specific value                      */
	PT_LOPROC = 0x70000000, /* First processor-specific value               */
	PT_HIPROC = 0x7fffffff /* Last  processor-specific value               */
};

/*---------------------------------------------------------------------------*/
/* Segment Permissions (value of "p_flags")                                  */
/*---------------------------------------------------------------------------*/
enum {
	PF_X = 0x1, /* Execute                 */
	PF_W = 0x2, /* Write                   */
	PF_R = 0x4, /* Read                    */
	PF_MASKOS = 0x0ff00000, /* OS-specific mask        */
	PF_MASKPROC = 0xf0000000 /* Processor-specific mask */
};

/*****************************************************************************/
/* Sections                                                                  */
/* PP. 1-9                                                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Section Header Data Structure                                             */
/*---------------------------------------------------------------------------*/
struct Elf32_Shdr {
	Elf32_Word sh_name; /* Section name (offset into string section)   */
	Elf32_Word sh_type; /* Section type                                */
	Elf32_Word sh_flags; /* Section flags                               */
	Elf32_Addr sh_addr; /* Address in memory image                     */
	Elf32_Off sh_offset; /* File offset of section data                 */
	Elf32_Word sh_size; /* Size of the section in bytes                */
	Elf32_Word sh_link; /* Link to the section header table            */
	Elf32_Word sh_info; /* Extra information depending on section type */
	Elf32_Word sh_addralign; /* Address alignment constraints               */
	Elf32_Word sh_entsize; /* Size of fixed-size entries in section       */
};

/*---------------------------------------------------------------------------*/
/* Special Section Indexes                                                   */
/*---------------------------------------------------------------------------*/
enum {
	SHN_UNDEF = 0, /* Referenced by undefined values          */
	SHN_LORESERVE = 0xff00, /* First reserved index                    */
	SHN_LOPROC = 0xff00, /* First processor-specific index          */
	SHN_HIPROC = 0xff1f, /* Last  processor-specific index          */
	SHN_LOOS = 0xff20, /* First OS-specific index                 */
	SHN_HIOS = 0xff3f, /* Last  OS-specific index                 */
	SHN_ABS = 0xfff1, /* Referenced by absolute values           */
	SHN_COMMON = 0xfff2, /* Referenced by common values             */
	SHN_XINDEX = 0xffff, /* Indirect index reference (escape value) */
	SHN_HIRESERVE = 0xffff /* Last reserved index                     */
};

/*---------------------------------------------------------------------------*/
/* Section Types (value of "sh_type")                                        */
/*---------------------------------------------------------------------------*/
enum {
	SHT_NULL = 0, /* Inactive section                      */
	SHT_PROGBITS = 1, /* Application-specific information      */
	SHT_SYMTAB = 2, /* Symbol table                          */
	SHT_STRTAB = 3, /* String table                          */
	SHT_RELA = 4, /* Relocation entries (explicit addends) */
	SHT_HASH = 5, /* Symbol hash table                     */
	SHT_DYNAMIC = 6, /* Dynamic linking information           */
	SHT_NOTE = 7, /* Miscellaneous information             */
	SHT_NOBITS = 8, /* Contains no data in file              */
	SHT_REL = 9, /* Relocation entries (no expl. addends) */
	SHT_SHLIB = 10, /* Shared library                        */
	SHT_DYNSYM = 11, /* Dynamic symbol table                  */
	SHT_INIT_ARRAY = 14, /* Pointers to initialization functions  */
	SHT_FINI_ARRAY = 15, /* Pointers to termination functions     */
	SHT_PREINIT_ARRAY = 16, /* Pointers to pre-init functions        */
	SHT_GROUP = 17, /* Section group                         */
	SHT_SYMTAB_SHNDX = 18, /* Section indexes for SHN_XINDEX refs.  */
	SHT_LOOS = 0x60000000, /* First OS-specific type                */
	SHT_HIOS = 0x6fffffff, /* Last  OS-specific type                */
	SHT_LOPROC = 0x70000000, /* First processor-specific type         */
	SHT_HIPROC = 0x7fffffff, /* Last  processor-specific type         */
	SHT_LOUSER = 0x80000000, /* First application-specific type       */
	SHT_HIUSER = 0xffffffff /* Last  application-specific type       */
};

/*---------------------------------------------------------------------------*/
/* Section Attribute Flags (value of "sh_flags")                             */
/*---------------------------------------------------------------------------*/
enum {
	SHF_WRITE = 0x1, /* Writable during process execution  */
	SHF_ALLOC = 0x2, /* Loaded into processor memory       */
	SHF_EXECINSTR = 0x4, /* Contains executable instructions   */
	SHF_MERGE = 0x10, /* Can be merged                      */
	SHF_STRINGS = 0x20, /* Contains null-terminated strings   */
	SHF_INFO_LINK = 0x40, /* sh_info contains a section index   */
	SHF_LINK_ORDER = 0x80, /* Maintain section ordering          */
	SHF_OS_NONCONFORMING = 0x100, /* OS-specific processing required    */
	SHF_GROUP = 0x200, /* Member of a section group          */
	SHF_TLS = 0x400, /* Contains Thread-Local Storage      */
	SHF_MASKOS = 0x0ff00000, /* Mask of OS-specific flags          */
	SHF_MASKPROC = 0xf0000000 /* Mask for processor-specific flags  */
};

/*---------------------------------------------------------------------------*/
/* Section Group Flags                                                       */
/*---------------------------------------------------------------------------*/
enum {
	GRP_COMDAT = 0x1, /* Common data; only one is kept by linker */
	GRP_MASKOS = 0x0ff00000, /* Mask for OS-specific group flags        */
	GRP_MASKPROC = 0xf0000000 /* Mask for processor-specific group flags */
};

/*****************************************************************************/
/* Symbol Table                                                              */
/* PP. 1-18                                                                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Symbol Table Entry Data Structure                                         */
/*---------------------------------------------------------------------------*/
struct Elf32_Sym {
	Elf32_Word st_name; /* String table offset for symbol name */
	Elf32_Addr st_value; /* Symbol value                        */
	Elf32_Word st_size; /* Symbol size                         */
	uint8_t st_info; /* Symbol type and binding             */
	uint8_t st_other; /* Symbol visibility                   */
	Elf32_Half st_shndx; /* Symbol type / defining section      */
};

/*---------------------------------------------------------------------------*/
/* Undefined Symbol Index                                                    */
/*---------------------------------------------------------------------------*/
enum {
	STN_UNDEF = 0 /* First symbol table entry is always undefined */
};

/*---------------------------------------------------------------------------*/
/* Symbol Binding and Type Utility Functions.                                */
/*---------------------------------------------------------------------------*/
static inline uint8_t ELF32_ST_BIND(uint8_t i) {
	return (i >> 4);
}
static inline uint8_t ELF32_ST_TYPE(uint8_t i) {
	return (i & 0xf);
}
static inline uint8_t ELF32_ST_INFO(uint8_t b, uint8_t t) {
	return ((b << 4) + (t & 0xf));
}
static inline uint8_t ELF32_ST_VISIBILITY(uint8_t o) {
	return (o & 0x3);
}

/*---------------------------------------------------------------------------*/
/* Symbol Binding (value returned by ELF32_ST_BIND())                        */
/*---------------------------------------------------------------------------*/
enum {
	STB_LOCAL = 0, /* Symbol does not have external linkage */
	STB_GLOBAL = 1, /* Symbol has external linkage           */
	STB_WEAK = 2, /* Symbol has weak external linkage      */
	STB_LOOS = 10, /* First OS-specific binding             */
	STB_HIOS = 12, /* Last  OS-specific binding             */
	STB_LOPROC = 13, /* First processor-specific binding      */
	STB_HIPROC = 15 /* Last  processor-specific binding      */
};

/*---------------------------------------------------------------------------*/
/* Symbol Types (value returned by ELF32_ST_TYPE())                          */
/*---------------------------------------------------------------------------*/
enum {
	STT_NOTYPE = 0, /* Unspecified type                        */
	STT_OBJECT = 1, /* Associated with a data object           */
	STT_FUNC = 2, /* Associated with executable code         */
	STT_SECTION = 3, /* Associated with a section               */
	STT_FILE = 4, /* Associated with a source file           */
	STT_COMMON = 5, /* Labels an uninitialized common block    */
	STT_TLS = 6, /* Specifies a thread-local storage entity */
	STT_LOOS = 10, /* First OS-specific type                  */
	STT_HIOS = 12, /* Last  OS-specific type                  */
	STT_LOPROC = 13, /* First processor-specific type           */
	STT_HIPROC = 15 /* Last  processor-specific type           */
};

/*---------------------------------------------------------------------------*/
/* Symbol Visibility (value returned by ELF32_ST_VISIBILITY())               */
/*---------------------------------------------------------------------------*/
enum {
	STV_DEFAULT = 0, /* Visibility specified by binding type               */
	STV_INTERNAL = 1, /* Like STV_HIDDEN, with processor-specific semantics */
	STV_HIDDEN = 2, /* Not visible to other components                    */
	STV_PROTECTED = 3 /* Visible in other components but not preemptable    */
};

/*****************************************************************************/
/* Relocation                                                                */
/* PP. 1-22                                                                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Relocation Entries Data Structures                                        */
/*---------------------------------------------------------------------------*/
struct Elf32_Rel {
	Elf32_Addr r_offset; /* Offset of the relocatable value in the section */
	Elf32_Word r_info; /* Symbol table index and relocation type         */
};

struct Elf32_Rela {
	Elf32_Addr r_offset; /* Offset of the relocatable value in the section */
	Elf32_Word r_info; /* Symbol table index and relocation type         */
	Elf32_Sword r_addend; /* Constant addend used to compute new value      */
};

/*---------------------------------------------------------------------------*/
/* Relocation Symbol and Type Utility Functions.                             */
/*---------------------------------------------------------------------------*/
static inline uint32_t ELF32_R_SYM(uint32_t i) {
	return (i >> 8);
}
static inline uint8_t ELF32_R_TYPE(uint32_t i) {
	return (i & 0xFF);
}
static inline uint32_t ELF32_R_INFO(uint32_t s, uint8_t t) {
	return ((s << 8) + t);
}

/*****************************************************************************/
/* Dynamic Section                                                           */
/* PP. 2-8                                                                   */
/*****************************************************************************/
struct Elf32_Dyn {
	Elf32_Sword d_tag;
	union {
		Elf32_Word d_val;
		Elf32_Addr d_ptr;
	} d_un;
};

/* Name                 Value           d_un        Executable  Shared Obj. */
/* ----                 -----           ----        ----------  ----------- */
enum {
	DT_NULL = 0, /* ignored     mandatory   mandatory   */
	DT_NEEDED = 1, /* d_val       optional    optional    */
	DT_PLTRELSZ = 2, /* d_val       optional    optional    */
	DT_PLTGOT = 3, /* d_ptr       optional    optional    */
	DT_HASH = 4, /* d_ptr       mandatory   mandatory   */
	DT_STRTAB = 5, /* d_ptr       mandatory   mandatory   */
	DT_SYMTAB = 6, /* d_ptr       mandatory   mandatory   */
	DT_RELA = 7, /* d_ptr       mandatory   optional    */
	DT_RELASZ = 8, /* d_val       mandatory   optional    */
	DT_RELAENT = 9, /* d_val       mandatory   optional    */
	DT_STRSZ = 10, /* d_val       mandatory   mandatory   */
	DT_SYMENT = 11, /* d_val       mandatory   mandatory   */
	DT_INIT = 12, /* d_ptr       optional    optional    */
	DT_FINI = 13, /* d_ptr       optional    optional    */
	DT_SONAME = 14, /* d_val       ignored     optional    */
	DT_RPATH = 15, /* d_val       optional    ignored     */
	DT_SYMBOLIC = 16, /* ignored     ignored     optional    */
	DT_REL = 17, /* d_ptr       mandatory   optional    */
	DT_RELSZ = 18, /* d_val       mandatory   optional    */
	DT_RELENT = 19, /* d_val       mandatory   optional    */
	DT_PLTREL = 20, /* d_val       optional    optional    */
	DT_DEBUG = 21, /* d_ptr       optional    ignored     */
	DT_TEXTREL = 22, /* ignored     optional    optional    */
	DT_JMPREL = 23, /* d_ptr       optional    optional    */
	DT_BIND_NOW = 24, /* ignored     optional    optional    */
	DT_INIT_ARRAY = 25, /* d_ptr       optional    optional    */
	DT_FINI_ARRAY = 26, /* d_ptr       optional    optional    */
	DT_INIT_ARRAYSZ = 27, /* d_val       optional    optional    */
	DT_FINI_ARRAYSZ = 28, /* d_val       optional    optional    */
	DT_RUNPATH = 29, /* d_val       optional    optional    */
	DT_FLAGS = 30, /* d_val       optional    optional    */
	DT_ENCODING = 32, /* unspecified unspecified unspecified */
	DT_PREINIT_ARRAY = 32, /* d_ptr       optional    ignored     */
	DT_PREINIT_ARRAYSZ = 33, /* d_val       optional    ignored     */
	DT_LOOS = 0x60000000, /* unspecified unspecified unspecified */
	DT_HIOS = 0x6ffff000, /* unspecified unspecified unspecified */
	DT_LOPROC = 0x70000000, /* unspecified unspecified unspecified */
	DT_HIPROC = 0x7fffffff /* unspecified unspecified unspecified */
};

/*---------------------------------------------------------------------------*/
/* DT_FLAGS values.                                                          */
/*---------------------------------------------------------------------------*/
enum {
	DF_ORIGIN = 0x01, /* loaded object may reference $ORIGIN subst. string */
	DF_SYMBOLIC = 0x02, /* changes dynamic linker symbol resolution          */
	DF_TEXTREL = 0x04, /* do not allow relocation of non-writable segments  */
	DF_BIND_NOW = 0x08, /* don't use lazy binding                            */
	DF_STATIC_TLS = 0x10, /* do not load this file dynamically                 */
	DF_DIRECT_DEPENDENT = 0x20, /* limit global sym lookup to dependent list   */
	DF_WORLD = 0x40 /* Linux style global sym lookup, breadth-first      */
};

/*---------------------------------------------------------------------------*/
/* Dynamic Tag Database.                                                     */
/*---------------------------------------------------------------------------*/

/* Specifiers for which d_un union member to use                             */

enum {
	EDYN_UNTYPE_IGNORED,
	EDYN_UNTYPE_VAL,
	EDYN_UNTYPE_PTR,
	EDYN_UNTYPE_UNSPECIFIED
};

/* Specifiers for executable/shared object file requirements                 */

enum {
	EDYN_TAGREQ_IGNORED,
	EDYN_TAGREQ_MANDATORY,
	EDYN_TAGREQ_OPTIONAL,
	EDYN_TAGREQ_UNSPECIFIED
};

/* Data structure for one dynamic tag database entry                         */

struct EDYN_TAG {
	const char* d_tag_name; /* tag name string                     */
	Elf32_Sword d_tag_value; /* DT_* tag value                      */
	Elf32_Word d_untype; /* which d_un union member to use      */
	Elf32_Word d_exec_req; /* requirement for executable files    */
	Elf32_Word d_shared_req; /* requirement for shared object files */
};

extern const struct EDYN_TAG EDYN_TAG_DB[];

/*****************************************************************************/
/* Special Section Database                                                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Special Section Names                                                     */
/*---------------------------------------------------------------------------*/
#define ESCN_BSS_name            ".bss"
#define ESCN_COMMENT_name        ".comment"
#define ESCN_DATA1_name          ".data1"
#define ESCN_DATA_name           ".data"
#define ESCN_DEBUG_name          ".debug"
#define ESCN_DYNAMIC_name        ".dynamic"
#define ESCN_DYNSTR_name         ".dynstr"
#define ESCN_DYNSYM_name         ".dynsym"
#define ESCN_FINI_ARRAY_name     ".fini_array"
#define ESCN_FINI_name           ".fini"
#define ESCN_GOT_name            ".got"
#define ESCN_HASH_name           ".hash"
#define ESCN_INIT_ARRAY_name     ".init_array"
#define ESCN_INIT_name           ".init"
#define ESCN_INTERP_name         ".interp"
#define ESCN_LINE_name           ".line"
#define ESCN_NOTE_name           ".note"
#define ESCN_PLT_name            ".plt"
#define ESCN_PREINIT_ARRAY_name  ".preinit_array"
#define ESCN_RELA_name           ".rela"
#define ESCN_REL_name            ".rel"
#define ESCN_RODATA1_name        ".rodata1"
#define ESCN_RODATA_name         ".rodata"
#define ESCN_SHSTRTAB_name       ".shstrtab"
#define ESCN_STRTAB_name         ".strtab"
#define ESCN_SYMTAB_SHNDX_name   ".symtab_shndx"
#define ESCN_SYMTAB_name         ".symtab"
#define ESCN_TBSS_name           ".tbss"
#define ESCN_TDATA1_name         ".tdata1"
#define ESCN_TDATA_name          ".tdata"
#define ESCN_TEXT_name           ".text"
#define ESCN_ATTRIBUTES_name     "__TI_build_attributes"
#define ESCN_ICODE_name          "__TI_ICODE"
#define ESCN_XREF_name           "__TI_XREF"

/*---------------------------------------------------------------------------*/
/* Special Section Information Data Structure.                               */
/*---------------------------------------------------------------------------*/
struct ESCN {
	const char *name;
	Elf32_Word sh_type;
	Elf32_Word sh_entsize;
	Elf32_Word sh_flags;
};

extern const struct ESCN ESCN_DB[];

#endif /* ELF32_H */
