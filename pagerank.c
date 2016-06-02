#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergeSort.h"
#include "readData.h"
#include "graph.h"
#include "queue.h"

//the page rank node structure used to sort pages by their
//page ranks, aswell as their number of out degrees
typedef struct prn {
	float pr;
	int outdeg;
	char *name;
} *prNode;


void pageRank(Graph, float, float, int);
void printPRtoFile(Graph, float *);


// this comparison function is used in mergeSort to compare
// the page rank values of nodes, so that they may be ordered by
// page rank on output
int cmpfp (const void *a, const void *b)
{
	//tolerance epsilon of 0.0000001 used because of dealing with floating
	//point values (important!)
	return ((((prNode)b)->pr - ((prNode)a)->pr ) > 0.0000001) ? 0 : 1;
}

// this comparison function is used in mergeSort to compare the number
// of outdegrees to internally sort the elements by their out degrees, before
// they are sorted on pagerank (and this is maintained because mergesort is STABLE)
int cmpfpEdge (const void *a, const void *b)
{
	return ((((prNode)b)->outdeg - ((prNode)a)->outdeg ) > 0) ? 0 : 1;
}

//the main function, used to coordinate the creation of data structures and
//to pipe data into functions to print the page rank values
int main(int argc, char *argv[]){
	//checks to ensure the user has provided the correct number of inputs
	if(argc < 4){
		printf("Usage: %s <Dampening factor> <Diff Value> <Max Iterations>\n", argv[0]);
		exit(0);
	}

	float damf, diffPR;
	int maxIt;
	damf = atof(argv[1]);	// the dampening factor
	diffPR = atof(argv[2]);	// the difference tolerance value
	maxIt = atof(argv[3]);	// max number of iterations

	//create the data structures
	Queue q = newQueue();
	getCollection(q);
	Graph g = newGraph(queueSize(q));
	getGraph(g, q);

    	//showGraph(g, 0);
	pageRank(g, damf, diffPR, maxIt);

	//save alllllllll the memory
	disposeGraph(g);
	disposeQueue(q);
	return 1;
}

// a modified version of the page rank function, that calculates page rank for a series
// of graph nodes, when given a graph and a series of constant parametres
void pageRank(Graph g, float damf, float diffPR, int maxIt){
	int i, j, prOld, it=0, n=nVertices(g);
	float sum, diff=diffPR, PR[nVertices(g)];
	for(i=0; i<n; i++)
		PR[i] = 1 / (float) n;
        
     //while the exit conditions have not been met...
	while(it < maxIt && diff >= diffPR){
		diff = 0;
		it++;
		for(i=0; i<n; i++){
			prOld = PR[i];
			sum = 0;
			for(j = 0; j<n; j++){
				if (isConnected(g, vIDName(g, j), vIDName(g, i)))
					sum += PR[j] / nEdges(g, j);
			}
			// Google's original page rank formula in all it's glory
			PR[i] = (1 - damf)/n + damf*sum;
			diff += fabs(PR[i] - prOld);
		}
	}
	// given the graph and array of pageranks, print these to a file
	printPRtoFile(g, PR);
}

void printPRtoFile(Graph g, float *PR){
	int i, n=nVertices(g);
         
	//create an array of pagerank Nodes 
	prNode arr[n];

	for(i=0; i<n; i++){
		prNode new = malloc( sizeof( prNode ) );
		new->pr = PR[i];
		new->outdeg = nEdges(g, i);
		new->name = strdup(vIDName(g, i));
		arr[i] = new;
	}
	//sort the page rank nodes by their outbound edges
	mergeSort((void*)arr, 0, nVertices(g) - 1, sizeof(prNode), cmpfpEdge);
	//and then sort by pagerank value (internal sorting of outbound edges will be retain,
	//as merge sort is inherently STABLE)
	mergeSort((void*)arr, 0, nVertices(g) - 1, sizeof(prNode), cmpfp);

	FILE *fp;
	//just incase we don't have write access, we don't want to crash
	if((fp = fopen("pagerankList.txt", "w")) != NULL){
		for(i=0; i<n; i++){
			printf("%s, %d, %f\n", (arr[i])->name, (arr[i])->outdeg, (arr[i])->pr);
			fprintf(fp, "%s, %d, %f\n", (arr[i])->name, (arr[i])->outdeg, (arr[i])->pr);
			free(arr[i]->name); //free memory we don't need
			free(arr[i]);
		}
		fclose(fp);
	}
}
