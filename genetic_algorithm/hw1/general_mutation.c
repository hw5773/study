#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int mutation(int n)
{
	unsigned long seed = get_nano_seconds();
	srand(seed);
	// m_prob: the probability to be mutated
	double m_prob = 0.0; 
	
	int i = 0;

	for (i = 0; i < SIZE; i++)
	{
		m_prob = (double)rand() / RAND_MAX;

		if (m_prob < M_THRE)
		{
			offsprings[n]->ch[i] = (unsigned char) rand() % 2;
		}
	}

	printf("mutation result: ");
	print_chromosome(offsprings[n]);

	return 1;
}
