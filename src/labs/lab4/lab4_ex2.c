#include <stdio.h>

/* prototype for showBits function */
void showBits( unsigned int );

/* main function begins here */
int main( void )
{

   unsigned int num1; 
   unsigned int num2;
   unsigned int mask1;
   unsigned int mask2;
   
   /* show an example of bitwise and & */
   num1 = 0xDEADBEEF;
   mask1 = 1;
   printf( "Operation - bitwise & between:\n");
   showBits(num1);
   showBits(mask1);
   printf( "Result is:\n");
   showBits( num1 & mask1 );

   /* show an example of bitwise or | */
   num1 = 0xDEADBEEF;
   mask1 = 1;
   printf( "Operation - bitwise | between:\n");
   showBits(num1);
   showBits(mask1);
   printf( "Result is:\n");
   showBits( num1 | mask1 );

   /* show an example of bitwise xor ^ */
   num1 = 0xDEADBEEF;
   mask1 = 1;
   printf( "Operation - bitwise ^ between:\n");
   showBits(num1);
   showBits(mask1);
   printf( "Result is:\n");
   showBits( num1 ^ mask1 );

   /* show an example of bitwise complement ~ */
   num1 = 0xDEADBEEF;
   mask1 = 1;
   printf( "Operation - bitwise ~ between:\n");
   showBits(num1);
   showBits(mask1);
   printf( "Result is:\n");
   showBits(~ num1 );
 
   /* check if the third bit from the right of an */
   /* unsigned int is a 1 or a zero */
   num1 = 0xDEADBEEF;
   mask1 = 4;
   if(num1 & mask1){
	printf("3rd from right is 1\n");
   }
   else{
 	printf("3rd from right is 0\n");
   }
   /* change the first 8 bits of a */
   /* number to 10101010 keep the rest intact */
   num2 = num1 & 0xFF000000;
   num1 = num1 - num2;
   num1 += 0xAA000000;
   showBits(num1);
  
   return 0;

}

void showBits( unsigned int num )
{
   /* shows the bits for each byte of */
   /* an unsigned int value           */
    
   unsigned int count; /* counter variable */
   unsigned int mask; /* mask for isolating bits */
   mask = 1 << 31; /* prep mask for first bit */

   printf("%10u, Bits = ",num);
   for(count = 0; count < 32; count++)
   {
      if( num & mask )
      {
         putchar( '1' );
      } 
      else 
      {
         putchar( '0' );
      } 
    
      /* put a space in for formatting */
      if( (count+1) % 8 == 0 ) putchar( ' ' );
      /* shift input num left by 1 */
      num = num << 1;
   }
   putchar('\n');
}
