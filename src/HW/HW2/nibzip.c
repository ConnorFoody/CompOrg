#include <stdio.h>
#include <stdlib.h>

/*	NAME: Connor Foody
 * 	LAB: 1
 *	INPUT: flag to describe operation code will perform,
 *		input file, output file
 *	Output: processed input file depending on flag, either
 * 		compressed or decompressed
 *
 */

// prototype functions to be called by main
void usage();  
int analyze(char* raw, int size);  
int compress(char* raw, int size, FILE* outfp);
int decompress(char* raw, int size, FILE* outfp);

int main(int argc, char** argv){
    // check that the args are OK

    char* infile; // store the name of the input file
    char* outfile; // store the name of the ouput file

    FILE* infp; // file pointer for the input file
    FILE* outfp; // file pointer for the output file

    char *values; // store values read from file
    int num_values; // the number of values in the file
    int i; // loop counter
    
    // check command line args
    if(argc != 4){
	// if wrong number of args
	usage();
	return 0;
    }
    if(strcmp(argv[1],"-h") == 0){
	// if help flag passed in
	usage();
	return 0;
    }

    // read in file
    infile = argv[2];
    outfile = argv[3];

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

    // get the size of array to alloc
    if(fseek(infp, 0, SEEK_END) != 0){
	fprintf(stderr, "ERROR: fseek had trouble\n");
    }
    num_values = ftell(infp); // number of characters in input file
    fseek(infp, 0L, SEEK_SET); // seek back to the begining of the input file
    
    // alloc array to read input file into
    if((values = (char *) malloc(num_values * sizeof(char))) == NULL){
	fprintf(stderr, "error allocating memory\n");
	exit(1);
    }

    // read values from file into array
    for(i = 0; i < num_values; i++){
	fscanf(infp, "%c", &values[i]);
    }

    // process input file based on flag
    if(strcmp(argv[1],"-a") == 0){
	// if -a (analyze) flag
	if(analyze(values, num_values)){
	    fprintf(stderr, "ERROR: invalid character\n");
	    exit(1);
	}
    }
    else if(strcmp(argv[1],"-c") == 0){
	// if -c (compress) flag
	   compress(values, num_values, outfp);
    }
    else if(strcmp(argv[1],"-d") == 0){
	// if -d (decompress) flag
	decompress(values, num_values, outfp);
    }
    else{
	// if unknown flag passed
	usage();
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

// helper function to get the char value of the hex input
// @param hex input char containing hex corresponding to the char
char get_char_from_hex(unsigned char hex){
    // simple helper function to convert compressed hex value to char
    if(hex == 0x0) return '0';
    if(hex == 0x01) return '1';
    if(hex == 0x02) return '2';
    if(hex == 0x03) return '3';
    if(hex == 0x04) return '4';
    if(hex == 0x05) return '5';
    if(hex == 0x06) return '6';
    if(hex == 0x07) return '7';
    if(hex == 0x08) return '8';
    if(hex == 0x09) return '9';
    if(hex == 0x0a) return ',';
    if(hex == 0x0b) return '-';
    if(hex == 0x0c) return '.';
    if(hex == 0x0d) return ' ';
    if(hex == 0x0e) return '\n';
    fprintf(stderr, "ERROR: fallthrough on get_char_from_hex\n");
    return '0';
}

// decompress the input file and write it to the output file
// @param raw array containing contents of input file
// @param size size of raw
// @param outfp pointer to output file
int decompress(char* raw, int size, FILE* outfp){
    int i; // counter 
    int buff_mod; // helper for writing to buffer
    unsigned char* buffer; // buffer for writing 4 chars at a time
    unsigned char to_write; // holder for read 

    
    int j; // counter for analyze_buffer
    unsigned char* analyze_buffer; // holds characters to send over to analyze

    // allocate buffer for reading characters from file
    if((buffer = (char*) malloc(4 * sizeof(char))) == NULL){
	fprintf(stderr, "ERROR: could not allocate memory\n");
	exit(1);
    }

    // allocate array to send over to analyze function
    if((analyze_buffer = (char *) malloc(size * 2 * sizeof(char))) == NULL){
	fprintf(stderr, "ERROR: could not allocate memory\n");
	exit(1);
    }
    j = 0;
    
    // loop through the input array 
    for(i = 0; i < size; i++){
	if(i%2 == 0){
	    // if two characters have been read into the
	    // buffer, write the buffer to the output file
	    if(fprintf(outfp, "%s", buffer) < 0){
		fprintf(stderr, "ERROR: could not write buffer to file\n");
	    	exit(1);
	    }
	}	
	// put next char into holder
	to_write = raw[i];

	// keep track of which side of the buffer we are on (%2 because each char holds 2 chars)
	if(i%2 == 1){
	    buff_mod = 2;
	}else{
	    buff_mod = 0;
	}

	// write char to buffer (shift to isolate right side of char first)
	buffer[buff_mod] = get_char_from_hex(to_write >> 4);
	// to_write shifts isolate value on the right of char (by moving left), move it back right then subtract it
	buffer[buff_mod + 1] = get_char_from_hex(to_write - ((to_write >> 4) << 4));

	// write the files into the buffer to send over to the analyze function
	analyze_buffer[j] = buffer[buff_mod];
	analyze_buffer[j+1] = buffer[buff_mod + 1];
	j += 2;
    }
    if(size % 2 != 0){
	// only print part of the buffer if buffer didn't get filled up all the way
	if(fprintf(outfp, "%c%c", buffer[0], buffer[1]) < 0){
	    fprintf(stderr, "ERROR: could not write buffer to file\n");
	    exit(1);
	}
    }
    else{
	// if we filled the buffer, print the whole thing
	if(fprintf(outfp, "%s", buffer) < 0){
	    fprintf(stderr, "ERROR: could not write buffer to file\n");
	    exit(1);
	}
    }

    // check that all the characters were good
    if(analyze(analyze_buffer, j) != 0){
	fprintf(stderr, "ERROR: failed to decompress, invalid character\n");
	exit(1);
    }
    printf("   was uncompresed successfully. File size is %d bytes.\n", i * 2);
    return 0;
}

// function to compress input file
// @param raw array containing contents of input file
// @param size size of raw
// @param outfp output file pointer
int compress(char* raw, int size, FILE* outfp){
    int i; // loop counter 
    unsigned char to_write; // character to be written to output file 
    unsigned char holder; // buffer to hold character to write
    if(analyze(raw,size) != 0){
	fprintf(stderr, "ERROR: invalid characters\n");
	return 0;
    }
    for(i = 0; i < size; i++){
	// fill int
	if(i % 2 == 0){
	    if(i != 0 && i % 4 == 0){
		// int is full, write it to the file
		if(fprintf(outfp, "%c%c", holder, to_write) < 0){
		    fprintf(stderr, "ERROR: writing int to outfile\n");
		    exit(1);
		}
		holder = 0;
	    }
	    else {
		// store to_write in the holder
		holder = to_write;
	    }
	    // reset to_write to 0
	    to_write = 0;
	}
	// shift it over to build properly
	to_write = to_write << 4;

	// convert input character to hex and add it to the output file
	if(raw[i] == '0')to_write = to_write | 0x00;		
	else if(raw[i] == '1') to_write = to_write | 0x01;
        else if(raw[i] == '2') to_write = to_write | 0x02;
	else if(raw[i] == '3') to_write = to_write | 0x03;
	else if(raw[i] == '4') to_write = to_write | 0x04;
	else if(raw[i] == '5') to_write = to_write |0x05;
	else if(raw[i] == '6') to_write = to_write | 0x06;
	else if(raw[i] == '7') to_write = to_write | 0x07;
	else if(raw[i] == '8') to_write = to_write | 0x08;
	else if(raw[i] == '9') to_write = to_write | 0x09;
	else if(raw[i] == ',') to_write = to_write | 0x0A;
	else if(raw[i] == '-') to_write = to_write | 0x0B;
	else if(raw[i] == '.') to_write = to_write | 0x0C;
	else if(raw[i] == ' ') to_write = to_write |0x0D;
	else if(raw[i] == '\n') to_write = to_write |0x0E;
	else{fprintf(stderr, "ERROR: unknown character: %c\n", raw[i]);} // shouldn't hit this b/c analyze will take care of it

    }
    // write the unwritten part of the buffer to the output file
    if(size%4 == 0){
	// all of buffer got written
	return 0;
    }
    if(size%4 <= 2){
	// only first part of buffer got written
	fprintf(outfp, "%c", to_write);
    }
    else{
	// write the whole buffer
	fprintf(outfp, "%c%c", to_write, holder);
    }
    printf("   was compressed successfully. New size is %d bytes.\n", size / 2);
    return 0;
}

int analyze(char* raw, int size){
    // struct for holding counts of each item
    struct table{
	char val;
	int count;
    };

    // loop counters
    int i;
    int j;
    
    struct table* counts; // array to hold counts for all the items
    if((counts = (struct table *) malloc(16 * sizeof(struct table))) == NULL){
	fprintf(stderr, "error allocating memory\n");
	exit(1);
    }
   
    // set all the count variables 
    counts[0].val = '0';
    counts[1].val = '1';
    counts[2].val = '2';
    counts[3].val = '3';
    counts[4].val = '4';
    counts[5].val = '5';
    counts[6].val = '6';
    counts[7].val = '7';
    counts[8].val = '8';
    counts[9].val = '9';
    counts[10].val = ',';
    counts[11].val = '-';
    counts[12].val = '.';
    counts[13].val = ' ';
    counts[14].val = '\n'; 

    // set all counts to zero
    for(i = 0; i < 16; i++){
	counts[i].count = 0;
    }
    
    for(i = 0; i < size; i++){
	// find the item in the table to add this char to 
	for(j = 0; j < 15; j++){
	    if(raw[i] == counts[j].val){
		counts[j].count += 1;
		break;
	    }
	}
	if(j == 15){
	    // none of the known values matched
	    counts[15].count += 1;
	}
    }
    
    // print out all the characters and their counts
    for(i = 0; i < 14; i++){
	printf("%c%16d\n", counts[i].val, counts[i].count);
    }

    // print special cases by themselves
    printf("%s%15d\n", "\\n", counts[14].count);
    printf("%s%12d\n", "other", counts[15].count);

    return counts[15].count;
}

// function to print the usage for this file
void usage(void){
    printf("usage: nibzip.c -acdh input_file [output_file]\n");
    printf(" -a Analyze/Check file (will output a data distribution table)\n");
    printf(" -c Compress input_file and write compressed data to output_file\n");
    printf(" -d Decompress the input_file and write the results to output_file\n");
    printf(" -h Display this message\n");
}
