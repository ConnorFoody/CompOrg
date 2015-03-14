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
	jal find_highest_power	# find the highest power of 4 the number is divisible by
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

########################################################################
########################################################################
find_highest_power:
	# find the highest power of 4 that evenly divides the input number
	
	lw $t0, 8 ($sp) 	# load original number off the stack
	
	li $t1, 0		# loop counter
	li $t2, 1		# number to div by 
	li $t3, 0		# holder for remainder
	li $t4, 0		# loop end condition
	li $t5, 4		# const for multiplying by

	div $t0, $t2		# need to check before entering loop
	mfhi $t3		# move remainder to t3

highest_power_loop: bne $t4, $t3, end_highest_power_loop # loop until the remainder != 0 

	bgt $t2, $t0, end_highest_power_loop # if the divisor is greater than original num, end loop
	
	move $a0, $t2		# debugging message
	li $v0, 1
	syscall

	la $a0, newln_char 
	li $v0, 4
	syscall

	addi $t1, $t1, 1 	# increase loop counter by 1

	mult $t2, $t5		# multiply by 4
	mflo $t2		# move number over to t2

	div $t0, $t2		# divide to get remainder
	mfhi $t3		# move remainder over to t3
	
	j highest_power_loop	# jump back up to the top of the loop

end_highest_power_loop:
	beq $t4, $t1, no_sub_highest_power	# loop will put us one over, so subtract if we were in it
	sub $t1, $t1, 1		# move power down one to make everything happy (check for < 0?)	
no_sub_highest_power:
	addi $sp, $sp, -4	# make room on stack	
	sw $t1, 0 ($sp)		# save highest power to the stack 
	
	la $a0, newln_char
	li $v0, 4
	syscall 

	# print for testing
	lw $a0, 0 ($sp)
	li $v0, 1
	syscall

	# print newline string
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

ones_loop: beq $t1, $t2, end_ones_loop
	
	# check if slot is a one
	and $t3, $a0, $t1	# bitwise and the number with the mask
	
	blt $t3, 1, prep_for_next_ones_loop # result of bitwise and with mask is zero, prep for next loop
	# else, add to counter
	addi $t0, $t0, 1	# add one to loop counter
	
prep_for_next_ones_loop: 
	# increase mask
	sll $t1, $t1, 1		# increase mask
	j ones_loop		# go back up loop

end_ones_loop: 
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
