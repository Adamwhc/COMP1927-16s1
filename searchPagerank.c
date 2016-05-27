#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergeSort.h"
#include "readData.h"
#include "graph.h"
#include "queue.h"

int isInArgs(int , char **, char *);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s [space separated search terms]\n", argv[0]);
        exit(0);
    }

    char *buff;
    char *pch;
    size_t len = 0;
    Set allS = newSet();
    Set finalS = newSet();
    Set *s = malloc(sizeof(Set) * (argc-1));
    int k, j, count=0, nSets=0;
    
    FILE *fp = fopen("invertedIndex.txt", "r");
    
    if (fp != NULL){
   	while (getline(&buff, &len, fp) != -1) {
       	 	pch = strtok (buff, " \n");
       	 	if (isInArgs(argc, argv, pch)){
       	 		s[nSets] = newSet();
       	 		pch = strtok (NULL, " \n");
  			while (pch != NULL)
 			{
 				printf("FOUND: '%s'", pch);
 				insertInto(s[nSets], cleanString(pch), 0);
 				insertInto(allS, cleanString(pch), 0);
    				pch = strtok (NULL, " \n");
  			}
  			nSets++;
 		}

    	} 
    } else {
        fprintf(stderr,  "Error: invertedIndex.txt NOT FOUND\n");
        exit(0);
    }
    
printf("\n nsets %d\n", nSets);

    //go through and find duplicates
    if (nSets > 1){
	for(k=0; k<nElems(allS); k++){
		count = 0;
		buff = cleanString(indexElement(allS, k));
		for(j=0; j<nSets; j++){
			if(isElem(s[j], buff))
				count++;
		}
		if (count == nSets)
			insertInto(finalS, buff, 0);
		//free(buff);
	}
    } else if (nSets == 1){
    	disposeSet(finalS);
    	finalS = allS;
    	allS = NULL;
    } else
    {
    	disposeSet(finalS);
    	disposeSet(allS);
    	free(s);
    	free(buff);
   	fprintf(stderr,  "Error: NO MATCHING KEYWORDS\n");
    	exit(0);
    }
    
    

	showSet(finalS, stdout);
	
	
	
    if ((fp=fopen("pagerankList.txt", "r"))){
   	while (getline(&buff, &len, fp) != -1) {
			//printf("before strtok\n");
       	 	pch = strtok (buff, " ,\n");
       	 	printf("PRE '%s'\n", cleanString(pch));
       	 	if (isElem(finalS, cleanString(pch))){
       	 		printf("%s\n", pch);
       	 		if(++count == 10) break;
			}
			//if(strcmp(cleanString(pch), "url34") == 0) break;
    	}
    } else {
        fprintf(stderr,  "Error: pagerankList.txt NOT FOUND\n");
        exit(0);
    }

	disposeSet(finalS);
	if(allS != NULL) disposeSet(allS);
	for(k=0; k<nSets; k++)
		disposeSet(s[k]);
	free(s);

    return 1;
    
}

int isInArgs(int argc, char **argv, char *str){
	int i;
	for (i = 1; i < argc; i++){
	if(strcmp(argv[i], str) == 0)
		return 1;
	}
	return 0;
}


