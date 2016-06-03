// set.c ... 
// Written by John Shepherd, September 2015
// Taken from COMP1927 LAB8 16S1
// Comments added on 30th May 2016 by Nicholas
// Changed the structure of the set
// -Node now contains an array(vertex) that stores other items. (in our case, it stores associated URLs)
// -SetRep now contains another integer(arraylen), the maximum size that a Node->vertex can reach
// Added several helper functions
// -insertIntoNode: inserts an item(in this case, URL strings) into the array in a node.
// -nArraylen: returns SetRep->arraylen as integer.
// -getElements: returns the base set as an array. Base set is the set without the added array.
// -getArrayOfNode: returns an array of all the items in a node.
// -indexElement: defunct
// Made some changes to several existing functions
// -insertInto: It now takes an extra argument of type integer. Changes SetRep->arraylen.
// -showSet: Changed how the set is printed. It now takes another argument of type FILE, so user can specify the output (stdout or file).

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h" //changed from "Header/set.h" - Nicholas Mulianto

#define strEQ(s,t) (strcmp((s),(t)) == 0)
#define strLT(s,t) (strcmp((s),(t)) < 0)

typedef struct Node *Link;

typedef struct Node {
	char *val;
	char **vertex;
	Link  next;
} Node;
	
typedef struct SetRep {
	int   nelems;
	int   arraylen;
	Link  elems;
} SetRep;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *,int);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);

static Link newNode(char *, int);
static void disposeNode(Link);
static int  findNode(Link,char *,Link *,Link *);


// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->arraylen = 0;
	new->elems = NULL;
	return new;
}

// disposeSet(Set)
// - clean up memory associated with Set
void disposeSet(Set s)
{
	if (s == NULL) return;
	Link next, curr = s->elems;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
	free(s);
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto(Set s, char *str, int max)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str, max);
	s->arraylen = max;
	
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

void insertIntoNode(Set s, char *str, char *url) {
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if(!found) { return; } //not found
	
	int i;
	for (i = 0; i < s->arraylen; i++) {
		if (curr->vertex[i] != NULL) {
			if (!strcmp(curr->vertex[i], url)) { return; }
			else continue;
		}
		else if (curr->vertex[i] == NULL) {
			curr->vertex[i] = malloc(sizeof(char) * strlen(url));
			strcpy(curr->vertex[i], url);
			//printf( "Successful.\n" );
			return;
		}
	}
	
}

char ** getArrayOfNode(Set s, char *str) {
	Link curr, prev;
	int found = findNode(s->elems, str, &curr, &prev);
	if(!found) { printf("Node %s Not Found.", str); return NULL; }
	char **copy = malloc(s->arraylen*sizeof(char*));
	int i;
	for (i = 0; curr->vertex[i] != NULL; i++) {
		copy[i] = strdup(curr->vertex[i]);
		//copy[i] = malloc( (1+strlen(curr->vertex[i])) * sizeof(char));
		//strcpy(copy[i], curr->vertex[i]);
	}
	return copy;
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (!found) return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	disposeNode(curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	curr = malloc(sizeof(Link));
	prev = malloc(sizeof(Link));
	int ret =  findNode(s->elems,str,&curr,&prev);
	return ret;
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

//returns max size of array
int  nArraylen(Set s)
{
	assert(s != NULL);
	return s->arraylen;
}


// showSet(Set)
// - display Set (for debugging)
void showSet(Set s, FILE *fp)
{
	Link curr;
	if (s->nelems == 0)
		printf("Set is empty\n");
	else {
		fprintf(fp, "\nSet has %d elements:\n",s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			fprintf(fp, "\t%s\t", curr->val);
			int i;
			for (i = 0; curr->vertex[i] != NULL; i++)
				fprintf(fp, "%s\t", curr->vertex[i]);
			fprintf(fp, "\n");
			id++;
			curr = curr->next;
		}
	}
}


// char ** getElements(Set)
// Stores all the elements in an array and return the array
char ** getElements(Set s) 
{
	int i = 0;
	char **elements = malloc(sizeof(char*) * nElems(s));
	Link curr = s->elems;

	while( curr != NULL) {
		int len = strlen(curr->val);
		(elements)[i] = malloc(len+1);					//insert url to array
		strcpy((elements)[i], curr->val);
		curr = curr->next;
		i++;
	}

	return elements;
}

// Helper functions

static Link newNode(char *str, int max)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = strdup(str);
	new->next = NULL;
	new->vertex = malloc(max*sizeof(char *));
	int i;
	for (i = 0; i < max; i++)
		new->vertex[i] = NULL;
	return new;
}


static void disposeNode(Link curr)
{
	assert(curr != NULL);
	int i;
	for (i = 0; curr->vertex[i]!=NULL; i++) {
		free(curr->vertex[i]);
	}
	free(curr->vertex);
	free(curr->val);
	free(curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode(Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	char nstr[999];
	strcpy(nstr, str);
	while (curr != NULL && strLT(curr->val,nstr)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr; *pre = prev;
	return (curr != NULL && strEQ(nstr,curr->val));
}


