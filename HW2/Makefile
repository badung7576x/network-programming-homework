CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: resolver

resolver: main.o validate.o convert.o  
	${CC} main.o validate.o convert.o -o resolver

main.o: main.c
	${CC} ${CFLAGS} main.c

convert.o: convert.c
	${CC} ${CFLAGS} convert.c

validate.o: validate.c
	${CC} ${CFLAGS} validate.c

clean:
	rm -f *.o *~