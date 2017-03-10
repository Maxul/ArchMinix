		.ref __ISR_Handler, Exception_service_routine
		.def nmi_isr,int_lvl2
		
		.text

int_lvl2:
		ADD     4, B15, B0
		AND		0xFFFFFFF8, B0, B0
		ADDK	-144, B0
		STDW	A1:A0, *-B0[18]
||		MV		B0, A0	
		
		MV		B1, A1
||		LDW		*B15++[1], B1
		
		STDW	A3:A2, *-A0[16]
||		STDW	B3:B2, *-B0[15]

		STDW	A5:A4, *-A0[14]
||		STDW	B5:B4, *-B0[13]

		STDW	A7:A6, *-A0[12]
||		STDW	B7:B6, *-B0[11]

		STDW	A9:A8, *-A0[10]
||		STDW	B9:B8, *-B0[9]

		STDW	A11:A10, *-A0[8]
||		STDW	B11:B10, *-B0[7]

		STDW	A13:A12, *-A0[6]
||		STDW	B13:B12, *-B0[5]

		STDW	A15:A14, *-A0[4]
||		STDW	B15:B14, *-B0[3]

		MVKL	__ISR_Handler, B8
||		STDW	A17:A16, *-A0[2]
||		STDW	B17:B16, *-B0[1]

		MVKH	__ISR_Handler, B8
||		STDW	A19:A18, *+A0[0]
||		STDW	B19:B18, *+B0[1]

		MVC		NRP , B2
||		STDW	A21:A20, *+A0[2]
||		STDW	B21:B20, *+B0[3]

		MVC		CSR , B3
||		MV		B2, A2
||		STDW	A23:A22, *+A0[4]
||		STDW	B23:B22, *+B0[5]

		LDW		*B15[0], B0

		MVC		ITSR , B4
||		MV		B3, A3
||		STDW	A25:A24, *+A0[6]
||		STDW	B25:B24, *+B0[7]

		MVC		RILC, B5
||		MV		B4, A4
||		STDW	A27:A26, *+A0[8]
||		STDW	B27:B26, *+B0[9]

		MVC		ILC , B6
||		MV		B5, A5
||		STDW	A29:A28, *+A0[10]
||		STDW	B29:B28, *+B0[11]

		MVC		IRP , B7
||		MV		A0, B15
||		STDW	A31:A30, *+A0[12]
||		STDW	B31:B30, *+B0[13]

		MV		B15, A4
||		STDW	A5:A4, *+A0[15]
||		STDW	B7:B6, *+B15[16]

		ADDK	-160, B15
||		ADDK	-144, A4
||		STDW	A3:A2, *+A0[14]
||		STDW	B1:B0, *-B15[17]

		B	B8
		MVKL  loc_int,B3
		MVKH  loc_int,B3
		mv  A4, B4
		mv  A1, A4
		NOP

loc_int:
		DINT
		ADDK	160, B15

		MV		B15, A0
||		LDDW	*+B15[15], B1:B0

		LDDW	*+A0 [14], A3:A2
||		LDDW	*-B15[15], B3:B2

		LDDW	*+A0[16], A5:A4
||		LDDW	*-B15[13], B5:B4

		LDDW	*-A0 [12], A7:A6
||		LDDW	*-B15[11], B7:B6

		LDDW	*-A0 [10], A9:A8
||		LDDW	*-B15[9], B9:B8

		LDDW	*-A0 [8], A11:A10
||		LDDW	*-B15[7], B11:B10

		LDDW	*-A0 [6], A13:A12
||		LDDW	*-B15[5], B13:B12

		LDDW	*-A0 [2], A17:A16
||		LDDW	*-B15[1], B17:B16

		LDDW	*+A0 [0], A19:A18
||		LDDW	*+B15[1], B19:B18

		LDDW	*+A0 [2], A21:A20
||		LDDW	*+B15[3], B21:B20

		LDDW	*+A0 [4], A23:A22
||		LDDW	*+B15[5], B23:B22

		LDDW	*+A0 [6], A25:A24
||		LDDW	*+B15[7], B25:B24

		LDDW	*+A0 [8], A27:A26
||		LDDW	*+B15[9], B27:B26

		MVC		B0, ITSR
||		MV		A2, B0
||		LDDW	*+A0 [10], A29:A28
||		LDDW	*+B15[11], B29:B28

		MVC		B1, RILC
||		MV		A3, B1
||		LDDW	*+A0 [12], A31:A30
||		LDDW	*+B15[13], B31:B30

		MVC		B0, NRP
||		MV		A4, B0
||		LDDW	*-A0 [4], A15:A14

		MVC		B1, CSR
||		MV		A5, B1
||		LDDW	*-A0[16], A3:A2

		MVC		B0, ILC
||		LDDW	*-A0[18], A1:A0
||		LDDW	*-B15[3], B15:B14
;||		LDW		*-B15[5], B15

		MVC		B1, IRP
