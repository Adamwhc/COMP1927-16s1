/*
  mergeSort.c

  The mergesort helper functions to perform a mergesort
*/
#include <stdlib.h>
#include "mergeSort.h"

/* this the main mergeSort function, which calls itself recursivley */
void mergeSort(void *arr[], int lo, int hi, int size, int(*less)(const void*, const void*))
{
    if (hi <= lo) return; // base case
    int mid = (lo+hi)/2; // mid point
    mergeSort(arr, lo, mid, size, less);       //merge left
    mergeSort(arr, mid+1, hi, size, less);     //merge right
    merge(arr, lo, mid, hi, size, less);  //actually merge
}

/* recursive merge function called upon with various params */
void merge(void *arr[], int lo, int mid, int hi, int size, int(*less)(const void*, const void*))
{
    int  i, j, k, nitems = hi-lo+1;
    void **tmp = malloc(nitems * size);    //setup requirements

    i = lo; j = mid+1; k = 0;

    while (i <= mid && j <= hi) {   //do its thing
	if (less(arr[i],arr[j]))
	    tmp[k++] = arr[i++];
	else
	    tmp[k++] = arr[j++];
    }

    while (i <= mid)
	tmp[k++] = arr[i++];     //shuffle
    while (j <= hi)
	tmp[k++] = arr[j++];

    for (i = lo, k = 0; i <= hi; i++, k++) 
	arr[i] = tmp[k];
    free(tmp);     //free memory
}
