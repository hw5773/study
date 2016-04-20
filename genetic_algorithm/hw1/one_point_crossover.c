#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define ONE_POINT 3

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	init_chromosome(&offsprings[i]);

	int n=0;
	for(n=1; n<=SIZE; n++)
	{
		if (n<=ONE_POINT)
			memcpy(&offsprings[i]->ch[n], &population[p1]->ch[n], sizeof(unsigned char));
		else
			memcpy(&offsprings[i]->ch[n], &population[p2]->ch[n], sizeof(unsigned char));
	}

	return 1;
}
