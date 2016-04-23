COMPILER=gcc
OUT=maxcut
HEADER=genetic.h
SRC=maxcut.c \
	chromosome.c \
	random_selection.c \
	fixedonepoint_crossover.c \
	cost.c \
	general_mutation.c \
	general_replacement.c \
	timestamp_stop_condition.c \
	common.c
DEFAULT=maxcut.o \
	chromosome.o \
	random_selection.o \
	fixedonepoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	timestamp_stop_condition.o \
	common.o
RATE=maxcut.o \
	chromosome.o \
	random_selection.o \
	fixedonepoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
ROULETTE=maxcut.o \
	chromosome.o \
	roulette_selection.o \
	fixedonepoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
TORNAMENT=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	fixedonepoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
MULTI=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	fixedmultipoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
MULTI_TIME=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	fixedmultipoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	timestamp_stop_condition.o \
	common.o
UNIFORM=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	uniform_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	timestamp_stop_condition.o \
	common.o
ORDER_UNIFORM=maxcut.o \
	chromosome.o \
	order_based_selection.o \
	uniform_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
ORDER_MULTI=maxcut.o \
	chromosome.o \
	order_based_selection.o \
	fixedmultipoint_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
SET1=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	uniform_crossover.o \
	cost.o \
	general_mutation.o \
	preselection_replacement.o \
	rate_stop_condition.o \
	common.o
SET2=maxcut.o \
	chromosome.o \
	order_based_selection.o \
	uniform_crossover.o \
	cost.o \
	general_mutation.o \
	preselection_variant_replacement.o \
	rate_stop_condition.o \
	common.o

all: $(DEFAULT) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(DEFAULT)

rate: $(RATE) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(RATE)

roulette: $(ROULETTE) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(ROULETTE)

tornament: $(TORNAMENT) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(TORNAMENT)

multi: $(MULTI) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(MULTI)

multi_time: $(MULTI_TIME) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(MULTI_TIME)

uniform: $(UNIFORM) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(UNIFORM)

order_uniform: $(ORDER_UNIFORM) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(ORDER_UNIFORM)

order_multi: $(ORDER_MULTI) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(ORDER_MULTI)

set1: $(SET1) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(SET1)

set2: $(SET2) $(HEADER)
	$(COMPILER) -g -Wall -o $(OUT) $(SET2)

.c.o:
	$(COMPILER) -g -c $<

clean:
	rm -rf $(OUT) $(DEFAULT) $(RATE) $(ROULETTE) $(TORNAMENT)

