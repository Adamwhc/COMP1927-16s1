// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *,int);
void insertIntoNode(Set,char *,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
int nArraylen(Set);
void showSet(Set, FILE *);
char ** getElements(Set);
char ** getArrayOfNode(Set, char*);
char *indexElement(Set, int);

#endif
