#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 		num_of_vertex
#define S_RATE		0.7
#define M_THRE		0.015
#define N			100
#define K			10

#define ONE_POINT	3	// the parameter for the one point crossover
#define K_FIT		4	// the order of the fitness btw the best and the worst


FILE 			*in, *out;
extern int 		num_of_vertex;
extern int 		num_of_edge;
extern unsigned long	start_time;

extern FILE		*log_file; //temporal file for the logging.


typedef struct 	chromosome_t
{
	int size;
	unsigned char *ch;
	int cost;
} Chromosome;

Chromosome 		*population[N+1];	// The array of the populations
Chromosome 		*offsprings[K+1];	// The array of the offsprings

// if the stop condition is satisfied then returns 1, if not, returns 0
int 			stop_condition(void); 

// operations in GA
// returns 1 if the selection is done rightly. returns -1 if fails
// p1, p2 are the index of the parents in the population
// i is the index of the offspring in the offsprings
int				selection(int *p1, int *p2); 
int 			crossover(int i, int p1, int p2);
int 			mutation(int i);
int 			replacement(int edge[][SIZE+1]);

// the functions related to the representation.
unsigned long 	get_nano_seconds(void);
int 			init_population(void);
int 			free_population(void);
int 			init_offsprings(void);
int 			free_offsprings(void);
int				init_cost(int edge[][SIZE+1]);
int 			init_chromosome(Chromosome **c);
int 			free_chromosome(Chromosome *c);
int				sort_population(void);
void 			print_chromosome(Chromosome *c);

// the functions used in common.
unsigned long	get_seconds(void);
unsigned long	get_nano_seconds(void);
