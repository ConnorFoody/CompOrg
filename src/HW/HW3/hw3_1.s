	.data 
	.space 32
prompt: .asciiz "Type in a positive integer: "
at_end:	.asciiz "done\n"
newln_char: .asciiz "\n"
	
	.text
	.globl main

	# 1) prompt user for input
	# 2) read input from user (check?)
	# 3) calc output info
	# 4) print data
	# 5) exit
main: 
	# prompt user for input
	li $v0, 4		# command to print string
	la $a0, prompt 		# load start addr. of string
	syscall			# print

	# read the input
	li $v0, 5		# command to load integer into buffer
	syscall			# load integer into buffer

	# put integer onto the stack
	addi $sp, $sp, -4	# make room for one integer
	sw $v0, 0 ($sp)		# put the integer onto the stack

	jal find_num_zeros_right # find the number of zeros on the right side of the number
	jal find_num_ones_left	# find the number of ones on the left of the number
	jal print		# jump over to print	
	j exit			# exit


########################################################################
########################################################################
find_num_zeros_right:
	# find the number of zeros in the right half of number
	# bitwise and the input number with 65535 to isolate the right side
	
	lw $t0, 0 ($sp)		# load the number off the stack
	andi $a0, $t0, 65535	# bitwise and it to isolate the right side

	# make room on stack for jump register and new number
	addi $sp, $sp, -8	# make room
	sw $31, 0 ($sp)		# put return address on stack
	
	jal find_num_ones	# go look for the number of ones

	lw $31, 0 ($sp)		# load return location off of the stack
	addi $sp, $sp, 4	# clean the stack up
	
	# subtrack the number we found from 16 to find number of zeros
	lw $t0, 0 ($sp)		# load number to temp register
	li $t1, 16		# constant to subtract from
	sub $t0, $t1, $t0	# subtrack number of ones from 16
	sw $t0, 0 ($sp)		# save number of zeros to the stack

	jr $31			# return back up

########################################################################
########################################################################
find_num_ones_left:
	# find the number of ones in the right half of the number
	
	lw $t0, 4 ($sp)		# load original number off the stack
	li $t1, 65535		# mask to isolate right side (number is shifted over so left becomes right)
	srl $t0, $t0, 16	# shift number over because find_num_ones checks 1st 16 bits
	
	and $a0, $t0, $t1	# do bitwise and on input number

	addi $sp, $sp, -8	# make room on stack for return address and new number
	sw $31, 0 ($sp)		# put return address on stack

	jal find_num_ones	# go look for the number of ones

	lw $31, 0 ($sp)		# load return location off the stack
	addi $sp, $sp, 4	# clean the stack up

	jr $31			# return back up 

	# make room on stack for jump register and new number

########################################################################
########################################################################
find_num_ones:
	# find the number of one's in an input number
	# a0 --> input number
	# t0 --> loop counter
	# t1 --> counter for number of 1's

	li $t0, 0		# counter for number of zeros
	li $t1, 1		# loop counter \ mask
	li $t2, 65536 		# end condition for the loop

ones_loop: beq $t1, $t2, end_loop
	
	# check if slot is a one
	and $t3, $a0, $t1	# bitwise and the number with the mask
	
	blt $t3, 1, prep_for_next_loop # result of bitwise and with mask is zero, prep for next loop
	# else, add to counter
	addi $t0, $t0, 1	# add one to loop counter
	
prep_for_next_loop: 
	# increase mask
	sll $t1, $t1, 1		# increase mask
	j ones_loop		# go back up loop

end_loop: 
	# put the return value onto the stack
	sw $t0, 4 ($sp)		# put num zeros onto the stack
	
	# print for testing
	#lw $a0, 4 ($sp)
	#li $v0, 1
	#syscall

	# print newline string
	#la $a0, newln_char
	#li $v0, 4
	#syscall
	
	jr $31
 
########################################################################
########################################################################
print:
	# print number of zeros on the right
	lw $a0, 0($sp)		# load data off the stack
	addi $sp, $sp, 4	# clean the stack back up
	li $v0, 1		# code for printing integer
	syscall

	# print new line
	la $a0, newln_char	# load newline character
	li $v0, 4		# code to print string
	syscall
	
	jr $31			# return back to main


exit: 
	# print finished message
	la $a0, at_end
	li $v0, 4
	syscall
	
	# exit cleanly once done
	li $v0, 10
	syscall
