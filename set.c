// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h"

#define strEQ(s,t) (strcmp((s),(t)) == 0)
#define strLT(s,t) (strcmp((s),(t)) < 0)

typedef struct Node *Link;

typedef struct Node {
	char *val;
	Link  next;
} Node;
	
typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;

// Function signatures
static Link newNode(char *);
static void disposeNode(Link);
static int  findNode(Link,char *,Link *,Link *);


// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->elems = NULL;
	return new;
}

char *indexElement(Set s, int n){
	if (n > (s->nelems - 1)) return NULL;
	int i;
	Link temp = s->elems;
	for(i = 0; i < n; i++){
		temp = temp->next;
	}
	char *retStr = strdup(temp->val);
	return retStr;
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
void insertInto(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
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
	char *nstr;
	Link curr = NULL, prev = NULL;
	nstr = strdup(str);
	assert(s != NULL);
	int retVal = findNode(s->elems,nstr,&curr,&prev);
	free(nstr);
	return retVal;
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet(Set s)
{	
	if (s == NULL || s->nelems ==0 ) return;
	Link curr;
	/*if (s->nelems == 0)
		printf("Set is empty\n");
	else {*/
		//printf("Set has %d elements:\n",s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			//printf("[%03d] %s\n", id, curr->val);
			printf("'%s'@%p - ", curr->val, curr->val);
			id++;
			curr = curr->next;
		}
	//}
}

// Helper functions

static Link newNode(char *str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = strdup(str);
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
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
	//char nstr[999];
	//strcpy(nstr, str);
	while ((curr != NULL) && (strcmp(curr->val, str) < 0)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr; *pre = prev;
	int result = (curr != NULL && strEQ(str, curr->val));
	//free(nstr);
	
	//printf("%i", result);
	return result;
}
