/*******************************************************************************
 * Name        : quicksort.c
 * Author      : Javier Diaz
 * Date        : June 1,2020
 * Description : Quicksort implementation.
 * Pledge      :
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "quicksort.h"

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*comp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*comp) (const void*, const void*));

/**
 * Compares two integers passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to int pointers.
 * Returns:
 * -- 0 if the integers are equal
 * -- a positive number if the first integer is greater
 * -- a negative if the second integer is greater
 */
int int_cmp(const void *a, const void *b) {
    // TODO
    a =(int *)malloc(sizeof(int));
    b =(int *)malloc(sizeof(int));
    if ( *(a) == *(b) ){
    	return 0;
    }
    else if ( *(a) > *(b) ){
    	return 4;
    }
    else {
    	return -4;
    }
}

/**
 * Compares two doubles passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to double pointers.
 * Returns:
 * -- 0 if the doubles are equal
 * -- 1 if the first double is greater
 * -- -1 if the second double is greater
 */
int dbl_cmp(const void *a, const void *b) {
    // TODO
    a =(double *)malloc(sizeof(double));
    b =(double *)malloc(sizeof(double));
    if ( *(a) == *(b) ){
    	return 0;
    }
    else if ( *(a) > *(b) ){
    	return 1;
    }
    else {
    	return -1;
    }
}

/**
 * Compares two char arrays passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to char* pointers (i.e. char**).
 * Returns the result of calling strcmp on them.
 */
int str_cmp(const void *a, const void *b) { 
    // TODO
    a =(char *)malloc(sizeof(char));
    b =(char *)malloc(sizeof(char));
    return (strcmp(*(a), *(b)));
}

/**
 * Swaps the values in two pointers.
 *
 * Casts the void pointers to character types and works with them as char
 * pointers for the remainder of the function.
 * Swaps one byte at a time, until all 'size' bytes have been swapped.
 * For example, if ints are passed in, size will be 4. Therefore, this function
 * swaps 4 bytes in a and b character pointers.
 */
static void swap(void *a, void *b, size_t size) {
    // TODO
    if (size == 4){
    	int *temp;
  		while (*a){
  			*temp++ = *b++;
  			*b++ = a*++
  			*a++ = *temp++ 
  		}
    }
    else if (size == 8){
 		double *temp;
 		while (*a){
  			*temp++ = *b++;
  			*b++ = a*++
  			*a++ = *temp++ 
  		}
    }
    else {
		char *temp;
		while (*a){
  			*temp++ = *b++;
  			*b++ = a*++
  			*a++ = *temp++ 
  		}
    }
    
}

/**
 * Partitions array around a pivot, utilizing the swap function.
 * Each time the function runs, the pivot is placed into the correct index of
 * the array in sorted order. All elements less than the pivot should
 * be to its left, and all elements greater than or equal to the pivot should be
 * to its right.
 * The function pointer is dereferenced when it is used.
 * Indexing into void *array does not work. All work must be performed with
 * pointer arithmetic.
 */
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*comp) (const void*, const void*)) {
    // TODO
    int p = *(array);
    size_t s = left;
    for (size_t i = left + 1; i <= right; i++){
    	if (*(array + i) < p) {
    		s++;
    		swap(*(array + s),*(array + i))
    	}
    }
    swap(*(array + left),*(array + s);
    return s;
}

/**
 * Sorts with lomuto partitioning, with recursive calls on each side of the
 * pivot.
 * This is the function that does the work, since it takes in both left and
 * right index values.
 */
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*comp) (const void*, const void*)) {
    // TODO
}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*comp) (const void*, const void*)) {
    // TODO
    quicksort_helper(array, 0, len - 1, elem_sz,)
}
