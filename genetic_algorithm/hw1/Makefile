COMPILER=gcc
OUT=maxcut
SRC=maxcut.c \
	chromosome.c \
	random_select.c \
	one_point_crossover.c \
	fitness.c \
	general_mutation.c \
	general_replacement.c
OBJ=maxcut.o \
	chromosome.o \
	random_select.o \
	one_point_crossover.o \
	fitness.o \
	general_mutation.o \
	general_replacement.o

all: $(OBJ)
	$(COMPILER) -g -Wall -o $(OUT) $(OBJ)

.c.o:
	$(COMPILER) -c $<

maxcut.o: maxcut.c
chromosome.o: chromosome.c
mock.o: mock.c
random_select.o: random_select.c
one_point_crossover.o: one_point_crossover.c
fitness.o: fitness.c
general_mutation: general_mutation.c
