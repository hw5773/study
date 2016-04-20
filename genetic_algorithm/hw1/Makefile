COMPILER=gcc
OUT=maxcut
SRC=maxcut.c \
	chromosome.c \
	random_select.c \
	one_point_crossover.c \
	cost.c \
	general_mutation.c \
	general_replacement.c \
	timestamp_stop_condition.c \
	common.c
DEFAULT=maxcut.o \
	chromosome.o \
	random_select.o \
	one_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	timestamp_stop_condition.o \
	common.o
RATE=maxcut.o \
	chromosome.o \
	random_select.o \
	one_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
ROULETTE=maxcut.o \
	chromosome.o \
	roulette_selection.o \
	one_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
TORNAMENT=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	one_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
MULTI=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	multi_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o
MULTI_TIME=maxcut.o \
	chromosome.o \
	tornament_selection.o \
	multi_point_crossover.o \
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
	multi_point_crossover.o \
	cost.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o

all: $(DEFAULT)
	$(COMPILER) -g -Wall -o $(OUT) $(DEFAULT)

rate: $(RATE)
	$(COMPILER) -g -Wall -o $(OUT) $(RATE)

roulette: $(ROULETTE)
	$(COMPILER) -g -Wall -o $(OUT) $(ROULETTE)

tornament: $(TORNAMENT)
	$(COMPILER) -g -Wall -o $(OUT) $(TORNAMENT)

multi: $(MULTI)
	$(COMPILER) -g -Wall -o $(OUT) $(MULTI)

multi_time: $(MULTI_TIME)
	$(COMPILER) -g -Wall -o $(OUT) $(MULTI_TIME)

uniform: $(UNIFORM)
	$(COMPILER) -g -Wall -o $(OUT) $(UNIFORM)

order_uniform: $(ORDER_UNIFORM)
	$(COMPILER) -g -Wall -o $(OUT) $(ORDER_UNIFORM)

order_multi: $(ORDER_MULTI)
	$(COMPILER) -g -Wall -o $(OUT) $(ORDER_MULTI)

.c.o:
	$(COMPILER) -g -c $<

clean:
	rm -rf $(OUT) $(DEFAULT) $(RATE) $(ROULETTE) $(TORNAMENT)

