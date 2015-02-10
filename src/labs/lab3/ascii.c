#include <stdio.h>

int main(void){

	// print the header
	printf("dec hex  oct   char\n");
	int dec = 0; 
	for(dec = 32; dec < 128; dec++){
		char c = dec; 
		// printing octal is kind of hack-y
		printf("%-3d %#0x Oo%-6o %c\n", dec, dec, dec, dec);
	}
	return 0; 
}
