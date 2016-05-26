#ifndef READDATA_H
#define READDATA_H

#include "graph.h"
#include "queue.h"
#include "BSTree.h"
#include "set.h"

void getCollection(Queue);
void getGraph(Graph, Queue);
BSTree getBSTree(BSTree, Queue);
char *cleanString(char *);

#endif
