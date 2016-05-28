#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define ONE_POINT 3

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	init_chromosome(&offsprings[i]);

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	int n=0;
	for(n=1; n<=SIZE; n++)
	{
		if (n<=ONE_POINT)
		{	
			offsprings[i]->node[n] = population[p1]->node[n];
			offsprings[i]->ch[n] = population[p1]->ch[n];
		}
		else
		{
			offsprings[i]->node[n] = population[p2]->node[n];
			offsprings[i]->ch[n] = population[p2]->ch[n];
		}
	}

	return 1;
}
