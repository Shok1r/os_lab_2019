CC=gcc
CFLAGS=-w -c

all: thread_sum

thread_sum: utils.o threadfun.o parallel_sum.o
	$(CC) utils.o threadfun.o parallel_sum.o -o threads -lpthread

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c -o utils.o

threadfun.o: threadfun.c
	$(CC) $(CFLAGS) threadfun.c -o threadfun.o

parralel_sum.o: parralel_sum.c
	$(CC) $(CFLAGS) parallel_sum.c -o parallel_sum.o

clean:
	rm *.o threads
