#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>

unsigned long get_nano_seconds()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return 1000000000*(ts.tv_sec) + ts.tv_nsec;
}

int init_chromosome(Chromosome **c)
{
	*c = (Chromosome *)malloc(sizeof(Chromosome));
	(*c)->ch = (unsigned char *)malloc(SIZE);

	int i=0;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	for (i=0; i<SIZE; i++) // zero base
		(*c)->ch[i] = rand()%2;

	return 1;
}

int free_chromosome(Chromosome *c)
{
	free(c->ch);
	free(c);
}

int init_population(void)
{
	int i=1, j=0;

	for (i=1; i<=N; i++) // first chromosome is in the index 1.
	{
		init_chromosome(&population[i]);

		printf("%d: ", i);
		print_chromosome(population[i]);
	}

	return 1;
}

int free_population(void)
{
	int i=1;

	for (i=1; i<=N; i++)
	{
		free(population[i]->ch);
		free(population[i]);
	}

	return 1;
}

int init_offsprings(void)
{
	int i=1;
	
	for (i=1; i<=K; i++)
		init_chromosome(&offsprings[i]);

	return 1;
}

int free_offsprings(void)
{
	int i=1;

	for (i=1; i<=K; i++)
		free_chromosome(offsprings[i]);

	return 1;
}

void print_chromosome(Chromosome *c)
{
	int i;

	for (i=0; i<SIZE; i++)
		printf("%x", c->ch[i]);

	printf("\n");
}
