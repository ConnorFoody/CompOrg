#include <stdio.h>
#include <stdlib.h>

void usage();
int analyze(char* raw, int size);
int compress(char* raw, int size, FILE* outfp);
void showBits(unsigned int bits);

int main(int argc, char** argv){
    // check that the args are OK
    

    char* infile;
    char* outfile;

    FILE* infp;
    FILE* outfp;

    char *values;
    int num_values;
    int i;
    
    // check command line args
    if(argc != 4){
	usage();
	return 0;
    }
    if(strcmp(argv[1],"-h") == 0){
	usage();
	return 0;
    }

    // read in file
    infile = argv[2];
    outfile = argv[3];
    printf("ifile is: %s\tofile is: %s\n", infile, outfile);

    if((infp = fopen(infile, "r")) == NULL){
	printf("ERROR: could not open input file for reading\n");
	exit(1);
    }

    if((outfp = fopen(outfile, "w")) == NULL){
	printf("ERROR: could not open output file for writing\n");
	exit(1);
    }

    // get the size of array to alloc
    if(fseek(infp, 0, SEEK_END) != 0){
	printf("ERROR: fseek had trouble\n");
    }
    num_values = ftell(infp);
    fseek(infp, 0L, SEEK_SET);
    
    if(num_values == 0){
	printf("ERROR: could not read num values from input file: %d\n", num_values);
	exit(1);
    }
    
    // alloc array
    if((values = (char *) malloc(num_values * sizeof(char))) == NULL){
	fprintf(stderr, "error allocating memory\n");
	exit(1);
    }

    // read values from file into array
    for(i = 0; i < num_values; i++){
	fscanf(infp, "%c", &values[i]);
    }

    // print array 
    printf("%s\n", values);

    if(strcmp(argv[1],"-a") == 0){
	if(analyze(values, num_values)){
	    printf("ERROR: invalid character\n");
	    exit(1);
	}
    }
    else if(strcmp(argv[1],"-c") == 0){
	compress(values, num_values, outfp);

    }
    else if(strcmp(argv[1],"-d") == 0){

    }
    else{
	usage();
	exit(1);
    }

    if(fclose(infp) == EOF){
	printf("ERROR: closing input file\n");
	exit(1);
    }

    if(fclose(outfp) == EOF){
	printf("ERROR: closing output file\n");
	exit(1);
    }

    return 0;
}

int compress(char* raw, int size, FILE* outfp){
    int i; 
    unsigned char to_write; 
    unsigned char holder;
    if(analyze(raw,size)){
	printf("ERROR: invalid characters\n");
    }
    for(i = 0; i < size; i++){
	// fill int
	if(i % 2 == 0){
	    if(i != 0 && i % 4 == 0){
		// int is full, write it to the file
		if(fprintf(outfp, "%c%c", holder, to_write) < 0){
		    printf("ERROR: writing int to outfile\n");
		    exit(1);
		}
		printf("wrote: %x%x\n", to_write, holder);
		holder = 0;
	    }
	    else {
		holder = to_write;
	    }
	    to_write = 0;
	}
	// shift it over to build properly
	to_write = to_write << 4;

	if(raw[i] == '1')to_write = to_write | 0x01;		
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
	
	//to_write = to_write << 1;
	//to_write = 0x01 << 4;
	//to_write = to_write | 0x02;
	printf("char: %c\tto_write: %x\n", raw[i], to_write);	
	// shift everything over
    }
    printf("to_write: %x\tholder: %x\n", to_write, holder);
    if(size%4 == 0){
	return 0;
    }
    if(size%4 <= 2){
	printf("adding just to_write: %x\n", to_write);
	fprintf(outfp, "%c", to_write);
    }
    else{
	fprintf(outfp, "%c%c", to_write, holder);
    }
    return 0;
}

int analyze(char* raw, int size){
    int i;
    int* counts;

    if((counts = (int *) malloc(15 * sizeof(int))) == NULL){
	fprintf(stderr, "error allocating memory\n");
	exit(1);
    }


    for(i = 0; i < size; i++){
    	if(raw[i] == '0') counts[0]++;
	else if(raw[i] == '1') counts[1]++;
        else if(raw[i] == '2') counts[2]++;
        else if(raw[i] == '3') counts[3]++;
        else if(raw[i] == '4') counts[4]++;
        else if(raw[i] == '5') counts[5]++;
        else if(raw[i] == '6') counts[6]++;
        else if(raw[i] == '7') counts[7]++;
        else if(raw[i] == '8') counts[8]++;
        else if(raw[i] == '9') counts[9]++;
        else if(raw[i] == ',') counts[10]++;
        else if(raw[i] == '-') counts[11]++;
        else if(raw[i] == '.') counts[12]++;
        else if(raw[i] == ' ') counts[13]++;
        else if(raw[i] == '\n'){
	    counts[14]++; 
	}
	else counts[15]++;
    }
    printf("Character   Count\n");
    for(i = 0; i < 10; i++){
	printf("%d%16d\n", i, counts[i]);
    }
    printf("%c%16d\n", ',', counts[10]);
    printf("%c%16d\n", '-', counts[11]);
    printf("%c%16d\n", '.', counts[12]);
    printf("%c%16d\n", ' ', counts[13]);
    printf("%s%15d\n", "\\n", counts[14]);
    printf("%s%12d\n", "other", counts[15]);

    return counts[15];
}

void usage(void){
    printf("usage: nibzip.c -acdh input_file [output_file]\n");
    printf(" -a Analyze/Check file (will output a data distribution table)\n");
    printf(" -c Compress input_file and write compressed data to output_file\n");
    printf(" -d Decompress the input_file and write the results to output_file\n");
    printf(" -h Display this message\n");
}


// delete this...
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
