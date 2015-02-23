#include <stdio.h>
#include <stdlib.h>

int main(void) 
{

   char *charPointer; 
   int *intPointer; 
   float *floatPointer; 
   double *doublePointer; 
   charPointer = calloc(4, sizeof(char));
   intPointer = calloc(4, sizeof(int));
   floatPointer = calloc(4, sizeof(float));
   doublePointer = calloc(4, sizeof(double));

   printf("charPointer is at: %#0x \t charPointer + 1 is at: %#0x\n", charPointer, charPointer+1); 
   printf("intPointer is at: %#0x \t intPointer + 1 is at: %#0x\n", intPointer, intPointer+1); 
   printf("floatPointer is at: %#0x \t floatPointer + 1 is at: %#0x\n", floatPointer, floatPointer+1); 
   printf("doublePointer is at: %#0x \t doublePointer + 1 is at: %#0x\n", doublePointer, doublePointer+1); 
   return 0;

}
