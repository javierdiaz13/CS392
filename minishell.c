/*******************************************************************************
 * Name        : minishell.c
 * Author      : Valentina Bustamante and Javier Diaz
 * Date        : 06/22/2020
 * Description : Sleeps for the number of seconds entered by user
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <pwd.h>

#define MAX_STRLEN   300
#define MAX_ELEMENTS 1024
#define BUFSIZE 	 4096
#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"


volatile sig_atomic_t signal_val = 0;
bool cd_too_many_args = false, malformed_command = false;
char *name_with_spaces;
//cjeck that read and wait are not e interrupted
/* Signal handler -- keep it simple */
void catch_signal(int sig) {
    signal_val = sig;
}

int remove_char(char* str, const char chr){
	
	int len = strlen(str);
	int j=0;
	int char_occurences = 0;
	for(int i = 0; i < len; i++){
		if(str[i] == chr){
			char_occurences++;
			for(j=i;j<len; j++){
				str[j] = str[j+1];
			}
			len--;
			i--;
		}
	}
	if(char_occurences  % 2 != 0){
		return 1;
	}
	return 0;	
}

bool starts_with(const char *str, const char *prefix) {
	
    if(strlen(prefix) > strlen(str)){
    
    	return false;
    }
	
	int len = strlen(prefix);
    char temp[len + 1];
    temp[len]='\0'; 
   
    for(int i = 0; i < strlen(prefix); i++){
    	temp[i] = str[i];
    }
 
    if(strcmp(temp,prefix) == 0){

    	return true;
    }
    
	return false;
}

int commandHandlerCD(char **buf, size_t len){
	printf("in handler");
	
	struct passwd *pwd = getpwuid(getuid());
	
	if (pwd == NULL) {
		fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
		return 0;
	}
	
	char *new_dir;
	new_dir = pwd->pw_dir;
	
	
	bool homedir = false;
	char *print;
	
	if(len == 1){
		
		homedir = true;
		print = buf[0];
	}else{
		if(len > 2){
			
			/*for(int i = 1; i < len; i++){
				print = strcat(print, buf[i]);
			}*/
			
			//if(malformed_command == true){
				//fprintf(stderr, "Error: Malformed command.\n");
			//	return 0;
			//}else if(cd_too_many_args == true){
				fprintf(stderr, "Error: Too many arguments to cd.\n");
				return 0;
			//}else{
				
				//printf("Name: %s\n", name_with_spaces);
				//print = name_with_spaces;
			}
				
		//}else{
			print = buf[1];
			int error = 0;
			if((error = remove_char(print, '"')) == 1){
				fprintf(stderr, "Error: Malformed command.\n");
				return 0;
			}
	//	}
		
		if(starts_with(print, new_dir)){
			homedir = true;
			new_dir = print;
		}
		else if(strcmp(print, "~") == 0){
			homedir = true;
		}else if(print[0] == '~' ){
			new_dir = strcat(new_dir, print + 1);
			homedir = true;
		}
			
		if(homedir == false){
			char current_dir[PATH_MAX];
			
			
			if ((getcwd(current_dir, sizeof(current_dir))) == NULL) {
				fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
				return EXIT_FAILURE;
			}
			
			if(strcmp(print, "..") == 0 && strcmp(current_dir, "/") != 0){
					
					char* ptr = strrchr(current_dir, '/');
					
					int len = ptr - current_dir;
					current_dir[len+1] = '\0';
					
					new_dir = current_dir;
					
			}else{
			
				char *dir = strcat(current_dir, (char*)"/");
				new_dir = strcat(dir, print);
	
			}
		}
	}
	

	if(chdir(new_dir) == -1){
		fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", print, strerror(errno));
		return EXIT_FAILURE;
	}
	
	return 0;
}

size_t argument_parser(char **data, char *str){

	size_t j = 0;

	/*int error =0;
	char str_cpy[BUFSIZE];
	strcpy(str_cpy, str);
	//char first_half;
	//char second_half;
	//strcpy(cpy_str, *(str));
	
	//split string before quoted file and after quoted file and then concat
	char *dir;
	if((dir = strchr(str_cpy, '"')) != NULL){
		int num_quotes = 1;
		
		char *dir_end = strrchr(dir, '"');
		int len = dir_end - dir;
		dir[len+1] = '\0';
		char dir_temp[BUFSIZE];
		strcpy(dir_temp, dir);
		
		name_with_spaces = dir;
		
		
		if((error = remove_char(name_with_spaces, '"')) == 1){
			malformed_command = true;
		}
		printf("%s\n", name_with_spaces);
		printf("%s\n", dir_temp);
		
		char *cur = strchr(dir_temp, '"');
		if(cur != NULL && strlen(cur) > 1){
			while((cur = strchr(cur + 1, '"')) != NULL && malformed_command != true){
				if(strlen(cur) < 2){
					break;
				}
				else if(num_quotes == 2){
					if(cur[1] == ' '){
						cd_too_many_args = true;
						break;
					}
					num_quotes = 0;
				}else{
					num_quotes++;
				}
			}
		}
	}*/
	printf("STR:%s\n", str);
    char *eoln;
	char *ptr = strtok(str, " ");
	
	while(ptr != NULL){
		
		eoln = strchr(ptr, '\n');
        if (eoln == NULL) {
            ptr[MAX_STRLEN] = '\0';
        } else {
            *eoln = '\0';
        }
        
		if((data[j] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char))) == NULL) {
			fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
			return 0;
		}
		strcpy(data[j], ptr);
		ptr = strtok(NULL, " ");
		
		j++;
	}

	return j;
}

