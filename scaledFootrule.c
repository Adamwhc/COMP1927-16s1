/*
   scaledFootrule.c

   Written by Glenn & Nicholas

   This file is for Part 3 of the COMP1927 assignment.
   Given one or more files that contain a ranking position for
   certain pages, the minimum scale distance is returned along with the
   associated correct ordering of the pages

   OPTIMIZATIONS MADE TO MAKE ALGORITHM SMART (EXTRA MARK PLS):

   1) NULL SET
		Set with no elements.
		The intersection of sets with no 'shared' items is a null set.

   2) UNIQUE ITEMS
		Distinct elements/items of a rank.

   3) LOCKED IN ITEMS
		When an element is present in multiple ranks, and in the same position. 
		It's P value can be locked in.
		This will ensure minimum scaled Footrule for that item

   4 -> (MAX_INT)
   

 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "set.h"

static int usage(int);
static int readRanks(char*, char***);
static void permute(int *, int, int, double *, int[], char***, char**, int, int *, int, int[]);
static int findIndex(char **, char *, int);
static double calcSFR(int[], int[], int, int, char ***, char **) ;

int main(int argc, char *argv[]) {
    int i, j;								//indexing
    if(!usage(argc)) return -1;				//checks command line args
    char **ranks = malloc((argc-1) * sizeof(char*));	//store the name of rank file(s)
    char **rankTable[argc-1];				//2d array to store content of rank file(s)
    int rowSize[argc-1];					//size of each column of 'rankTable'
    int totalSize = 0;
    Set tempSet = newSet();					//set to find union of rankTable columns

    for (i = 0; i < (argc-1); i++) {
	char buffer[255];					//buffer for string
	int len;							//length of string
	strcpy(buffer, argv[i+1]);			//copies argv into buffer
	len = strlen(buffer);				//gets length of string
	ranks[i] = malloc(len+1);			//correctly allocate size for a ranks[i] index + 1(null char)
	strcpy(ranks[i], buffer);			//copies buffer into the array index

	rankTable[i] = NULL;				
	rowSize[i] = readRanks(ranks[i], &rankTable[i]); //read through the file. store number of urls in rankTable[i]
	totalSize += rowSize[i];
	for(j = 0; j < rowSize[i]; j++)
	    insertInto(tempSet, rankTable[i][j], 1); //insert every member of array into set to get rid of duplicates
    }

    int uniqueItems = nElems(tempSet);		//get number of unique items in set
    char **elements = getElements(tempSet);		//array of all elements(no duplicates)
    int pValue[uniqueItems];			//array for the P values of the elements
    int fpValue[uniqueItems];			//final P values that gives the lowest scaled foot rule after going through all permutations
    int lockedValues[uniqueItems];			
    int lockedValues2[uniqueItems];
    int minSize = INT_MAX;				//cant get bigger than this to store minimum column size
    int maxSize = INT_MIN;				//cant get lower than this to store maximum column size
    int locked = 0;
    //initializes array.
    for(i = 0; i < uniqueItems; i++) {
		pValue[i] = i;
		fpValue[i] = 0;
		lockedValues2[i] = -1;
    }

    //find max row size and min row size etc etc
    for(i = 0; i < (argc-1); i++) {
		if(rowSize[i] < minSize) minSize = rowSize[i];		//finding minimum array size
		if(rowSize[i] > maxSize) maxSize = rowSize[i];		//finding maximum array size
    }
    //IF uniqueItems == totalSize, means the intersection of all ranks is a null set.
    //Let x be items in rank 1 and y be items in rank 2
    //x1 y1 x2 y2 x3 y3 .... xn yn would give you the lowest scaled footrule
    if (uniqueItems == totalSize) {
		int k = 0;
		for(i = 0; i < maxSize; i++) {
			for(j = 0; j < argc-1; j++) {
			if(i >= rowSize[j]) continue;
			int index = findIndex(elements, rankTable[j][i], uniqueItems);
			pValue[index-1] = k;
			k++;
			}
		}
		printf("%lf\n", calcSFR(pValue, rowSize, uniqueItems, argc-1, rankTable, elements));
		for(i = 0; i < maxSize; i++) {
			for(j = 0; j < argc-1; j++) {
			if(i >= rowSize[j]) continue;
			printf("%s\n", rankTable[j][i]);
			}
		}

		disposeSet(tempSet);
		for (i = 0; i < argc-1; i++) {
			for (j = 0; j < rowSize[i]; j++)
			free(rankTable[i][j]);
			free(rankTable[i]);
			free(ranks[i]);
		}
		free(ranks);
		for (i = 0; i < uniqueItems; i++)
			free(elements[i]);
		free(elements);
		return 0;
		}

    //lock em up
    //if an item exist in the those sets and they exist in the same position, you can lock it in the P value
    //for example if an item is ranked at index 3 in all of the ranks, you can lock that value in place.
    for(i = 0; i < minSize; i++) {
		lockedValues[i] = i;
		for(j = 1; j < (argc-1); j++) {
			if(strcmp(rankTable[0][i], rankTable[j][i]) != 0) lockedValues[i] = -1;
		}
		if(lockedValues[i] != -1)
			locked++;
    }

    //re-index the lockedValues to match elements[] array. (should've done that in the beginning)
    //this is done to make comparison easier.
    for(i = 0; i < minSize; i++) {
		if(lockedValues[i] != -1) {
			int index = findIndex(elements, rankTable[0][i], uniqueItems);
			lockedValues2[index-1] = lockedValues[i];
		}
    }

    //unlocked values
    //separating unlocked values from locked values into another array.
    int upValue[uniqueItems-locked];
    j = 0;
    for(i = 0; i < uniqueItems; i++) {
		if (lockedValues[i] == pValue[i]) { j++; continue; }
		upValue[i-j] = pValue[i];
    }

    double totalSFR = DBL_MAX;
    permute(upValue, 0, uniqueItems-locked, &totalSFR, rowSize, rankTable, elements, argc-1, fpValue, locked, lockedValues2);
    printf("%.6f\n", totalSFR);
    //prints the rank in order

    for(i = 0; i < uniqueItems; i++) {
		for (j = 0; j < uniqueItems-1; j++)
			if(i == fpValue[j]) break;
		printf("%s\n", elements[j]);
		fpValue[j] = -1;
    }

    //freeing stuff up
    disposeSet(tempSet);
    for (i = 0; i < argc-1; i++) {
		for (j = 0; j < rowSize[i]; j++)
			free(rankTable[i][j]);
		free(rankTable[i]);
		free(ranks[i]);
    }
    free(ranks);
    for (i = 0; i < uniqueItems; i++)
		free(elements[i]);
    free(elements);
    return 0;
}

//usage of program
static int usage(int i) {
    if (i < 2) {
	printf("Usage: ./scaledFootrule %%rank1.txt %%rank2.txt ... %%rankn.txt\n");
	return 0;
    }
    else return 1;
}

// static void readRanks(FILE *, Set, int);
// Description: read through files and store the ranking in an array
//		read through all argv
static int readRanks(char * fileName, char ***rC) {
    int size = 0, len = 0;
    char buffer[255];
    FILE *fp = fopen(fileName, "r");
    char **rankColumn = malloc(sizeof(char*) * 1);

    while( fscanf(fp, "%s", buffer) != EOF) {
	if(strstr(buffer, "url") != NULL) {
	    size++;									//increases count everytime scan is successful
	    len = strlen(buffer);
	    rankColumn = realloc(rankColumn, sizeof(char*)*(size));			//dynamically change size
	    (rankColumn)[size-1] = malloc(len+1);					//insert url to array
	    strcpy((rankColumn)[size-1], buffer);
	}
    }
    *rC = rankColumn;
    fclose(fp);
    return size;
}

//static void permute()
//find all permutation of an integer array
//only permutate through unlocked array
//and then merge unlocked array with locked array
//then calculate scaledFootrule
static void permute(int *array,int i,int length, double *totalSFR, int rowSize[], char ***rankTable, char **elements, int colSize, int *fpValue, int locked, int lockedValues[]) { 
    if (length == i){
	int i;
	int k = 0;
	int combinedArray[length + locked];
	if(*totalSFR == 0) return;
	//combine array of locked values with the permutated values
	//fill the array with the positions of the items in locked array`
	//fill the array with the unlocked values in order.
	for(i = 0; i < length+locked ;i++) {
	    if(lockedValues[i] != -1 && k < locked) {
		combinedArray[i] = lockedValues[i];
		k++;
	    }
	    else {
		combinedArray[i] = array[i-k];
	    }
	}
	double curSFR =	calcSFR(combinedArray, rowSize, length+locked, colSize, rankTable, elements);
	int j;
	if(curSFR < *totalSFR) {
	    *totalSFR = curSFR;
	    for (j = 0; j < length; j++) {
		fpValue[j] = (int)(combinedArray)[j];
	    }
	}
	return;
    }
    int j = i;
    for (j = i; j < length; j++) { 
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
	permute(array,i+1,length, totalSFR, rowSize, rankTable, elements, colSize, fpValue, locked, lockedValues);
	temp = array[j];
	array[j] = array[i];
	array[i] = temp;
    }
    return;
}

//static int findIndex()
//locate the index of a string in an array + 1, if not found return 0
static int findIndex(char **array, char *word, int length) {
    int i;
    for (i = 0; i < length; i++){
	if(strcmp(array[i], word) == 0) return i+1;
    }
    return 0;
}

//static double calcSFR
//calculate scaledFootrule
static double calcSFR(int array[], int rowSize[], int uniqueItems, int colSize, char ***rankTable, char **elements) {
    double totalSFR = 0;
    int i;
    int j;
    for(i = 0; i < uniqueItems; i++) {
		for(j = 0; j < colSize ; j++){
			//1. find the index of an element in their respective rows/columns.
			//	 divide that by the size of the row/column
			//2. get the element's P value and divide it by the total number of elements
			//3. add 1 and 2 up.
			int index = findIndex(rankTable[j], elements[i], rowSize[j]);
			if(index == 0) continue;
			double a = (double)index/(double)rowSize[j];
			double b = (double)(array[i]+1)/(double)uniqueItems;
			double indivSFR = fabs(a-b);
			totalSFR += indivSFR;
		}
    }
    return totalSFR;
}
