#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int replacement(int edge[][SIZE+1])
{
	int i=0;

	for (i=1; i<=K; i++)
		offsprings[i]->cost = calc_cost(offsprings[i], edge);

	for (i=1; i<=K; i++)
	{
		free(population[offsprings[i]->p1]);
		population[offsprings[i]->p1] = offsprings[i];
	}
	
	return 1;
}
