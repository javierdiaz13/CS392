/*******************************************************************************
 * Name        : cpumodel.c
 * Author      : Valentina Bustamante and Javier Diaz
 * Date        : 06/14/2020
 * Description : Prints out cpu model
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//pass in argv[0] into pfind

bool starts_with(const char *str, const char *prefix) {
     /*TODO:
       Return true if the string starts with prefix, false otherwise.
       Note that prefix might be longer than the string itself.
     */
    if(strlen(prefix) > strlen(str)){
    	return false;
    }

    char temp[10]; 
    
    for(int i = 0; i < strlen(prefix); i++){
    	temp[i] = str[i];
    }
    
    if(strcmp(temp,prefix) == 0){
    	return true;
    }
    
	return false;
}

int main() {
    /* TODO:
       Open "cat /proc/cpuinfo" for reading.
       If it fails, print the string "Error: popen() failed. %s.\n", where
       %s is strerror(errno) and return EXIT_FAILURE.
     */
	FILE *pipe;
	//unsigned long bytes_read;
	int status;
	if((pipe = popen("cat /proc/cpuinfo", "r")) == NULL){
		fprintf(stderr, "Error: popen() failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}

    /* TODO:
       Allocate an array of 256 characters on the stack.
       Use fgets to read line by line.
       If the line begins with "model name", print everything that comes after
       ": ".
       For example, with the line:
       model name      : AMD Ryzen 9 3900X 12-Core Processor
       print
       AMD Ryzen 9 3900X 12-Core Processor
       including the new line character.
       After you've printed it once, break the loop.
     */
	char lines[256];
	//strchar == find the index of 
	while(fgets(lines, 256, pipe)){
	
		 char *eoln = strchr(lines, '\n');
		 
		 if (eoln == NULL) {
        
            //This should not happen.
            //After 128 characters, make last character \0
            lines[256] = '\0';
        } else {
       
            *eoln = '\0';
            
            bool prefix = starts_with(lines, "model name");
            
            if(prefix){
            	char *ret;
            	ret = strchr(lines, ':') + 2;
            	
            	printf("%s\n", ret);
            	break;
            }
        }
	}

    /* TODO:
       Close the file descriptor and check the status.
       If closing the descriptor fails, print the string
       "Error: pclose() failed. %s.\n", where %s is strerror(errno) and return
       EXIT_FAILURE.
     */
	if ((status = pclose(pipe)) < 0) {
		fprintf(stderr, "Error: pclose() failed. %s.\n", strerror(errno));
		return 	EXIT_FAILURE;   	 
	} else {
		return status;
	}
	
    return !(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);
}
