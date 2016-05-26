#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "Header/graph.h"
#include "Header/queue.h"

static void populateGraph(Graph, char*);
static void insertionSort(int, float[]);
static int usage(int);

int main(int argc, char *argv[]) {
	if(!usage(argc)) return -1;
	char urls[255], fileName[255] = "files/collection.txt";
	int urlNum = 0;
	Queue urlCollection = newQueue();
	FILE *fp = fopen(fileName, "r");		//open file
	
	while( fscanf(fp, "%s", urls)==1 ) { enterQueue(urlCollection, urls); urlNum++; } //add all url in collection to queue
	//printf("urlNum: %i", urlNum);
	//showQueue(urlCollection);
	Graph urlGraph = newGraph(urlNum); //create graph with number of vertices equal to number of URLs(urlNum)
	while(!emptyQueue(urlCollection)) populateGraph(urlGraph, leaveQueue(urlCollection)); //populate directed graph with the URLs
	fclose(fp);
	float urlPR[urlNum];
	for (urlNum = 0; urlNum < nVertices(urlGraph); urlNum++) { urlPR[urlNum] = (float)1/nVertices(urlGraph); printf("Pagerank of %s is %.3f\n", vIDName(urlGraph, urlNum), urlPR[urlNum]); } //initialize PR value for each URL 1/nV
	

	//main pagerank algo
	int maxIterations = atoi(argv[3]);
	float dampingFactor = atof(argv[1]);
	float diffPR = atof(argv[2]);
	float diff = diffPR;

	printf("Enter dampening factor, difference, and maxiterations (Format is %%f %%f %%i)\n");
	//scanf("%f %f %i", &dampingFactor, &diffPR, &maxIterations);
	printf("Damping Factor is %f\n", dampingFactor);
	printf("diffPR is %f\n", diffPR);
	printf("maxIter is %i\n", maxIterations);
	
	int iteration = 0;
	while (iteration < maxIterations && diff >= diffPR) {
		iteration++;
		//diff = 0;
		float oldPR;
		for (urlNum = 0; urlNum < nVertices(urlGraph); urlNum++) {
			oldPR = urlPR[urlNum];
			int count; 
			float sum = 0.0f;

			for (count = 0; count < nVertices(urlGraph); count++) {
				if (count == urlNum || !isConnected(urlGraph, vIDName(urlGraph, count), vIDName(urlGraph, urlNum))) continue;
				sum = sum + urlPR[count]/nEdges(urlGraph, count);
			}

			urlPR[urlNum] = (1-dampingFactor)/nVertices(urlGraph) + dampingFactor * sum;
			diff = diff + fabs(oldPR - urlPR[urlNum]);
		}
	}
	//end of main pagerank algo
	for (urlNum = 0; urlNum < nVertices(urlGraph); urlNum++) 
		printf("Pagerank of %s is %.3f\n", vIDName(urlGraph, urlNum), urlPR[urlNum]);
	
	float sortedPR[nVertices(urlGraph)];
	for (urlNum = 0; urlNum < nVertices(urlGraph); urlNum++)
		sortedPR[urlNum] = urlPR[urlNum];
		
	//showGraph(urlGraph, 1);
	char fileToWrite[255] = "files/pagerankList.txt";
	fp = fopen(fileToWrite, "w");
	//write to file
	insertionSort(nVertices(urlGraph), sortedPR);
	//float totalPR = 0;
	//for (urlNum = 0; urlNum < nVertices(urlGraph); urlNum++) totalPR += urlPR[urlNum];
	//printf("Total PR = %f\n", totalPR);
	for (urlNum = nVertices(urlGraph); urlNum > 0; urlNum--) {
		//fprintf(fp, "%s, %d, %f\n", urlPR[i]);
		int i;
		for (i = 0; i < nVertices(urlGraph); i++) {
			if(sortedPR[urlNum-1] == urlPR[i]) break;
		}

		char * urlName = vIDName(urlGraph, i);
		char newLine[] = "\n";
		int outgoingLinks = nEdges(urlGraph, i);
		char ol[100];
		sprintf(ol, "%d", outgoingLinks);
		fwrite(urlName, sizeof(char), strlen(urlName)*sizeof(char), fp);
		fwrite(", ", sizeof(char), 2*sizeof(char), fp);
		fwrite(ol, sizeof(char), strlen(ol)*sizeof(char), fp);
		sprintf(ol, "%f", urlPR[i]);
		fwrite(", ", sizeof(char), 2*sizeof(char), fp);
		fwrite(ol, sizeof(char), strlen(ol)*sizeof(char), fp);
		fwrite(newLine, sizeof(char), strlen(newLine)*sizeof(char), fp);
		urlPR[i] = -1;
	}

	fclose(fp);
	disposeQueue(urlCollection);
	disposeGraph(urlGraph);
	return 0;
}

static void populateGraph(Graph g, char* url) {
	char *urlFile;
	char buffer[255];
	urlFile = malloc( 6 + strlen(url) + 4 + 1 ); // Add 1 for null terminator.
	strcpy(urlFile, "files/");
	strcat(urlFile, url);
	strcat(urlFile, ".txt");
	FILE *tempFile = fopen(urlFile, "r");
	
	while( fscanf(tempFile, "%s", buffer)==1 ) {
		if(strstr(buffer, "url")) {
			addEdge(g, url, buffer, 1);
		}else if (strstr(buffer, "#end")) break;
	}
}

static void insertionSort(int ar_size, float *  ar) {
    	int i, j;
    	for (i = 1; i < ar_size; i++) {
		float buffer = ar[i]; //placeholder for current item
		
		for (j = i-1; (j >= 0) && (ar[j] > buffer); j--) {
		    ar[j+1] = ar[j]; //move every item before placeholder that is also larger than it up one index
		}
		ar[j+1] = buffer; //place the placeholder in the new spot
    	}
}

//usage of program
static int usage(int i) {
	if (i < 4) {
		printf("Usage: ./pagerank %%d %%diffpr %%maxiterations\n");
		return 0;
	}
	else return 1;
}

