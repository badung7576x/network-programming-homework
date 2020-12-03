CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: output

output: main.o file.o account.o
	${CC} main.o file.o account.o -o output

main.o: main.c
	${CC} ${CFLAGS} main.c

file.o: file.c
	${CC} ${CFLAGS} file.c

account.o: account.c
	${CC} ${CFLAGS} account.c

clean:
	rm -f *.o *~