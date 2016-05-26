// BSTree.h ... interface to binary search tree ADT

#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNode *BSTree;

// create an empty BSTree
BSTree newBSTree();

// free memory associated with BSTree
void dropBSTree(BSTree);

// display a BSTree
void PrintBST(BSTree, FILE *);

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char *, char *);

#endif
