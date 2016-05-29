#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	int n, m;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	int num[2];
	double p = 0.0;				// the probability to take
	num[0] = rand() % SIZE + 1;
	p = rand() * 1.0 / RAND_MAX;

	init_chromosome(&offsprings[i]);

	for (n=0; n<=SIZE; n++)
		offsprings[i]->node[n] = 0;

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	int s[2], tmp;

	if (p < 0.5)
	{	
		s[0] = p1; s[1] = p2;
	}
	else
	{
		s[0] = p2; s[1] = p1;
	}

	int complete = 0;
	int order = 0, loc = 1;
	int curr = population[s[order]]->node[loc];

	while (!complete)
	{
		offsprings[i]->node[loc] = curr;

		curr = population[s[(order+1)%2]]->node[loc];

		for (n=1; n<=SIZE; n++)
		{
			if (curr == population[s[order]]->node[n])
			{
				loc = n;
				break;
			}
		}

		if (offsprings[i]->node[loc] > 0)
		{
			for (n=1; n<=SIZE; n++)
			{
				if (offsprings[i]->node[n] == 0)
				{
					loc = n;
					order = (order + 1) % 2;
					curr = population[s[order]]->node[loc];
					break;
				}
			}

			if (n > SIZE)
				complete = 1;
		}
	}

	return 1;
}
