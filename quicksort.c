/*******************************************************************************
 * Name        : quicksort.c
 * Author      : 
 * Date        : 
 * Description : Quicksort implementation.
 * Pledge      :
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "quicksort.h"

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz, int (*comp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz, int (*comp) (const void*, const void*));

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
    int *aa = (int *)malloc(sizeof(int));
    int *bb = (int *)malloc(sizeof(int));
    return ( *(aa) - *(bb) );
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
    double *aa = (double *)malloc(sizeof(double));
    double *bb = (double *)malloc(sizeof(double));
    int result = ( *(aa) - *(bb) );
    if (result){
    	return 0;
    }
    else if (result > 0){
    	return 1;
    }
    else {
    	return -1
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
    char *aa = (char *)malloc(sizeof(char));
    char *bb = (char *)malloc(sizeof(char));
    return strcmp(*(aa),*(bb));
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
    char *aa = (char *)malloc(sizeof(char) * size);
    char *bb = (char *)malloc(sizeof(char) * size);
    char *temp = (char *)malloc(sizeof(char) * size);
    while (aa*){
		*temp++ = *aa++;
		*aa++ = *bb++;
		*bb++ = *temp++;
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
    char *arr = (char *)array;
    char *p = array + left * elem_sz;
    int s = left;
    for (size_t i = left + 1; i <= right; i++){
    	if (*(array + i) < *(p)){
    		s++;
    		swap(*(array + s * elem_sz), *(array + i * elem_sz), elem_sz);
    	}
    }
    swap(*(array + s * elem_sz), *(array + i * elem_sz), elem_sz);
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
    int s = lomuto_partition(array, left, right, elem_sz);
    
}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*comp) (const void*, const void*)) {
    // TODO
    if (elem_sz == 4){
    	return quicksort_helper(void *array, 0, len - 1; 4, (*int_cmp) (const void*, const void*));
    }
    else if (elem_sz == 8){
    	return quicksort_helper(void *array, 0, len - 1; 8, (*dbl_cmp) (const void*, const void*));
    }
    else {
    	return quicksort_helper(void *array, 0, len - 1; 1, (*str_cmp) (const void*, const void*));
    }
}
