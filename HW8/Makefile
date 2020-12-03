CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client server

client: client.o file.o account.o
	${CC} client.o file.o account.o -o client

server: server.o file.o account.o
	${CC} server.o file.o account.o -o server

server.o: server.c
	${CC} ${CFLAGS} server.c

client.o: client.c
	${CC} ${CFLAGS} client.c

file.o: file.c
	${CC} ${CFLAGS} file.c

account.o: account.c
	${CC} ${CFLAGS} account.c

clean:
	rm -f *.o *~