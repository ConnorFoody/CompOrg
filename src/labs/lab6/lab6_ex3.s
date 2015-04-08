# This MAL program will prompt the user for one character
# It will then check whether the character is a digit or not
# and then print an appropriate message

   .data # Data section of program

   # To get a single character from the keyboard
   # we need to read the subsequent newline
   # character typed by the user and also provide
   # space for the null, ’\0’, character.
   # Thus the buffer must be at least 3 spaces
   inchar:  .space 3  # 3 bytes of data
   prompt:  .asciiz "Type a char: "
   yes_ans: .asciiz "Yes - It is a digit\n"
   no_ans:  .asciiz "No - It is not a digit\n"

   .text # Text section of program
   .globl main

   # Main program section
   main:
      # Prompt user for a character
      li   $v0, 4          # command for print string
      la   $a0, prompt     # remember to load start addr.
      syscall              # of prompt string

      # TODO: Read the character from the user
      li $v0, 12 # load the char into buffer
      syscall
      blt $v0, 48, not_dig
      bgt $v0, 57, not_dig

      # TODO: Check whether the character is a digit

      # TODO: Section to print yes answer and stop
   yes_dig:
      li $v0, 4
      la $a0, yes_ans
      syscall
      li $v0, 10
      syscall

      # Section to print no answer and stop
   not_dig:
      li   $v0, 4
      la   $a0, no_ans
      syscall
      li   $v0, 10
      syscall
