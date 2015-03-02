#include <stdio.h>
#include <stdlib.h>

void usage();
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

    if((infp = fopen(outfile, "w")) == NULL){
	printf("ERROR: could not open output file for writing\n");
	exit(1);
    }

    // get the size of array to alloc
    fscanf(infp,"%d", &num_values);
    printf("size of the array is: %d\n", num_values);
    if(num_values == 0){
	printf("ERROR: could not read num values from input file\n");
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

void usage(void){
    printf("usage: nibzip.c -acdh input_file [output_file]\n");
    printf(" -a Analyze/Check file (will output a data distribution table)\n");
    printf(" -c Compress input_file and write compressed data to output_file\n");
    printf(" -d Decompress the input_file and write the results to output_file\n");
    printf(" -h Display this message\n");
}
