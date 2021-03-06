#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int selection(int *p1, int *p2)
{
	int tmp, tmp1, tmp2, i, bigger;
	int num[2];
	unsigned long seed = get_nano_seconds();
	double r = 0.0;
	srand(seed);

	for (i=0; i<2; i++)
	{
		tmp1 = rand() % N + 1;
		r = (double)rand() / INT_MAX;

		do {
			tmp2 = rand() % N + 1;
		} while (tmp1 == tmp2);

		if (population[tmp1]->cost < population[tmp2]->cost)
		{
			tmp = tmp1;
			tmp1 = tmp2;
			tmp2 = tmp;
		}

		if (r < T)
			num[i] = tmp1;
		else
			num[i] = tmp2;
	}

	*p1 = num[0];
	*p2 = num[1];

	return 1;
}
