#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void transpose( int n, int blocksize, int *dst, int *src ) 
{
    int i,j;
    /* TODO: add blocking to this basic loop */
    //for( i = 0; i < n; i++ )
    //    for( j = 0; j < n; j++ )
    printf("before the loop\n");
    for(i = 0; i < ((n*n)/blocksize)+1; i++)
	for(j = 0; j < blocksize; j++)
	    if(i*blocksize+j < n*n)
            	dst[((j+i*blocksize)%n)*n + (i*blocksize + j)/n] = src[j+i*blocksize];
}
void transpose_old( int n, int blocksize, int *dst, int *src ) 
{
    int i,j;
    for(i = 0; i < n; i++){
	for(j = 0; j < n; j++){
	    dst[j + i*n] = dst[i + j*n];
	}
    }
}
int main( int argc, char **argv ) 
{
    int n = 2000,i,j;
    int blocksize = 1000; /* TODO: experiment with block size */

    /* allocate an n*n block of integers for the matrices */
    int *A = (int*)malloc( n*n*sizeof(int) );
    int *B = (int*)malloc( n*n*sizeof(int) );
    if(A == NULL || B == NULL){
	printf("ERORR: failed to alloc all the memory\n");
    }
    printf("alloced\n");
    /* initialize A,B to random integers */
    srand48( time( NULL ) ); /* seed random # generator */
    for( i = 0; i < n*n; i++ ) A[i] = lrand48( );
    for( i = 0; i < n*n; i++ ) B[i] = lrand48( );
    /* measure performance */
    struct timeval start, end;
    gettimeofday( &start, NULL );
    transpose( n, blocksize, B, A );
    gettimeofday( &end, NULL );

    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "%g milliseconds\n", seconds*1e3 );

    /* check correctness */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            if( B[j+i*n] != A[i+j*n] ) {
	        printf("Error: Transpose does not result in correct answer!!\n");
	        exit( -1 );
            }
  
    /* release resources */
    free( A );
    free( B );
    return 0;

}
