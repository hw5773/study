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
	int n, cost1, cost2;
	Chromosome *t1, *t2;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	double p = 0.0;

	init_chromosome(&offsprings[i], edge);
	init_chromosome(&t1, edge);
	init_chromosome(&t2, edge);
	

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
		{
			t1->ch[n] = population[offsprings[i]->p2]->ch[n];
			t2->ch[n] = population[offsprings[i]->p2]->ch[n] % 2;
		}
		else
		{
			t1->ch[n] = population[offsprings[i]->p1]->ch[n];
			t2->ch[n] = 1 - population[offsprings[i]->p1]->ch[n] % 2;
		}
	}

	cost1 = calc_cost(t1, edge);
	cost2 = calc_cost(t2, edge);

	if (cost1 < cost2)
		offsprings[i]->ch = t2->ch;
	else
		offsprings[i]->ch = t1->ch;

	return 1;
}
