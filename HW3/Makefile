CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: output

output: main.o file.o account.o validate.o convert.o
	${CC} main.o file.o account.o validate.o convert.o -o output

main.o: main.c
	${CC} ${CFLAGS} main.c

file.o: file.c
	${CC} ${CFLAGS} file.c

account.o: account.c
	${CC} ${CFLAGS} account.c

validate.o: validate.c
	${CC} ${CFLAGS} validate.c

convert.o: convert.c
	${CC} ${CFLAGS} convert.c

clean:
	rm -f *.o *~