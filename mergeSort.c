#include <stdlib.h>
#include "mergeSort.h"

void mergesort(void *arr[], int lo, int hi, int size, int(*less)(const void*, const void*))
{
    if (hi <= lo) return; // base case
    int mid = (lo+hi)/2; // mid point
    mergesort(arr, lo, mid, size, less);
    mergesort(arr, mid+1, hi, size, less);
    merge(arr, lo, mid, hi, size, less);
}


void merge(void *arr[], int lo, int mid, int hi, int size, int(*less)(const void*, const void*))
{
    int  i, j, k, nitems = hi-lo+1;
    void **tmp = malloc(nitems * size);

    i = lo; j = mid+1; k = 0;

    while (i <= mid && j <= hi) {
	if (less(arr[i],arr[j]))
	    tmp[k++] = arr[i++];
	else
	    tmp[k++] = arr[j++];
    }

    while (i <= mid)
	tmp[k++] = arr[i++];
    while (j <= hi)
	tmp[k++] = arr[j++];

    for (i = lo, k = 0; i <= hi; i++, k++) 
	arr[i] = tmp[k];
    free(tmp);
}
