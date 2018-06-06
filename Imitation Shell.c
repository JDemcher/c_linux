/*

 Joseph Demcher
 March 2018
 CS 288
 
 A C program which mimics the functions of a Bash shell both by user input and batch files in the form of text files.
 Special commands: "quit" will end the program
                   "barrier" will cease all further input till all in-progress jobs are completed
                   adding "&" to any command (ex. "ls -la&") will allow the user to input another command immediately as the entered command executes in the background
 
 To launch the program for user input:
 arguments: ./your_program
 
 To launch the program with a batch file:
 arguments: ./your_program    batch_file.txt
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

void remove_all_chars(char* str, char c) {
    char *pntr = str;
    char *pow = str;
    while (*pntr) {
        *pow = *pntr++;
        pow += ( *pow != c );
    }
    *pow = '\0';
}

int main(int argc, char* argv[]){
    
    int BUFFER_SIZE = 10000;
    char i[BUFFER_SIZE];
    int status;
    pid_t pid1, pid2, wpid;
    
    int *cp_count = mmap( NULL, sizeof(pid_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
    
    *cp_count = 0;
    
    if ( argc == 1 ) {
        //interactive mode
        while (1) {
            
            printf("prompt>: ");
            fgets(i, BUFSIZ, stdin);
            
            char *string = i;
            char *arguments[10000] = {};
            char *found;
            int bFlag = 0;
            int index = 0;
            int stdout;
            
            //test for quit command
            if( strcmp(i, "quit\n") == 0 ){
                while( *cp_count != 0 ){
                    wait(&status);
                    *cp_count -= 1;
                }
                return 0;
            }
            
            //test for barrier command
            if( strcmp(i, "barrier\n") == 0 || strcmp(i, "barrier&\n") == 0 ){
                bFlag = 1;
                while( *cp_count != 0 ){
                    wait(&status);
                    *cp_count -= 1;
                }
            }
            
            //remove excess characters
            string = strsep(&string, "\n");
            remove_all_chars(string, '\t');
            
            //check for & symbol
            char lastLetter;
            int lastLetterIndex = strlen(string)-1;
            while(1){
                lastLetter = string[lastLetterIndex];
                if(lastLetter == ' '){
                    lastLetterIndex -= 1;
                }
                else{
                    break;
                }
            }

            remove_all_chars(string, '&');
            
      //test for redirection and form arguments for cmds
 	    int redirectFlag = 0;
            while( (found = strsep(&string," ")) != NULL ){
                if( strcmp(found, ">") == 0 ){
                    redirectFlag = 1;
                    break;
                }
                if(strcmp(found, "") != 0){
                    arguments[index] = found;
                    index++;
                }
            }
            arguments[index] = NULL;
            
            if ( lastLetter == '&' && !(bFlag) ) {
                // do not wait for child process
                *cp_count += 1;
                pid1 = fork();
                if(pid1 == 0){
                    //child process
                    //get redirection path if needed
                    int fd;
                    if(redirectFlag){
                        int k = 0;
                        char redirect_path[100] = {};
                        int p = 0;
                        while( string[k] != '\0' ){
                            redirect_path[p] = string[k];
                            k += 1;
                            p += 1;
                        }
                        fd = open( redirect_path, O_CREAT | O_TRUNC | O_WRONLY, 0600 );
                        stdout = dup(1);
                        dup2(fd, 1);
                    }
                    if ( execvp(arguments[0], arguments) == -1 ){
                        printf("%s: No such file or directory exists.\n", i);
                    }
                    if(redirectFlag){
                        dup2(stdout, 1);
                        close(stdout);
                    }
                    exit(0);
                }
                //parent process does not need to wait
            }
            if( !(bFlag)  && lastLetter != '&' ){
                //wait for child process
                if( bFlag ){
                    while ( (wpid = wait(&status)) > 0 );
                }
                else{
                    pid2 = fork();
                    if(pid2 == 0){
                        //child process
                         //get redirection path if needed
                        int fd;
                        if(redirectFlag){
                            int k = 0;
                            char redirect_path[100] = {};
                            int p = 0;
                            while( string[k] != '\0' ){
                                redirect_path[p] = string[k];
                                k += 1;
                                p += 1;
                            }
                            fd = open( redirect_path, O_CREAT | O_TRUNC | O_WRONLY, 0600 );
                            stdout = dup(1);
                            dup2(fd, 1);
                        }
                        if ( execvp(arguments[0], arguments) == -1 ){
                            printf("%s: No such file or directory exists.\n", i);
                        }
                        if(redirectFlag){
                            dup2(stdout, 1);
                            close(stdout);
                        }
                        exit(1);
                    }
                    waitpid(pid2, &status, 0);
                }
            }
        }
    }
    else{
        if( argc >= 3 ){
            puts("Too many arguments.");
            return 0;
        }
        else if( access( argv[1], F_OK ) == -1 ) {
            // file doesn't exist
            puts("Batch file does not exist.");
            return 0;
        }
        else {
            FILE *fp = fopen( argv[1], "r" );
            if( fp == NULL ){
                //batch file cannot be opened
                puts("Batch file cannot be opened.");
                return 0;
            }
            //recieve input from batch file
            *cp_count = 0;
            int Joseph = 0;
            
            while (1) {

                if( fgets(i, BUFSIZ, fp) == NULL ) {
                    while( *cp_count != 0 ){
                        wait(&status);
                        *cp_count -= 1;
                    }
                    return 0;
                }
                else{
                    
                    char *string = i;
                    char *arguments[10000] = {};
                    char *found;
                    int index = 0;
                    int bFlag = 0;
                    int stdout;
                    
                    //testing and handling of quit cmd
                    if( strcmp(i, "quit\n") == 0 || strcmp(i, "quit") == 0 ){
                        puts("quit");
                        while( *cp_count != 0 ){
                            wait(&status);
                            *cp_count -= 1;
                        }
                        return 0;
                    }
                    
                    //testing and handling of barrier cmd
                    if( strcmp(i, "barrier\n") == 0 || strcmp(i, "barrier&\n") == 0 ){
                        bFlag = 1;
                        puts("barrier");
                        while( *cp_count != 0 ){
                            wait(&status);
                            *cp_count -= 1;
                        }
                    }
                    
                    if( !(bFlag) ){
                        printf("%s", i);
                    }
                    
                    //remove excess characters
                    remove_all_chars(string, '\t');
                    remove_all_chars(string, '\n');

                    //check for & symbol
                    char lastLetter;
                    int lastLetterIndex;
                    if( i[strlen(i)-1] == '\n'){
                       lastLetterIndex = strlen(string)-2;
                    }
                    else{
                        lastLetterIndex = strlen(string)-1;
                    }
                    while( 1 && !(bFlag)){
                        lastLetter = string[lastLetterIndex];
                        if(lastLetter == ' '){
                            lastLetterIndex -= 1;
                        }
                        else{
                            break;
                        }
                    }
                
                    remove_all_chars(string, '&');
	        
                    //test for redirection and form arguments for cmds
                    int redirectFlag = 0;
                    while( (found = strsep(&string," ")) != NULL ){
                        if( strcmp(found, ">") == 0 ){
                            redirectFlag = 1;
                            break;
                        }
                        if(strcmp(found, "") != 0){
                            arguments[index] = found;
                            index++;
                        }
                    }
                    arguments[index] = NULL;
                    
                    if ( lastLetter == '&' && !(bFlag) ) {
                        // do not wait for child process
                        *cp_count += 1;
                        pid1 = fork();
                        if(pid1 == 0){
                            //child process
                            //get address for redirected file if needed
                            int fd;
                            if(redirectFlag){
                                int k = 0;
                                char redirect_path[100] = {};
                                int p = 0;
                                while( string[k] != '\0' ){
                                    redirect_path[p] = string[k];
                                    k += 1;
                                    p += 1;
                                }
                                fd = open( redirect_path, O_CREAT | O_TRUNC | O_WRONLY, 0600 );
                                dup2(fd, 1);
                            }
                            if ( execvp(arguments[0], arguments) == -1 ){
                                printf("%s: No such file or directory exists.\n", i);
                            }
                            if(redirectFlag){
                                dup2(stdout, 1);
                                close(stdout);
                            }
                            exit(0);
                        }
                        //parent process does not need to wait
                    }
                    
                    if( !(bFlag)  && lastLetter != '&' ){
                        //wait for child process
                        if( bFlag ){
                            while ( (wpid = wait(&status)) > 0 );
                        }
                        else{
                            pid2 = fork();
                            if(pid2 == 0){
                                //child process
                                //get address for redirected file if needed
                                int fd;
                                if(redirectFlag){
                                    int k = 0;
                                    char redirect_path[100] = {};
                                    int p = 0;
                                    while( string[k] != '\0' ){
                                        redirect_path[p] = string[k];
                                        k += 1;
                                        p += 1;
                                    }
                                    fd = open( redirect_path, O_CREAT | O_TRUNC | O_WRONLY, 0600 );
                                    dup2(fd, 1);
                                }
                                if ( execvp(arguments[0], arguments) == -1 ){
                                  printf("%s: No such file or directory exists.\n", i);
                                }
                                if(redirectFlag){
                                    dup2(stdout, 1);
                                    close(stdout);
                                }
                                exit(1);
                            }
                            waitpid(pid2, &status, 0);
                        }
                    }
                }
            }
        }
    }
}
