/*
   searchPagerank.c

   Written by Glenn & Nicholas

   This file is for Part 1-C of the COMP1927 assignment.
   Given a list of keywords, outputs the top ten pages that contain
   these keywords in order of pagerank.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergeSort.h"
#include "readData.h"
#include "graph.h"
#include "queue.h"

/*function pointer that checks if a certain string
  was provided in the program arguments*/
int isInArgs(int , char **, char *);

//main function to do all the stuff
int main(int argc, char *argv[]){
    if(argc < 2){  //makes sure there is atleast one keyword
	printf("Usage: %s [space separated search terms]\n", argv[0]);
	exit(0);
    }

    char *buff = NULL;  //used to read in
    char *pch;	 //used for strtok to take components
    size_t len = 0;//used to record length of string

    Set allS = newSet(); //set containing all unique pages (that contain the keywords)
    Set finalS = newSet(); //set containing the pages that appear with ALL keywords
    Set s[argc-1]; //an array of sets for each keyword
    int k, j, count=0, nSets=0;

    FILE *fp;
    if((fp = fopen("invertedIndex.txt", "r")) != NULL){ 
	//open the file and ensure there was no issues
	while (getline(&buff, &len, fp) != -1) {  //keep going until file done
	    pch = strtok (buff, " \n");		  //grab the keyword
	    if (isInArgs(argc, argv, pch)){	  //if it something we're looking for
		s[nSets] = newSet();		  //give it a set
		pch = strtok (NULL, " \n");	  //grab the first page name
		while (pch != NULL)		  //and while it isn't null...
		{
		    insertInto(s[nSets], cleanString(pch), 0);//add to it's own set
		    insertInto(allS, cleanString(pch), 0);    //and add to the overall set
		    pch = strtok (NULL, " \n");		      //grab next page name on line
		}
		nSets++; //increment count of sets
	    }
	}
	//if(buff) {
	//   free(buff);
	//}
	fclose(fp);   //close the file
    } else {
	fprintf(stderr,  "Error: invertedIndex.txt NOT FOUND\n");
    }

    //go through and find duplicates:
    if (nSets > 1){  //we had more than one keyword
	char **allElems = getElements(allS);  //get list of elements
	for(k=0; k<nElems(allS); k++){	      //for each of the elements
	    count = 0;
	    buff = allElems[k];		//get next string
	    for(j=0; j<nSets; j++){	//for each of our keyword sets
		if(isElem(s[j], buff))  //if the keword set has the string in it 
		    count++;            //increment number of times we have seen the string
	    }
	    if (count == nSets)		//if the string exists in all sets..
		insertInto(finalS, buff, 0);  //add it to the final set
	}
    } else if (nSets == 1){  //we only had one keyword
	disposeSet(finalS);  //dispose final set, as we only have one set
	finalS = allS;       //redefine pointer for generic use later on..
	allS = NULL;	     //kill pointer of old set
    } else		     //end the program, we didn't find any keywords
    {
	disposeSet(finalS); //free all the memory
	disposeSet(allS);
	fprintf(stderr,  "Error: NO MATCHING KEYWORDS\n");
	exit(0);
    }

    /*now go through the page rank list, and print page names
      from our final state set when we see them, thus printing
      in page rank order!*/
    if ((fp=fopen("pagerankList.txt", "r")) != NULL){
	while (getline(&buff, &len, fp) != -1) {   //while there are still lines to get..
	    pch = strtok (buff, " ,\n");	   //grab the pagerank page name
	    if (isElem(finalS, cleanString(pch))){ //if it was one we are looking for..
		printf("%s\n", pch);		   //print to stdout...
		if(++count == 10) break;	   //maximum of ten items
	    }
	}
    } else {
	fprintf(stderr,  "Error: pagerankList.txt NOT FOUND\n");
    }

    disposeSet(finalS);
    //if(allS != NULL) disposeSet(allS);
    for(k=0; k<nSets; k++)
	disposeSet(s[k]);

    return 1;

}

/*function to check if the keyword we have is something we are
  actually looking for.. if it is we return 1, otherwise 0*/
int isInArgs(int argc, char **argv, char *str){
    int i;
    for (i = 1; i < argc; i++){
	if(strcmp(argv[i], str) == 0)
	    return 1;
    }
    return 0;
}


