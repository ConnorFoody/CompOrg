rm a.out; 
rm bonus;
gcc -c -g bonus.c; 
gcc -g bonus.c -o bonus; 
valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./bonus sample.s output.lst;
