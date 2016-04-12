#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_fitness(int edge[][SIZE+1])
{
	printf("init_fitness\n");
	int i=0;
	for (i=1; i<=N; i++)
	{
		population[i]->fitness = calc_fitness(population[i], edge);
		print_chromosome(population[i]);
	}

	printf("\n");

	return 1;
}

int calc_fitness(Chromosome *c, int edge[][SIZE+1])
{
	int i, j, result = 0;
	
	for (i=0; i<SIZE; i++)
	{
		for (j=0; j<SIZE; j++)
		{
			if (i == j)
				continue;
			else if (c->ch[i] != c->ch[j])
				result = result + edge[i][j];
		}
	}

	return result;
}
