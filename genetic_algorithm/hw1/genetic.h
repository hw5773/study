#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 	16
#define N		50
#define K		10

FILE *in, *out;

typedef struct chromosome_t
{
	int size;
	unsigned char *ch;
} Chromosome;

Chromosome *population[N+1];
Chromosome *offsprings[K+1];

int stop_condition(); // if the stop condition is satisfied then returns 1, if not, returns 0

// operations in GA
// returns 1 if the selection is done rightly. returns -1 if fails
// p1, p2 are the index of the parents in the population
// i is the index of the offspring in the offsprings
int selection(int *p1, int *p2); 
int crossover(int i, int p1, int p2);
int mutation(int i);
int replacement(void);

// the functions related to the representation.
int init_population(void);
int free_population(void);
int init_chromosome(Chromosome **c, int);
int free_chromosome(Chromosome **c);
