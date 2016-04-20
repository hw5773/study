#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_crossover(int *point[])
{
	int i, j, complete=0;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	for (i=0; i<POINTS; i++)
	{
		do {
			*point[i] = rand()%SIZE;
			complete = 1;
			for (j=0; j<i; j++)
			{
				if (*point[j] == *point[i])
					complete = 0;
			}
		} while (!complete);
	}

	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	init_chromosome(&offsprings[i]);

	int n=0, m=0;
	for (n=1; n<=SIZE; n++)
	{
		for (m=0; m<=POINTS; m++)
		{
			// TODO: 
		}
	}

	return 1;
}
