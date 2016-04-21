#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_cost(int edge[][SIZE+1])
{
	int i=0;
	for (i=1; i<=N; i++)
		population[i]->cost = calc_cost(population[i], edge);

	return 1;
}

int calc_cost(Chromosome *c, int edge[][SIZE+1])
{
	int i, j, result = 0;
	
	for (i=1; i<=SIZE; i++)
	{
		for (j=i; j<=SIZE; j++)
		{
			if (i == j)
				continue;
			else if (c->ch[i] != c->ch[j])
				result = result + edge[i][j];
		}
	}

	return result;
}
