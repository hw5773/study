#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_chromosome(Chromosome **c, int edge[][SIZE+1])
{
	*c = (Chromosome *)malloc(sizeof(Chromosome));
	(*c)->node = (int *)malloc((SIZE+1) * sizeof(int));
	(*c)->ch = (unsigned char *)malloc(SIZE+1);

	int i=0;
	unsigned long seed = get_nano_seconds();
	srand(seed);

	(*c)->size = SIZE;

	init_permutation((*c)->node);

	for (i=1; i<=SIZE; i++) // zero base
		(*c)->ch[i] = rand()%2;

	(*c)->p1 = 0;
	(*c)->p2 = 0;

	return 1;
}

int free_chromosome(Chromosome *c)
{
	free(c->node);
	free(c->ch);
	free(c);
}

int init_population(int edge[][SIZE+1])
{
	int i=1, j=0;

	for (i=1; i<=N; i++) // first chromosome is in the index 1.
		init_chromosome(&population[i], edge);

	return 1;
}

int free_population(void)
{
	int i=1;

	for (i=1; i<=N; i++)
	{
		free(population[i]->node);
		free(population[i]->ch);
		free(population[i]);
	}

	return 1;
}

int init_offsprings(int edge[][SIZE+1])
{
	int i=1;
	
	for (i=1; i<=K; i++)
		init_chromosome(&offsprings[i], edge);

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

	printf("node: \n");
	for (i=1; i<=SIZE; i++)
		printf("%d ", c->node[i]);
	printf("\n");

	printf("set: \n");
	for (i=1; i<=SIZE; i++)
		printf("%d", c->ch[i]);

	printf(": %d\n", c->cost);
}
