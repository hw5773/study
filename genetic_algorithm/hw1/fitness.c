#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_fitness()
{
	int i=0;
	for (i=0; i<=N; i++)
		fitness[i] = 0;

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

	print_chromosome(c);
	printf("fitness: %d\n", result);

	return result;
}

int stop_condition(int edge[][SIZE+1])
{
	int i=0, max_val=0, count=0;
	double rate = 0.0;

	for (i=1; i<=SIZE; i++)
		fitness[i] = calc_fitness(population[i], edge);

	for (i=1; i<=SIZE; i++)
	{
		if (fitness[i] > max_val)
		{
			max_val = fitness[i];
			count = 1;
		}
		else if (fitness[i] == max_val)
			count++;
		else
			continue;
	}

	rate = ((double)count) / SIZE;

	printf("rate: %lf\n", rate);
	
	if (rate >= S_RATE)
		return 1;
	else
		return 0;
}	
