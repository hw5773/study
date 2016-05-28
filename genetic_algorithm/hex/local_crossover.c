#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define ONE_POINT point

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2, int edge[][SIZE+1])
{
	int n=0;
	double t1, t2, point;
	unsigned long seed = get_nano_seconds();
	int permutation[SIZE+1];

	srand(seed);
	t1 = (double)rand() / RAND_MAX;
	t2 = (double)rand() / RAND_MAX;
	point = rand() % SIZE + 1;

	init_chromosome(&offsprings[i]);

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	Chromosome *c1, *c2;
	init_chromosome(&c1);
	init_chromosome(&c2);

	init_permutation(permutation);

	int improved1 = 1, improved2 = 1;

	for (n=0; i<=SIZE; i++)
	{
		c1->ch[i] = population[p1]->ch[i];
		c2->ch[i] = population[p2]->ch[i];
	}

	c1->cost = calc_cost(c1, edge);
	c2->cost = calc_cost(c2, edge);

	int cost1 = c1->cost;
	int cost2 = c2->cost;

	while (improved1 || improved2)
	{
		improved1	=	0;
		improved2	=	0;

		for (i=1; i<=SIZE; i++)
		{
			int tmp	=	0;
			if (permutation[i] < SIZE/2)
			{
				c1->ch[permutation[i]] = (1 - c1->ch[permutation[i]]) % 2;
				tmp = calc_cost(c1, edge);

				if (tmp > cost1)
				{
					improved1 = 1;
					cost1 = tmp;
					c1->cost = tmp;
				}
			
				else
					c1->ch[permutation[i]] = (1 - c1->ch[permutation[i]]) % 2;

			}
			else
			{
				c2->ch[permutation[i]] = (1 - c2->ch[permutation[i]]) % 2;
				tmp = calc_cost(c2, edge);

				if (tmp > cost2)
				{
					improved2 = 1;
					cost2 = tmp;
					c2->cost = tmp;
				}
				else
					c2->ch[permutation[i]] = (1 - c2->ch[permutation[i]]) % 2;
			}
		}
	}

	for(n=1; n<=SIZE; n++)
	{
		if (n<=ONE_POINT)
			offsprings[i]->ch[n] = c1->ch[n];
		else
			offsprings[i]->ch[n] = c2->ch[n];
	}

	offsprings[i]->cost = calc_cost(offsprings[i], edge);

	return 1;
}


