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
	int cost			=	(offsprings[n]->cost = calc_cost(offsprings[n], edge));
	int permutation[SIZE+1];
	int pair[SIZE+1];

	Chromosome *c[4];
	for (i=0; i<4; i++)
	{
		init_chromosome(&(c[i]), edge);
		for (j=1; j<=SIZE; j++)
			c[i]->ch[j] = offsprings[n]->ch[j];
	}

	int num = 0, idx = -1;

	init_permutation(permutation);
//	init_permutation(pair);
	for (i=1; i<=SIZE; i++)
		pair[SIZE+1-i] = permutation[i];

	while (improved)
	{
		improved	=	0;
		idx			=	-1;	

		for (i=1; i<=PAIRS; i++)
		{
			int tmp	=	0;
			// 0
			c[0]->ch[permutation[i]] = 0;
			c[0]->ch[pair[i]] = 0;
//			tmp = calc_cost(c[0], edge);
//			printf("c[%d] = %d\n", 0, tmp);

			// 1
			c[1]->ch[permutation[i]] = 0;
			c[1]->ch[pair[i]] = 1;
//			tmp = calc_cost(c[1], edge);
//			printf("c[%d] = %d\n", 1, tmp);
			

			// 2
			c[2]->ch[permutation[i]] = 1;
			c[2]->ch[pair[i]] = 0;
//			tmp = calc_cost(c[2], edge);
//			printf("c[%d] = %d\n", 2, tmp);

			// 3
			c[3]->ch[permutation[i]] = 1;
			c[3]->ch[pair[i]] = 1;
//			tmp = calc_cost(c[3], edge);
//			printf("c[%d] = %d\n", 3, tmp);

			for (j=0; j<4; j++)
			{
				tmp = calc_cost(c[j], edge);

				if (tmp > cost)
				{
//					printf("tmp: %d\n", tmp);
//					printf("cost: %d\n", cost);
					improved = 1;
					cost = tmp;
					idx = j;
				}
			}

			if (idx > 0)
			{
				offsprings[n]->ch[permutation[i]] = c[idx]->ch[permutation[i]];
				offsprings[n]->ch[pair[i]] = c[idx]->ch[pair[i]];
				cost = calc_cost(offsprings[n], edge);
			}
			for (j=0; j<4; j++)
			{
				for (k=1; k<=SIZE; k++)
				{
					c[j]->ch[k] = offsprings[n]->ch[k];
				}
			}
		}
//		printf("improved: %d\n", improved);
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
