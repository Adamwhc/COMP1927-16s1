#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "readData.h"

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
    if((fp = fopen("files/collection.txt", "r")) == NULL){
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

void getGraph(Graph g, Queue q){
    char *curFile;
    char buff[nameSize];
    char filename[nameSize];
    FILE *fp;
    while(!emptyQueue(q)){   //while there are still links left to check
    	curFile = leaveQueue(q);
	sprintf(filename, "files/%s.txt", curFile); 
	fp = fopen(filename, "r");
       	while(strcmp(buff, "Section-1") != 0)
       		fscanf(fp, "%s", buff);	        //store first word after header
       	fscanf(fp, "%s", buff);	
	while(strcmp(buff, "#end") != 0){
		addEdge(g, curFile, buff, 1);
		fscanf(fp, "%s", buff);
		//printf("\tFrom: %s\n", curFile);
		//printf("\tTo: %s\n", buff);
	}
	fclose(fp);
	free(curFile);
    }
}


BSTree getBSTree(BSTree t, Queue q){
    int nullTree = 1;
    char *curFile;
    char buff[nameSize] = "";
    char filename[nameSize];
    FILE *fp;
    while(!emptyQueue(q)){
       curFile = leaveQueue(q);
       sprintf(filename, "files/%s.txt", curFile); 
       fp = fopen(filename, "r");
       
       	while(strcmp(buff, "Section-2") != 0) 
       	    fscanf(fp, "%s", buff);	        //store first word after header
       	fscanf(fp, "%s", buff);	

	while(strcmp(buff, "#end") != 0){
	   if (nullTree)
	        t = BSTreeInsert(t, cleanString(buff), cleanString(curFile));
	    else {
	        BSTreeInsert(t, cleanString(buff), cleanString(curFile));
	        nullTree = 0;
	    }
		    fscanf(fp, "%s", buff);
	    }
      	 free(curFile);
	    fclose(fp);       
    } 
    return t;  
}


char *cleanString(char *s) {
    int i;
    for(i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
    	return s;

    end = s + size - 1;
    while ((end >= s && isspace(*end))  || (end >= s && !isalnum(*end)))
    	end--;
    *(end + 1) = '\0';

    while ((*s && isspace(*s)) || (*s && !isalnum(*s)))
    	s++;

    return s;
}


