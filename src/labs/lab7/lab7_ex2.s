# This MAL program will find the maximum value in an
# integer array of size 12

            .data
arr:        .word     1, 2048, 4, 512, 16, 64, 32, 128, 8, 512, 1024, 2
outstr:     .asciiz   "The maximum value in the array is "
newline:    .asciiz   "\n"

            .text
            .globl main
main:       # TODO: Initialize constants, etc.
	li $a0, 0 # load array slot counter to 0
	li $a1, 0 # load max value as 0

loop:       # TODO: Find the maximum value in the array
	blt $a0, 12, endloop # loop while counter < size (12)
	la $s0, arr # store address of array
	bgt $a1, $a0 (arr) 
new_max: 

not_max: 


endloop: 

            # TODO: Print out the max value
print:

            li      $v0, 10         # Exit: to the bat mobile...
            syscall
