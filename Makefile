CC = gcc
CFLAGS1 = -Wall -g -c
CFLAGS2 = -g

all: deliver server

# Target for the "deliver" executable.
deliver: deliver.o
	${CC} ${CFLAGS2} -o deliver deliver.o

# Target for the "server" executable.
server: server.o
	${CC} ${CFLAGS2} -o server server.o

# Compile the "deliver" source file to an object.
deliver.o: deliver.c
	${CC} ${CFLAGS1} -o deliver.o deliver.c

# Compile the "server" source file to an object.
server.o: server.c
	${CC} ${CFLAGS1} -o server.o server.c

clean:
	rm -f deliver server
