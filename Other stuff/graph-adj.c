#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph-adj.h"


typedef struct vNode *VList;

struct vNode { char * v; VList next; };


typedef struct GraphRep {
	int   nV;     // #vertices
	int   nE;     // #edges
	int maxV;
	VList *edges; // array of lists
} GraphRep;

Graph newGraph(int maxV)
{
	Graph g = malloc(sizeof(GraphRep));
	int i, j;
	VList *e = malloc(maxV*sizeof(VList));

	for (i = 0; i < maxV; i++) {
		e[i] = NULL;
	}
	
	g->nV = 0;
	g->maxV = maxV;
	g->nE = 0;
	g->edges = e;
	return g;
}

void disposeGraph(Graph g)
{
	int i;
    	//First free all linked lists
    	for(i = 0; i < g->nV; i++){
		VList curr = g->edges[i];
		VList prev = NULL;
		while(curr != NULL){
		    prev = curr;
		    curr = curr->next;
		    free(prev);
		}
    	}
	//Free array
	free(g->edges);
	//Free the graph struct
	free(g);
}

int addEdge(Graph g, char *src, char *dest)
{
	/*
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	if (v < 0) {
		if (g->nV >= g->maxV) return 0;
		v = addVertex(src,g->vertex,g->nV);
		g->nV++;
	}
	int w = vertexID(dest,g->vertex,g->nV);
	if (w < 0) {
		if (g->nV >= g->maxV) return 0;
		w = addVertex(dest,g->vertex,g->nV);
		g->nV++;
	}
	g->edges[v][w] = 1;
	return 1;*/
	assert(g != NULL);
	int v
}
