#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#define GRADIENT 0.001
<<<<<<< HEAD
#define INIT_THRE 0.015
#define THRESHOLD 0.05
=======
#define INIT_THRE 0.05
#define THRESHOLD 0.30
>>>>>>> ecb25b98a91bc5216d182e478c684f362b044526

int mutation(int n)
{
	unsigned long seed = get_nano_seconds();
	unsigned long curr_time = get_seconds();
	srand(seed);
	double m_thre = 0.0, m_prob = 0.0; 
	int i = 0, swp = 0, tmp;
	unsigned char t;

	m_thre = GRADIENT * (curr_time - start_time) + INIT_THRE;

	if (m_thre > THRESHOLD)
		m_thre = THRESHOLD;

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
