.init main
.segment	32768


main:
	sub sp, sp, $24		# Make room in the stack for program variables
	lda r10, $2
	stw r10, $0(sp)

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $4(sp)

	lda r10, $text1
	stw r10, $8(sp)

	lda r10, $text2
	stw r10, $12(sp)

	ldw r6, $4(sp)	 # Load rhs var from mem for IF
	ldw r5, $0(sp)	 # Load lhs var from mem for IF
	beq r5, r6, JABIF0

	ldw r10, $12(sp)		# Load existing variable
	stw r10, $16(sp)

	ldw r50, $16(sp)		# ldw for print
	call _printstr

	JABIF0:			# Top of an IF statemnt

	ldw r10, $8(sp)		# Load existing variable
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr

	exit


text1: .string " \n\t[SHOULD SEE]\n"
text2: .string " \n\t[SHOULD NOT SEE]\n"


