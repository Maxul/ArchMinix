# Docs

## RTEMS 移植说明

在充分理解RTEMS操作系统的设计以及比对同类嵌入式系统和通用系统后，笔者对源码树结构进行了调整，使其更加扁平化，有助于后续的模块化和管理维护。

测试人员将该版本与4.11 release进行比对，对部分函数的声明进行了修改，使其符合newlib的头文件声明原型。
但整体框架是一致的。

调整原则：尽量保留rtems的文件夹命名。

整体目录说明：

docs: 文档说明。

libcpu (cpukit/score/cpu)
- c66x：和体系结构相关(C6000系列DSP)的上下文切换、中断向量表以及部分汇编指令编写的辅助函数。

libbsp (c/src/lib/libbsp)
- console: UART串口的初始化、设备注册以及中断注册。
- clock: 系统时钟ISR的中断注册。
- timer: EVM开发板的0号定时器的初始化以及中断注册。在start.c中被调用。
- network：GMAC(PA)网卡的初始化、中断挂载以及链路层和驱动的接口设计。
- start: TI工具链下的入口地址位置main，包括控制寄存器的使能以及跳转到系统入口函数boot_card。

libfs (cpukit/libfs)
- devfs: 设备文件系统，给设备提供"/dev"目录。
- imfs: 内存文件系统。
- dosfs: FAT格式文件系统。
- pipe: FIFO设计，用于IMFS。

libblock (cpukit/libblock)
- 块设备，包括norflash支持。

libnetworking (cpukit/libnetworking)
- RTEMS使用的BSD协议栈，移植时主要注意TI工具链对结构体的对齐问题。

cpukit (内核代码)
- score: rtems内核核心。
- posix: POSIX接口。
- rtems: RTEMS接口。
- sapi: service API。
该目录下，笔者对组件进行了划分。

libc (cpukit/libcsupport) (运行时库)
- libcsupport: RTEMS修改的newlibc代码，和内核耦合性紧密。
- rts: TI运行时库。实际工程中使用预先编译好的rts6000_elf.lib。
- newlib: 和GCC和RTEMS原生配合的运行时。包含需要的libc和libm。

drivers
- drv (`pdk_C6678_*/packages/ti/drv`): EVM开发板上的固件和驱动。
- platform (`pdk_C6678_*/packages/ti/platform/evmc6678l/platform_lib/src`): 硬件驱动接口封装。
- csl (`pdk_C6678_*/packages/ti/csl`): 片上支持，包括片上对外设的寄存器读写接口。

loader
- 动态加载：支持多个库同时读取。

include
- 注意包含的先后顺序，先包含系统，然后是运行时库。
- 包括c/c++的支持，包含了newlib的头文件，并将和newlib不一致的定义注释。
	- rtems_4_11
		- 将cpukit下各个目录的include合并到一起。
		- rtems.h以及config.h和confdefs.h需要调整下位置。

	- cpu.h 和 cpuopts.h 是移植的关键。cpuopts.h 和CCS的predefine负责条件编译(一定要一致！！！)。
		- `newlibc_`
		- `rts_8_1_0`

## 中断控制器

TMS320C6678采用KeyStone架构的中断控制器INTC（Interrupt Controller）。

首先配置中断向量表，启动CPU中断功能。

TMS320C6678的CPU可接收15个中断，其中：1个硬件异常(EXCEP)、1个不可屏蔽中断(NMI)、1个复位(RESET)和12个可屏蔽中断(INT4 - INT15)，中断源支持最多128个。

每个核心通过事件控制器产生事件（Event），触发核间中断（IPI）和其他核心进行通信。

在TMS320C6678中，核间中断（IPC_LOCAL）默认对应91号事件，而核间中断属于可屏蔽中断，通过中断控制器可以映射到INT4 - INT15任意一个中断上。

为了实现核间中断，必须按以下方式进行设置：
（1）控制状态寄存器（CSR）中的全局中断使能位置为1，使能全局中断；
（2）中断使能寄存器（IER）中的NMIE位置为1，使能可屏蔽中断；
（3）中断使能寄存器（IER）将要映射的可屏蔽中断的相应位置1；
（4）选择91号事件作为中断源，映射事件到指定的物理中断号。中断发生后，将中断标志寄存器（IFR）的相应位置1。

