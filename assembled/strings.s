.init main
.segment	32768


main:
	sub sp, sp, $32		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)

	lda r10, $text2
	stw r10, $4(sp)

	lda r10, $text3
	stw r10, $8(sp)

	ldw r10, $8(sp)		# Load existing variable
	stw r10, $12(sp)

	lda r10, $text4
	stw r10, $16(sp)

	lda r10, $text5
	stw r10, $20(sp)

	lda r10, $text6
	stw r10, $24(sp)

	ldw r10, $24(sp)		# Load existing variable
	stw r10, $28(sp)

	ldw r50, $28(sp)		# ldw for print
	call _printstr

	exit


text1: .string " Some thing not mentioned\n "
text2: .string " \n\nHello World, The"
text3: .string " Awesome!\n\n"
text4: .string " CS Profs "
text5: .string " are  "
text6: .string " \n\nHello World, The CS Profs  are   Awesome!\n\n"


