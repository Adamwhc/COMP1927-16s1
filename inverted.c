#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Header/queue.h"
#include "Header/set.h"

static void populateSet(Set, char*, int);
static void strlower(char*);
static void normalise(char*);

int main(int argc, char *argv[]) {
	char urls[255], fileName[255] = "files/collection.txt";
	int urlNum = 0;
	Queue urlCollection = newQueue();
	FILE *fp = fopen(fileName, "r");		//open file
	Set urlSet = newSet();
	//char **urlArray = malloc(sizeof(char*) * 1024);
	//while( fscanf(fp, "%s", urls)==1 ) { urlArray[urlNum] = strdup(urls); urlNum++;} //add all url in collection to queue
	//for (i = 0; i < urlNum; i++)
	//	insertInto(urlSet, urlArray[i], urlNum);
	while( fscanf(fp, "%s", urls)==1 ) { enterQueue(urlCollection, urls); urlNum++;} //add all url in collection to queue

	fclose(fp);

	while( !emptyQueue(urlCollection) ) {
		populateSet(urlSet, leaveQueue(urlCollection), urlNum);
	}
	
	fp = fopen("files/invertedIndex.txt", "w");
	showSet( urlSet, fp);
	showSet( urlSet, stdout);
	disposeQueue(urlCollection);
	disposeSet(urlSet);
	fclose(fp);
	//free(fp);
	return 1;
}

static void populateSet(Set s, char* url, int max) {
	char *urlFile;
	char buffer[255];
	urlFile = malloc( 6 + strlen(url) + 4 + 1 ); // Add 1 for null terminator.
	strcpy(urlFile, "files/");
	strcat(urlFile, url);
	strcat(urlFile, ".txt");

	FILE *tempFile = fopen(urlFile, "r");
	int foundSection2 = 0;
	while( fscanf(tempFile, "%s", buffer) == 1 ) {
		if(strstr(buffer, "#start")) {
			fscanf(tempFile, "%s", buffer);
			//if(strstr(buffer, "Section-1")) {foundSection1 = 1; printf("Section 1 found\n"); continue;}
			if(strstr(buffer, "Section-2")) {foundSection2 = 1; continue;}
		}
		if (strstr(buffer, "#end")) {
			fscanf(tempFile, "%s", buffer);
			//if(strstr(buffer, "Section-1")) {foundSection1 = 0; printf("Section 1 end\n"); continue;}
			if(strstr(buffer, "Setion-2")) {foundSection2 = 0; break;}
		}
		//if (!foundSection2 ) continue;
	
		if(foundSection2) {
			strlower(buffer);
			normalise(buffer);
			insertInto(s, buffer, max);
			insertIntoNode(s, buffer, url); 
		}
	}
	foundSection2 += foundSection2;
	free(urlFile);
	fclose(tempFile);
	//free(tempFile);
	return;
}

static void strlower(char *s)
{
	for ( ; *s != '\0'; s++)
		*s = tolower(*s);
}

static void normalise(char *line) {
	int i, j;
	for(i=0; line[i]!='\0'; ++i) {
		while (!( ((line[i]>='a')&&(line[i]<='z')) || ((line[i]>='A') && (line[i]<='Z')) || (line[i]=='\0'))) {
		    	for(j=i;line[j]!='\0';++j)
		  		line[j]=line[j+1];
		    	line[j]='\0';
		}
	}
}
