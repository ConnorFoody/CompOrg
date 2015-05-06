	.data
msg:	.word 0
nln:	.asciiz "\n"
.globl main
	.text
main:
	# a:s0 b:s1
	li $19, 10 # loop counter
	li $s0, 0 # a = 0
	li $s1, 5 # b = 5
loop: 	beq $s0, $19, endloop # if a = 10, exit loop
	add $s1, $s1, $s0 # b = b + a
	addi $s0, $s0, 1   # a = a + 1
	move $a0, $s1  # move value to be printed (b) into a0
	li $v0, 1 # code for printing int 
	syscall

	li $v0, 4
	la $a0, nln
	syscall
	j loop
endloop:

li $v0, 10
syscall
