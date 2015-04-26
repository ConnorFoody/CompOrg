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
#include <string.h>

struct line_value{
	int value;
	struct line_value* next;
};

struct identifier_reference{
	char name[11];
	int definition; 
	struct line_value* head;
	struct identifier_reference* next;
};

int analyze_file(FILE* infp, FILE* outfp);

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

	// analyze the file
	analyze_file(infp, outfp);

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

char* add_line_number(char* line, int number){
	char* to_add; 				// line to add	

	// alloc the memory 
	if((to_add = (char*) malloc(4 * sizeof(char))) == NULL){
		fprintf(stderr, "ERROR: could not alloc enough memory for line string\n");
		exit(1);
	}

	// print the string and concat
	sprintf(to_add, "%d\t", number);
	strcat(to_add, line);

	// return the new line
	return to_add;
}

int analyze_file(FILE* infp, FILE* outfp){
	// reads the input file, puts line numbers on the front then moves on

	int line_count = 1;		// track line numbers
	char* in_line; 				// line buffer
	
	// alloc with size 85 to fit line numbers
	if((in_line = (char*) malloc(85 * sizeof(char))) == NULL){ // alloc the line buffer
		fprintf(stderr, "ERROR: could not alloc enough memory for in_line\n");
		exit(1);
	}

	printf("about to enter the loop\n");
	while(1){
	
		// read the next line
		if(fgets(in_line, 80, infp) == NULL){
			// at end of file
			printf("reached the end of the file\n");
			break;
		}
		
		// add the line numbers
		in_line = add_line_number(in_line, line_count);	
		// write the modified line to the output
		if(fputs(in_line, outfp) == EOF){
			fprintf(stderr, "ERROR: could not write line to the outfile\n");
			exit(1);
		}
		line_count++;
	}
	return 0;
}