||		LDDW	*-A0 [14], A5:A4
||		LDDW	*-B15[17], B1:B0

		B		IRP
		NOP		5
		
nmi_isr:
		ADD     4, B15, B0
		AND		0xFFFFFFF8, B0, B0
		ADDK	-144, B0
		STDW	A1:A0, *-B0[18]
||		MV		B0, A0
		
		MV		B1, A1
||		LDW		*B15++[1], B1
		
		STDW	A3:A2, *-A0[16]
||		STDW	B3:B2, *-B0[15]

		STDW	A5:A4, *-A0[14]
||		STDW	B5:B4, *-B0[13]

		STDW	A7:A6, *-A0[12]
||		STDW	B7:B6, *-B0[11]

		STDW	A9:A8, *-A0[10]
||		STDW	B9:B8, *-B0[9]

		STDW	A11:A10, *-A0[8]
||		STDW	B11:B10, *-B0[7]

		STDW	A13:A12, *-A0[6]
||		STDW	B13:B12, *-B0[5]

		STDW	A15:A14, *-A0[4]
||		STDW	B15:B14, *-B0[3]

		MVKL	Exception_service_routine, B8
||		STDW	A17:A16, *-A0[2]
||		STDW	B17:B16, *-B0[1]

		MVKH	Exception_service_routine, B8
||		STDW	A19:A18, *+A0[0]
||		STDW	B19:B18, *+B0[1]

		MVC		NRP , B2
||		STDW	A21:A20, *+A0[2]
||		STDW	B21:B20, *+B0[3]

		MVC		CSR , B3
||		MV		B2, A2
||		STDW	A23:A22, *+A0[4]
||		STDW	B23:B22, *+B0[5]

		LDW		*B15[0], B0

		MVC		ITSR , B4
||		MV		B3, A3
||		STDW	A25:A24, *+A0[6]
||		STDW	B25:B24, *+B0[7]

		MVC		RILC, B5
||		MV		B4, A4
||		STDW	A27:A26, *+A0[8]
||		STDW	B27:B26, *+B0[9]

		MVC		ILC , B6
||		MV		B5, A5
||		STDW	A29:A28, *+A0[10]
||		STDW	B29:B28, *+B0[11]

		MVC		IRP , B7
||		MV		A0, B15
||		STDW	A31:A30, *+A0[12]
||		STDW	B31:B30, *+B0[13]

		MV		B15, A4
||		STDW	A5:A4, *+A0[15]
||		STDW	B7:B6, *+B15[16]

		ADDK	-160, B15
||		ADDK	-144, A4
||		STDW	A3:A2, *+A0[14]
||		STDW	B1:B0, *-B15[17]

		B	B8
		MVKL  loc_nmi,B3
		MVKH  loc_nmi,B3
		mv  A4, B4
		mv  A1, A4
		NOP

loc_nmi:
		DINT

		ADDK	160, B15

		MV		B15, A0
||		LDDW	*+B15[15], B1:B0

		LDDW	*+A0 [14], A3:A2
||		LDDW	*-B15[15], B3:B2

		LDDW	*+A0[16], A5:A4
||		LDDW	*-B15[13], B5:B4

		LDDW	*-A0 [12], A7:A6
||		LDDW	*-B15[11], B7:B6

		LDDW	*-A0 [10], A9:A8
||		LDDW	*-B15[9], B9:B8

		LDDW	*-A0 [8], A11:A10
||		LDDW	*-B15[7], B11:B10

		LDDW	*-A0 [6], A13:A12
||		LDDW	*-B15[5], B13:B12

		LDDW	*-A0 [2], A17:A16
||		LDDW	*-B15[1], B17:B16

		LDDW	*+A0 [0], A19:A18
||		LDDW	*+B15[1], B19:B18

		LDDW	*+A0 [2], A21:A20
||		LDDW	*+B15[3], B21:B20

		LDDW	*+A0 [4], A23:A22
||		LDDW	*+B15[5], B23:B22

		LDDW	*+A0 [6], A25:A24
||		LDDW	*+B15[7], B25:B24

		LDDW	*+A0 [8], A27:A26
||		LDDW	*+B15[9], B27:B26

		MVC		B0, ITSR
||		MV		A2, B0
||		LDDW	*+A0 [10], A29:A28
||		LDDW	*+B15[11], B29:B28

		MVC		B1, RILC
||		MV		A3, B1
||		LDDW	*+A0 [12], A31:A30
||		LDDW	*+B15[13], B31:B30

		MVC		B0, NRP
||		MV		A4, B0
||		LDDW	*-A0 [4], A15:A14

		MVC		B1, CSR
||		MV		A5, B1
||		LDDW	*-A0[16], A3:A2

		MVC		B0, ILC
||		LDDW	*-A0[18], A1:A0
||		LDDW	*-B15[3], B15:B14
;||		LDW		*-B15[5], B15

		MVC		B1, IRP
||		LDDW	*-A0 [14], A5:A4
||		LDDW	*-B15[17], B1:B0

		B		NRP
		NOP		5
		
	.end
