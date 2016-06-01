#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "set.h"
//	TODOne: Permutate the array
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
		for(j = 0; j < rowSize[i]; j++)
			insertInto(tempSet, rankTable[i][j], 0); //insert every member of array into set to get rid of duplicates
	}

	int uniqueItems = nElems(tempSet);		//get number of unique items in set
	char **elements = getElements(tempSet);		//array of all elements(no duplicates)
	int pValue[uniqueItems];			//array for the P values of the elements
	int fpValue[uniqueItems];			//final P values that gives the lowest scaled foot rule after going through all permutations
	int lockedValues[uniqueItems];
	int lockedValues2[uniqueItems];
	int minSize = INT_MAX;
	int maxSize = INT_MIN;
	int locked = 0;
	//initializes array.
	for(i = 0; i < uniqueItems; i++) {
		pValue[i] = i;
		fpValue[i] = 0;
		lockedValues2[i] = -1;
		printf("%s ", elements[i]);
	}

	//lock em up
	for(i = 0; i < (argc-1); i++) {
		if(rowSize[i] < minSize) minSize = rowSize[i];
		if(rowSize[i] > maxSize) maxSize = rowSize[i];
	}
	
	for(i = 0; i < minSize; i++) {
		lockedValues[i] = i;
		for(j = 1; j < (argc-1); j++) {
			if(strcmp(rankTable[0][i], rankTable[j][i]) != 0) lockedValues[i] = -1;
		}
		if(lockedValues[i] != -1) {
			printf("%s locked at %i\n", rankTable[0][i], lockedValues[i]);
			locked++;
		}
	}

	for(i = 0; i < minSize; i++) {
		if(lockedValues[i] != -1) {
			int index = findIndex(elements, rankTable[0][i], uniqueItems);
			lockedValues2[index-1] = lockedValues[i];
			printf("%s %s\n", elements[index-1], rankTable[0][i]);
		}
	}
	for(i = 0; i < uniqueItems; i++) {
		printf("%i ", lockedValues2[i]);
	}
	printf("\n");
	//unlocked values
	int upValue[uniqueItems-locked];
	j = 0;
	for(i = 0; i < uniqueItems; i++) {
		if (lockedValues[i] == pValue[i]) {j++; continue; }
		upValue[i-j] = pValue[i];
		printf("%i %i\n", lockedValues2[i], pValue[i]);
		printf("%i is unlocked\n", upValue[i-j]);
	}

	double totalSFR = INT_MAX;
	permute(upValue, 0, uniqueItems-locked, &totalSFR, rowSize, rankTable, elements, argc-1, fpValue, locked, lockedValues2);
	printf("%lf\n", totalSFR);
	//prints the rank in order
	for(i = 0; i < uniqueItems; i++) {
		for (j = 0; j < uniqueItems-1; j++)
			if(i == fpValue[j]) break;
		printf("%d %s\n", i, elements[j]);
		fpValue[j] = -1;
	}
	//disposeSet(tempSet);
	//TODO: free arrays
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
static int readRanks(char * fileName, char ***rC) {
	int size = 0, len = 0;
	char buffer[255];
	FILE *fp = fopen(fileName, "r");
	char **rankColumn = malloc(sizeof(char*) * 1);
	
	while( fscanf(fp, "%s", buffer) != EOF) {
		if(strstr(buffer, "url") != NULL) {
			size++;									//increases count everytime scan is successful
			len = strlen(buffer);
			rankColumn = realloc(rankColumn, sizeof(char*)*(size));		//dynamically change size
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
static void permute(int *array,int i,int length, double *totalSFR, int rowSize[], char ***rankTable, char **elements, int colSize, int *fpValue, int locked, int lockedValues[]) { 
	if (length == i){
		int i;
		int k = 0;
		int combinedArray[length + locked];
		//printf("%i\n", length+locked);
		//for(i = 0; i < length; i++) {
		//	printf("%i ", array[i]);
		//}
		//printf("\n%i\n", length+locked);
		for(i = 0; i < length+locked ;i++) {
			if(lockedValues[i] != -1 && k < locked) {
				combinedArray[i] = lockedValues[i];
				k++;
				//printf("Here locked %i. %i \n", i, lockedValues[i]);
			}
			else {
				combinedArray[i] = array[i-k];
				//printf("Here %i. %i \n", i-k, array[i-k]);
			}
		}
		double curSFR =	calcSFR(combinedArray, rowSize, length+locked, colSize, rankTable, elements);
		int j;
		if(curSFR <= *totalSFR) {
			printf("%lf < %lf\n", curSFR, *totalSFR);
			*totalSFR = curSFR;
			for (j = 0; j < length; j++) {
				fpValue[j] = (int)(combinedArray)[j];
				printf("%i ", combinedArray[j]);
			}
			printf("\n");
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
//locate the index of a string in an array, if not found return 0
static int findIndex(char **array, char *word, int length) {
	int i;
	for (i = 0; i < length; i++){
		if(strcmp(array[i], word) == 0) return i+1;
	}
	return 0;
}

static double calcSFR(int array[], int rowSize[], int uniqueItems, int colSize, char ***rankTable, char **elements) {
	//printf("Calculating ScaledFootrule\n");
	double totalSFR = 0;
	int i;
	int j;
	for(i = 0; i < uniqueItems; i++) {
		//printf("%s: ", elements[i]);
		for(j = 0; j < colSize ; j++){
			int index = findIndex(rankTable[j], elements[i], rowSize[j]);
			//printf("index of %s at rankTable[%d] is %d\n", elements[i], j, index);
			if(index == 0) continue;
			double a = (double)index/(double)rowSize[j];
			double b = (double)(array[i]+1)/(double)uniqueItems;
			double indivSFR = fabs(a-b);
			//printf("%d. %lf ", j+1, indivSFR);
			totalSFR += indivSFR;
		}
		
	}
	//printf("curSFR = %lf\n", totalSFR);
	return totalSFR;
}
