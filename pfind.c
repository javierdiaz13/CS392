/*******************************************************************************
 * Name        : pfind.c
 * Author      : 
 * Date        : 
 * Description : 
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define DIR_NAME_LEN  255
#define PER_LEN 		9

extern char *optarg;
char buf[PATH_MAX];
char directory_name[DIR_NAME_LEN];
char permission_string[PER_LEN];
int opt;
int permission_string_check = 0;
int directory_flag = 0;
int permission_flag = 0;
int args_passed = 0;
DIR *dir;

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};



void display_usage(){
	puts("Usage: ./pfind -d <directory> -p <permission_string> [-h]");
}

char* permission_string_finder(struct stat *statbuf) {
	char *permissions = (char*)malloc(11 * sizeof(char));
	permissions[0] = '-';
	int permission_valid = 0;
	
	for (int i = 0; i < 9; i += 3) { 
	
		
		permission_valid = statbuf->st_mode & perms[i]; 
		
		if (permission_valid) { 
			permissions[i+1] =  'r';
		} 
		else { 
			permissions[i+1] =  '-';
			
		} 
		
		permission_valid = statbuf->st_mode & perms[i+1]; 
		
		if (permission_valid) { 
			permissions[i + 2] =  'w';
		} else { 
			permissions[i + 2] =  '-';
		} 
		
		permission_valid = statbuf->st_mode & perms[i+2]; 
		
		if (permission_valid) { 
			 permissions[i + 3] =  'x';
		} else { 
			permissions[i + 3] =  '-';
		} 
	}
	permissions[10] = '\0';
	return permissions;

}

int main(int argc, char **argv) {

	while((opt = getopt(argc, argv, ":hd:p:")) != -1) 
	{  
        switch(opt)  
        {   
            case 'h':  
            	display_usage();
            	exit(EXIT_SUCCESS);  
                break;  
            case 'd':  
                directory_flag = 1;
                strcpy(directory_name, optarg); 
                break;
            case 'p':
            	permission_flag = 1;
                strcpy(permission_string, optarg);
            	break;   
            case '?':  
                printf("Error: Unknown option '-%c' received.\n", optopt);
                break;
        }  
    }
    
    if( argc > 5){
   		printf("Error: Too many flags or arguments specified.\n");
   		exit(EXIT_FAILURE);
   	}
    
    if (directory_flag == 0) {
    	printf("Error: Required argument -d <directory> not found.\n");
    	exit(EXIT_FAILURE);
    } 
    else if (permission_flag == 0) {
    	printf("Error: Required argument -p <permission_string> not found.\n");
    	exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < PER_LEN; i++){
    	if (i % 3 == 0 && (*(permission_string + i) == 'r' || *(permission_string + i) == '-')){
    		permission_string_check ++;
    	}
    	else if (i % 3 == 1 && (*(permission_string + i) == 'w' || *(permission_string + i) == '-')){
    		permission_string_check++;
    	}
    	else if (i % 3 == 2 && (*(permission_string + i) == 'x' || *(permission_string + i) == '-')){
    		permission_string_check++;
    	}
    }
    
    if (permission_string_check != 9 || strlen(permission_string) != 9) {
    	printf("Error: Permissions string '%s' is invalid.\n", permission_string);
    }
    
    printf("Directory Name: %s\n", directory_name);
    printf("Permissions String: %s\n", permission_string);
    
    
    
    //FIX THIS
    if (realpath(directory_name, buf) == NULL) {
        fprintf(stderr, "Error: Cannot get full path of file '%s'. %s\n",
                directory_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("Buf after Real Path: %s\n", buf);
    
	//FIX THIS
    if ((dir = opendir(buf)) == NULL) {
        fprintf(stderr, "Error: Cannot open '%s'. %s\n",
                buf, strerror(errno));
        exit(EXIT_FAILURE);
    }
 
    printf("Successfully opened file\n");
    
    struct dirent *current_directory;
    struct stat statbuf;
    //char full_filename[PATH_MAX];
    //size_t pathlen = 0;
    
    
    if (lstat(directory_name, &statbuf) < 0) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", directory_name,
                strerror(errno));
        return EXIT_FAILURE;
    }
    
    if ((current_directory = readdir(dir)) != NULL) {
    	printf("Directory Name: %s\n", current_directory->d_name);
    } 
    
    


}
