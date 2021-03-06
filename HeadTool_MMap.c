
/*

Joseph Demcher
CS288 - NJIT
February 2018

A C program that uses mmap() to print out the first part of a text file onto screen, similar to the head tool in Linux.
    
arguments: program_name   -n    #lines   pathname
#lines: the number of lines at the beginning of the file that should be printed out onto the screen.
pathname: the pathname of the file

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>


int main(int argc, char *argv[]){
    
	int fd;
	int i;
	char *map;
	
	int lineNum = atoi( argv[2] );
	int numCount = 0;
	
	fd = open( argv[3], O_RDONLY );
	
	if( fd < 0 ){
		printf( "Error opening the file.\n" );
		close(fd);
		return -1;
	}

    	struct stat st;
    	if( stat(argv[3], &st ) != 0 ) {
        	return 0;
    	}   
	size_t fileSize = st.st_size;

	map = mmap( 0, fileSize, PROT_READ, MAP_SHARED, fd, 0 );

	if( map == MAP_FAILED ){
		close( fd );
		printf( "Error mapping the file.\n" );
		return -1;
	}
	
	for( i = 0; numCount < lineNum ; i++ ){
		char c;
		c = map[i];
		printf("%c", c );
		if( c == '\n' ){
			numCount++;
		}
		if( i == ( fileSize - 1 ) ){
			numCount = lineNum;
		}
	}

	if( munmap( map, fileSize ) == -1 ){
		printf("Error un-mapping the file.\n");
		close( fd );
		return -1;
	}
	
	close(fd);
	return 0;
}
