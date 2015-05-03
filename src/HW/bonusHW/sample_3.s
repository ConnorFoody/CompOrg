	.data 
hello2_str: .asciiz "hello2 x: "
hello1_str: .asciiz "hello x: "
main_str: .asciiz "main x: "
nl_str: .asciiz "\n"

	.text
	.globl main
main:
	addi $sp, $sp, -4 # alloc room for jump register, x,a0
	li $a0, 1 # store x in register
	sw $a0, 0($sp) # store x on the stack

	jal hello # call hello

	la $a0, main_str # print main string
	li $v0, 4 # printing string
	syscall

	lw $a0, 0 ($sp) # load x off the stack
	addi $sp, $sp, 4 # clear stack back up
	li $v0, 1 # print a char
	syscall

	la $a0, nl_str # print newline string
	li $v0, 4 #printing string
	syscall

	li $v0, 10 # exit once done
	syscall
		
		
hello:
	addi $sp, $sp, -4 # make room for register and arg
	sw $31, 0 ($sp) # save the calling function's register to the stack

	
	addi $a0, $a0, 1 # increment x by one	
	addi $sp, $sp, -4 # make room for x on stack
	sw $a0, 0($sp) # save argument onto the stack

	jal hello2 # jump to hello 2

	la $a0, hello1_str # load hello string
	li $v0, 4 # printing string
	syscall
	
	lw $a0, 0($sp) # load arg off the stack
	addi $sp, $sp, 4 # move back up the stack
	li $v0, 1
	syscall

	la $a0, nl_str # print new line character
	li $v0, 4 # printing string
	syscall

	lw $31, 0($sp) # load register out of the stack 
	addi $sp, $sp, 4 # move stack back up
	jr $31

hello2:
	addi $sp, $sp, -4 # make room for a0
	addi $a0, $a0, 1 # x = x + 1
	sw $a0, 0($sp) # save x to stack

	la $a0, hello2_str # load hello2 string
	li $v0, 4 # printing string
	syscall

	lw $a0, 0($sp) # get x off the stack
	li $v0, 1 # print character
	syscall

	la $a0, nl_str # print new line character
	li $v0, 4 # printing string
	syscall

	addi $sp, $sp, 4 # clear stack back up
	jr $31
