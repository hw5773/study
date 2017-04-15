GCC=gcc
SRC=sync_func.c hw3.c
OBJ=sync sync_func.o sync_main.o hw3.o
LDFLAGS=-lpthread

all: sync_func.o sync_main.o hw3.o
	${GCC} -o ${OBJ} ${LDFLAGS}

sync_func.o: sync_func.c increment.h
	${GCC} -c sync_func.c

sync_main.o: sync_main.c increment.h
	${GCC} -c sync_main.c

hw3.o: hw3.c increment.h
	${GCC} -c hw3.c

clean:
	rm ${OBJ}
