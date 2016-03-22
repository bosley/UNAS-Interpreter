.init main
.segment	32768


main:
	sub sp, sp, $76		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)

	ldw r50, $0(sp)		# ldw for print
	call _printstr
				# Calculate:  1    1+


	lda r10, $1

	lda r10, $1


	lda r11, $1

	lda r11, $1

	add r10, r10, r11
	stw r10, $4(sp)		# Copy result into mem index 

	ldw r10, $4(sp)		# Load existing variable
	stw r10, $8(sp)

	ldw r59, $8(sp)		# ldw for print
	call _printint

	lda r10, $text2
	stw r10, $12(sp)

	ldw r50, $12(sp)		# ldw for print
	call _printstr
				# Calculate:  a    4*

	ldw r10, $4(sp)


	lda r11, $4

	lda r11, $4

	mul r10, r10, r11
	stw r10, $16(sp)		# Copy result into mem index 

	ldw r10, $16(sp)		# Load existing variable
	stw r10, $20(sp)

	ldw r59, $20(sp)		# ldw for print
	call _printint

	lda r10, $text3
	stw r10, $24(sp)

	ldw r50, $24(sp)		# ldw for print
	call _printstr
				# Calculate:  b    a+

	ldw r10, $16(sp)

	ldw r11, $4(sp)

	add r10, r10, r11
	stw r10, $28(sp)		# Copy result into mem index 

	ldw r10, $28(sp)		# Load existing variable
	stw r10, $32(sp)

	ldw r59, $32(sp)		# ldw for print
	call _printint

	lda r10, $text4
	stw r10, $36(sp)

	ldw r50, $36(sp)		# ldw for print
	call _printstr
				# Calculate:  4    2*    22  +  b-


	lda r10, $4

	lda r10, $4


	lda r11, $2

	lda r11, $2

	mul r10, r10, r11

	lda r11, $22

	lda r11, $22

	add r10, r10, r11
	ldw r11, $16(sp)

	sub r10, r10, r11
	stw r10, $28(sp)		# Copy result into mem index 

	ldw r10, $28(sp)		# Load existing variable
	stw r10, $40(sp)

	ldw r59, $40(sp)		# ldw for print
	call _printint

	lda r10, $text5
	stw r10, $44(sp)

	ldw r50, $44(sp)		# ldw for print
	call _printstr
				# Calculate:  20    2    8+  /  2 -


	lda r10, $20

	lda r10, $20


	lda r11, $2

	lda r11, $2


	lda r12, $8

	lda r12, $8

	add r11, r11, r12
	div r10, r10, r11

	lda r11, $2

	lda r11, $2

	sub r10, r10, r11
	stw r10, $28(sp)		# Copy result into mem index 

	ldw r10, $28(sp)		# Load existing variable
	stw r10, $48(sp)

	ldw r59, $48(sp)		# ldw for print
	call _printint

	lda r10, $text6
	stw r10, $52(sp)

	ldw r50, $52(sp)		# ldw for print
	call _printstr
				# Calculate:  2    10 ^


	lda r10, $2

	lda r10, $2


	lda r11, $10

	lda r11, $10

	mov r3, r10
	mov r4, r11
	call pwri		# Call the power funtion
	mov r10, r5

	stw r10, $28(sp)		# Copy result into mem index 

	ldw r10, $28(sp)		# Load existing variable
	stw r10, $56(sp)

	ldw r59, $56(sp)		# ldw for print
	call _printint

	lda r10, $text7
	stw r10, $60(sp)

	ldw r50, $60(sp)		# ldw for print
	call _printstr

	exit

# raise r3 to r4, result in r5
pwri:
        lda r6, $1
        lda r5, $1 
        pwritop:
                bgt r6, r4, pwribott
                mul r5, r5, r3
                add r6, r6, $1
                jmp pwritop
        pwribott:
                ret



text1: .string " \n[ a eq 1 + 1 ] : "
text2: .string " \n[ b eq a * 4 ] : "
text3: .string " \n[ c eq b + a ] : "
text4: .string " \n[ c eq (4 * 2) + 22 - b ] : "
text5: .string " \n[ c eq 20 / (2 + 8) - 2 ] : "
text6: .string " \n[ c eq 2 ^ 10 ] : "
text7: .string " \n\n"


