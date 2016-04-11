COMPILER=gcc
OUT=maxcut
SRC=maxcut.c \
	chromosome.c
OBJ=maxcut.o \
	chromosome.o

all: maxcut.o chromosome.o
	$(COMPILER) -o $(OUT) $(OBJ)

.c.o:
	$(COMPILER) -c $<

maxcut.o: maxcut.c
chromosome.o: chromosome.c
