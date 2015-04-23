/**
 * author: Connor Foody 
 * 
 * Bonus homework assignment for CompOrg
 * 
 * Input is an s file, output is a lst file
 *
 * Output is same as input, but with line nums appended
 * Output table of identifiers with definition line, and
 * use lines
 */

// struct to hold identifier reference table data

#include <stdio.h>
#include <stdlib.h>

struct line_value{
	int value;
	struct line_value* next;
};

struct identifier_reference{
	char name[11];
	int definition; 
	struct line_value* head;
};

int main(int argc, char** argv){
	printf("hello world\n");


	char* infile; // store the name of the input file
	char* outfile; // store the name of the ouput file

	FILE* infp; // file pointer for the input file
	FILE* outfp; // file pointer for the output file

	// check command line args
	if(argc != 3){
		// TODO: make a proper usage function
		printf("ERROR: incorrect number of inputs, expected 3 and got %d\n", argc);
		return 0;
	}

	// read in file
	infile = argv[1];
	outfile = argv[2];

	// open the input file for reading
	if((infp = fopen(infile, "r")) == NULL){
		fprintf(stderr, "ERROR: could not open input file for reading\n");
		exit(1);
	}

	// open the output file for reading
	if((outfp = fopen(outfile, "w")) == NULL){
		fprintf(stderr, "ERROR: could not open output file for writing\n");
		exit(1);
	}

	// close input file
	if(fclose(infp) == EOF){
		fprintf(stderr, "ERROR: closing input file\n");
		exit(1);
	}

	// close output file
	if(fclose(outfp) == EOF){
		fprintf(stderr, "ERROR: closing output file\n");
		exit(1);
	}

	return 0;	
}
