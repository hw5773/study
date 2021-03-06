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
int crossover(int i, int p1, int p2, int edge[][SIZE+1])
{
	int n;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	double p = 0.0;

	init_chromosome(&offsprings[i]);

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	if (population[p1]->cost > population[p2]->cost)
	{
		int tmp;
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	for (n=1; n<=SIZE; n++)
	{
		p = ((double) rand()) / RAND_MAX;
		if (p < P0)
			offsprings[i]->ch[n] = population[offsprings[i]->p2]->ch[n];
		else
			offsprings[i]->ch[n] = population[offsprings[i]->p1]->ch[n];
	}

	return 1;
}