中断发生时，由事先配置好的中断向量表跳入中断服务程序（ISR），完成核间通信。

时钟：本地时钟每个核各有一个。
时钟中断：4。一级事件64号，使用中断源：本地timer低32位寄存器，
核间中断：5
网卡中断：7

中断一共分为两级，由中断控制器接收事件，配置好给哪个核，。
如果核上的系统有相应的ISR，同时寄存器也使能，则响应该中断。
难点在于如何配置二级中断。

C6678中断控制器：
6678最多支持128个事件，被路由到12个CPU中断中(4到15)。该过程由CIC(片上中断控制器)完成，核内中断选择器由此减负。
前4核使用CIC0;
后4核使用CIC1；

对于一级中断而言，Primary Interrupts，可以不用CIC映射。
二级举例：(对应tms320c6678手册164页)

```
#define CSL_INTC0_UARTINT            (0x00000094)
#define CSL_INTC0_URXEVT             (0x00000095)
#define CSL_INTC0_UTXEVT             (0x00000096)

uiCIC_out_num= 33;//in C6678, CIC out 33 is also INTC input 22

if(DNUM<4) //for core 0, 1, 2, 3
	CIC_Regs= gpCIC0_regs;
else
	CIC_Regs= gpCIC1_regs;

/*map UART ERR INT, RX EVTto CIC out*/
KeyStone_CIC_event_map(CIC_Regs, CSL_INTC0_UARTINT, uiCIC_out_num);
KeyStone_CIC_event_map(CIC_Regs, CSL_INTC0_URXEVT, uiCIC_out_num+1);

/* Enable Global host interrupts. */
CIC_Regs->GLOBAL_ENABLE_HINT_REG= 1;

/* map UART ERR interrupt to INT4 */
/* map UART RX interrupt to INT5 */
gpCGEM_regs->INTMUX1 = 
	(CSL_GEM_INTC0_OUT_33_PLUS_11_MUL_N_OR_INTC0_OUT_33_PLUS_11_MUL_N_MINUS_4<<CSL_CGEM_INTMUX1_INTSEL4_SHIFT)|\
	(CSL_GEM_INTC0_OUT_34_PLUS_11_MUL_N_OR_INTC0_OUT_34_PLUS_11_MUL_N_MINUS_4<<CSL_CGEM_INTMUX1_INTSEL5_SHIFT);

/* enable INT4, 5 */
CPU_interrupt_enable((1<<4)|(bRxIntEnable<<5));
```

对HyperLink而言：
```
/*map HyperLink Interrupt events (111) to CIC0 out33*/
KeyStone_CIC_event_map(gpCIC0_regs, CSL_INTC0_VUSR_INT_O, 33);
	
/*on Nyquist, CIC0 out64 event number are 22 on core 0		
on Shannon, CIC0 out33 event number are 22 on core0		
map this event 22 to INT6 */
gpCGEM_regs->INTMUX1 = (22<<CSL_CGEM_INTMUX1_INTSEL6_SHIFT);

//enable INT6
CPU_interrupt_enable(1<<6);

GPIO：
//combine all GPIO interrupts to INTC event 2
gpCGEM_regs->EVTMASK[2]= ~((1<<(CSL_GEM_GPINTN- 64))|
	(1<<(CSL_GEM_GPINT8-64))|
	(1<<(CSL_GEM_GPINT9-64))|
	(1<<(CSL_GEM_GPINT10-64))|
	(1<<(CSL_GEM_GPINT11-64))|
	(1<<(CSL_GEM_GPINT12-64))|
	(1<<(CSL_GEM_GPINT13-64))|
	(1<<(CSL_GEM_GPINT14-64))|
	(1<<(CSL_GEM_GPINT15-64)));
	
/* map INTC event 2 to INT15 */
gpCGEM_regs->INTMUX3 = CSL_GEM_EVT2<<CSL_CGEM_INTMUX3_INTSEL15_SHIFT;

/* enable INT15 */
CPU_interrupt_enable(1<<15);
```

