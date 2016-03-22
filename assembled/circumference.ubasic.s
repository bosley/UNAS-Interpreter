.init main
.segment	32768


main:
	sub sp, sp, $48		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)
	ldf f10, cfloat1
	stf f10, $4(sp)
	ldf f10, cfloat2
	stf f10, $8(sp)

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $12(sp)

	ldw r50, $12(sp)		# ldw for print
	call _printstr

	lda r10, $text2
	stw r10, $16(sp)

	ldw r50, $16(sp)		# ldw for print
	call _printstr

	call _getf
	stf f2, $4(sp)

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr
				# Calculate:  2    pi  *  r*


	ldf f10, cfloat3

	ldf f11, $8(sp)

	mulf f10, f10, f11
	ldf f11, $4(sp)

	mulf f10, f10, f11
	stf f10, $24(sp)		# Copy result into mem index 

	lda r10, $text3
	stw r10, $28(sp)

	ldw r50, $28(sp)		# ldw for print
	call _printstr

	ldf f10, $24(sp)		# Load existing variable
	stf f10, $32(sp)

	ldf f59, $32(sp)		# ldf for print
	call _printfloat

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $36(sp)

	ldw r50, $36(sp)		# ldw for print
	call _printstr

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $40(sp)

	ldw r50, $40(sp)		# ldw for print
	call _printstr

	exit


text1: .string " \n"
text2: .string " Please put in a radius :  "
text3: .string " Circumference :  "


cfloat1: .float  0.0
cfloat2: .float  3.141592653589793238462643383279502
cfloat3: .float 2.0
