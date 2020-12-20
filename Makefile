CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 
PTHREAD =  -g3 -pthread

CLIENT      :=  client
SERVER      :=  server
COMMON		:= common

all: client server

client: cli.o common.o clim.o
	${CC} ${PTHREAD} cli.o common.o clim.o -o bincli

server: ser.o common.o serm.o
	${CC} ${PTHREAD} ser.o common.o serm.o -o binser


# make file server
serm.o: ${SERVER}/main.c
	${CC} ${CFLAGS} ${SERVER}/main.c -o serm.o

ser.o: ${SERVER}/server.c
	${CC} ${CFLAGS} ${SERVER}/server.c -o ser.o

# make file client
clim.o: ${CLIENT}/main.c
	${CC} ${CFLAGS} ${CLIENT}/main.c -o clim.o
cli.o: ${CLIENT}/client.c
	${CC} ${CFLAGS} ${CLIENT}/client.c -o cli.o

# make file common
common.o: ${COMMON}/common.c
	${CC} ${CFLAGS} ${COMMON}/common.c -o common.o

clean:
	rm -f *.o *~