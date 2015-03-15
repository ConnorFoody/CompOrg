	
	.data
	# message for printing
prompt_text_msg: .asciiz "Enter an input line: "
prompt_pattern_msg: .asciiz "Enter a pattern to search for: "
newln_char: .asciiz "\n"
done_msg: .asciiz "done\n"

	# arrays for holding strings
input_arr: .byte 0:100 	# input string is at most 100 chars
pattern_arr: .byte 0:11	# patter string is at most 11 chars

	.text
	.globl main

main:
	# print message to read line
	la $a0, prompt_text_msg	# load the print message
	li $v0, 4	# print string command
	syscall

	# read in the input string
	li $v0, 8	# command to read input string
	la $a0, input_arr # address of input array
	li $a0, 100	# size of the buffer
	syscall

	# print message to read pattern
	la $a0, prompt_patter_msg # load the pattern message
	li $v0, 4	# print string command
	syscall
	
	# read in the pattern string
	li $v0, 8	# command to read string
	la $a0, pattern_arr # address of pattern array
	li $a0, 11	# size of the buffer
	syscall
	
	j exit

str_len:
	# (based on response from daniweb question "mips string length")
	# finds length of string and stores it in input loc
	# a0 --> string
	# a1 --> where to put length of string
	
	move $t0, $zero
	j str_len_test

str_len_loop:
	addi $a0, $a0, 1 # increment the string pointer
	addi $t0, $t0, 1 # increment the count 

str_len_test:
	lb $t1, 0 ($a0) # load next char
	bnez $t1, str_len_loop 
	
	



exit: 
	# finish the program
	la $a0, done_msg # load the done msg
	li $v0, 4	# printing string code
	syscall 

	li $v0, 10	# code for exiting the program
	syscall		
			
