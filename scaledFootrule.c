#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "Header/set.h"
/*
**	TODO: Permutate the array
**	
**
*/
static int usage(int);
static int readRanks(char*, char***);
static void permute(int *, int, int);
static int findIndex(char **, char *, int);

int main(int argc, char *argv[]) {
	int i, j;
	if(!usage(argc)) return -1;
	char **ranks = malloc((argc-1) * sizeof(char*));
	char **rankTable[argc-1];
	int rowSize[argc-1];
	Set tempSet = newSet();

	for (i = 0; i < (argc-1); i++) {
		char buffer[255];
		int len;
		strcpy(buffer, argv[i+1]);
		len = strlen(buffer);
		ranks[i] = malloc(len+1);
		strcpy(ranks[i], buffer);
		rankTable[i] = NULL;
		rowSize[i] = readRanks(ranks[i], &rankTable[i]);

		printf("%s: ", ranks[i]);
		for(j = 0; j < rowSize[i]; j++) {
			printf("%s ", rankTable[i][j]);
			insertInto(tempSet, rankTable[i][j], 0);
		}			
		printf("\n");
	}

	int uniqueItems = nElems(tempSet);
	char **elements = getElements(tempSet);
	int pValue[uniqueItems];
	printf("Number of Unique Items: %d\nUniques: ", uniqueItems);
	for(i = 0; i < uniqueItems; i++) {
		printf("%s ", elements[i]);
		pValue[uniqueItems-1-i] = i;
	}
	printf("\n");

	//find W(C, P)
	printf("Calculating ScaledFootrule\n");
	double totalSFR = 0;
	for(i = 0; i < uniqueItems; i++) {
		printf("%s: ", elements[i]);
		for(j = 0; j < argc-1 ; j++){
			int index = findIndex(rankTable[j], elements[i], rowSize[j]);
			//printf("index of %s at rankTable[%d] is %d\n", elements[i], j, index);
			if(index == 0) continue;
			double a = (double)index/(double)rowSize[j];
			double b = (double)(pValue[i]+1)/(double)uniqueItems;
			double indivSFR = fabs(a-b);
			printf("%d. %lf ", j+1, indivSFR);
			totalSFR += indivSFR;
		}
		printf("\n");
	}
	printf("Total Scaled Footrule is %lf\n", totalSFR);
	
	permute(pValue, 0, uniqueItems);
	
	

	/*
		int n = a.length;
		int[] p = new int[n];  // Weight index control array initially all zeros. Of course, same size of the char array.
		int i = 1; //Upper bound index. i.e: if string is "abc" then index i could be at "c"
		while (i < n) {
			if (p[i] < i) { //if the weight index is bigger or the same it means that we have already switched between these i,j (one iteration before).
				int j = ((i % 2) == 0) ? 0 : p[i];//Lower bound index. i.e: if string is "abc" then j index will always be 0.
				swap(a, i, j);
				// Print current
				p[i]++; //Adding 1 to the specific weight that relates to the char array.
				i = 1; //if i was 2 (for example), after the swap we now need to swap for i=1
			}
			else { 
				p[i] = 0;//Weight index will be zero because one iteration before, it was 1 (for example) to indicate that char array a[i] swapped.
				i++;//i index will have the option to go forward in the char array for "longer swaps"
			}
		}
	*/
	

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
static void permute(int *array,int i,int length) { 
	if (length == i){
		int j;
		for(j = 0; j < length; j++);
			//printf("%d", (array)[j]); 
		//printf("\n");
		return;
	}
	int j = i;
	for (j = i; j < length; j++) { 
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
		permute(array,i+1,length);
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
