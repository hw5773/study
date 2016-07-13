#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int mutation(int n)
{
	unsigned long seed = get_nano_seconds();
	srand(seed);
	double m_prob = 0.0; 
	
	int i = 0;

	for (i=1; i<=SIZE; i++)
	{
		m_prob = (double)rand() / RAND_MAX;

		if (m_prob < M_THRE)
		{
			offsprings[n]->ch[i] = (unsigned char) rand() % 2;
		}
	}

	return 1;
}
