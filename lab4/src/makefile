CC=gcc
CFLAGS=-I.

all : process_memory

process_memory : process_memory.o
	$(CC) -o process_memory process_memory.o $(CFLAGS)

process_memory.o : process_memory.c
	$(CC) -o process_memory.o -c process_memory.c $(CFLAGS)

clean :
	rm process_memory.o process_memory
