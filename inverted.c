#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BSTree.h"
#include "graph.h"
#include "queue.h"
#include "readData.h"

typedef struct node *vnode;
struct node {
    char *page;
    vnode next;
};

void printInvItoFile(BSTree);

int main(int argc, char *argv[]){
    Queue q = newQueue();
    getCollection(q);
 
    BSTree t = newBSTree();
    t = getBSTree(t, q);
    
    printInvItoFile(t);
  
    disposeQueue(q);
    dropBSTree(t);
    return 1;
}


void printInvItoFile(BSTree t){
	FILE *fp = fopen("invertedIndex.txt", "w");
   	PrintBST(t, stdout);
   	PrintBST(t, fp);
	fclose(fp);
}
