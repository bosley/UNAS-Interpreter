.init main
.segment	32768


main:
	sub sp, sp, $24		# Make room in the stack for program variables

	lda r10, $text1
	stw r10, $0(sp)
	lda r10, $0
	stw r10, $4(sp)
	lda r10, $5
	stw r10, $8(sp)
	lda r40, $10		# Load FOR LOOP limit [10]

FLT1:

	ldw r10, $4(sp)		# Load existing variable
	stw r10, $12(sp)

	ldw r59, $12(sp)		# ldw for print
	call _printint

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $16(sp)

	ldw r50, $16(sp)		# ldw for print
	call _printstr

	ldw r7, $4(sp)		# Load X to test for loop condition
	ldw r8, $8(sp)		# Load X to test for loop condition

				# Test loop conditions
	add r7, r7, r8		# Add step
	stw r7, $4(sp)		# Store back in VAR location
	ble r7, r40, FLT1	# Test if for loop conditions are met by gt

	lda r10, $text2
	stw r10, $20(sp)

	ldw r50, $20(sp)		# ldw for print
	call _printstr
	lda r10, $1
	stw r10, $24(sp)
	lda r10, $1
	stw r10, $28(sp)
	lda r40, $3		# Load FOR LOOP limit [3]

FLT2:
	lda r10, $1
	stw r10, $32(sp)
	lda r10, $1
	stw r10, $36(sp)
	lda r41, $5		# Load FOR LOOP limit [5]

FLT3:

	ldw r10, $32(sp)		# Load existing variable
	stw r10, $40(sp)

	ldw r59, $40(sp)		# ldw for print
	call _printint

	ldw r10, $0(sp)		# Load existing variable
	stw r10, $44(sp)

	ldw r50, $44(sp)		# ldw for print
	call _printstr

	ldw r7, $32(sp)		# Load j to test for loop condition
	ldw r8, $36(sp)		# Load j to test for loop condition

				# Test loop conditions
	add r7, r7, r8		# Add step
	stw r7, $32(sp)		# Store back in VAR location
	ble r7, r41, FLT3	# Test if for loop conditions are met by gt

	lda r10, $text3
	stw r10, $48(sp)

	ldw r50, $48(sp)		# ldw for print
	call _printstr

	ldw r7, $24(sp)		# Load I to test for loop condition
	ldw r8, $28(sp)		# Load I to test for loop condition

				# Test loop conditions
	add r7, r7, r8		# Add step
	stw r7, $24(sp)		# Store back in VAR location
	ble r7, r40, FLT2	# Test if for loop conditions are met by gt

	exit


text1: .string " \n"
text2: .string " \t---------\n"
text3: .string " \t---------\n"


