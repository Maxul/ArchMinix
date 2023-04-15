/*===============================================================*\
| Project: RTEMS TMS320C6678                                      |
+-----------------------------------------------------------------+
| Partially based on the code references which are named below.   |
| Adaptions, modifications, enhancements and any recent parts of  |
| the code are:                                                   |
|                 Copyright (c) 2016                              |
|                 Maxul Lee <lmy2010lmy@gamil.com>                |
|                                                                 |
+-----------------------------------------------------------------+
| The license and distribution terms for this file may be         |
| found in the file LICENSE in this distribution or at            |
|                                                                 |
| http://www.rtems.org/license/LICENSE.                           |
|                                                                 |
+-----------------------------------------------------------------+
| this file contains the linker command                           |
\*===============================================================*/

-heap  0x10000
-stack 0x4000

#define CORE 0

MEMORY
{
	/* Shared L2 2~4MB*/
	SHRAM: 		o = 0x0C000000  l = 0x00400000

	/* Local L2, 0.5~1MB*/
	LL2_VECTORS: 	o = 0x10800000+(CORE*0x1000000)  l = 0x00000200
	LL2_RW_DATA: 	o = 0x10800200+(CORE*0x1000000)  l = 0x0007FE00

	/* External DDR3, upto 256MB per core */
	DDR3_RESERVED: 	o = 0x80000000+(CORE*0x10000000)  l = 0x01000000
	DDR3_DLBLOB: 	o = 0x82000000+(CORE*0x10000000)  l = 0x04000000
	DDR3_SYSTEM: 	o = 0x86000000+(CORE*0x10000000)  l = 0x01000000
	DDR3_WORKSPACE: o = 0x87000000+(CORE*0x10000000)  l = 0x09000000
}

SECTIONS
{
	// 中断向量表
	.vector       	> LL2_VECTORS

	.csl_vect		> DDR3_WORKSPACE

	.WorkArea		> DDR3_WORKSPACE

	.blob			> DDR3_DLBLOB

	GROUP
	{
		// 全局变量和局部变量保留的空间
		// 在程序上电时，.cinit空间中的数据复制出来并存储到.bss空间中
		.bss: {
			_bss_start = .;
			*(.bss)
			_bss_end = .;
		}

		.dsbt
		.got
		.neardata
		.rodata
	}				>    LL2_RW_DATA
	.far            >    LL2_RW_DATA
	.fardata        >    LL2_RW_DATA
	.cio            >    LL2_RW_DATA
	.init_array     >    LL2_RW_DATA

	// 所有可执行代码和常量
	.text           >    DDR3_SYSTEM

	// 全局变量和静态变量的C初始化记录
	.cinit          >    LL2_RW_DATA

	// 字符串常量和初始化的全局变量和静态变量
	.const          >    LL2_RW_DATA

	// switch声明列表
	.switch         >    LL2_RW_DATA

	// 系统栈
	.stack          >    DDR3_SYSTEM

	// 堆存储区
	.sysmem         >    DDR3_SYSTEM

	.cppi: load >> DDR3_SYSTEM
	.qmss: load >> DDR3_SYSTEM
	platform_lib: load >> DDR3_SYSTEM
	.nimu_eth_ll2: load >> LL2_RW_DATA
	.resmgr_memregion: load >> LL2_RW_DATA align = 0x4
	.resmgr_handles: load >> LL2_RW_DATA align = 0x4
	.resmgr_pa: load >> LL2_RW_DATA align = 0x4
}
