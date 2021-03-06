/*

Joseph Demcher
February 2018
CS 288

A C program that calculates the total size of the files in a directory and the sub-directories of the directory. 
Similiar to the du command in Linux.

arguments: ./your_program    pathname_of_directory

*/

#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1	

#include <stdio.h>
#include <ftw.h>

static unsigned int total_size = 0;

int sum( const char *fp, const struct stat *st, int flags, struct FTW *ftwb );

int main(int argc, char **argv) {
	
	if( argc != 2 ){
		printf("Invalid command line arguments.\n");
		return 0;
	}
	
	int flag;
	flag = FTW_PHYS;
	if ( nftw( argv[1], &sum, flag, 1 ) ) {
		return -1;
    	}
    	printf( "%u\n", total_size );
    	return 0;
}

int sum( const char *fp, const struct stat *st, int flags, struct FTW *ftwb ) {
	total_size += st->st_size;
	return 0;
}
