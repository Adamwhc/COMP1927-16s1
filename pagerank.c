#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergeSort.h"
#include "readData.h"
#include "graph.h"
#include "queue.h"

typedef struct prn {
    float pr;
    int outdeg;
    char *name;
} *prNode;

void pageRank(Graph, float, float, int);
void printPRtoFile(Graph, float *);

int cmpfp (const void *a, const void *b)
{
    return ((((prNode)b)->pr - ((prNode)a)->pr ) > 0.0000001) ? 0 : 1;
}

int cmpfpEdge (const void *a, const void *b)
{
    return ((((prNode)b)->outdeg - ((prNode)a)->outdeg ) > 0) ? 0 : 1;
}

int main(int argc, char *argv[]){
    if(argc < 4){
        printf("Usage: %s <Dampening factor> <Diff Value> <Max Iterations>\n", argv[0]);
        exit(0);
    }

    float damf, diffPR;
    int maxIt;
    damf = atof(argv[1]);
    diffPR = atof(argv[2]);
    maxIt = atof(argv[3]);

    Queue q = newQueue();
    getCollection(q);
    Graph g = newGraph(queueSize(q));
    getGraph(g, q);

    showGraph(g, 0);
    pageRank(g, damf, diffPR, maxIt);

    disposeGraph(g);
    disposeQueue(q);
    return 1;
}

void pageRank(Graph g, float damf, float diffPR, int maxIt){
    int i, j, prOld, it=0, n=nVertices(g);
    float sum, diff=diffPR, PR[nVertices(g)];
    for(i=0; i<n; i++)
        PR[i] = 1 / (float) n;
        
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
            PR[i] = (1 - damf)/n + damf*sum;
            diff += fabs(PR[i] - prOld);
        }
    }
   printPRtoFile(g, PR);
}

void printPRtoFile(Graph g, float *PR){
   int i, n=nVertices(g);
          
    prNode arr[n];

    for(i=0; i<n; i++){
	    prNode new = malloc( sizeof( prNode ) );
	    new->pr = PR[i];
	    new->outdeg = nEdges(g, i);
	    new->name = strdup(vIDName(g, i));
	    arr[i] = new;
    }
    mergeSort((void*)arr, 0, nVertices(g) - 1, sizeof(prNode), cmpfpEdge);
    mergeSort((void*)arr, 0, nVertices(g) - 1, sizeof(prNode), cmpfp);

    //puts("\nPageRank pagerankList.txt output:");
    FILE *fp = fopen("pagerankList.txt", "w");
    for(i=0; i<n; i++){
	    printf("%s, %d, %f\n", (arr[i])->name, (arr[i])->outdeg, (arr[i])->pr);
	    fprintf(fp, "%s, %d, %f\n", (arr[i])->name, (arr[i])->outdeg, (arr[i])->pr);
	   free(arr[i]->name);
	   free(arr[i]);
	}
	fclose(fp);
}