int level = 1;
int children = 0;

int main(int argc, char *argv[]) {
	
	char directory[PATH_MAX];
	
	struct sigaction action;

    	memset(&action, 0, sizeof(struct sigaction));
    	action.sa_handler = catch_signal;
    	//action.sa_flags = SA_RESTART; /* Restart syscalls if possible */
	
    	if (sigaction(SIGINT, &action, NULL) == -1) {
        	fprintf(stderr, "Cannot register signal handler. %s.\n", strerror(errno));
        	return EXIT_FAILURE;
    	}
    	if (sigaction(SIGTSTP, &action, NULL) == -1) {
        	fprintf(stderr, "Cannot register signal handler. %s.\n", strerror(errno));
        	return EXIT_FAILURE;
    	}

		
	
	
	while(true){
		char buf[BUFSIZE];
		
		if (getcwd(directory, sizeof(directory)) == NULL) {
			fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
			return EXIT_FAILURE;
		}
		
		printf("[%s%s%s]$ ", BRIGHTBLUE, directory, DEFAULT);
		
		if(fgets(buf, BUFSIZE ,stdin) == NULL) {
		
			if(signal_val == SIGINT){
				signal_val = 0;
				printf("\n");
            	continue;
			}
			
			fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
			return EXIT_FAILURE;
		}
		char *buf2 = strtok(buf, "\n");
		
		if(buf2 == NULL){
			continue;
		}
		
		char **data;
		if((data = (char **)malloc(MAX_ELEMENTS * sizeof(char*))) == NULL) {
			fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
			continue;
		}
		
		size_t index = argument_parser(data, buf2);
		
		for(int i =0; i < index; i++){
			printf("%s\n", data[i]);
		}
		
		//Change Directory
		if(strcmp(data[0], "cd") == 0){
			commandHandlerCD(data, index);
		}
		//Terminate when 'exit' is passed
		else if(starts_with(buf2, "exit") ){
			level--;
			if (level == 0){
				for(int i = 0; i < index;i++){
    				free(data[i]);
    			}
    			free(data);
				break;
			}
		}else if (starts_with(buf2, argv[0])){
			level++;
		}else if (signal_val == SIGINT) {
			//Does not allow for Crtl + C to terminate the program
            signal_val = 0;
            printf("\n");
            for(int i = 0; i < index;i++){
    			free(data[i]);
    		}
    			
    		free(data);
            continue;
        }else if (!signal_val){
        	//index - 1 is needed
        	//lets you know how many times to wait
        	//execvp
        	//printf("Parent PID: %ld\n", (long)getpid());
        	pid_t pid;
        	char command[MAX_STRLEN];
        	strcpy(command, data[0]);
        
        	
        	data[index] = (char *)NULL;
        	
        	/*for(int i = 0; i < data_length; i++){
        		printf("Args: %s\n", data[i]);
        	}
        	*/
        
        	if ((pid = fork()) < 0) {        
        		fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));        
        		return EXIT_FAILURE;    
        	}
        	else if (pid == 0){
        		//printf("Child PID: %ld\n", (long)getpid());
        		//printf("Command: %s\n", command);
        		
        		if(execvp(command, data) == -1){
            		fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            		//return EXIT_FAILURE;
            		for(int i = 0; i < index;i++){
    					free(data[i]);
    				}
    					
    				free(data);
            		break;	
        		}
        	
        	}	
        	//pid = 0
        	printf("Parent Function\n");
        	int status;
        	pid_t waiting = waitpid(pid, &status, 0);
 			while (waiting == -1 && errno == EINTR){
 				fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
 				waiting = waitpid(pid, &status, WUNTRACED | WCONTINUED);
 			}
        }
       
        for(int i = 0; i < index;i++){
    		free(data[i]);
    	}
    		
    	free(data);
    	
	}

	
	return EXIT_SUCCESS;
}

/*
char first_half[BUFSIZE];
	strcpy(first_half, str);
	
	dir = strchr(first_half, '"');
	int len = dir - first_half;
	first_half[len+1] = '\0';
	
	char second_half[BUFSIZE];
	strcpy(second_half, str);
	second_half = strrchr(second_half, '"');
	
	
	printf("STR:%s\n", str);
	char *ptr = strtok(first_half, " ");
	while(ptr != NULL){
		data[j] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
		strcpy(data[j], ptr);
		ptr = strtok(NULL, " ");
		j++;
	}
	
	strcpy(data[j], name_with_spaces);
	j++;
	
	*ptr = strtok(second_half, " ")
	while(ptr != NULL){
		data[j] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
		strcpy(data[j], ptr);
		ptr = strtok(NULL, " ");
		j++;
	}

	return j;
*/
