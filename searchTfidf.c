/*
   searchTfidf.c

   Written by Glenn & Nicholas

   This file is for Part 2 of the COMP1927 assignment.
   Given a list of keywords, outputs the top ten pages in 
   descending order of summation of tf-idf values of all search terms.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "queue.h"
#include "set.h"
#include "graph.h"
#include "readData.h"

static void readThroughIndex(FILE *, Set, int);
static int readThroughCollection(FILE *, char***);
static void insertionSort(int, double[], char**, int[]);
static void populateGraph(Graph, char **, char **, int);
static void strlower(char*);

//main function to actually do stuff
int main(int argc, char *argv[]) {
    if(argc < 2){ //we must have atleast one keyword
	printf("Usage: %s <Space separated kewords>\n", argv[0]);
	exit(0);
    }
    
    //open the collection files
    FILE *files = fopen("files/collection.txt", "r");
    char **allURLs;
    int i, urlCount = readThroughCollection(files, &allURLs);
    fclose(files);	

    files = fopen("invertedIndex.txt", "r");
    Set urlSet = newSet();


    // reads command line arguments
    // stores arguments into an array and also add them into the set
    char **keywords = NULL;
    if (argc > 1) {
	keywords = malloc( (argc-1)*sizeof(char*) );
	int i;	
	for (i = 0; i < (argc-1); i++)	{
	    insertInto(urlSet, argv[i+1], urlCount);
	    keywords[i] = strdup(argv[i+1]);
	}	
    }

    readThroughIndex(files, urlSet, urlCount);
    fclose(files);

    int map[urlCount];
    for (i = 0; i < urlCount; i++) {
	map[i] = 1;
    }
    //Find common urls that are associated with the keywords
    for (i = 0; i < argc-1; i++) {
	char ** temp = getArrayOfNode(urlSet, keywords[i]);
	int j;
	for(j = 0; j < urlCount; j++) {
	    int k, found = 0;
	    for(k = 0; temp[k] != NULL; k++)
		if(strcmp(allURLs[j], temp[k]) == 0) {found = 1; break;}

	    map[j] = map[j]&found;
	}
	for(j = 0; j < urlCount; j++) {
	    temp[j] = NULL;
	}
	free(temp);
    }
    //char **urls = findIntersection(urlSet, keywords);
    //for (i = 0; i < urlCount; i++) {
    //	if(urls[i] == NULL) continue;
    //	printf("%s\n", urls[i]);
    //}
    Graph urlGraph = newGraph(urlCount+nElems(urlSet));
    populateGraph(urlGraph, allURLs, keywords, urlCount);
    //showSet(urlSet, stdout);
    //showGraph(urlGraph, 0);
    //showGraph(urlGraph, 1);	
    double tfidf[urlCount];
    double idf[argc-1];

    for (i = 0; i < urlCount; i++) tfidf[i] = 0.0f; //initialize array

    //printf("\nTotal Number of Documents: %i\nTotal Vertex Count = %i\n", urlCount, nVertices(urlGraph));
    for (i = 0; keywords[i] != NULL; i++) {
	int * row = getRow(urlGraph, keywords[i]);
	int nV = nVertices(urlGraph)-(argc-1);
	int j, docWithWord = 0;
	//printf("idf(%s) ", keywords[i]);
	for (j = 0; j < nV; j++) {
	    if(row[j] > 0) docWithWord++;
	}
	if (docWithWord == 0) docWithWord++;
	idf[i] = log10((double)nV/abs(docWithWord));
	//printf("%i/%i", nV, docWithWord);
	//printf("\n");

	for(j = 0; j < urlCount; j++) {
	    //calculate tfidf
	    tfidf[j] += (row[j] * idf[i]);
	}
    }

    insertionSort(urlCount, tfidf, allURLs, map);

    int printedVal = 0;
    for (i = urlCount-1; i > 0; i--) {
	if(map[i] == 0) {; continue;}
	printf("%s %lf\n", allURLs[i], tfidf[i]);
	printedVal++;
	if(printedVal == 10) break;							//breaks when 10 urls are printed
    }
    if (!printedVal) { printf("No URLS Found.\n"); return -1;}

    //for (i = 0; i < urlCount; i++) printf("%lf ", tfidf[i]);
    disposeSet(urlSet);
    disposeGraph(urlGraph);
    return 0;
}

// static void readThroughIndex(FILE *, Set, int);
// Description: read through the inverted index file and insert urls associated to existing nodes
static void readThroughIndex(FILE *fp, Set s, int urlCount) {
    char wordBuffer[255];
    char buffer[255];
    while( fscanf(fp, "%s", buffer) == 1 ) {
	if(!strstr(buffer, "url")) {
	    strcpy(wordBuffer, buffer);
	} else if (strstr(buffer, "url")){			
	    insertIntoNode(s, wordBuffer, buffer); 
	}
    }
}

// static int readThroughCollection(FILE *);
// Description: read through a file and return the number of urls contained in it
static int readThroughCollection(FILE *fp, char ***allURLs) {
    char buffer[255];
    int urlCount = 1;
    *allURLs = malloc(sizeof(char*) * 1);
    while( fscanf(fp, "%s", buffer) == 1) {
	*allURLs = realloc(*allURLs, sizeof(char*) * urlCount);
	(*allURLs)[urlCount-1] = malloc(strlen(buffer)+1);
	strcpy((*allURLs)[urlCount-1], buffer);
	urlCount++;		
    }		
    return urlCount-1;
}

// static char ** findIntersection(Set, char **);
// finds common urls that are associated to every keyword. 
// returns an array containing the urls
/*static char ** findIntersection(Set s, char **keywords) {
  int i;
  char **urls = getArrayOfNode(s, keywords[0]);
  for (i = 1; i < nElems(s); i++) {
  char ** temp = getArrayOfNode(s, keywords[i]);
  int j;
  for (j = 0; j < nArraylen(s); j++) {
  int k; int found = 0;
  if(urls[j] == NULL) continue;
  for(k = 0; temp[k] != NULL; k++) {
  if(strcmp(urls[j], temp[k]) == 0) { found = 1; break; }
  }
  if(!found) {
  urls[j] = NULL;
  }
  }

  for(j = 0; j < nElems(s); j++) {
  if(temp[j] == NULL) break;
  free(temp[j]);
  temp[j] = NULL;
  }
  free(temp);
  }
  return urls;
  }*/

