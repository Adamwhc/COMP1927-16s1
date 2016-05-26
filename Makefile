CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2 -pg
LIBS=graph.o queue.o set.o readData.o mergeSort.o BSTree.o

all: 	inverted.o pagerank.o searchPagerank.o $(LIBS)
	$(CC) $(CFLAGS) -o pagerank pagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o inverted inverted.o $(LIBS) 
	$(CC) $(CFLAGS) -o searchPagerank searchPagerank.o $(LIBS) 
	mkdir -p bin/ && mv *.o bin/
	
inverted.o : inverted.c
pagerank.o : pagerank.c
searchPagerank.o : searchPagerank.c
BSTree.o : BSTree.c BSTree.h
set.o : set.c set.h
queue.o : queue.c queue.h
graph.o : graph.c graph.h
readData.o: readData.c readData.h
mergeSort.o: mergeSort.c mergeSort.h

clean :
	rm -f pagerank inverted searchPagerank *.o core
