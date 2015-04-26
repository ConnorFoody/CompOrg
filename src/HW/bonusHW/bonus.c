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
	char* name;
	int definition; 
	struct line_value* head;
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

void check_identifier(char* token, struct identifier_reference* identifiers, int* num_identifiers){
	// checks if a token is an identifier
	int i = 0; 

	printf("\t\tchecking if \"%s\" is an identifier\n", token);
	while(i != *num_identifiers){
		if(strcmp(identifiers[i].name, token) == 0){
			// found an identifier already in the list, add it
			printf("\t\ttoken \"%s\" is an identifier, updating table\n", token);

			// alloc new line_value
			struct line_value* tmp;
			if((tmp = (struct line_value*) malloc(sizeof(struct line_value))) == NULL){
				fprintf(stderr, "ERROR: could not alloc mem for line_value\n");
				exit(1);
			}
			struct line_value* head = identifiers[i].head;
			tmp.next = NULL;
			tmp.value = 0;

			// add the new line value onto the end of the list
			while(head.next != NULL){
				head = head.next;
			}
			head.next = tmp;

			// get out
			return; 
		}
		i++;
	}
	
}

void add_new_identifier(char* identifier, struct identifier_reference* identifiers, int* num_identifiers){
	
	int i = 0; 
	
	if(strlen(identifier) > 11){
		printf("WARNING: tried to add an identifier that was too long\n");	
		return;
	}

	// fix the token 
	identifier[strlen(identifier) - 1] = '\0';
	
	while(i != *num_identifiers){
		if(strcmp(identifiers[i].name, identifier) == 0){
			// found an identifier already in the list, add it
			printf("\t\tWARNING: identifier found in table\n");
			return; 
		}
		i++;
	}

	printf("\t\tidentifier is not already in the list, adding it in: %s\n", identifier);
	// build and add new identifier
	struct identifier_reference tmp; 
	if((tmp.name = (char*) malloc(11 * sizeof(char))) == NULL){ // give the right ammount of space
		fprintf(stderr, "ERROR: could not alloc memory to add a new identifier\n");
		exit(1);
	}
	strcpy(tmp.name, identifier); // need to check for error?
	identifiers[i] = tmp;

	// update count
	(*num_identifiers)++;

	return;
}

void analyze_line(char* line, struct identifier_reference* identifiers, int* num_identifiers){
	// analyzes the line
	//printf("analyzing line \" %s \" \n", line);
		
	// deliminate by commas and spaces	
	char* tkn = strtok(line, ",\n \t");

	// check if the line is empty or is a comment line
	if(tkn == NULL || tkn[0] == '\n' || tkn[0] == '#'){
		printf("\tline is empty or comment line\n");
		return;
	}

	// loop through all the tokens
	while(tkn != NULL){

		// if line has no more useful info, return	
		if(tkn[0] == '#' || tkn[0] == '\n' || tkn[0] == '.') { return; }
	
		// if register token, skip it
		if(tkn[0] == '$'){
			tkn = strtok(NULL, ", ");
			continue;
		}

		printf("\tlooking at token: %s\n", tkn);	
		if(tkn[strlen(tkn) - 1] == '\n'){
			printf("\tcaught new line, going to fix\n");
			tkn[strlen(tkn) - 1] = '\0';
		}

		if(tkn[strlen(tkn) - 1] == ':'){
			printf("\t\tfound an identifier definition: %s\n", tkn);
			add_new_identifier(tkn, identifiers, num_identifiers);
		}	
		else{
			check_identifier(tkn, identifiers, num_identifiers);
		}
		//printf("\tlast char is: |%c|\n", tkn[strlen(tkn) -1 ]);

		tkn = strtok(NULL, ", \t\n");
	}

}

int analyze_file(FILE* infp, FILE* outfp){
	// reads the input file, puts line numbers on the front then moves on

	int line_count = 1;		// track line numbers
	char* in_line; 				// line buffer
	struct identifier_reference* identifiers; // array of used identifiers
	int num_identifiers = 0; // keeps track of how many identifiers we have found

	// alloc with size 85 to fit line numbers
	if((in_line = (char*) malloc(85 * sizeof(char))) == NULL){ // alloc the line buffer
		fprintf(stderr, "ERROR: could not alloc enough memory for in_line\n");
		exit(1);
	}

	// alloc identifier array
	if((identifiers = (struct identifier_reference*) malloc(100 * sizeof(struct identifier_reference))) == NULL){
		fprintf(stderr, "ERROR: could not alloc enough memory for identifiers array\n");
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

		// analyze the line 
		analyze_line(in_line, identifiers, &num_identifiers);
		
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
