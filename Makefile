C_SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

OBJ = ${C_SOURCES:.c=.o}

CFLAGS = -g -Wall -Wextra

CC = gcc

all: hdb

run: all
	./hdb ${params}

hdb: ${OBJ}
	${CC} -o hdb $^ -L/usr/lib

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -fr hdb
	rm -fr src/*.o 

