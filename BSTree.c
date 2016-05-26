// BSTree.c ... implementation of binary search tree ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"

typedef struct listNode *listLink;
struct listNode {
	char *pageName;
	listLink next;
} listNode;

typedef struct BSTNode *BSTLink;
struct BSTNode {
	char *word;
	listLink page;
	BSTLink left, right;
} BSTNode;


BSTLink newBSTNode(char *, char *);
listLink newLinkNode(char *);
void dropList(listLink);
void showLinkNode(listLink, FILE *);
void showBSTreeNode(BSTree, FILE *);
void addLinkToList(BSTree, listLink, char *);

// make a new node containing a value
BSTLink newBSTNode(char *word, char *p)
{
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->page = newLinkNode(p);
	new->word = strdup(word);
	new->left = new->right = NULL;
	return new;
}

listLink newLinkNode(char *p){
    listLink new = malloc(sizeof(listNode));
    assert(new != NULL);
    new->pageName = strdup(p);
    new->next = NULL; 
    return new;
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL) return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	dropList(t->page);
	free(t->word);
	free(t);
}

void dropList(listLink l)
{
    if(l==NULL) return;
    dropList(l->next);
    free(l->pageName);
    free(l);
}

void showLinkNode(listLink l, FILE *stream){
 	if (l == NULL) return;
    //printf(" %s", l->pageName);
    showLinkNode(l->next, stream);
    fprintf(stream, " %s", l->pageName);
}

// display BSTree root node
void showBSTreeNode(BSTree t, FILE *stream)
{
	if (t == NULL) return;
	fprintf(stream, "%s", t->word);
	//printf("\n%s", t->word);
	showLinkNode(t->page, stream);
	fprintf(stream, "\n");
}

// print values in infix order
void PrintBST(BSTree t, FILE *stream)
{
	if (t == NULL) return;
	PrintBST(t->left, stream);
	showBSTreeNode(t, stream);
	PrintBST(t->right, stream);
}


// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *v, char *p)
{
	if (t == NULL)
		return newBSTNode(v, p);
	else if (strcmp(v, t->word) < 0)
		t->left = BSTreeInsert(t->left, v, p);
	else if (strcmp(v, t->word) > 0)
		t->right = BSTreeInsert(t->right, v, p);
	else // (v == t->value)
	    addLinkToList(t, t->page, p);
		/* don't insert duplicates */;
	return t;
}

void addLinkToList(BSTree t, listLink l, char *p){
    if (l == NULL)
    	t->page = newLinkNode(p);
    else{
    	if (strcmp(l->pageName, p) == 0) return;
    	if (strcmp(l->pageName, p) < 0){
    		listLink nn = newLinkNode(p);
		nn->next = l;
		t->page = nn;
		return;
	} 
    	while (l->next!=NULL){
    		if (strcmp(l->next->pageName, p) == 0) return;
    		
		if ((strcmp(l->pageName, p) > 0) && (strcmp(l->next->pageName, p) < 0)){
			listLink nn = newLinkNode(p);
			nn->next = l->next;
			l->next = nn;
			return;
		}
		l = l->next;
    	}
    	listLink nn = newLinkNode(p);
    	l->next = nn;
    }
}

