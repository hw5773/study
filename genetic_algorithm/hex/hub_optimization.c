#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>

int	init_permutation(int *, int edge[][SIZE+1]);
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

	init_permutation(permutation, edge);

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

	free_chromosome(offsprings[n]);
	offsprings[n] = c;

	return 1;
}

int	init_permutation(int *permutation, int edge[][SIZE+1])
{
	int i, j, k;
	int weight[SIZE+1];

	for (i=1; i<=SIZE; i++)
	{
		permutation[i] = 0;
		weight[i] = 0;
		for (j=1; j<=SIZE; j++)
			weight[i] += edge[i][j];
	}

	for (i=1; i<=SIZE; i++)
	{
		permutation[i] = 0;
		for (j=1; j<=i; j++)
		{
			if (permutation[j] == 0)
				permutation[j] = i;
			else if (weight[permutation[j]] < weight[i])
			{
				for (k=i-1; k>=j; k--)
					permutation[k+1] = permutation[k];
				permutation[j] = i;
				break;
			}
		}
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
