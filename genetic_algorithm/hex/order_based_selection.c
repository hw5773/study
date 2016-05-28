#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int fitness[N+1];	// The array of the fitness

int calc_fitness();	// Calculating the fitness

int selection(int *p1, int *p2)
{
	int point, sum, sum_of_fitness, i, j;
	int num[2];
	unsigned long seed = get_nano_seconds();
	srand(seed);

	sum_of_fitness = calc_fitness();

	for (i=0; i<2; i++)
	{
		point = rand() % sum_of_fitness;
		sum = 0;
		
		for (j=1; j<=N; j++)
		{
			sum += fitness[j];
			if (point < sum)
			{
				num[i] = j;
				break;
			}
		}
	}

	*p1 = num[0];
	*p2 = num[1];			

	return 1;
}

int calc_fitness()
{
	int max_cost = 0, min_cost = INT_MAX, i=0, sum_of_fitness=0;

	for (i=1; i<=N; i++)
	{
		if (population[i]->cost > max_cost)
			max_cost = population[i]->cost;

		if (population[i]->cost < min_cost)
			min_cost = population[i]->cost;
	}

	for (i=1; i<=N; i++)
	{
		fitness[i] = max_cost + (i-1)*(min_cost - max_cost)/(N-1);
		sum_of_fitness += fitness[i];
	}

	return sum_of_fitness;
}
