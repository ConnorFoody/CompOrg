	
	.data
	# message for printing
prompt_text_msg: .asciiz "Enter an input line: "
prompt_pattern_msg: .asciiz "Enter a pattern to search for: "
newln_char: .asciiz "\n"
done_msg: .asciiz "done\n"
debug_msg: .asciiz "debug\n"

	# messages once done
num_whitespaces_msg: .asciiz "# of whitespace characters: "
num_words_msg: 	.asciiz "# of words: "

	# arrays for holding strings
input_arr: .byte 0:100 	# input string is at most 100 chars
pattern_arr: .byte 0:11	# patter string is at most 11 chars

	# other values
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
	li $a1, 100	# size of the buffer
	syscall
	

	# print message to read pattern
	la $a0, prompt_pattern_msg # load the pattern message
	li $v0, 4	# print string command
	syscall
	
	# read in the pattern string
	li $v0, 8	# command to read string
	la $a0, pattern_arr # address of pattern array
	li $a1, 11	# size of the buffer
	syscall
	
	la $a0, input_arr # load address of input string to a0
	jal str_len	# find length of input string
	move $s0, $v0	# move the size of input arr to s0

	la $a0, pattern_arr # load address of pattern string to a0
	jal str_len	# find length of pattern string
	move $s1, $v0	# move the size of pattern string to s1


	jal analyze_input # do the stuff on the input
	jal print	# print the data we found
	j exit
#####################################################################
#####################################################################
analyze_input:
	# finds number of whitespace characters, number of words 
	# and if the pattern is found in the string
	# num_whitespace --> s2
	# num_words	 --> s3
	# pattern_found	 --> s4

	# set up s registers
	li $s2, 0 	# num whitespaces = 0
	li $s3, 0	# num words = 0
	li $s4, 0	# pattern_found = false
	
	
	addi $sp, $sp, -4 # make room for return addr on stack
	sw $31, 0 ($sp)	# save return address on stack

	# set up temp vars
	li $t0, 0	# looping over main string index
	la $t1, input_arr # stack pointer for string

	li $t2, 0	# pattern string index
	la $t3, pattern_arr # stack pointer for string

	li $t4, 1	# previous was whitespace = true

analyze_input_loop: 
	bge $t0, $s0, analyze_input_loop_end # if the loop reaches the end of thet string, exit
	
	# words happen when char transitions from whitespace to non-whitespace
	lb $a0, 0 ($t1) # load the current slot of t1
	jal check_is_whitespace_char # check if the current char is a whitespace char
	
	li $t8, 0 	# store value of false so we can compare
	beq $v0, $t8, not_whitespace # if check_is_whitespace_char == false, not whitespace
	
	li $t4, 1	# prev_was_whitespace = true
	addi $s2, $s2, 1 # num_whitespaces ++

	j prep_next_loop # go down the loop

not_whitespace:

	li $t8, 0	# store value of true so we can compare
	beq $t4, $t8, no_new_word # if prev_was_whitespace == false, no new word
	addi $s3, $s3, 1 # increase the word count by one

	# debugging msg 
	lb $a0, 0 ($t1)
	li $v0, 1
	syscall
	
	la $a0, newln_char
	li $v0, 4
	syscall

no_new_word:
	li $t4, 0 # prev_was_whitespace = false 
	
prep_next_loop:

	addi $t0, $t0, 1 # increase the index by one
	addi $t1, $t1, 1 # increase the pointer by one
	
	j analyze_input_loop # jump back up to top	

analyze_input_loop_end:

	lw $31, 0 ($sp)	# load return address off the stack
	jr $31		# return back up

#####################################################################
#####################################################################
check_is_whitespace_char: 
	# checks if a char is whitespace
	li $v0, 0	# load return address to be false
	
	li $t8, 32 # load space char 	
	beq $a0, $t8, check_is_whitespace_char_true # if char == whitespace char, return true
	
	li $t8, 9 # load tab char
	beq $a0, $t8, check_is_whitespace_char_true # if char == tab char, return true

	li $t8, 13 # load new line char
	beq $a0, $t8, check_is_whitespace_char_true # if char == newline char, return true
	
	li $t8, 10 # load endline char
	beq $a0, $t8, check_is_whitespace_char_true # if char == endline char, return true

	jr $31		# if didn't make it through the checks

check_is_whitespace_char_true:
	
	li $v0, 1	# load return address to be true
	jr $31		# go back up

#####################################################################
#####################################################################
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
	bnez $t1, str_len_loop  # check for null char
	move $v0, $t0	# move length of string to return var
	
	jr $31		# return back up

#####################################################################
#####################################################################
print:
	# print the number of whitespaces found
	la $a0, num_whitespaces_msg # load num whitespaces message
	li $v0, 4	# command for printing a string
	syscall

	move $a0, $s2 	# load number of whitespace
	li $v0, 1	# command for printing integer
	syscall
	
	la $a0, newln_char # load message for newline
	li $v0, 4 	# command for printing string
	syscall

	# print the number of words found
	la $a0, num_words_msg # load num words message
	li $v0, 4	# command for printing a string
	syscall

	move $a0, $s3 	# load number of words
	li $v0, 1	# command for printing integer
	syscall

	la $a0, newln_char # load message for newline
	li $v0, 4	# command for printing a string
	syscall

	jr $31 		# return back up
#####################################################################
#####################################################################

exit: 
	# finish the program
	la $a0, done_msg # load the done msg
	li $v0, 4	# printing string code
	syscall 

	li $v0, 10	# code for exiting the program
	syscall		
			
