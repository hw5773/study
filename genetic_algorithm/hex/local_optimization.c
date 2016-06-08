#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>

int local_optimization(int n, int edge[][SIZE+1])
{
	int i, j, k;
	int	improved		=	1;
	int	modify			=	0;
	int permutation[SIZE+1];

	Chromosome *c;
	init_chromosome(&c, edge);
	
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


