// graph.c ... Graph of strings (adjacency matrix)
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

#define strEQ(g,t) (strcmp((g),(t)) == 0)

typedef struct node *vnode;
struct node { 
  int val; 
  vnode next; 
} node;

typedef struct GraphRep {
	int   nV;
	int   maxV;
	char **vertices; //array of vertices
	vnode *adj;	 //array of linked list connections
} GraphRep;

// Function signatures

Graph newGraph();
void  disposeList(vnode n);
void  disposeGraph(Graph);
int   addEdge(Graph, char *, char *);     
int   nVertices(Graph);
int   nEdges(Graph, int);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);
int   vertexID(Graph, char *);
int   addVertex(Graph, char *);

// newGraph()
// - create an initially empty Graph
Graph newGraph(int maxV)
{
	Graph new = malloc(sizeof(GraphRep));
	assert(new != NULL);
	int i;
	new->nV = 0;
	new->maxV = maxV;
	//new->vertex = malloc(maxV*sizeof(char *));
	//assert(new->vertex != NULL);
	new->vertices = malloc(maxV * sizeof(char*));
	new->adj = malloc(sizeof(node) * maxV);
	assert(new->adj != NULL);
	for (i = 0; i < maxV; i++) {
		new->vertices[i] = NULL;
		new->adj[i] = NULL;
	//	new->edges[i] = malloc(maxV*sizeof(Num));
	//	assert(new->edges[i] != NULL);
	//	for (j = 0; j < maxV; j++)
	//		new->edges[i][j] = 0;
	}
	return new;
}

// disposeGraph(Graph)
// - clean up memory associated with Graph
void disposeGraph(Graph g)
{
      if (g == NULL) return;
	int i;
	for (i = 0; i < g->maxV; i++) {
	    if (g->adj[i] == NULL) continue;
	    disposeList(g->adj[i]);
    	}
    	for (i = 0; i < g->nV; i++)
    		free(g->vertices[i]);
    	free(g->adj);
    	free(g->vertices);
    	free(g);
}

void disposeList(vnode n){
	if (n == NULL) return;
	disposeList(n->next);
	free(n);
}

// addEdge(Graph,Src,Dest)
// - add an edge from Src to Dest
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
int addEdge(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(g, src);
	if (v < 0) { //vertex doesn't exist
		if (g->nV >= g->maxV) return 0;
		v = addVertex(g, src);
		g->nV++;
	}
	int w = vertexID(g, dest);
	if (w < 0) {
		if (g->nV >= g->maxV) return 0;
		w = addVertex(g, dest);
		g->nV++;
	}
	
	if (!isConnected(g, src, dest)){
		vnode temp = g->adj[v];
		vnode new = malloc(sizeof(node));
		new->val = w;
		new->next = NULL;
		if (temp != NULL){
			while(temp->next != NULL){
				temp = temp->next;
			}
			temp->next = new;
		}
		else
			g->adj[v] = new;
		return 1;
	} 
	else
		return 0;
}

// isConnected(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest
int isConnected(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(g, src);
	int w = vertexID(g, dest);
	if (v < 0 || w < 0)
		return 0;
	else{
		vnode temp = g->adj[v];
		while(temp != NULL){
			if(temp->val == w) return 1;
			temp = temp->next;
		}
		return 0;
	}
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
		printf("\nGraph has %d vertices:\n",g->nV);
		int i;
		if (mode == 0) {
			for (i = 0; i < g->nV; i++) {
				printf("%s:\t", g->vertices[i]);
				vnode temp = g->adj[i];
				while(temp != NULL){
					printf("%s -> ", g->vertices[temp->val]);
					temp = temp->next;
				}
				puts("null");
			}
		}
		else {
			for (i = 0; i < g->nV; i++) {
				/*printf("Vertex: %s\n", g->vertices[i]);
				printf("connects to\n");
				for (j = 0; j < g->nV; j++) {
					if (g->edges[i][j])
						printf("   %s\n",g->vertex[j]);
				}*/
				if (g->adj[i] != NULL)
					printf("%s=", g->vertices[g->adj[i]->val]);
			}
		}
	}
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
int vertexID(Graph g, char *str)
{
	int i;
	for (i = 0; i < g->nV; i++)
		if (strEQ(str, g->vertices[i])) return i;
	return -1;
}

char *vIDName(Graph g, int i)
{
	if (i > g->nV-1) return NULL;
	else return g->vertices[i];
}

int nEdges(Graph g, int i)
{
	if (i > g->nV-1) return 0;
	int count = 0;
	vnode temp = g->adj[i];
	if (temp == NULL) return 0;
	while (temp != NULL){
		count++;
		temp = temp->next;
	}
	return count;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
int addVertex(Graph g, char *str)
{
	g->vertices[g->nV] = strdup(str);
	return g->nV;
}
