	.data 
	.space 32
prompt: .asciiz "Type in a positive integer: "
in_loop: .asciiz "in loop \n"
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

	jal find_num_zeros 	# jump to num zeros
	j print			# jump over to print	

########################################################################
########################################################################
find_num_zeros:
	# find the number of zeros in the right half of number
	# bitwise and the input number with 65535 to isolate the right side
	
	# t0 --> original number
	# t1 --> bitwise and'ed number
	# t2 --> counter for number of zeros
	# t3 --> loop counter
	lw $t0, 0 ($sp)		# load the number off the stack
	andi $a0, $t0, 65535	# bitwise and it to isolate the right side

	# make room on stack for jump register and new number
	addi $sp, $sp, -8	# make room
	sw $31, 0 ($sp)		# put return address on stack
	
	jal find_num_ones	# go look for the number of ones

	lw $31, 0 ($sp)		# load return location off of the stack
	addi $sp, $sp, 4	# clean the stack up
	
	# print for testing 
	lw $a0, 0 ($sp)
	li $v0, 1
	syscall

	la $a0, newln_char
	li $v0, 4
	syscall 

	jr $31			# return back up

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
	lw $a0, 4 ($sp)
	li $v0, 1
	syscall

	# print newline string
	la $a0, newln_char
	li $v0, 4
	syscall
	
	jr $31
 
########################################################################
########################################################################
print:
	# print all the data
	lw $a0, 0($sp)		# load data off the stack
	addi $sp, $sp, 4	# clean the stack back up
	li $v0, 1		# code for printing integer
	syscall

	# print finished message
	la $a0, at_end
	li $v0, 4
	syscall

exit: 
	# exit cleanly once done
	li $v0, 10
	syscall
