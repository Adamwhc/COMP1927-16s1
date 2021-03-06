// graph.c ... Graph of strings (adjacency matrix)
// Written by John Shepherd, September 2015
// Taken from COMP1927 LAB8 16S1
// Comments added on 30th May 2016 by Nicholas
// Added several helper functions
// -vIDName: takes in index of a vertex, returns the name of the vertex
// -getRow:  takes in the name of a vertex, returns the array(of edges) associated with the vertex.
// -nEdges:	 takes in index of a vertex, returns the number of edges connected to that vertex where weight > 0.
// Made some changes to an existing function
// -addEdge: now takes another argument(of type int). That integer is then inserted as the weight between the vertices(instead of binary weights of 1 or 0).


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h" //changed from "Header/graph.h" - Nicholas Mulianto

#define strEQ(g,t) (strcmp((g),(t)) == 0)

typedef struct GraphRep {
    int   nV;
    int   maxV;
    char  **vertex;
    int   **edges;
} GraphRep;

// Function signatures

Graph newGraph();
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *, int);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);

static int vertexID(char *, char **, int);
int addVertex(char *, char **, int);

// newGraph()
// - create an initially empty Graph
Graph newGraph(int maxV)
{
    Graph new = malloc(sizeof(GraphRep));
    assert(new != NULL);
    int i, j;
    new->nV = 0;
    new->maxV = maxV;
    new->vertex = malloc(maxV*sizeof(char *));
    assert(new->vertex != NULL);
    new->edges = malloc(maxV*sizeof(int *));
    assert(new->edges != NULL);
    for (i = 0; i < maxV; i++) {
	new->vertex[i] = NULL;
	new->edges[i] = malloc(maxV*sizeof(int));
	assert(new->edges[i] != NULL);
	for (j = 0; j < maxV; j++)
	    new->edges[i][j] = 0;
    }
    return new;
}

// disposeGraph(Graph)
// - clean up memory associated with Graph
// + added dispose of Graph itself (Glenn)
void disposeGraph(Graph g)
{
    if (g == NULL) return;
    int i;
    for (i = 0; i < g->nV; i++) {
	free(g->vertex[i]);
    }
    for (i = 0; i < g->maxV; i++) {
	free(g->edges[i]);
    }
    free(g->edges);
    free(g->vertex);
    free(g);
}

// addEdge(Graph,Src,Dest,Weight)
// - add an edge from Src to Dest of weight Weight
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
int addEdge(Graph g, char *src, char *dest, int weight)
{
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
    g->edges[v][w] = weight;
    return 1;
}

// isConnected(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest
int isConnected(Graph g, char *src, char *dest)
{
    assert(g != NULL);
    int v = vertexID(src,g->vertex,g->nV);
    int w = vertexID(dest,g->vertex,g->nV);
    if (v < 0 || w < 0)
	return 0;
    else
	return g->edges[v][w];
}

// nVertices(Graph)
// - return # vertices currently in Graph
int nVertices(Graph g)
{
    assert(g != NULL);
    return (g->nV);
}

// showGraph(Graph)
// - display Graph
void showGraph(Graph g, int mode)
{
    assert(g != NULL);
    if (g->nV == 0)
	printf("Graph is empty\n");
    else {
	printf("Graph has %d vertices:\n",g->nV);
	int i, j;
	if (mode == 1) {
	    for (i = 0; i < g->nV; i++) {
		for (j = 0; j < g->nV; j++)
		    printf("%d",g->edges[i][j]);
		putchar('\n');
	    }
	}
	else {
	    for (i = 0; i < g->nV; i++) {
		printf("Vertex: %s\n", g->vertex[i]);
		printf("connects to\n");
		for (j = 0; j < g->nV; j++) {
		    if (g->edges[i][j])
			printf("   %s\n",g->vertex[j]);
		}
	    }
	}
    }
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
static int vertexID(char *str, char **names, int N)
{
    int i;
    for (i = 0; i < N; i++)
	if (strEQ(str,names[i])) return i;
    return -1;
}

// char * vIDName(Graph,Int)
// - takes index of vertex as argument
// - returns NULL if not found
// - returns the name of the vertex otherwise
char * vIDName(Graph g, int i)
{
    if (i > g->nV-1) return NULL;
    else return g->vertex[i];
}

// getRow(Graph, char *)
// - takes in a string(vertex name) as one of the argument
// - returns the row(pointer to an array) associated to that vertex 
int * getRow(Graph g, char *str) {
    int i;
    for (i = 0; i < g->nV; i++)
	if (strEQ(str,g->vertex[i])) break;
    return (int *)g->edges[i];
}

// int * getRow(Graph, Int)
// - takes index of vertex as argument
// - returns 0 if vertex not found
// - returns the number of non-zero edges connected to said vertex
//		- ignores self links
int nEdges(Graph g, int i)
{
    if (i > g->nV-1) return 0;
    int vertexNum, count = 0;
    for (vertexNum = 0; vertexNum < g->nV; vertexNum++) {
	if (i == vertexNum) continue; //ignore self links
	if (g->edges[i][vertexNum] != 0) count++;
    }
    return count;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
int addVertex(char *str, char **names, int N)
{
    names[N] = strdup(str);
    return N;
}
