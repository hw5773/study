COMPILER=gcc
OUT=maxcut
HEADER=genetic.h
SRC=maxcut.c \
	binary_chromosome.c \
	tornament_selection.c \
	reverse_crossover.c \
	cost.c \
	general_mutation.c \
	general_replacement.c \
	rate_stop_condition.c \
	local_optimization.c \
	common.c
OBJ=maxcut.o \
	binary_chromosome.o \
	tornament_selection.o \
	reverse_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	local_optimization.o \
	common.o

all: $(OBJ) $(HEADER)
	$(COMPILER) -Wall -g -o $(OUT) $(OBJ)

.c.o:
	$(COMPILER) -g -c $<

clean:
	rm -rf $(OUT) $(OBJ)

maxcut.o: maxcut.c genetic.h
cost.o: cost.c
common.o: common.c
binary_chromosome.o: binary_chromosome.c genetic.h
tornament_selection.o: tornament_selection.c
reverse_crossover.o: reverse_crossover.c
uniform_crossover.o: uniform_crossover.c
general_mutation.o: general_mutation.c
general_replacement.o: general_replacement.c
rate_stop_condition.o: rate_stop_condition.c
local_optimization.o: local_optimization.c
