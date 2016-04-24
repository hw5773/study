#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define GRADIENT 0.001
#define INIT_THRE 0.05
#define THRESHOLD 0.15

int mutation(int n)
{
	unsigned long seed = get_nano_seconds();
	unsigned long curr_time = get_seconds();
	srand(seed);
	double m_thre = 0.0, m_prob = 0.0; 
	int i = 0;
	m_thre = GRADIENT * (curr_time - start_time) + INIT_THRE;

	if (m_thre > THRESHOLD)
		m_thre = THRESHOLD;

	for (i=1; i<=SIZE; i++)
	{
		m_prob = (double)rand() / RAND_MAX;

		if (m_prob < m_thre)
		{
			offsprings[n]->ch[i] = (unsigned char) rand() % 2;
		}
	}

	return 1;
}
