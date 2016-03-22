.init main
.segment	32768


main:
	sub sp, sp, $20		# Make room in the stack for program variables
	lda r10, $1
	stw r10, $0(sp)

	lda r10, $text1
	stw r10, $4(sp)

	ldw r50, $4(sp)		# ldw for print
	call _printstr

	JABGT0:			# Top of a goto statemnt
	ldw r10, $0(sp)		# Load existing variable
	stw r10, $8(sp)

	ldw r59, $8(sp)		# ldw for print
	call _printint

	lda r10, $text2
	stw r10, $12(sp)

	ldw r50, $12(sp)		# ldw for print
	call _printstr
				# Calculate:  a    1+

	ldw r10, $0(sp)


	lda r11, $1

	lda r11, $1

	add r10, r10, r11
	stw r10, $0(sp)		# Copy result into mem index 


	jmp JABGT0		# Bottom of a goto statement

	exit


text1: .string " \n This has printed "
text2: .string "  times\n"


