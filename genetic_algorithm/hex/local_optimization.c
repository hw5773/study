#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>

int	init_permutation(int *);
int	swap(int *, int, int);

int local_optimization(int n, int edge[][SIZE+1])
{
	int i, j, k;
	int	improved		=	1;
	int	modify			=	0;
	int permutation[SIZE+1];

	Chromosome *c;
	init_chromosome(&c);
	
	for (i=0; i<=SIZE; i++)
	{
		c->node[i] = offsprings[n]->node[i];
		c->ch[i] = offsprings[n]->ch[i];
	}

	calc_cost(c, edge);

	int num = 0;

	init_permutation(permutation);

	while (improved)
	{
		improved	=	0;

		for (i=1; i<=SIZE; i++)
		{
			c->ch[permutation[i]] = (1 - c->ch[permutation[i]]) % 2;
			
			calc_cost(c, edge);

			if (c->cost > offsprings[n]->cost)
			{
				offsprings[n]->ch[permutation[i]] = c->ch[permutation[i]];
				improved = 1;
				calc_cost(offsprings[n], edge);
			}
			else
				c->ch[permutation[i]] = (1 - c->ch[permutation[i]]) % 2;
		}
	}

	return 1;
}

int	init_permutation(int *permutation)
{
	int	i, loc = 0;
	unsigned long	seed	=	get_nano_seconds();
	srand(seed);

	for (i=0; i<=SIZE; i++)
		permutation[i] = i;

	for (i=SIZE; i>=2; i--)
	{
		loc = rand()%i + 1;
		if (i != loc)
			swap(permutation, i, loc);
	}

	return 1;
}

int swap(int *permutation, int a, int b)
{
	int tmp = permutation[a];
	permutation[a] = permutation[b];
	permutation[b] = tmp;

	return 1;
}