// static void populateGraph (Graph, char**, char**, int)
// creates a table of url vs keywords
static void populateGraph(Graph g, char **urlList, char **keywords, int urlCount) {
    char buffer[255];
    int keywordSize, i;
    keywordSize = 0;
    for (i = 0; keywords[i] != NULL; i++) keywordSize++; 
    int wordCount[keywordSize];	
    for (i = 0; keywords[i] != NULL; i++) wordCount[i] = 0; 

    for (i = 0; i < urlCount; i++) {
	addEdge(g, urlList[i], urlList[i], 0);		
    }
    for (i = 0; i < keywordSize; i++) {
	addEdge(g, keywords[i], keywords[i], 0);
    }

    for (i = 0; i < urlCount; i++) {
	if (urlList[i] == NULL) continue;
	char *urlFile;
	int j;
	urlFile = malloc( 6 + strlen(urlList[i]) + 4 + 1 ); // Add 1 for null terminator.
	strcpy(urlFile, "files/");
	strcat(urlFile, urlList[i]);
	strcat(urlFile, ".txt");
	FILE *fp = fopen(urlFile, "r");

	while( fscanf(fp, "%s", buffer) == 1) {
	    cleanString(buffer);
	    strlower(buffer);
	    //printf("%s\n", buffer);
	    for (j = 0; keywords[j] != NULL; j++) {
		if (strcmp(keywords[j], buffer) == 0) {
		    wordCount[j]++;
		}
	    }	
	}

	for (j = 0; keywords[j] != NULL; j++) {
	    addEdge(g, urlList[i], keywords[j], wordCount[j]);
	    addEdge(g, keywords[j], urlList[i], wordCount[j]);
	    wordCount[j] = 0;
	}
	free(urlFile);
	fclose(fp);
    }
}

// static void strlower(char*)
// converts string to all lower cases
static void strlower(char *s)
{
    for ( ; *s != '\0'; s++)
	*s = tolower(*s);
}

// static void insertionSort(int, float*, float*)
// sorts by tfidf vals
static void insertionSort(int ar_size, double *  ar, char ** ar2, int * ar3) {
    int i, j;
    for (i = 1; i < ar_size; i++) {
	char *temp;
	temp = ar2[i];
	double buffer = ar[i]; //placeholder for current item
	int buffer2 = ar3[i];		

	for (j = i-1; (j >= 0) && (ar[j] > buffer); j--) {
	    ar[j+1] = ar[j]; //move every item before placeholder that is also larger than it up one index
	    ar2[j+1] = ar2[j];
	    ar3[j+1] = ar3[j];
	}
	ar[j+1] = buffer; //place the placeholder in the new spot
	ar2[j+1] = temp;
	ar3[j+1] = buffer2;
    }
}