## 核间通信

关于多核启动：IBL将每个镜像分别加载到各自分配好的内存区域中，每个核心都对自己的寄存器做初始化，使能中断，开启cacheable能力。
随后由主核0核进行外部设备的初始化，其他核心进入idle状态，等待0核的INTC。

核间互斥：同一个核心上的所有线程在访问资源时由系统做统一调度，在BSP上做信号量的互斥访问，保证线程对设备的访问不被干扰。
响应中断时屏蔽同一级中断，保证ISR的处理完整。
我们把OS看成一个整体，只要保证OS对硬件资源的访问在核间是互斥的即可。
核间的互斥用硬件信号量来完成，硬件信号量分为两种，直接和间接，直接访问模式下如果访问失败，会进入阻塞模式。
这样就保证了AMP下的互斥。

只有当该数据结构会被内核代码和ISR访问时才选择关闭中断，否则如果是线程互斥的话，用锁机制，调度会在条件不足时将其挂起。
ISR不能挂起，无法休眠，所以屏蔽中断。

## 多核启动

### Stage0

IBL二级引导，解析ELF文件格式，加载到每个核的指定位置。
将0核作为引导核，即AMP架构下的主核。以下如非特殊说明，都是主核的说明。

### Stage1(Chip_init)

寄存器初始化
SP已由RTS设定，这里不再修改；B3作为函数调用返回地址寄存器，不可修改。
对中断控制寄存器进行使能，包括ERP/ARP/CSR/ICR/IER。
ISTP指向中断向量表。

6678共有16个timer,根据核号选择一个进行配置。

### Stage2(Board_init)

L1/L2/L3缓存初始化，
Cppi(pktDMA)/SRIO/QMSS/PCIe/SPI外设初始化和总线配置。
发送核间中断给从核。

### Stage3(BSP_startup)

clock/console/network　ISR挂载。
I/O设备注册(文件系统注册驱动回调函数)。

### Stage4(OS_startup)

系统堆分配。(链接脚本里指定每个核使用不同内存区域)
系统初始化。
多任务调度。

注：从核只在stage2与其他核不同，为idle闲置过程，等待核间中断发生，直接执行stage3。

## 设备初始化

对DDR初始化参数应和板子的布线一致，否则会导致内存的访问出现问题。内存开启ECC后会进行校验，可能校验出错，
或者被保护机制拒绝访问。

PA：配置MAC过滤，不是除了自己MAC和广播地址才会进入队列中。

### Flash存储

RTEMS自带的页面管理机制是服务于NOR的。因为NOR没有坏页。

NAND实现相比于NOR更复杂。
NAND对verity和blank有要求。一定要提供实现verify实现，否则会出错。

对于NAND的读写代码以及坏块判断代码，参考附件的nandwriter.c。
该实现调用了驱动接口，实现对NAND设备的写入。

页面管理机制简介：
保证页面使用均衡（每一页擦除一次寿命减一），每次写入文件都往新的页写，不够的时候（写满所有页后）进行一次垃圾回收，把之前已经删除的文件占有页格式化，同时在分区表处标记为可用。
算法：LRU和AVL树，详见libblock/include/bdbuf.h。


## 网卡

802.3
mdio
负责phy的管理，link up或down会触发事件。6678EVM的phy比较简单，不做初始化。

switch
三端口交换机，网口连接2号端口(1号是内部连接)。使用ALE地址查找引擎，用于路由表(表项大小1K)。

mac
配置为千兆全双工模式。

pass
该加速器在初始化加入网卡mac地址，可用硬件过滤以太网帧。LUT最多有64个映射。

cppi
dma，发送帧不需要cpu参与。

pktDMA
9个Tx Channel, 24个Rx Channel。最大支持32个Rx FDQ队列。

网络通信时使用高优先级的累计队列，对应事件源头48。
以太网帧到来后，门限到达，触发48号event。
ISR从队列中取出一个帧，缓存到内存中。
该过程将连续到达的多个frame存储下来，直到调度BSP的Rx线程，转发给协议栈。

### 链接脚本

链接脚本：

