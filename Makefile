# Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g -pg
LIBS=graph.o queue.o set.o


all: 	inverted.o pagerank.o searchPagerank.o searchTfidf.o $(LIBS)
	$(CC) $(CFLAGS) -o pagerank pagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o inverted inverted.o $(LIBS) 
	$(CC) $(CFLAGS) -o searchPagerank searchPagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o searchTfidf searchTfidf.o -lm $(LIBS)
	mkdir -p bin/ && mv *.o bin/
scaledFootrule:	scaledFootrule.o
	$(CC) $(CFLAGS) -o scaledFootrule scaledFootrule.o $(LIBS)
searchTfidf: searchTfidf.o $(LIBS)
	gcc $(CFLAGS) -gdwarf-2 -o searchTfidf searchTfidf.o -lm $(LIBS)
searchPagerank: searchPagerank.o $(LIBS)
	gcc $(CFLAGS) -o searchPagerank searchPagerank.o $(LIBS)
inverted : inverted.o $(LIBS)
	gcc $(CFLAGS) -o inverted inverted.o $(LIBS)
pagerank : pagerank.o $(LIBS)
	gcc $(CFLAGS) -o pagerank pagerank.o $(LIBS)
	mkdir -p bin/ && mv *.o bin/


pagerank.o: pagerank.c
inverted.o: inverted.c
set.o : set.c Header/set.h
queue.o : queue.c Header/queue.h
graph.o : graph.c Header/graph.h


clean :
	rm -f inverted pagerank searchPagerank searchTfidf *.o core
