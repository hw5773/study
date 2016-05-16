#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define ONE_POINT SIZE/2

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	int n=0;
	double t1, t2;
	unsigned long seed = get_nano_seconds();
	srand(seed);
	t1 = (double)rand() / INT_MAX;
	t2 = (double)rand() / INT_MAX;

	init_chromosome(&offsprings[i]);

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	for(n=1; n<=SIZE; n++)
	{
		if (n<=ONE_POINT)
		{
			if (t1 <= C)
				memcpy(&offsprings[i]->ch[n], &population[p1]->ch[ONE_POINT-n+1], sizeof(unsigned char));
			else	
				memcpy(&offsprings[i]->ch[n], &population[p1]->ch[n], sizeof(unsigned char));
		}
		else
		{
			if (t2 <= C)
				memcpy(&offsprings[i]->ch[n], &population[p2]->ch[ONE_POINT-n+1], sizeof(unsigned char));
			else
				memcpy(&offsprings[i]->ch[n], &population[p2]->ch[n], sizeof(unsigned char));
		}
	}

	return 1;
}
