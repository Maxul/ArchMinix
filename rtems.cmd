-c
-heap  0x8000
-stack 0x10000

MEMORY
{
	/* Local L2, 0.5~1MB*/
	VECTORS: 	o = 0x00800000  l = 0x00000200
	LL2_RW_DATA: 	o = 0x00800200  l = 0x0007FE00

	/* Shared L2 2~4MB*/
	SL2: 		o = 0x0C000000  l = 0x00400000

	/* External DDR3, upto 2GB per core */
    DDR3:           o = 0x80000000 l = 0x20000000   /* 1GB CE0 and CE1 external DDR3 SDRAM */
    DDR3_TEXT:      o = 0xA0000000 l = 0x20000000   /* 1GB CE0 and CE1 external DDR3 SDRAM */
    DDR3_WORKSPACE:           o = 0xC0000000 l = 0x40000000   /* 2GB CE0 and CE1 external DDR3 SDRAM */
}

SECTIONS
{
	.vector       	>    VECTORS
	.blob			> DDR3_WORKSPACE
	.WorkArea		> DDR3_WORKSPACE

	GROUP
	{
		.bss: {
			_bss_start = . ;
			*(.bss)
			_bss_end = .;
		}

		.dsbt
		.got
		.neardata
		.rodata
	}				>    LL2_RW_DATA

	// ���п�ִ�д���ͳ���
	.text           >    DDR3_TEXT

	// ȫ�ֱ����;�̬������C��ʼ����¼
	.cinit          >    LL2_RW_DATA

	// �ַ����ͳ�ʼ����ȫ�ֱ����;�̬����
	.const          >    DDR3_WORKSPACE

	// switch�����б�
	.switch         >    DDR3_WORKSPACE

	// ϵͳջ
	.stack          >    DDR3_WORKSPACE

	// �Ѵ洢��
	.sysmem         >    DDR3_WORKSPACE

	.far            >    DDR3_WORKSPACE
	.fardata        >    DDR3_WORKSPACE
	.cio            >    DDR3_WORKSPACE

	.cppi: load >> DDR3_WORKSPACE
    .qmss: load >> DDR3_WORKSPACE
	platform_lib: load >> DDR3_WORKSPACE
	.nimu_eth_ll2: load >> LL2_RW_DATA
    .resmgr_memregion: load >> LL2_RW_DATA align = 0x4
    .resmgr_handles: load >> LL2_RW_DATA align = 0x4
    .resmgr_pa: load >> LL2_RW_DATA align = 0x4

}
