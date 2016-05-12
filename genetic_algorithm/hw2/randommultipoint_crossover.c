#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int point[POINTS+1];

int init_crossover(void)
{
	return 1;
}

// i: the index number of the offspring
// p1, p2: the index number of the parents
int crossover(int i, int p1, int p2)
{
	init_chromosome(&offsprings[i]);

	int j, k, min=0, tmp, complete=0;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	point[0] = 1;

	for (k=1; k<=POINTS; k++)
	{
		do {
			point[k] = rand()%SIZE+1;
			complete = 1;
			for (j=0; j<k; j++)
			{
				if (point[j] == point[k])
					complete = 0;
			}
		} while (!complete);
	}

	for (k=1; k<=POINTS; k++)
	{
		min = point[k];
		for (j=k; j<POINTS; j++)
		{
			if (point[j] < min)
				min = point[j];
		}
		tmp = point[k];
		point[k] = point[j];
		point[j] = tmp;
	}


	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	int n=0, m=0;
	for (n=0; n<=POINTS; n++)
	{
		for (m=point[n]; (m<=SIZE || m<=point[n+1]); m++)
		{
			if (m % 2 == 0)
				memcpy(&offsprings[i]->ch[m], &population[p1]->ch[m], sizeof(unsigned char));
			else
				memcpy(&offsprings[i]->ch[m], &population[p2]->ch[m], sizeof(unsigned char));
		}
	}

	return 1;
}
