#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int mutation(int n)
{
	unsigned long seed = get_nano_seconds();
	srand(seed);
	double m_prob = 0.0; 
		
	int i = 0, swp = 0;
	int tmp;
	unsigned char t;

	for (i=1; i<=SIZE; i++)
	{
		m_prob = (double)rand() / RAND_MAX;
		swp = rand() % SIZE + 1;

		if (m_prob < M_THRE)
		{
			tmp = offsprings[n]->node[i];
			offsprings[n]->node[i] = offsprings[n]->node[swp];
			offsprings[n]->node[swp] = tmp;

			t = offsprings[n]->ch[i];
			offsprings[n]->ch[i] = offsprings[n]->ch[swp];
			offsprings[n]->ch[swp] = t;

			offsprings[n]->ch[i] = rand() % 2;
		}
	}

	return 1;
}
