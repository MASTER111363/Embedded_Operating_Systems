CC=gcc
CFLAGS=-Wall
ING_FLAGS=-lpthread

all: server.o client.o 
	${CC} ${CFLAGS} -c server.c
	${CC} ${CFLAGS} -c client.c
	${CC} client.o ${CFLAGS} ${ING_FLAGS} -o client
	${CC} server.o ${CFLAGS} ${ING_FLAGS} -o server

clean: 
	${RM} server client server.o client.o 
