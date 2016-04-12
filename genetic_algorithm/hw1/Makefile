COMPILER=gcc
OUT=maxcut
SRC=maxcut.c \
	chromosome.c \
	random_select.c \
	one_point_crossover.c \
	fitness.c \
	general_mutation.c \
	general_replacement.c \
	timestamp_stop_condition.c \
	common.c
DEFAULT=maxcut.o \
	chromosome.o \
	random_select.o \
	one_point_crossover.o \
	fitness.o \
	general_mutation.o \
	general_replacement.o \
	timestamp_stop_condition.o \
	common.o
RATE=maxcut.o \
	chromosome.o \
	random_select.o \
	one_point_crossover.o \
	fitness.o \
	general_mutation.o \
	general_replacement.o \
	rate_stop_condition.o \
	common.o


all: $(DEFAULT)
	$(COMPILER) -g -Wall -o $(OUT) $(DEFAULT)

rate: $(RATE)
	$(COMPILER) -g -Wall -o $(OUT) $(RATE)

.c.o:
	$(COMPILER) -g -c $<

clean:
	rm -rf $(OUT) $(DEFAULT) $(RATE)

maxcut.o: maxcut.c
chromosome.o: chromosome.c
mock.o: mock.c
random_select.o: random_select.c
one_point_crossover.o: one_point_crossover.c
fitness.o: fitness.c
general_mutation.o: general_mutation.c
timestamp_stop_condition.o: timestamp_stop_condition.c
common.o: common.c
