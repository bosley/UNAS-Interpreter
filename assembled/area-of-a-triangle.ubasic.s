.init main
.segment	32768


main:
	sub sp, sp, $56		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)
	lda r10, $0
	stw r10, $4(sp)
	lda r10, $0
	stw r10, $8(sp)

	lda r10, $text2
	stw r10, $12(sp)

	ldw r50, $12(sp)		# ldw for print
	call _printstr

	call _geti
	stw r2, $4(sp)

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $16(sp)

	ldw r50, $16(sp)		# ldw for print
	call _printstr

	lda r10, $text3
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr

	call _geti
	stw r2, $8(sp)
				# Calculate:  height    base  *  2/

	ldw r10, $4(sp)

	ldw r11, $8(sp)

	mul r10, r10, r11

	lda r11, $2

	lda r11, $2

	div r10, r10, r11
	stw r10, $24(sp)		# Copy result into mem index 

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $28(sp)

	ldw r50, $28(sp)		# ldw for print
	call _printstr

	lda r10, $text4
	stw r10, $32(sp)

	ldw r50, $32(sp)		# ldw for print
	call _printstr

	ldw r10, $24(sp)		# Load existing variable
	stw r10, $36(sp)

	ldw r59, $36(sp)		# ldw for print
	call _printint

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $40(sp)

	ldw r50, $40(sp)		# ldw for print
	call _printstr

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $44(sp)

	ldw r50, $44(sp)		# ldw for print
	call _printstr

	exit


text1: .string " \n"
text2: .string " Height: "
text3: .string " Base: "
text4: .string " Area of your triangle is : "


