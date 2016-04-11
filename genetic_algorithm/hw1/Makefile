COMPILER=gcc
OUT=maxcut
SRC=maxcut.c \
	chromosome.c \
	mock.c
OBJ=maxcut.o \
	chromosome.o \
	mock.o

all: $(OBJ)
	$(COMPILER) -g -Wall -o $(OUT) $(OBJ)

.c.o:
	$(COMPILER) -c $<

maxcut.o: maxcut.c
chromosome.o: chromosome.c
mock.o: mock.c
