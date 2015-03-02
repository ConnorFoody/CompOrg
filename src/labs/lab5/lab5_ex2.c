#include <stdio.h>

int isLittleEndian();

int main(void)
{
   
    int little = isLittleEndian();

    //Print the result of the union
    if (little) {
        printf("This machine is Little Endian\n");
    } else {
        printf("This machine is Big Endian\n");
    }

    return 0;

}

int isLittleEndian() {
   
   /* Create a union */
   union
   {
      int myint;
      /* Create an array of chars that matches */
      /* the size of the integer so we can display the bytes */
      char charArr[sizeof(int)];
   } myunion;
   int count = 0; 
   myunion.myint = 19088743;
   printf("Integer number used in Union: ");
   printf("%i\n", myunion.myint);
  
   /* Show the byte order */
   for(count = 0; count < 4; count++){
 	printf("%d ", myunion.charArr[count]);
   }
   printf("\n");
   /* Perform a test on the data and return 1 for little endian, 0 for big endian */
   printf("value of int in hex is: %x\n", myunion.myint);
  return myunion.charArr[0] > myunion.charArr[1]; 
}
