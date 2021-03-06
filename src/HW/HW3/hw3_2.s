
###############################################################
#	Homework 3 part 2
#	author: Connor Foody
#	lab section: 1
# 	
#	Inputs: a line of text and a pattern to search for
#	Outputs: the number of whitespace and non-whitespace 
#		 characters in the input line, the number of
#		 words in the input line and whether or not
#		 the given pattern was found
#
#	program reads the input and pattern lines, then 
#	loops through the input line looking for whitespaces, 
#	words and the pattern. Finally, it prints the info it
#	found out. 
#
###############################################################	
	.data
	# message for printing
prompt_text_msg: .asciiz "Enter an input line: "
prompt_pattern_msg: .asciiz "Enter a pattern to search for: "
newln_char: .asciiz "\n"
done_msg: .asciiz "done\n"
debug_msg: .asciiz "debug\n"

	# messages once done
num_whitespaces_msg: .asciiz "# of whitespace characters: "
num_non_whitespaces_msg: .asciiz "# of non-whitespace characters: "
num_words_msg: 	.asciiz "# of words: "
pattern_found_msg: .asciiz "The user pattern was found within the input line.\n"
pattern_not_found_msg: .asciiz "The user pattern was NOT found within the input line.\n"
only_whitespaces_msg: .asciiz "Line contains whitespace characters only!\n"

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

	li $t8, 1	# store value of 1 for use in sub (no sub immediate)
	sub $s1, $s1, $t8 # subtract one so we don't get end char

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
	
	# put return address on the stack	
	addi $sp, $sp, -4 # make room for return addr on stack
	sw $31, 0 ($sp)	# save return address on stack

	# set up temp vars
	li $t0, 0	# looping over main string index
	la $t1, input_arr # stack pointer for string

	li $t2, 0	# pattern string index
	la $t3, pattern_arr # stack pointer for string

	li $t4, 1	# previous was whitespace = true

	li $t5, 0	# pattern is matching currently

analyze_input_loop: 
	bge $t0, $s0, analyze_input_loop_end # if the loop reaches the end of thet string, exit
	
	# words happen when char transitions from whitespace to non-whitespace
	lb $a0, 0 ($t1) # load the current slot of t1
	jal check_is_whitespace_char # check if the current char is a whitespace char
	
	li $t8, 0 	# store value of false so we can compare
	beq $v0, $t8, not_whitespace # if check_is_whitespace_char == false, not whitespace
	
	li $t4, 1	# prev_was_whitespace = true
	addi $s2, $s2, 1 # num_whitespaces ++

	#j prep_next_loop # go down the loop
	j check_pattern_matches # go check if the pattern matches

not_whitespace:

	li $t8, 0	# store value of false so we can compare
	beq $t4, $t8, no_new_word # if prev_was_whitespace == false, no new word
	addi $s3, $s3, 1 # increase the word count by one

	# can fall down through here OK 

no_new_word:
	li $t4, 0	# prev_was_whitespace = false 

check_pattern_matches:	
	# check to see if the pattern is in the input
	# if the pattern counter == pattern length, then we have a word
	li $t8, 1	# store the value of true for loop
	beq $t8, $s4, prep_next_loop # if pattern already found in string, don't bother with this code
	bne $t2, $s1, pattern_not_at_end # index of pattern != length of pattern then we havn't found the word yet
	li $s4, 1	# if length of matching == length of pattern, match found
	j prep_next_loop # skip down to the last bit
		
pattern_not_at_end:

	lb $t8, 0 ($t1)	# load current input slot to t8
	lb $t9, 0 ($t3)	# load current pattern slot to t9
	bne $t8, $t9, pattern_not_match	# if input slot != pattern slot, pattern is broken
	
	li $t5, 1	# set was_matching to true
	addi $t2, $t2, 1 # increase the pattern index
	addi $t3, $t3, 1 # increase the pattern pointer

	j prep_next_loop # jump down to prep_next_loop

pattern_not_match: 
	li $t8, 0	# store value of false so we can compare
	beq $t5, $t8 prep_next_loop # if loop wasn't matching before, we don't need to reset it
	li $t5, 0	# if wasn't matching, set was_matching to false
	la $t3, pattern_arr # load start address of the pattern string back in
	li $t2, 0	# set index we are looking at in pattern string back to zero

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
	
	move $t0, $zero	# store the value of zero
	j str_len_test 	# jump to loop test condition

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
	# first check if the line is all whitespaces
	sub $a0, $s0, $s2 # num non_whitespace chars = num chars - num whitespace chars
	beq $a0, $zero, only_whitespaces # if num non-whitespace chars = 0, print message for only whitepsaces

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

	# print the number of non-whitespace characters
	la $a0, num_non_whitespaces_msg # load num non-whitespace 
	li $v0, 4	# command for printing a string
	syscall

	sub $a0, $s0, $s2 # num non_whitespace chars = num chars - num whitespace chars
	li $v0, 1	# command for printing integer
	syscall

	la $a0, newln_char # load message for newline
	li $v0, 4	# command for printing string
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

	li $t8, 1	# store the value of true for use
	beq $s4, $t8, print_pattern_found # when the pattern was found skip down
	la $a0, pattern_not_found_msg # if pattern not found, set the message to not found
	j print_found_msg # skip down to print 

print_pattern_found:
	la $a0, pattern_found_msg # load pattern found message 

print_found_msg:
	li $v0, 4 	# code for printing string
	syscall		

	j exit_print	# jump down to finish the program

only_whitespaces: 
	la $a0, only_whitespaces_msg # load only whitespaces message
	li $v0, 4	# commadn for printing a string
	syscall 

exit_print:
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
			
