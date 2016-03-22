.init main
.segment	32768


main:
	sub sp, sp, $16		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)

	lda r10, $text2
	stw r10, $4(sp)

	lda r10, $text3
	stw r10, $8(sp)

	lda r10, $text4
	stw r10, $12(sp)

	ldw r50, $12(sp)		# ldw for print
	call _printstr

	exit


text1: .string " Hello"
text2: .string " Josh"
text3: .string " Hello  my name is  Josh"
text4: .string " Hello  my name is  Josh \n"


