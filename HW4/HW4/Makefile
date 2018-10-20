#Sample Makefile

CFLAGS=-g -O0 -Wall 

all:	check

default: check
	

clean: 
	rm -rf *.o libmalloc.so

test1.o: t-test1.c
	gcc ${CFLAGS} -c -fpic -o test1.o t-test1.c

test1:	test1.o
	gcc ${CFLAGS} -g -o test1 test1.o -lpthread

linkedlist.o: linkedlist.c linkedlist.h
	gcc ${CFLAGS} -c -fpic -o linkedlist.o linkedlist.c

malloc.o: malloc.c linkedlist.h malloc.h
	gcc ${CFLAGS} -c -fpic -o malloc.o malloc.c

calloc.o: calloc.c malloc.h
	gcc ${CFLAGS} -c -fpic -o calloc.o calloc.c

free.o: free.c malloc.h
	gcc ${CFLAGS} -c -fpic -o free.o free.c

realloc.o: realloc.c malloc.h
	gcc ${CFLAGS} -c -fpic -o realloc.o realloc.c

libmalloc.so: malloc.o calloc.o realloc.o free.o linkedlist.o
	gcc ${CFLAGS} -shared -o libmalloc.so malloc.o calloc.o realloc.o free.o linkedlist.o

check:	libmalloc.so test1 
	LD_PRELOAD=`pwd`/libmalloc.so ./test1

res: 	test1
	./test1

dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar
