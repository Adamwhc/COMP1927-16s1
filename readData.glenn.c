#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "readData.h"

#define nameSize 30

char *cleanString(char *);

void getCollection(Queue q){
    char buff[nameSize];
    FILE *fp;
    Set s = newSet();
    fp = fopen("files/collection.txt", "r");
    printf("Adding pages to graph...\n");
    while(fscanf(fp, "%s", buff) > 0){
        printf("\tAdded Page to Graph: %s\n", buff);
	    if (!isElem(s, buff)){ //ignore duplicate links
		    enterQueue(q, buff);
		    insertInto(s, buff);
	    } 
    }
    disposeSet(s);
    fclose(fp);
}

void getGraph(Graph g, Queue q){
    char *curFile;
    char buff[nameSize];
    char filename[nameSize];
    FILE *fp;
    while(!emptyQueue(q)){
    	curFile = leaveQueue(q);
	sprintf(filename, "files/%s.txt", curFile); 
	fp = fopen(filename, "r");
       	while(strcmp(buff, "Section-1") != 0)
       		fscanf(fp, "%s", buff);	        //store first word after header
       	fscanf(fp, "%s", buff);	
	while(strcmp(buff, "#end") != 0){
		addEdge(g, curFile, buff);
		fscanf(fp, "%s", buff);
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


