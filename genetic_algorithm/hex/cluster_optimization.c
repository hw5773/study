#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int local_optimization(int n, int edge[][SIZE+1])
{
	int i, j, k;
	int loop			=	0;
	int	improved		=	1;
	int	modify			=	0;
	int cost, num;
	int idx				=	-1;
	unsigned long seed	=	get_nano_seconds();
	srand(seed);

	Chromosome *tmp;
	init_chromosome(&tmp, edge);

	for (i=0; i<=SIZE; i++)
	{
		tmp->node[i] = offsprings[n]->node[i];
		tmp->ch[i] = offsprings[n]->ch[i];
	}

	for (num=0; num<HUBS; num++)
	{

		int r				=	rand() % N + 1;
		int neighbors[SIZE+1];
		int n_count			=	0;

		for (i=1; i<= SIZE; i++)
		{
			if (edge[r][i] != 0)
				neighbors[++n_count] = i;
		}
		neighbors[0] = r;

		while (improved)
		{
			improved = 0;
			cost = calc_cost(offsprings[n], edge);
			for (i=0; i<= n_count; i++)
			{
				tmp->ch[neighbors[i]] = (1 - (tmp->ch[neighbors[i]])) % 2;
				calc_cost(tmp, edge);

				if (tmp->cost > cost)
				{
					cost = tmp->cost;
					idx = i;
					improved = 1;
				}
			
				tmp->ch[neighbors[i]] = (1 - (tmp->ch[neighbors[i]])) % 2;
			}

			if (improved)
			{
				offsprings[n]->ch[neighbors[idx]] = (1 - (offsprings[n]->ch[neighbors[idx]])) % 2;
				tmp->ch[neighbors[idx]] = (1 - (tmp->ch[neighbors[idx]])) % 2;
			}
		}
	}

	free_chromosome(tmp);

	return 1;
}
