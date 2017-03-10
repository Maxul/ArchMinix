;  FileName: cpu_asm.asm
;
;  Description: This file contains the basic algorithms for all assembly code used
;				in an specific CPU port of RTOS.  These algorithms must be implemented
;				in assembly language

	.global	$bss
	.if	__TI_EABI__
		.global  _vector  	; /* Memory location for default   */
	.else
		.global  __vector  	; /* Memory location for default   */
	.endif

;*****************************************************
; RegInit() function: Initialise the processors
;         registers with defined values.
;*****************************************************

	.if	__TI_EABI__
		.global RegInit
	.else
		.global _RegInit
	.endif

	.align	32
	.if	__TI_EABI__
RegInit:
	.else
_RegInit:
	.endif

	MVK 	0x0, B0
	MVC 	B0, AMR
	MVC 	B0, ISR
	MVC 	B0, ICR
	MVC 	B0, IER
	MVC 	B0, IRP
	MVC 	B0, NRP

	MVK 	0XFFFFFFFF, B1
	MVC 	B1, ICR

	MVK 	0x100, B0	; B0 = 0 big endian, B0 = 100 little endian
	MVC 	B0, CSR

	MVK 	0x0, B0

	.if	__TI_EABI__
		MVKL 	_vector, B0
		MVKH 	_vector, B0
	.else
		MVKL 	__vector, B0
		MVKH 	__vector, B0
	.endif
		MVC 	B0, ISTP

		B 		B3           ; Return address
		NOP 	5


	.if	__TI_EABI__
		.global GetCSR
	.else
		.global _GetCSR
	.endif

    .align   32
	.if	__TI_EABI__
GetCSR:
	.else
_GetCSR:
	.endif

		MVC 	CSR, B0
		MV 		B0, A4
		B 		B3
		NOP 	5


	.if	__TI_EABI__
		.global SetCSRBit
	.else
		.global _SetCSRBit
	.endif

    .align   32
	.if	__TI_EABI__
SetCSRBit:
	.else
_SetCSRBit:
	.endif

		MVC 	CSR, B0
		OR 		B0, A4, B0
		MVC 	B0, CSR
		B 		B3
		NOP 	5

	.if	__TI_EABI__
		.global CleanCSRBit
	.else
		.global _CleanCSRBit
	.endif

	.align   32
	.if	__TI_EABI__
CleanCSRBit:
	.else
_CleanCSRBit:
	.endif
		MVC 	CSR, B0
		AND 	B0, A4, B0
		MVC 	B0, CSR
		B 		B3
		NOP 	5

;/*
; *  _CPU_Context_switch
; *
; *  This routine performs a normal non-FP context switch.
; *
; *  void _CPU_Context_switch(
; *    Context_Control  *run,
; *    Context_Control  *heir
; *  )
; *
; */

	.if	__TI_EABI__
          .global  _CPU_Context_switch
	.else
          .global  __CPU_Context_switch
    .endif

;-----------------------------------------------------------;
;     Operating System Context task switch                  ;
;   Executing in always enabled interrupt mode              ;
;-----------------------------------------------------------;
	.align	32
	.if	__TI_EABI__
_CPU_Context_switch:
	.else
__CPU_Context_switch:
	.endif
		MVC		CSR, B17
||		MV		A4, B0
||		MV		B4, A0
||		LDW		*+B4[16], A3

		MVC		AMR, B16
||		AND		0xFFFFFFFE, B17, B1
||		MV		B17, A19
||		LDDW	*+A0[6], A21:A20
||		LDDW	*+B4[7], B21:B20

		MVC		B1, CSR
||		MV		B16, A18
||		STW		B3, *+A4[16]

		MVC		IER, B18
||		LDDW	*+A0[2], A13:A12
||		LDDW	*+B4[3], B13:B12

		MVC		IRP, B19
||		LDDW	*+A0[4], A15:A14
||		LDDW	*+B4[5], B15:B14

		MV		A3, B3
||		LDDW	*+A0[0], A11:A10
||		LDDW	*+B4[1], B11:B10

;		MVC		B20, IER
		MV		A20, B20
||		STDW	A13:A12, *+A4[2]
||		STDW	B13:B12, *+B0[3]

		MVC		B21, IRP
||		MV		A21, B21
||		STDW	A15:A14, *+A4[4]
||		STDW	B15:B14, *+B0[5]

		MVC		B20, AMR
||		STDW	A11:A10, *+A4[0]
||		STDW	B11:B10, *+B0[1]

		STDW	A19:A18, *+A4[6]
||		STDW	B19:B18, *+B0[7]

		B		B3
		MVC		B21, CSR
		NOP		4


;/*
; *  _CPU_Context_restore
; *
; *  This routine is generally used only to restart self in an
; *  efficient manner.  It may simply be a label in _CPU_Context_switch.
; *
; *  NOTE: May be unnecessary to reload some registers.
; *
; *  void _CPU_Context_restore(
; *    Context_Control *new_context
; *  )
; */

	.if	__TI_EABI__
		.global	_CPU_Context_restore
	.else
		.global	__CPU_Context_restore
	.endif

	.align	32
	.if	__TI_EABI__
_CPU_Context_restore:
	.else
__CPU_Context_restore:
	.endif
		MV		A4, B0
||		LDW		*+A4[16], B3

		LDDW	*+A4[0], A11:A10
||		LDDW	*+B0[1], B11:B10

		LDDW	*+A4[2], A13:A12
||		LDDW	*+B0[3], B13:B12

		LDDW	*+A4[4], A15:A14
||		LDDW	*+B0[5], B15:B14

		LDDW	*+A4[6], A21:A20
||		LDDW	*+B0[7], B21:B20

		NOP		4

		B		B3

		MVC		B20, IER
||		MV		A20, B20

		MVC		B21, IRP
||		MV		A21, B21

		MVC		B20, AMR
		MVC		B21, CSR

		NOP
