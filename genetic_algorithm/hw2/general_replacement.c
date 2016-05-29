#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int replacement(int edge[][SIZE+1])
{
	int i=0;

	for (i=1; i<=K; i++)
		offsprings[i]->cost = calc_cost(offsprings[i], edge);

//	sort_offsprings();
//	print_offsprings();

	for (i=1; i<=K; i++)
	{
//		memcpy(population[i], offsprings[i], sizeof(Chromosome));
		free_chromosome(population[i]);
		population[i] = offsprings[i];
	}

	return 1;
}

int sort_offsprings(void)
{
	int i=0, j=0, k=0;
	Chromosome *tmp;

	for (i=1; i<=K; i++)
	{
		for (j=1; j<=i; j++)
		{
			if (offsprings[j]->cost > offsprings[i]->cost)
			{
				tmp = offsprings[i];
				for (k=i-1; k>=j; k--)
				{
					offsprings[k+1] = offsprings[k];
				}
				offsprings[j] = tmp;
			}
		}
	}

	return 1;
}

int print_offsprings(void)
{
	int i;

	printf("offsprings: \n");
	for (i=1; i<=K; i++)
	{
		printf("%d ", offsprings[i]->cost);
		if (i % 40 == 0)
			printf("\n");
	}
}
