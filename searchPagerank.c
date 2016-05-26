#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Header/queue.h"
#include "Header/set.h"

static void readThroughIndex(FILE *, Set, int);
static int usage(int);
//static int readThroughPrlist(FILE *, char **, float *);

int main(int argc, char *argv[]) {
	if(!usage(argc)) return -1;
	FILE *pagerankList = fopen("files/pagerankList.txt", "r");
	FILE *invIndex = fopen("files/invertedIndex.txt", "r");
	char **urls = NULL; int i, urlCount = 0; float *pr = NULL;
	Set content = newSet();
	
	//Read through pagerank list
	float fbuffer;	int ignore;	char buffer[255];
	
	while( fscanf(pagerankList, "%[^,], %d, %f\n", buffer, &ignore, &fbuffer) != EOF) {
		urlCount++;									//increases count everytime scan is successful
		urls = realloc(urls, sizeof(char*)*(urlCount));					//dynamically change size
		pr = realloc(pr, sizeof(float*)*(urlCount));
		urls[urlCount-1] = strdup(buffer);						//insert url to array
		pr[urlCount-1] = fbuffer;							//insert pagerank value to another array
	}
	//done reading

	//Add command line arguments into array and set
	char **keywords = malloc( (argc-1)*sizeof(char*) );
	if (argc > 1) {
		for (i = 0; i < (argc-1); i++)	{
			insertInto(content, argv[i+1], urlCount);				//insert into basic set
			keywords[i] = strdup(argv[i+1]);			
		}
	}
	readThroughIndex(invIndex, content, urlCount);						//associate urls with set members
	
	int map[urlCount];
	for (i = 0; i < urlCount; i++) {
		map[i] = 1;
	}
	//Find common urls that are associated with the keywords
	for (i = 0; i < argc-1; i++) {
		char ** temp = getArrayOfNode(content, keywords[i]);
		int j;
		for(j = 0; j < urlCount; j++) {
			int k, found = 0;
			for(k = 0; temp[k] != NULL; k++) {
				if(strcmp(urls[j], temp[k]) == 0) {found = 1; break;}
			}
			map[j] = map[j]&found;
		}
		/*printf("%s\n", keywords[i]);
		char ** temp = getArrayOfNode(content, keywords[i]);
		int j;
		for (j = 0; j < urlCount; j++) {
			int k; int found = 0;
			if(urls[j] == NULL) continue;						//skips if index is NULL
			for(k = 0; temp[k] != NULL; k++)
				if(strcmp(urls[j], temp[k]) == 0) { found = 1; break; }		//compares two arrays, breaks if a common member is found
			if(!found) urls[j] = NULL;						//if not found, null the index
		}*/
		for(j = 0; j < urlCount; j++) {
			free(temp[j]);
			temp[j] = NULL;
		}
		free(temp);
	}	
	//urls are already sorted by pagerank, so just print them out
	int printedVal = 0;
	for (i = 0; i < urlCount; i++) {
		if(map[i] == 0) {; continue;}
		printf("\t%s\n", urls[i]);
		printedVal++;
		if(printedVal == 10) break;							//breaks when 10 urls are printed
	}
	if (printedVal == 0) printf("Not Found.\n");

	fclose(pagerankList);
	fclose(invIndex);
	free(urls);
	free(pr);
	free(keywords);
	disposeSet(content);
	return 0;
}

// static void readThroughIndex(FILE *, Set, int);
// Description: read through the inverted index file and insert urls associated to existing base nodes
static void readThroughIndex(FILE *fp, Set s, int urlCount) {
	char wordBuffer[255];
	char buffer[255];
	while( fscanf(fp, "%s", buffer) == 1 ) {
		if(!strstr(buffer, "url")) {
			insertInto(s, buffer, urlCount);
			strcpy(wordBuffer, buffer);
		} else if (strstr(buffer, "url")){			
			insertIntoNode(s, wordBuffer, buffer); 
		}
	}
}

//usage of program
static int usage(int i) {
	if (i < 2) {
		printf("Usage: ./searchPagerank %%keyword %%keyword ... %%keyword\n");
		return 0;
	}
	else return 1;
}

/*static int readThroughPrlist(FILE *fp, char **urls, float *pr) {
	int ignore; int sizeCount = 0;
	char buffer[255];
	float fbuffer;
	urls = malloc(sizeof(char*)*(sizeCount));
	pr = malloc(sizeof(float*)*(sizeCount));
	//url32, 1, 0.189635

	while( fscanf(fp, "%[^,], %d, %f\n", buffer, &ignore, &fbuffer) != EOF) {
		//printf("%s, %i, %f\n", buffer, ignore, fbuffer);
		sizeCount++;
		urls = realloc(urls, sizeof(char*)*(sizeCount));
		pr = realloc(pr, sizeof(float*)*(sizeCount));
		urls[sizeCount-1] = strdup(buffer);
		pr[sizeCount-1] = fbuffer;
		//printf("URL [%s] has Pagerank of %f.\n", urls[sizeCount-1], pr[sizeCount-1]);
	}
	
	return sizeCount;
}*/

/*
static int readThroughPrlist(FILE *fp, char ***urls, float **pr) {
	int ignore; int sizeCount = 0;
	char buffer[255];
	float fbuffer;
	printf("BEFORE MALLOC\n");
	*urls = malloc(sizeof(char*)*1000);
	*pr = malloc(sizeof(float*)*1000);
	//url32, 1, 0.189635

	while( fscanf(fp, "%[^,], %d, %f\n", buffer, &ignore, &fbuffer) != EOF) {
		//printf("%s, %i, %f\n", buffer, ignore, fbuffer);
		sizeCount++;
		printf("BEFORE REALLOC\n");
		*urls = realloc(*urls, sizeof(char*)*(sizeCount+1));
		*pr = realloc(*pr, sizeof(float*)*(sizeCount+1));
		printf("BEFORE INSERTING VALUE\n");
		
		*urls[sizeCount-1] = malloc(sizeof(char) * strlen(buffer));
		*pr[sizeCount-1] = malloc(sizeof(float));
		*urls[sizeCount-1] = strdup(buffer);
		*pr[sizeCount-1] = fbuffer;
				//printf("URL [%s] has Pagerank of %f.\n", urls[sizeCount-1], pr[sizeCount-1]);
	}
	
	return sizeCount;
}*/
