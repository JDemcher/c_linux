/*

Joseph Demcher
CS288 - NJIT
February 2018

A C program with C library functions, which prints out the first part of a text file onto screen, similar to the head tool in Linux.
 
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){


	/* 
	   arguments: program_name -n #lines pathname
	   #lines: the number of lines at the beginning of the file to be printed out onto the screen
	   pathname: the pathname of the file
	*/

	FILE *fp;
	
	int lineNum = atoi(argv[2]);
	int numCount = 0;
	
	fp = fopen(argv[3], "r");
	
	if( fp == NULL ){
		printf("There was a problem opening the file.")
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