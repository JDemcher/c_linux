/*

Joseph Demcher
March 2018
CS288

A pthread program that takes an integer command line argument n, spawns n threads that will each generate a random number between -100 and 100, and then computes and prints out the sum of these random numbers. Each thread prints out the random number it generates.

arguments: ./your_program   -n   number_of_threads

*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int counter;
int total = 0;
int numThreads;

void *random_num(void *threadid){
	int num;
	num = rand() / ( RAND_MAX / ( 200 + 1 ) + 1 );
	num -= 100;
	total += num;
	printf( "%d\n", num );
	counter += 1;
	if( counter == numThreads ){
		printf( "%d\n", total );
	}
	pthread_exit( NULL );
}


void main(int argc, char *argv[]){
	if( argc != 3 ){
		printf("Invalid command line arguments.\n");
		return 0;
	}
	numThreads = atoi( argv[2] );
	pthread_t threads[ numThreads ];

	counter = 0;
	srand( time(0) );

	int rc;
	long t;
	for(int i = 0 ; i < numThreads ; i++ )
		rc = pthread_create( &threads[t], NULL, random_num, (void *) t );

	pthread_exit(NULL);
}
