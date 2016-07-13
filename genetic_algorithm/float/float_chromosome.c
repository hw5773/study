#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_chromosome(Chromosome **c)
{
	*c = (Chromosome *)malloc(sizeof(Chromosome));

	int i=0, j=0, s=0, complete=0, v=0;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	s = rand()%SIZE + 1;
	(*c)->size = s;
	(*c)->ch = (unsigned int *)malloc(s+1);

	for (i=1; i<=s; i++) // zero base
	{
		complete = 0;
		while (!complete)
		{
			v = rand()%SIZE+1;

			for (j=1; j<=i; j++)
			{
				if ((*c)->ch[j] == v)
					break;
			}
			if (j==i+1)
			{
				(*c)->ch[i] = (unsigned int)v;
				complete = 1;
			}
		}
	}

	(*c)->p1 = 0;
	(*c)->p2 = 0;

	print_chromosome(*c);

	return 1;
}

int free_chromosome(Chromosome *c)
{
	free(c->ch);
	free(c);
}

int init_population(void)
{
	int i=1, j=0;

	for (i=1; i<=N; i++) // first chromosome is in the index 1.
		init_chromosome(&population[i]);

	return 1;
}

int free_population(void)
{
	int i=1;

	for (i=1; i<=N; i++)
	{
		free(population[i]->ch);
		free(population[i]);
	}

	return 1;
}

int init_offsprings(void)
{
	int i=1;
	
	for (i=1; i<=K; i++)
		init_chromosome(&offsprings[i]);

	return 1;
}

int free_offsprings(void)
{
	int i=1;

	for (i=1; i<=K; i++)
		free_chromosome(offsprings[i]);

	return 1;
}

int sort_population(void)
{
	int i=0, j=0, k=0;
	Chromosome *tmp;

	for (i=1; i<=N; i++)
	{
		for (j=1; j<=i; j++)
		{
			if (population[j]->cost > population[i]->cost)
			{
				tmp = population[i];
				for (k=i-1; k>=j; k--)
				{
					population[k+1] = population[k];
				}
				population[j] = tmp;
			}
		}
	}

	return 1;
}
					

void print_chromosome(Chromosome *c)
{
	int i;

	for (i=1; i<=c->size; i++)
		printf("%x ", c->ch[i]);

	printf(": %d\n", c->cost);
}
