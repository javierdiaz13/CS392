/*******************************************************************************
 * Name        : quicksort.h
 * Author      : Javier Diaz
 * Date        : June 1, 2020
 * Description : Quicksort header.
 * Pledge      : 
 ******************************************************************************/
/**
 * TODO - put all non-static function prototypes from quicksort.c inside
 * wrapper #ifndef.
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

/* Function prototypes */
int int_cmp(const void *a, const void *b)
double dbl_cmp(const void *a, const void *b)
char str_cmp(const void *a, const void *b)
void quicksort(void *array, size_t len, size_t elem_sz, int (*comp) (const void*, const void*))
               

#endif