-heap
和sysmem段搭配，用于rts的堆管理。

-stack
入口函数的栈大小，从intc_00->main->boot_card，之后发生调度就不再使用这个地方，设置8k就可以了。

链接脚本的内存布局说明：
rtems创建的所有对象都分配在WorkArea中。安排在0xA0000000开始的3G的DDR3内存中，目前静态分配为1G大小。
前1G内存布局说明如下：
前16MB用于网络传输需要的PA地址映射，余下的段用于存储系统的text、bss段等，同时包括RTS自身初始化分配的sysmem和stack等。

blob用于动态加载，大小设置为512MB

### 内存分区

内存划分：

4M共享内存不碰；

64KL1缓存，代码和数据各半，由工具链自行安排；

512KL2缓存，将中断向量表和代码段、部分只读数据放在这里；

由于驱动代码的加入，可能代码段体积超过1M，因此也会放在SYSTEM中。

DDR3内存共有256MB的空间，
对于0核而言，比较特殊，这个核负责多核启动时发送IPC中断，网卡也从该核初始化，0核的空间会被PA占用一部分。
分割如下：
DEVICE：16M(QMSS使用16K的RAM)
BLOB：64MB(用于动态加载时加载库存储空间)
SYSTEM：16MB RTS栈、堆和驱动固件代码。
WORKAREA：128MB RTEMS系统所有对象分配的区域。

### TCP/IP 协议栈 移植

预定义的宏：
```
NOPOLL
NOSELECT
_THREAD_SAFE
__BSD_VISIBLE
BOOTP_COMPAT
EFUSE_MAC
NFS
INET
DIAGNOSTIC
_KERNEL
__rtos__
__rtems__
```

全局变量控制：
```
cpukit/libnetworking/rtems/rtems_bsdnet.h

#define __INSIDE_RTEMS_BSD_TCPIP_STACK__
/*
 *  If this file is included from inside the Network Stack proper or
 *  a device driver, then __INSIDE_RTEMS_BSD_TCPIP_STACK__ should be
 *  defined.  This triggers a number of internally used definitions.
 */

#if defined(__INSIDE_RTEMS_BSD_TCPIP_STACK__)
#undef _KERNEL
#undef INET
#undef NFS
#undef DIAGNOSTIC
#undef BOOTP_COMPAT
#undef __BSD_VISIBLE

#define _KERNEL 1
#define INET 1
#define NFS 1
#define DIAGNOSTIC 1
#define BOOTP_COMPAT 1
#define __BSD_VISIBLE 1
#endif
```

-------------
cpukit/libnetworking/kern/kern_subr.c
cpukit/libnetworking/kern/uipc_socket.c

cpukit/libnetworking/nfs/bootp_subr.c

增加头文件定义:
```
#include <sys/uio.h>
```

------------------
cpukit/libnetworking/kern/kern_sysctl.c

增加声明:
```
//struct sysctl_oid * [];
struct sysctl_oid *__stop_set_sysctl_set[];
struct sysctl_oid *__start_set_sysctl_set[];
```

--------------------

cpukit/libnetworking/libc/res_stub.c

注释所有，gcc编译器使用弱引用。

------------------
不一定：
cpukit/libnetworking/libc/res_send.c
```
#define HAVE_STRINGS_H
#define __rtems__
#define NOPOLL
```

-------------
cpukit/libnetworking/net/ethernet.h
`#include <sys/ioccom.h>`

-------------------------------------
cpukit/libnetworking/net/if.h (60行)
cpukit/libnetworking/netinet/in.h
```
//#ifndef _KERNEL
#ifdef _KERNEL
```

----------------------
cpukit/libnetworking/netinet/ip_input.c
`#include <sys/queue.h>`

----------------------------------
cpukit/libnetworking/netinet/tcp.h
cpukit/libnetworking/sys/socket.h
`#define __BSD_VISIBLE 1`

--------------------
cpukit/libnetworking/rtems/rtems_bsdnet.h
`//#include <sys/cpuset.h>`

-------------------------
cpukit/libnetworking/rtems/rtems_glue.c
`//sysctl_register_all(0);`

