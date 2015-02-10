#include <stdio.h>

int main(void){

	// print the header
	printf("dec hex  oct   char\n");
	int dec = 0; 
	for(dec = 110; dec < 130; dec++){
		char c = dec; 
		int oct = dec;
		printf("%-3d %#0x Oo%-6o %c\n", dec, dec, dec, dec);
	}
	return 0; 
}