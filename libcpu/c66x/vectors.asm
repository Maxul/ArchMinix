	.def _vector
	.ref _c_int00,nmi_isr,int_lvl2
	
ISR_ENTRY	.macro procedure, vector
			MVKL	procedure, B0
	||		STW		B0, *B15[0]
			MVKH	procedure, B0
	||		STW		B1, *--B15[1]
			B	B0
			MVK  	vector, B1
			NOP 	4
			.align 32
			.endm

		.text
_vector .sect  ".vector"
		.align 1024
			MVKL  _c_int00, B0
			MVKH  _c_int00, B0
here:
			B     here
			NOP   5

			ISR_ENTRY	nmi_isr	 ,1
			ISR_ENTRY	int_lvl2 ,2
			ISR_ENTRY	int_lvl2 ,3
			ISR_ENTRY	int_lvl2 ,4
			ISR_ENTRY	int_lvl2 ,5
			ISR_ENTRY	int_lvl2 ,6
			ISR_ENTRY	int_lvl2 ,7
			ISR_ENTRY	int_lvl2 ,8
			ISR_ENTRY	int_lvl2 ,9
			ISR_ENTRY	int_lvl2,10
			ISR_ENTRY	int_lvl2,11
			ISR_ENTRY	int_lvl2,12
			ISR_ENTRY	int_lvl2,13
			ISR_ENTRY	int_lvl2,14
			ISR_ENTRY	int_lvl2,15
		.end
