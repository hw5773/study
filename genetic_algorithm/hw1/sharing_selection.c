#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <math.h>

int fitness[N+1];	// The array of the fitness
double sfitness[N+1];	// sharing fitness

double calc_fitness();	// Calculating the fitness

int selection(int *p1, int *p2)
{
	int i, j, num[2], point;
	double sum, sum_of_fitness;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	sum_of_fitness = calc_fitness();

	for (i=0; i<2; i++)
	{
		point = rand() % (int)sum_of_fitness;
		sum = 0;
		
		for (j=1; j<=N; j++)
		{
			sum += sfitness[j];
			if ((double)point < sum)
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

double calc_fitness()
{
	int max_cost = 0, min_cost = INT_MAX, i=0, j=0, sigma=0;
	double sum_of_fitness=0.0, sharing = 0.0;

	for (i=1; i<=N; i++)
	{
		if (population[i]->cost > max_cost)
			max_cost = population[i]->cost;

		if (population[i]->cost < min_cost)
			min_cost = population[i]->cost;
	}

	for (i=1; i<=N; i++)
		fitness[i] = max_cost + (i-1)*(min_cost - max_cost)/(N-1);

	sigma = max_cost - min_cost;

	for (i=1; i<=N; i++)
	{
		for (j=1; j<=N; j++)
		{
			if (i==j) continue;
			sharing += (1 - fabs(((double)(population[i]->cost - population[j]->cost))/sigma));
		}
		sfitness[i] = (double)fitness[i] / sharing;
		sum_of_fitness += sfitness[i];
	}

	return sum_of_fitness;
}
