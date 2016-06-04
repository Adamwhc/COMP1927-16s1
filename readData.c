/*
   readData.c

   Written by Glenn & Nicholas for Ass2 1927 

   A nice little helper file to provide
   functions to assist other sections of the
   assignment.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "readData.h"

//just a default name size for files
#define nameSize 30

/*function definition for the method that cleans a string
  (cleans of trailing and leading spaces/punctuation)*/
char *cleanString(char *);

/*this function, given a queue of pages to check, will open each file
  listed in the queue and assemble a graph of all the edges (representing links
  between pages, and the nodes (pages themselves)*/
void getCollection(Queue q){
    char buff[nameSize];
    FILE *fp;
    Set s = newSet(); //if we can't find the file list then leave
    if((fp = fopen("collection.txt", "r")) == NULL){
	printf("FILES NOT FOUND\n");
	exit(0);
    }

    while(fscanf(fp, "%s", buff) > 0){
	if (!isElem(s, buff)){ //ignore duplicate links
	    enterQueue(q, buff);
	    insertInto(s, buff, 0);
	} 
    }
    disposeSet(s);  //clear memory
    fclose(fp);	    //dispose of pointer
}

/* this function creates a graph given a queue
   full of files names to insert */
void getGraph(Graph g, Queue q){
    char *curFile;
    char buff[nameSize];
    char filename[nameSize];
    FILE *fp;
    while(!emptyQueue(q)){   //while there are still links left to check
	curFile = leaveQueue(q);     //get next file
	sprintf(filename, "%s.txt", curFile); 
	fp = fopen(filename, "r");
	while(strcmp(buff, "Section-1") != 0) //check for given keyword
	    fscanf(fp, "%s", buff);	        //store first word after header
	fscanf(fp, "%s", buff);	
	while(strcmp(buff, "#end") != 0){  //check again for given keyword
	    addEdge(g, curFile, buff, 1);
	    fscanf(fp, "%s", buff);
	}
	fclose(fp);
	free(curFile);  //free the memory
    }
}

/* this function takes in a queue of file names
   and goes through and stores them in a tree along
   with a linked list of their keywords */
BSTree getBSTree(BSTree t, Queue q){
    int nullTree = 1;
    char *curFile;
    char buff[nameSize] = "";
    char filename[nameSize];
    FILE *fp;
    while(!emptyQueue(q)){  //while still files left to check..
	curFile = leaveQueue(q);
	sprintf(filename, "%s.txt", curFile);  //insert the file name
	fp = fopen(filename, "r");

	while(strcmp(buff, "Section-2") != 0)   //while we aren't at the keyword section yet
	    fscanf(fp, "%s", buff);	        //store first word after header
	fscanf(fp, "%s", buff);	

	while(strcmp(buff, "#end") != 0){      //while we aren't at end of keywords
	    if (nullTree)   //implementation for BST is weird, we have to redefine if empty
		t = BSTreeInsert(t, cleanString(buff), cleanString(curFile));  //clean and insert
	    else {
		BSTreeInsert(t, cleanString(buff), cleanString(curFile));  //clean and insert
		nullTree = 0;
	    }
	    fscanf(fp, "%s", buff);  //store
	}
	free(curFile); //free memory
	fclose(fp);       
    } 
    return t;  
}

/* this function goes through strings and cleans
   them up nicely for processing */
char *cleanString(char *s) {
    int i;
    for(i = 0; s[i] != '\0'; i++)  //lowercase the string
	s[i] = tolower(s[i]);
    size_t size;
    char *end;

    size = strlen(s);  //grab info we need

    if (!size)
	return s;

    end = s + size - 1; //being to remove trailing and leading spaces
    while ((end >= s && isspace(*end))  || (end >= s && !isalnum(*end)))
	end--;
    *(end + 1) = '\0';
    //also remove trailing and leading punctuation
    while ((*s && isspace(*s)) || (*s && !isalnum(*s)))
	s++;

    return s;
}


