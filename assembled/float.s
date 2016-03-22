.init main
.segment	32768


main:
	sub sp, sp, $124		# Make room in the stack for program variables
	ldf f10, cfloat1
	stf f10, $0(sp)
	ldf f10, cfloat2
	stf f10, $4(sp)
				# Calculate:  pi    e*

	ldf f10, $0(sp)

	ldf f11, $4(sp)

	mulf f10, f10, f11
	stf f10, $8(sp)		# Copy result into mem index 
				# Calculate:  pi    2.0*

	ldf f10, $0(sp)


	ldf f11, cfloat3

	mulf f10, f10, f11
	stf f10, $12(sp)		# Copy result into mem index 

	lda r10, $text1
	stw r10, $16(sp)

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr

	lda r10, $text2
	stw r10, $24(sp)

	ldw r50, $24(sp)		# ldw for print
	call _printstr

	ldf f10, $0(sp)		# Load existing variable
	stf f10, $28(sp)

	ldf f59, $28(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $32(sp)

	ldw r50, $32(sp)		# ldw for print
	call _printstr

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $36(sp)

	ldw r50, $36(sp)		# ldw for print
	call _printstr

	lda r10, $text3
	stw r10, $40(sp)

	ldw r50, $40(sp)		# ldw for print
	call _printstr

	ldf f10, $4(sp)		# Load existing variable
	stf f10, $44(sp)

	ldf f59, $44(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $48(sp)

	ldw r50, $48(sp)		# ldw for print
	call _printstr

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $52(sp)

	ldw r50, $52(sp)		# ldw for print
	call _printstr

	lda r10, $text4
	stw r10, $56(sp)

	ldw r50, $56(sp)		# ldw for print
	call _printstr

	ldf f10, $8(sp)		# Load existing variable
	stf f10, $60(sp)

	ldf f59, $60(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $64(sp)

	ldw r50, $64(sp)		# ldw for print
	call _printstr

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $68(sp)

	ldw r50, $68(sp)		# ldw for print
	call _printstr

	lda r10, $text5
	stw r10, $72(sp)

	ldw r50, $72(sp)		# ldw for print
	call _printstr

	ldf f10, $12(sp)		# Load existing variable
	stf f10, $76(sp)

	ldf f59, $76(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $80(sp)

	ldw r50, $80(sp)		# ldw for print
	call _printstr

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $84(sp)

	ldw r50, $84(sp)		# ldw for print
	call _printstr
				# Calculate:  5.5    1.0    5  *  2.2    2.0*/+


	ldf f10, cfloat4


	ldf f11, cfloat5


	ldf f12, cfloat6

	mulf f11, f11, f12

	ldf f12, cfloat7


	ldf f13, cfloat8

	mulf f12, f12, f13
	divf f11, f11, f12
	addf f10, f10, f11
	stf f10, $88(sp)		# Copy result into mem index 

	lda r10, $text6
	stw r10, $92(sp)

	ldw r50, $92(sp)		# ldw for print
	call _printstr

	ldf f10, $88(sp)		# Load existing variable
	stf f10, $96(sp)

	ldf f59, $96(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $100(sp)

	ldw r50, $100(sp)		# ldw for print
	call _printstr

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $104(sp)

	ldw r50, $104(sp)		# ldw for print
	call _printstr
				# Calculate:  5.5    5.5    5  *  3.0    4.0*/+


	ldf f10, cfloat9


	ldf f11, cfloat10


	ldf f12, cfloat11

	mulf f11, f11, f12

	ldf f12, cfloat12


	ldf f13, cfloat13

	mulf f12, f12, f13
	divf f11, f11, f12
	addf f10, f10, f11
	stf f10, $108(sp)		# Copy result into mem index 

	lda r10, $text7
	stw r10, $112(sp)

	ldw r50, $112(sp)		# ldw for print
	call _printstr

	ldf f10, $108(sp)		# Load existing variable
	stf f10, $116(sp)

	ldf f59, $116(sp)		# ldf for print
	call _printfloat

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $120(sp)

	ldw r50, $120(sp)		# ldw for print
	call _printstr

	exit


text1: .string " \n"
text2: .string " Pi is : "
text3: .string " e is : "
text4: .string " Pi * e is : "
text5: .string " Pi * 2.0 is : "
text6: .string " [ N eq 5.5 + 1.0 * 5 / (2.2 * 2.0)] : "
text7: .string " [ X eq 5.5 + 5.5 * 5 / (3.0 * 4.0)] : "


cfloat1: .float  3.14
cfloat2: .float  2.718
cfloat3: .float 2.0
cfloat4: .float 5.5
cfloat5: .float 1.0
cfloat6: .float 5.0
cfloat7: .float 2.2
cfloat8: .float 2.0
cfloat9: .float 5.5
cfloat10: .float 5.5
cfloat11: .float 5.0
cfloat12: .float 3.0
cfloat13: .float 4.0
