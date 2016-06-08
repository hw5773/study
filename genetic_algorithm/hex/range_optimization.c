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
	int cost;
	int idx				=	-1;
	unsigned long seed	=	get_nano_seconds();
	srand(seed);

	int r				=	rand() % SIZE + 1;
	Chromosome *tmp;
	init_chromosome(&tmp, edge);

	for (i=0; i<=SIZE; i++)
	{
		tmp->node[i] = offsprings[n]->node[i];
		tmp->ch[i] = offsprings[n]->ch[i];
	}

	while (improved)
	{
		improved = 0;
		cost = calc_cost(offsprings[n], edge);
		for (i=0; i<RANGE; i++)
		{
			tmp->ch[((r+i) % SIZE) + 1] = (1 - (tmp->ch[((r+i) % SIZE) + 1])) % 2;
			calc_cost(tmp, edge);

			if (tmp->cost > cost)
			{
				cost = tmp->cost;
				idx = i;
				improved = 1;
			}
			
			tmp->ch[((r+i) % SIZE) + 1] = (1 - (tmp->ch[((r+i) % SIZE) + 1])) % 2;
		}

		if (improved)
		{
			offsprings[n]->ch[((r+idx) % SIZE) + 1] = (1 - (offsprings[n]->ch[((r+idx) % SIZE) + 1])) % 2;
			tmp->ch[((r+idx) % SIZE) + 1] = (1 - (tmp->ch[((r+idx) % SIZE) + 1])) % 2;
		}
	}

	free_chromosome(tmp);

	return 1;
}