---------------------
cpukit/libnetworking/sys/kernel.h
`#include <errno.h>`

-------------------
cpukit/libnetworking/sys/linker_set.h
`#ifdef __GNUC__`

--------------------------

cpukit/libnetworking/netinet/ip.h
使用GCC扩展：
`#define __GNUC__`
使用一字节对齐：
`__packed __aligned(1);`

强制对齐后输出：
```
               INTERNAL ERROR: Decomposition error


This may be a serious problem.  Please contact customer support with a
description of this problem and a sample of the source files that caused this
INTERNAL ERROR message to appear.

Cannot continue compilation - ABORTING!
```
找出所有头部定义，出现bit分配的对象，把u_int改为u_char。

net
以太网层

netinet
网络和传输层

rtems
粘合层

sys
系统内核对网络支持的头文件

### C66 网卡注意事项

PBM是TI驱动自带的缓冲区管理，
收到中断后，立马加入队列中，直到RxDaemon去队列中取数据包。

以以太网帧最小大小60字节。去掉帧的最后４位校验。

```
#define PKT_NUM_FRAMEBUF 1024
```

驱动实现说明：
当中断发生后，立马将包收到PBM中。

PBM是一个专门的包管理模块，所有驱动代码都围绕它展开。
ISR：
当中断启动后默认开启发送进程，将ARP广播出去。又接受中断的ISR负责发送信号，此时接收进程开启。

RXISR负责发送START_RECEIVE_EVENT信号。

启动时发送的ARP含义是什么？验证IP是否冲突。
```
Address Resolution Protocol (request/gratuitous ARP)
Frame 58: 60 bytes on wire (480 bits), 60 bytes captured (480 bits) on interface 0
    Interface id: 0 (enp1s0)
    Encapsulation type: Ethernet (1)
    Arrival Time: Mar  2, 2017 20:10:47.920773025 CST
    [Time shift for this packet: 0.000000000 seconds]
    Epoch Time: 1488456647.920773025 seconds
    [Time delta from previous captured frame: 0.581304244 seconds]
    [Time delta from previous displayed frame: 0.581304244 seconds]
    [Time since reference or first frame: 46.373341257 seconds]
    Frame Number: 58
    Frame Length: 60 bytes (480 bits)
    Capture Length: 60 bytes (480 bits)
    [Frame is marked: False]
    [Frame is ignored: False]
    [Protocols in frame: eth:ethertype:arp]
    [Coloring Rule Name: ARP]
    [Coloring Rule String: arp]
Ethernet II, Src: TexasIns_b9:3d:28 (40:5f:c2:b9:3d:28), Dst: Broadcast (ff:ff:ff:ff:ff:ff)
    Destination: Broadcast (ff:ff:ff:ff:ff:ff)
    Source: TexasIns_b9:3d:28 (40:5f:c2:b9:3d:28)
    Type: ARP (0x0806)
    Padding: 0000000000000000980c0bc0080000000000
Address Resolution Protocol (request/gratuitous ARP)
```

驱动设计：

中断发生后，就把数据准备到mbuf队列中，等rx任务调度后，直接给上层。

IP层数据不对，为啥？
```
/*
 * Structure of an internet header, naked of options.
 */
struct ip {
#ifdef _IP_VHL
	u_char	ip_vhl;			/* version << 4 | header length >> 2 */
#else
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
	u_int	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
#endif /* not _IP_VHL */
	u_char	ip_tos;			/* type of service */
	u_short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	u_short	ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
} __packed __aligned(4);
```

netinet/ip.h数据结构用了`__packed __aligned(4)`来对齐，可是TI工具链可能没有这么做，so...
用TI自己的方法来告诉编译器！！！
```
__GNUC__
#if defined(__GNUC__)
#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
```

问题：
```
void
b64_from_24bit(uint8_t B2, uint8_t B1, uint8_t B0, int n, int *buflen, char **cp)

#define b64_from_24bit _crypt_b64_from_24bit
void _crypt_b64_from_24bit(uint8_t, uint8_t, uint8_t, int, int *, char **);

//#define b64_from_24bit _crypt_b64_from_24bit
//void _crypt_b64_from_24bit(uint8_t, uint8_t, uint8_t, int, int *, char **);
void
b64_from_24bit(uint8_t B2, uint8_t B1, uint8_t B0, int n, int *buflen, char **cp);
```

