/*

Joseph Demcher
CS288 - NJIT
February 2018
 
A C program that uses C library functions to print out the first part of a text file onto the screen, similar to the head tool in Linux.
arguments: program_name -n #lines pathname
#lines: the number of lines at the beginning of the file to be printed out onto the screen
pathname: the pathname of the file

*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

	if( argc != 4 ){
		printf("Invalid command line arguments.\n");
		return 0;
	}

	FILE *fp;
	
	int lineNum = atoi(argv[2]);
	int numCount = 0;
	
	fp = fopen(argv[3], "r");
	
	if( fp == NULL ){
		printf("Error opening the file.\n");
		return -1;
	}

	int c;

	while( numCount < lineNum ){
		c = fgetc( fp );
		printf("%c", c);
		if( c == '\n'){			
			numCount++;
		}
	}

	fclose(fp);	

	return 0;

}
