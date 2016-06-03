// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *,int);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set, FILE *);

// custom functions
void insertIntoNode(Set,char *,char *);
int nArraylen(Set);
char ** getElements(Set);
char ** getArrayOfNode(Set, char*);

#endif
