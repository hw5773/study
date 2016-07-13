#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	init_chromosome(&offsprings[i]);

	unsigned long seed = get_nano_seconds();
	int op = 0, ssize = 0, bsize = 0;
	srand(seed);
	
	if (population[p1]->size < population[p2]->size)
	{
		ssize = population[p1]->size;
		bsize = population[p2]->size;
	}
	else
	{
		ssize = population[p2]->size;
		bsize = population[p1]->size;
	}

	op = (rand()%ssize) + 1;

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	offsprings[i]->size = bsize;

	int n=0;
	for(n=1; n<=SIZE; n++)
	{
		if (n<=b)
			memcpy(&offsprings[i]->ch[n], &population[p1]->ch[n], sizeof(unsigned char));
		else
			memcpy(&offsprings[i]->ch[n], &population[p2]->ch[n], sizeof(unsigned char));
	}

	return 1;
}
