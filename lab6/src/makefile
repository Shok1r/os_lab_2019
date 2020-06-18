CC=gcc

server : server.c 
	$(CC) server.c -c
	$(CC) server.o -L. -lmult_modulo -lpthread -o server

client : client.c mult_modulo.o
	$(CC) client.c -c
	$(CC) client.o -L. -lmult_modulo -lpthread -o client

mult_modulo.o : mult_modulo.c
	$(CC) mult_modulo.c -c

libmult_modulo.a : mult_modulo.o
	ar rc libmult_modulo.a mult_modulo.o
	ranlib libmult_modulo.a

all : libmult_modulo.a client server

clean : 
	rm server
	rm client
	rm client.o 
	rm server.o
	rm libmult_modulo.a
	rm mult_modulo.o
