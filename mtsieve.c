/*******************************************************************************
 * Name        : mtsieve.c
 * Author      : Valentina Bustamante and Javier Diaz
 * Date        : 06/30/2020
 * Description : Creates threads in order to find the number of primes between a 
 *				 start and end point
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <pthread.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/sysinfo.h>


typedef struct arg_struct {
	int start;
	int end;
} thread_args;

int total_count = 0;
pthread_mutex_t lock;
bool is_overflow = false;

bool check_for_three(int prime_number){
	int three_counter = 0;
	
	while (prime_number > 0){
		if (prime_number % 10 == 3){
			three_counter++;
		}
		prime_number = prime_number/10;
	}
	
	if (three_counter >= 2){
		return true;
	}
	return false;
}

void *sieve(void *ptr) {
	thread_args *ptr2 =(thread_args*)(ptr);
	
	int curr_count = 0;
	
	int len = ptr2->end - ptr2->start + 1;
	bool *high_primes = (bool *)malloc(len * sizeof(bool));
	memset(high_primes, true, len * sizeof(bool));
	
	int len2 = sqrt(ptr2->end);
	bool *low_primes = (bool *)malloc(len2+1 * sizeof(bool));
	memset(low_primes, true, len2 * sizeof(bool) + 1);
	
	int temp = 0;
	for(int i = 2; i <= len2; i++){
		 if(low_primes[i] == true){
		 	for(int j = 0; ((i*i) + j*i) <= len2; j++){
		 		temp = (i*i) + (j*i);
		 		low_primes[temp] = false;
		 	}
		 }	    
	}
	
	
	for(int p = 2; p < len2; p++){
		if(low_primes[p]){
			int i = ceil((double)ptr2->start/p) * p - ptr2->start;
			if(ptr2->start <= p){
				i+=p;
			}
			for(int j = i; j < len; j+=p){
				high_primes[j]= false;	
			}
		}
	}
	
	
	for(int i = 0; i < len; i++){
		
		if(high_primes[i] && check_for_three(i + ptr2->start)){
			curr_count++;
		}
	}
	
	free(high_primes);
    free(low_primes);
	
	
	int retval;
    if ((retval = pthread_mutex_lock(&lock)) != 0) {
    	fprintf(stderr, "Warning: Cannot lock mutex. %s.\n",
            strerror(retval));
    }

	 total_count = total_count + curr_count;
	
	
	if ((retval = pthread_mutex_unlock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n",
                strerror(retval));
    }
    
    
	pthread_exit(NULL);
}

	

bool is_integer(char *input) {
    int start = 0, len = strlen(input);

    if (len >= 1 && input[0] == '-') {
        if (len < 2) {
            return false;
        }
        start = 1;
    }
    for (int i = start; i < len; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

bool get_integer(char *input, int *value, int flag) {
    long long long_long_i;
    if (sscanf(input, "%lld", &long_long_i) != 1) {
        return false;
    }
    *value = (int)long_long_i;
    if (long_long_i != (long long)*value) {
        fprintf(stderr, "Error: Integer overflow for parameter '-%c'.\n", flag);
        is_overflow = true;
        return false;
    }
    return true;
}


int main(int argc, char **argv) {
	
	int opt;
	int count_flags = 0;
	int s_flag = 0;
	int e_flag = 0;
	int t_flag = 0;
	
	int start_num = 0;
	int end_num = 0;
	int thread_num = 0;
	

	while((opt = getopt(argc, argv, ":s:e:t:")) != -1)  
    {  
        switch(opt)  
        {   
            case 's':  
            	count_flags= count_flags + 1;
            	s_flag = 1;
            
            	 if (!is_integer(optarg) || !get_integer(optarg, &start_num, 's')) {
            		if(is_overflow){
            	 		return EXIT_FAILURE;
            	 	}
        			fprintf(stderr, "Error: Invalid input '%s' received for parameter '-s'.\n", optarg);
        			return EXIT_FAILURE;
    			} else if(start_num < 2){
    				fprintf(stderr, "Error: Starting value must be >=2.\n");
        			return EXIT_FAILURE;
    			}
                break;  
            case 'e':  
                count_flags= count_flags + 1;
                e_flag = 1;
              
            	 if (!is_integer(optarg) || !get_integer(optarg, &end_num, 'e')) {
            	 	if(is_overflow){
            	 		return EXIT_FAILURE;
            	 	}
        			fprintf(stderr, "Error: Invalid input '%s' received for parameter '-e'.\n", optarg);
        			return EXIT_FAILURE;
    			} else if(end_num < 2){
    				fprintf(stderr, "Error: Ending value must be >=2.\n");
        			return EXIT_FAILURE;
    			}
                break;
            case 't':  
                count_flags= count_flags + 1;
                t_flag = 1;
             
            	 if (!is_integer(optarg) || !get_integer(optarg, &thread_num, 't')) {
            	 	if(is_overflow){
            	 		return EXIT_FAILURE;
            	 	}
        			fprintf(stderr, "Error: Invalid input '%s' received for parameter '-t'.\n", optarg);
        			return EXIT_FAILURE;
    			} else if(thread_num < 1){
    				fprintf(stderr, "Error: Number of threads cannot be less than 1.\n");
        			return EXIT_FAILURE;
    			}
                break;   
            case ':':  
                if (optopt == 'e' || optopt == 's' || optopt == 't'){
                	fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
                } 
                return EXIT_FAILURE;
            case '?':
                if (isprint(optopt)){
                	fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
                }
                else {
                	fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
                }
                return EXIT_FAILURE;
        }  
    }
    
    if (argc == 1){
    	fprintf(stderr, "Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
    	return EXIT_FAILURE;
    }
    
    for(; optind < argc; optind++){    
    	fprintf(stderr, "Error: Non-option argument '%s' supplied.\n", argv[optind]);
    	return EXIT_FAILURE;
    } 
   
    if (s_flag == 0) {
    	fprintf(stderr, "Error: Required argument <starting value> is missing.\n");
    	return EXIT_FAILURE;
    } 
    
    if (e_flag == 0) {
    	fprintf(stderr, "Error: Required argument <ending value> is missing.\n");
    	return EXIT_FAILURE;
    } 
      
    if(t_flag == 0) {
    	fprintf(stderr, "Error: Required argument <num threads> is missing.\n");
    	return EXIT_FAILURE;
    }
    
    if (thread_num > (get_nprocs() * 2)) {
    	fprintf(stderr, "Error: Number of threads cannot exceed twice the number of processors(%d).\n", get_nprocs());
    	return EXIT_FAILURE;
    }
    
    if(start_num > end_num) {
    	fprintf(stderr, "Error: Ending value must be >= starting value.\n");
    	return EXIT_FAILURE;
    }
    
    int count = (end_num - start_num) + 1;
    int num_in_segments, remainder = 0;
    
    if (thread_num >= count){
    	num_in_segments = 1;
    	thread_num = count; 
    }
    else {
    	num_in_segments = count/thread_num; 
    	remainder = count % thread_num;
    }
 	
    thread_args targs[thread_num];
    
    pthread_t threads[thread_num];

  	int retval;
    if ((retval = pthread_mutex_init(&lock, NULL)) != 0) {
        fprintf(stderr, "Warning: Cannot create mutex. %s.\n", strerror(retval));
    }
    
    int rem_count = 1;
    int temp_start = start_num - 1;
    
	printf("Finding all prime numbers between %i and %i.\n", start_num, end_num);
	if (thread_num == 1){
		printf("%i segment:\n", thread_num);
	}
	else {
		printf("%i segments:\n", thread_num);
	}
	
    for(int i = 0; i < thread_num; i++){
    	if(remainder == 0){
    		rem_count = 0;
    	}
    	targs[i].start = temp_start + 1;
    	printf("	[%i, ", targs[i].start);
    	if(num_in_segments == 1){
    	
    		targs[i].end = targs[i].start + rem_count;
    		printf("%i]\n", targs[i].end );
    		
    	}else{
    	
    		targs[i].end = targs[i].start + (num_in_segments - 1) + rem_count;
    		printf("%i]\n", targs[i].end );
    	}
    	temp_start = targs[i].end;
    	remainder--;
    	if ((retval = pthread_create(&threads[i], NULL, sieve, (void *)(&targs[i]))) != 0) {
        	fprintf(stderr, "Error: Cannot create thread. %s.\n", 
                	strerror(retval));
             return EXIT_FAILURE;
   		}
    }
    
    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Warning: Thread %d did not join properly.\n",
                    i + 1);
        }
    }

    if ((retval = pthread_mutex_destroy(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot destroy mutex. %s.\n", strerror(retval));
    }
    
    printf("Total primes between %d and %d with two or more '3' digits: %d\n",start_num, end_num, total_count);
    return EXIT_SUCCESS;
}


















