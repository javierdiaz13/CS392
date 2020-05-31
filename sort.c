/*******************************************************************************
 * Name        : sort.c
 * Author      : Javier Diaz
 * Date        : June 1, 2020
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      :
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;

/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind. 
 */
int main(int argc, char **argv) {

	int opt;
	int flag_counter = 0;
	int is_error = 0;
	int file_count = 0;
	char *filename;
	elem_t flag_used;
      
      
    while((opt = getopt(argc, argv, ":i:d:")) != -1)  {  
    	switch(opt)  
        {  
            case 'i':  
                flag_counter++;  
                flag_used = INT;
                break;  
            case 'd':  
                flag_counter++; 
                flag_used = DOUBLE; 
                break;    
            case '?':  
                printf("Unknown option: %c received.\n", optopt); 
                is_error++;
                break;
            case ':':
            	printf("Error: No input file specified"); 
                is_error++;
                break;
        }
        if (flag_counter > 1){
        	printf("Error: Too many flags specified");
        	is_error++;
        }
    }
    
    if (!(flag_used == INT || flag_used == DOUBLE)){
    	flag_used = STRING;
    }
    
    while (optind < argc) {  
    	filename = argv[optind];    
        file_count++;
        optind++;
        if (file_count > 1){
    	printf("Error: Too many files specified");
    	is_error++;
    	}  
    } 
    if (argc <= 1 || is_error != 0) {
    	puts(
    		"Usage: ./sort [-i|-d] filename"
    		"	-i: Specifies the file contains ints."
    		"	-d: Specifies the file contains doubles."
    		"	filename: The file to sort."
    		"	No flags defaults to sorting strings."
    	);
    	return EXIT_FAILURE;
    } 

	char **data = (char **)malloc(MAX_ELEMENTS * sizeof(char));
	/*int *data = (int *)malloc(MAX_ELEMENTS * sizeof(int));*/
	/*double *data = (double *)malloc(MAX_ELEMENTS * sizeof(double));*/
	
	size_t len = read_data(filename, data);
	
	if (flag_used == STRING) {
		quicksort(data, len, 1, (*str_cmp) (const void*, const void*));
	}
	else if (flag_used == INT) {
		quicksort(data, len, 4, (*int_cmp) (const void*, const void*));
	}
	else {
		quicksort(data, len, 8, (*dbl_cmp) (const void*, const void*));
	}
	
	
	free(data);
    return EXIT_SUCCESS;
}


/**
 * Reads data from filename into an already allocated 2D array of chars.
 * Exits the entire program if the file cannot be opened.
 */
size_t read_data(char *filename, char **data) {
    // Open the file.
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open '%s'. %s.\n", filename,
                strerror(errno));
        free(data);
        exit(EXIT_FAILURE);
    }

    // Read in the data.
    size_t index = 0;
    char str[MAX_STRLEN + 2];
    char *eoln;
    while (fgets(str, MAX_STRLEN + 2, fp) != NULL) {
        eoln = strchr(str, '\n');
        if (eoln == NULL) {
            str[MAX_STRLEN] = '\0';
        } else {
            *eoln = '\0';
        }
        // Ignore blank lines.
        if (strlen(str) != 0) {
            data[index] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
            strcpy(data[index++], str);
        }
    }

    // Close the file before returning from the function.
    fclose(fp);

    return index;
}

