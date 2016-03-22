.init main
.segment	32768


main:
	sub sp, sp, $44		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)

	lda r10, $text2
	stw r10, $4(sp)

	ldw r50, $4(sp)		# ldw for print
	call _printstr
	lda r10, $1
	stw r10, $8(sp)

	call _geti
	stw r2, $8(sp)

	ldw r10, $8(sp)		# Load existing variable
	stw r10, $12(sp)

	ldw r59, $12(sp)		# ldw for print
	call _printint

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $16(sp)

	ldw r50, $16(sp)		# ldw for print
	call _printstr

	lda r10, $text3
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr
	ldf f10, cfloat1
	stf f10, $24(sp)

	call _getf
	stf f2, $24(sp)

	ldf f10, $24(sp)		# Load existing variable
	stf f10, $28(sp)

	ldf f59, $28(sp)		# ldf for print
	call _printfloat

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $32(sp)

	ldw r50, $32(sp)		# ldw for print
	call _printstr

	exit


text1: .string " \n"
text2: .string " Input an int :  "
text3: .string " Input a float : "


cfloat1: .float  0.0
