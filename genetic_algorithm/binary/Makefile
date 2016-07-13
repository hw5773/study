COMPILER=gcc
OUT=maxcut
HEADER=genetic.h
SRC=maxcut.c \
	binary_chromosome.c \
	order_based_selection.c \
	normuniform_crossover.c \
	cost.c \
	increase_mutation.c \
	general_replacement.c \
	bunch_stop_condition.c \
	cluster_optimization.c \
	common.c
OBJ=binary_chromosome.o \
	order_based_selection.o \
	normuniform_crossover.o \
	cost.o \
	increase_mutation.o \
	general_replacement.o \
	bunch_stop_condition.o \
	cluster_optimization.o \
	common.o
ORIJINAL=maxcut.c
TEST=maxcut_test.c
LOCAL=maxcut_local.c
LOCALTEST=maxcut_local_test.c

all: $(OBJ) $(HEADER)
	$(COMPILER) -Wall -g -o $(OUT) $(ORIJINAL) $(OBJ)

test: $(OBJ) $(HEADER)
	$(COMPILER) -Wall -g -o maxcut_test $(TEST) $(OBJ)

local: $(OBJ) $(HEADER)
	$(COMPILER) -Wall -g -o $(OUT) $(LOCAL) $(OBJ)

localtest: $(OBJ) $(HEADER)
	$(COMPILER) -Wall -g -o maxcut_local_test $(OBJ) $(LOCALTEST)

.c.o:
	$(COMPILER) -g -c $<

clean:
	rm -rf $(OUT) $(OBJ)

maxcut.o: maxcut.c genetic.h
cost.o: cost.c
common.o: common.c
binary_chromosome.o: binary_chromosome.c genetic.h
tornament_selection.o: tornament_selection.c
pmx_crossover.o: pmx_crossover.c
reverse_crossover.o: reverse_crossover.c
uniform_crossover.o: uniform_crossover.c
decrease_mutation.o: decrease_mutation.c
general_replacement.o: general_replacement.c
rate_stop_condition.o: rate_stop_condition.c
local_optimization.o: local_optimization.c