问题：
```
Description	Resource	Path	Location	Type
#20 identifier "getopt_data" is undefined	hexsyntax.c	/rtems_netdemo/libmisc/shell	line 72	C/C++ Problem
```
方法：
```
./newlib/libc/include/getopt.h:  } getopt_data;
```
移到/include/libc/newlib下。
同时修改该头文件第122行：
```
#define __need_getopt_newlib before including
```
问题：
```
Description	Resource	Path	Location	Type
#20 identifier "status" is undefined	cbcp.c	/rtems_netdemo/pppd	line 455	C/C++ Problem
pppd/cbcp.c
```
代码有问题：修改源码455行：
```
--    status = EXIT_CALLBACK;
++    pppd_status = EXIT_CALLBACK;
```

添加include文件：
rtems/serdbg.h

问题：
波特率BO和寄存器命名冲突：
方法：
修改cpu.h，通用寄存器名称前加上"reg_"前缀。
相应的，修改cpu.c文件。

修改include/libc，分成rts和newlibc。

以下三个rfs文件系统的头文件在rtems目录下，而不在rtems/rfs下：
rtems-rfs-format.h       rtems-rfs.h              rtems-rfs-shell.h

bsd协议栈是自包含的，所以只要包含这个目录就可以了！

zlib
把zconf.h.in后缀.in去除。

libmisc/dummy因该去掉。
等所有都完备了，才能加入shell。

说明：没有GNU工具链，无法进行libmisc下的stackchk！！！
因为stdio等的原因，暂时无法移入telnetd实现。
因BSP_output_char、BSP_poll_char和bsp提供的串口打印函数（printk）命名冲突，暂时不移如serdbg。
等console做好后，可以加入！！！

可以走POSIX接口，只要不定义
RTEMS_POSIX_API就可以。

console的注册需要内存文件系统的支持！！！ioman.c:rtems_io_register_name()

当前任务：
mklib制作rts运行时库。。。

参照之前的bsp，重新设计的clock的思路：
clock初始化去调用注册isr,然后才初始化timer，接着OS就有了心跳。
整体测试成功，该过程仿照别的bsp设计。
可以由系统初始化时启动timer,接着挂载clock的isr,发生时系统的tick++，接可以调度了。
所以，重新完成了clock和timer两个部件。

RTS
```
 free      /rts_8_1_0/rts6600_elf.lib<delete.obj>   
 lseek     /rts_8_1_0/rts6600_elf.lib<ftell.obj>    
 malloc    /rts_8_1_0/rts6600_elf.lib<dtor_list.obj>
 read      /rts_8_1_0/rts6600_elf.lib<fgetc.obj>    
 unlink    /rts_8_1_0/rts6600_elf.lib<remove.obj>   
 write     /rts_8_1_0/rts6600_elf.lib<fwrite.obj>  
```
直接使用rtems的c接口执行。
把文件系统和内存管理接口修改成__TI_前缀，避免命名冲突！！！

rtems++
将rts所有的头文件找出（包括c++），覆盖c头文件，因为已经改过。然后将rtems的.h再次覆盖。这样保留了原有的c头文件，又包括新的c++头文件。

现有的系统是不需要c++文件的！！！
将ti的c++头文件覆盖现有的newlibc的c++头文件：
exception　　e_exception　　iosfwd　　new e_new

tcp/udp头部校验和不对？？？
有特定平台相关的cksum代码，所以，咱们这个有问题！！！

ti工具链通常在编译时分配空间，如果没有正确包含某个头文，将导致各种指针出错的问题。
可以在ti的time.h中加入包含newlibc的include，也可以将newlibc的time.h再次放进来！

