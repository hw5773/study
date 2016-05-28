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
	int c1 = 0, c2 = 0;			// cutting point 1 and cutting point 2
	double p = 0.0;				// the probability to take
	num[0] = rand() % SIZE + 1;
	p = rand() * 1.0 / RAND_MAX;

	do {
		num[1] = rand() % SIZE + 1;
	} while (num[0] == num[1]);

	if (num[0] < num[1])
	{
		c1 = num[0];
		c2 = num[1];
	} else {
		c1 = num[1];
		c2 = num[0];
	}

	init_chromosome(&offsprings[i]);

	if (population[p1]->cost < population[p2]->cost)
	{
		offsprings[i]->p1 = p1;
		offsprings[i]->p2 = p2;
	} else {
		offsprings[i]->p1 = p2;
		offsprings[i]->p2 = p1;
	}

	int s1, s2, tmp;

	if (p < 0.5)
	{	
		s1 = p1; s2 = p2;
	}
	else
	{
		s1 = p2; s2 = p1;
	}

/////////
//	printf("c1: %d\n", c1);
//	printf("c2: %d\n", c2);
//	printf("s1: ");
//	print_chromosome(population[s1]);
//	printf("s2: ");
//	print_chromosome(population[s2]);
/////////

	for (n=1; n<=SIZE; n++)
	{
		if (n < c1 || n > c2)
		{
			offsprings[i]->node[n] 	=	population[s2]->node[n];
			offsprings[i]->ch[n] 	= 	population[s2]->ch[n];
		}
		else
		{
			offsprings[i]->node[n] 	= 	population[s1]->node[n];
			offsprings[i]->ch[n] 	= 	population[s1]->ch[n];
		}
	}

	int complete = 0;

	while (!complete)
	{
		complete = 1;
		for (n=1; n<=SIZE; n++)
		{
			if ((n >= c1) && (n <= c2))
				continue;
			
			tmp = offsprings[i]->node[n];

			for (m=c1; m<=c2; m++)
			{
				if (tmp == population[s1]->node[m])
				{
					complete = 0;
					offsprings[i]->node[n] 	= 	population[s2]->node[m];
					offsprings[i]->ch[n] 	= 	population[s2]->ch[m];
				}
			}
		}
	}

//	printf("off: ");
//	print_chromosome(offsprings[i]);

	return 1;
}
