#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>
/*
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
*/
int preprocessing(int edge[][SIZE+1])
{
	int i, j, k;
	int weight[SIZE+1];
	int hub[SIZE+1];

	for (i=1; i<=SIZE; i++)
	{
		hub[i] = 0;
		weight[i] = 0;
		for (j=1; j<=SIZE; j++)
			weight[i] += edge[i][j];
	}

	for (i=1; i<=SIZE; i++)
	{
		hub[i] = 0;
		for (j=1; j<=i; j++)
		{
			if (hub[j] == 0)
				hub[j] = i;
			else if (weight[hub[j]] < weight[i])
			{
				for (k=i-1; k>=j; k--)
					hub[k+1] = hub[k];
				hub[j] = i;
				break;
			}
		}

		printf("hub %d: \n", i);
		for (j=1; j<=SIZE; j++)
			printf("%d ", hub[j]);
		printf("\n");
	}

	printf("weight: \n");
	for (i=1; i<=SIZE; i++)
		printf("%d ", weight[i]);
	printf("\n");

	printf("hub: \n");
	for (i=1; i<=SIZE; i++)
		printf("%d ", hub[i]);
	printf("\n");

	return 1;
}