```
typedef struct {
    UINT8    VerLen;
    UINT8    Tos;
    UINT16   TotalLen;
    UINT16   Id;
    UINT16   FlagOff;
    UINT8    Ttl;
    UINT8    Protocol;
    UINT16   Checksum;
    IPN      IPSrc;
    IPN      IPDst;
    UINT8    Options[1];
} IPHDR;
```
rtems的坑：
在ip.h中比BSD代码多定义了：
```
_IP_VHL
```
比如说这个重要的ip头部结构体：
```
struct ip {
#ifdef _IP_VHL
	u_char	ip_vhl;			/* version << 4 | header length >> 2 */
#else
#if BYTE_ORDER == LITTLE_ENDIAN
	u_int	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
	u_int	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
#endif /* not _IP_VHL */
	u_char	ip_tos;			/* type of service */
	u_short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	u_short	ip_off;			/* fragment offset field */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
} __packed __aligned(4);
```
这个默认情况下　`_IP_VHL`　是没有定义的。所以会去根据大小端编译，但是在ip_input.c实现中，却在头部定义了
`#define	_IP_VHL`，所以编译的版本其实不对。
链接也检查不出这个问题。最终收到包后，检查ip头部，得出校验和不正确的结论，所以IP层直接把包丢掉。。。
注释所有#`define _IP_VHL`的地方。

网络移植说明：

底层驱动来自NDK。初始化时启用了PA加速器和CPPI的PKTDMA。中断源是高优先级的accumlation队列事件。当mac门限触发时，触发该硬件队列中断，随即跳入Rx ISR。ISR负责将队列中的以太网帧拷贝到PBM（Packet Buffer Manager）中。PBM是NDK设计的缓冲区域，当网络中断连续触发时，PBM会收容所有的帧。等到系统正常调度后，BSP的RxDaemon会去这里取出所有帧（拷贝方式），并向上传给协议栈。该思想类似Linux的中断处理机制，分为上下半部，把以太网帧的接收作为ISR，提高响应的实时性；而处理向后推迟(也和Vxworks的WorkQ思想一致)。发送以太网帧不需要cpu参与，直接Qmss_push到硬件队列中即可(DMA)。

NDK中的PBM堆使用了独立于OS的内存区域，把系统堆和网络堆分开，有助于避免碎片的相互影响。目前的移植直接调用了rtems提供的malloc/free接口，即没有对内存进行区分，而rtems_bsdnet_malloc实际也是对rtems的malloc的封装。也就是说，驱动层和协议栈的内存分配效率完全依赖于现有的rtems内存管理设计。我们的ISR在拷贝以太网帧到PBM堆时，每次都分配1054个字节(MTU上限)。根据NDK和TI论坛解释，可用于测试网络极端情况（即每次接收都是大帧），在设计上同时避免了内部碎片。当前移植沿用了该做法，没有修改。PBM支持巨帧jumboframe，我们没有把这部分功能容纳进来。

在测试整体协议栈和驱动代码时，ping一个1054的大包时耗时是SYS/BIOS+NDK的三倍。
BIOS+NDK平均用时0.64ms；rtems+bsd协议栈平均用时1.98ms。初步考虑，原因如下：
BIOS测试用例使用了MCSDK的hua，链接的是现成库，可能用了O3优化。论坛上说开启-O3后吞吐量会提升。我们的工程还没这么测试。
bsd协议栈在ethernet到ip层使用了拷贝，传输层到应用层用uio拷贝。校验和目前没有用汇编加速，都可能是性能比不上原生的原因。
至于中断处理，开关中断的位置和原生NDK的位置相同。rtems使用低优先级不可抢占式创建后台任务；NDK使用轮询机制处理以太网帧，也可能是原因。

### 对齐

It is illegal to implicitly or explicitly cast the address of a packed struct member as a pointer to any non-packed type except an unsigned char. In the following example, p1, p2, and the call to foo are all illegal.
```
void foo(int *param);
struct packed_struct ps;
int *p1 = &ps.i;
int *p2 = (int *)&ps.i;
foo(&ps.i);
```
However, it is legal to explicitly cast the address of a packed struct member as a pointer to an unsigned char:
```
unsigned char *pc = (unsigned char *)&ps.i;
```
The TI compiler also supports an unpacked attribute for an enumeration type to allow you to indicate that the representation is to be an integer type that is no smaller than int; in other words, it is not packed.
