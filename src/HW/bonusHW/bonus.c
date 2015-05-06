/*
 * Bonus homework assignment for CompOrg
 * Author: Connor Foody 
 * 
 * Inputs: input-file output-file
 * 			- input-file is a valid mips assembly file
 * 			- output file is any file type
 *
 * Outputs: input-file with line numbers added, and
 * 					table of identifier definitions and use
 * 					lines written to output file 
 *
 * Program finds the identifiers, their definitions
 * and the use lines then writes them to the output
 * file. Also puts line numbers infront of the text 
 * from the input file
 *
 * Lab Section 1 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LL node for identifier use lines
struct line_value{
	int value; // use line number
	struct line_value* next; // next node in the list (null if at end)
}; // end line_value

// identifier definition and use line data
struct identifier_reference{
	char* name; // identifier name
	int definition; // line number of identifier definition 
	struct line_value* head; // head of line_value linked-list
}; // end identifier_reference

// prototype for function called by main
void analyze_file(FILE* infp, FILE* outfp);

// main program function
int main(int argc, char** argv){

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
} // end main

// finds the number of digits in a positive number
// inputs: 
// 	number - int, number to find the number of digits for
//
// output: 
// 	int, the number of digits in the input number
int find_num_digits(int number){
	if(number < 10) { return 1; } 
	return 1 + find_num_digits(number / 10);	
} // end find_num_digits

// adds the line number to the front of the input string
// inputs: 
// 	line - string, line to add the line number to
// 	number - int, the line number for the input string
//
// outputs:
//  string, the input string with the line number and a tab insert on the front
char* add_line_number(char* line, int number){
	char* to_add; // line we will return	
	
	// find how many spaces we need to allocate for the number and the tab
	int size_of_number = find_num_digits(number) + 1;
	
	// size of the string is the legnth of the input string + the length of the number string + breathing room	
	// to_add is returned to analyze_file where it is used then freed
	if((to_add = (char*) malloc((2 + size_of_number + strlen(line))* sizeof(char))) == NULL){
		fprintf(stderr, "ERROR: could not alloc enough memory for line string\n");
		exit(1);
	}

	// print the line numbers in then concat the old string on
	// line and to_add point to different memory locations
	sprintf(to_add, "%d\t", number);
	strcat(to_add, line);

	return to_add;
} // end add_line_number

// checks if an identifier is in the list and updates use lines accordingly
// inputs: 
// 	token - string, the potential identifier
// 	identifiers - identifier_reference array, identifier data
// 	num_identifiers - int, how many identifiers we have found so far
// 	line_number - int, which line the token is on
//
// output:
//  none
void check_identifier(char* token, struct identifier_reference* identifiers, int num_identifiers, int line_number){
	// checks if a token is an identifier
	int i = 0; 

	while(i != num_identifiers){
		if(strcmp(identifiers[i].name, token) == 0){
			// found an identifier already in the list, add it

			// alloc new line_value
			struct line_value* tmp;
			if((tmp = (struct line_value*) malloc(sizeof(struct line_value))) == NULL){
				fprintf(stderr, "ERROR: could not alloc mem for line_value\n");
				exit(1);
			}
			tmp->next = NULL;
			tmp->value = line_number;

			// get the head of the line_value LL
			struct line_value* head = identifiers[i].head;

			if(head == NULL){
				// if the identifier has not yet been used, add it as the head 
				identifiers[i].head = tmp;
				return;
			}

			// add the new line value onto the end of the list
			while(head->next != NULL){
				head = head->next;
			}
			head->next = tmp;

			// get out
			return; 
		}
		i++;
	}
} // end check_identifier

// adds a new identifier definition
// inputs:
// 	identifier - string, the potential identifier
// 	identifiers - identifier_reference array, the identifier data
// 	num_identifiers - int, how many identifiers have been found so far. Pass by reference so it can be incremented
// 	line_number - int, which line the identifier is on
//
// output:
// 	none
void add_new_identifier(char* identifier, struct identifier_reference* identifiers, int* num_identifiers, int line_number){
	int i = 0; 
	
	if(strlen(identifier) > 11){
		fprintf(stderr, "WARNING: tried to add an identifier that was too long\n");	
		exit(1);
	}

	// token has ':' at end, replace with null character 
	if(identifier[strlen(identifier) - 1] != ':'){
		printf("\tWARNING: expected token to end in ':'\n");
		return;
	}

	// chop the ':' off the identifier with a null termination
	identifier[strlen(identifier) - 1] = '\0';
	
	// look for the identifier	
	while(i != *num_identifiers){
		if(strcmp(identifiers[i].name, identifier) == 0){
			// found an identifier already in the list
			// should not happen b/c it would mean a redefinition
			fprintf(stderr, "ERROR: redefinition of identifier\n");
			return; 
		}
		i++;
	}

	// build and add new identifier
	struct identifier_reference tmp; 
	if((tmp.name = (char*) malloc(11 * sizeof(char))) == NULL){ // give the right ammount of space
		fprintf(stderr, "ERROR: could not alloc memory to add a new identifier\n");
		exit(1);
	}
	strcpy(tmp.name, identifier);
	tmp.head = NULL;
	tmp.definition = line_number;
	
	// put it in the array
	identifiers[i] = tmp;

	// update count
	(*num_identifiers)++;

	return;
} // end add_new_identifier

// processes the line, looking for identifier definitions or use lines
// inputs: 
// 	line - string, the line read from the file
// 	identifiers - identifier_reference array, the identifier data
// 	num_identifiers - int, how many identifiers have been found so far, pass by reference so can update when new identifier found
// 	line_number - int, which line was read from the input file
// 	scan_definition - bool, 1 = looking for definitions only, 0 = looking for use lines only
//
// output:
// 	none
void analyze_line(char* line, struct identifier_reference* identifiers, int* num_identifiers, int line_number, int scan_definition){
		
	// deliminate by commas and spaces	
	char* tkn = strtok(line, ",\n \t");

	// check if the line is empty or is a comment line
	if(tkn == NULL || tkn[0] == '\n' || tkn[0] == '#'){
		// if the line is empty or we reached a comment, we are done with this line
		return;
	}

	// loop through line until there are no more tokens
	while(tkn != NULL){

		// if line has no more useful info, return	
		if(tkn[0] == '#' || tkn[0] == '\n' || tkn[0] == '.') { return; }
	
		// if register token, skip it
		if(tkn[0] == '$'){
			tkn = strtok(NULL, ", ");
			continue;
		}

		// null terminate the token
		if(tkn[strlen(tkn) - 1] == '\n'){
			tkn[strlen(tkn) - 1] = '\0';
		}

		if(tkn[strlen(tkn) - 1] == ':' && scan_definition){
			// if we found a definition and we are looking for definitions
			add_new_identifier(tkn, identifiers, num_identifiers, line_number);
		}	
		else if(!scan_definition){
			// if we are looking for use lines, check if the token is an identifier
			check_identifier(tkn, identifiers, *num_identifiers, line_number);
		}

		// get the next token
		tkn = strtok(NULL, ", \t\n");
	}
} // end analyze line

// prototypes for functions called by analyze_file
void print_table(FILE* outfp, struct identifier_reference* identifiers, int num_identifiers);
void free_identifiers(struct identifier_reference* identifiers, int num_identifiers);

// reads from the input file, analyzes the info, then writes to the output file
// inputs:
// 	infp - the input file to be processed
// 	outfp - the output file to be written to
// 
// outputs:
// 	none
void analyze_file(FILE* infp, FILE* outfp){

	int line_count = 1;		// track line numbers
	char* in_line; 				// line buffer
	char* line_cpy; 			// so that the line can be changed
	char* tmp; 						// holds pointers so in_line doesn't get all messed up

	struct identifier_reference* identifiers; // array of used identifiers
	int num_identifiers = 0; // keeps track of how many identifiers we have found

	// alloc with size 82 to leave a little extra breathing room
	if((in_line = (char*) malloc(82 * sizeof(char))) == NULL){ // alloc the line buffer
		fprintf(stderr, "ERROR: could not alloc enough memory for in_line\n");
		exit(1);
	}
	if((line_cpy = (char*) malloc(82 * sizeof(char))) == NULL){ // alloc the line copy
		fprintf(stderr, "ERROR: could not alloc enough memory for line cpy\n");
		exit(1);
	}

	// alloc identifier array
	if((identifiers = (struct identifier_reference*) malloc(100 * sizeof(struct identifier_reference))) == NULL){
		fprintf(stderr, "ERROR: could not alloc enough memory for identifiers array\n");
		exit(1);
	}

	// jumps can be referenced before they are defined
	// scan file twice, first for definitions then for use lines
	while(1){ // first scan
	
		// read the next line
		if(fgets(in_line, 80, infp) == NULL){
			// at end of file, end the loop
			break;
		}
	
		// copy the line so analyze_line and it's subfunctions can change it
		strcpy(line_cpy, in_line);		

		if(strlen(in_line) > 80){
			fprintf(stderr, "ERROR: the input line is too long\n");
			exit(1);
		}

		// scan the line for definitions 
		analyze_line(line_cpy, identifiers, &num_identifiers, line_count, 1);

		// add the line numbers
		// tmp is alloc'd in add_line_number and points to different mem than in_line
		tmp = add_line_number(in_line, line_count);	

		// write the modified line to the output
		if(fputs(tmp, outfp) == EOF){
			fprintf(stderr, "ERROR: could not write line to the outfile\n");
			exit(1);
		}

		// take care of mem
		free(tmp); // free memory alloc'd in add_line_number
		line_count++;
	}

	// move the filepointer and line count back to the start
	fseek(infp, 0L, SEEK_SET);
	line_count = 1; 

	// loop through a second time to look for the line definitions
	while(1){ // second scan

		// read in the input line		
		if(fgets(in_line, 80, infp) == NULL){
			// if at the end of the input file, break
			break;
		}

		// check that the input line is the right size
		if(strlen(in_line) > 80){
			printf("WARNING: input line is longer than we were expecting\n");
			exit(1);
		}
		
		// scan the line for use lines
		analyze_line(in_line, identifiers, &num_identifiers, line_count, 0);
	
		// move up the linecount
		line_count++;
	}

	// write identifier table to the output file
	print_table(outfp, identifiers, num_identifiers);

	// clean up the memory
	free(in_line);
	free_identifiers(identifiers, num_identifiers);
	free(line_cpy);
} // end analyze_file

// prints the identifier table at the end of the output file
// inputs: 
// 	outfp - file pointer to the output file
// 	identifiers - array of identifier_reference structs, holds the identifier data
// 	num_identifiers - int number of identifiers in the array
//
// outputs:
// 	none
void print_table(FILE* outfp, struct identifier_reference* identifiers, int num_identifiers){
	// prints out a table with the identifiers on it at the end
	char* to_print;
	int i = 0; 
	
	// alloc space for to_print, 50 should be more than enough
	// TODO: find how long this actually needs to be
	if((to_print = (char*) malloc(500*sizeof(char))) == NULL){
		fprintf(stderr, "ERROR: could not alloc memory for to_print\n");
		exit(1);
	}

	// print out the table header
	sprintf(to_print, "\nIdentifier Reference Table\n\n\tIdentifier\tDefinition-Line\tUse Line(s)\n");
	if(fputs(to_print, outfp) == EOF){
		fprintf(stderr, "ERROR: could not write lein to the outfile\n");
		exit(1);
	}

	// print all the identifiers and their data
	while(i < num_identifiers){

		// name, definition
		sprintf(to_print, "\t%-9s\t%-15d\t", identifiers[i].name, identifiers[i].definition);

		// append on use lines
		struct line_value* tmp = identifiers[i].head;	
		while(tmp != NULL){
			sprintf(to_print, "%s%d",to_print, tmp->value);

			// take care of multiple use line 
			if(tmp->next != NULL){
				sprintf(to_print, "%s\t", to_print);
			}
			
			// move the pointer along
			tmp = tmp->next;
		}

		// print this row of the table to the file
		sprintf(to_print + strlen(to_print), "\n");
		if(fputs(to_print, outfp) == EOF){
			fprintf(stderr, "ERROR: could not write line to the outfile\n");
			exit(1);
		}
		i++;
	}
	
	// make sure to free memory
	free(to_print);

} // end print_table

// frees a linked list of use lines
// inputs: 
// 	node - line_value pointer, node to be freed
// 
// outputs:
// 	none
void free_linked_list(struct line_value* node){
	if(node == NULL){ return; }
	free_linked_list(node->next);
	free(node);
} // end free_linked_list

// helper function to free the identifier_reference array 
// inputs:
// 	identifiers - identifier_reference array, the identifier data
// 	num_identifiers - int, number of identifiers found
//
// outputs:
// 	none
void free_identifiers(struct identifier_reference* identifiers, int num_identifiers){
	int i; 			// loop counter

	for(i = 0; i < num_identifiers; i++){
		// free the string for the name
		free(identifiers[i].name);

		// free the linked list
		free_linked_list(identifiers[i].head);
	}
	// free the array it's self
	free(identifiers);
} // end free_identifiers
