/*
 
 Joseph Demcher
 March 2018
 CS288
 
 A C program that takes an integer command line argument n, spawns n processes that will each generate a random number between -100 and 100, and then computes and prints out the sum of these random numbers. Each process print sout the random number it generates.
 
 arguments: ./your_program   -n   number_of_processes
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>

void main( int argc, char *argv[] ){

	if( argc != 3 ){
		printf("Invalid command line arguments.\n");
		return 0;
	}

	int *counter = mmap( NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
	
	int num;
	int n = atoi( argv[2] );
	int status = 0;
	
	for( int i = 0 ; i < n ; i++ ){
		pid_t pid = fork();
		if( pid < 0 ){
			printf("Error spawning a child process.\n");
			return 0;
		}
		if( pid == 0 ){
			srand( time(NULL ) ^ (getpid() << 16 ));
			num = rand() / ( RAND_MAX / ( 200 + 1 ) + 1 );
			num -= 100;
			printf( "%d\n", num );
			*counter += num;
			exit(0);
		}
		wait( &status );
	}
	printf( "%d\n", *counter );
}


