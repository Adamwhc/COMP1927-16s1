/*
   inverted.c

   Written by Glenn & Nicholas

   This file is for Part 1-B of the COMP1927 assignment.
   It uses a BST data structure to print all the words across
   the files supplied, and gives a list of pages these words are
   used in. AKA an 'inverted' index
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BSTree.h"
#include "graph.h"
#include "queue.h"
#include "readData.h"

/*typedef struct node *vnode;
struct node {
    char *page;
    vnode next;
};*/

//a function header for the printing function
//that prints the BST in infix order to print
//in ascending order
void printInvItoFile(BSTree);

//do all the stuff
int main(int argc, char *argv[]){
    Queue q = newQueue();
    getCollection(q); //store pages

    BSTree t = newBSTree(); //create a BST
    t = getBSTree(t, q); //fill tree with pages

    printInvItoFile(t);	//print to a file

    disposeQueue(q); //save memory
    dropBSTree(t);   //save memory
    return 1;
}

//prints all the data to a file (and stdout)
void printInvItoFile(BSTree t){
    PrintBST(t, stdout); //print to STDOUT
    FILE *fp;
    //be sure that we don't try to address null memory
    if((fp = fopen("invertedIndex.txt", "w")) != NULL){
	PrintBST(t, fp);     //and to the file
	fclose(fp);
    } else {
	printf("\n***CANNOT OPEN FILE TO SAVE INDEX");
    }
    return;
}
