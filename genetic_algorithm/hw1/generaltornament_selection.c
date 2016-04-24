#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <math.h>

int selection(int *p1, int *p2)
{
	int i, j, k, t, complete=0;
	int num[2];
	int num_of_candidates = (int)pow(2, K_TOR);
	int candidate[num_of_candidates];
	unsigned long seed = get_nano_seconds();
	double r = 0.0;
	srand(seed);

	for (i=0; i<2; i++)
	{
		for (j=0; j<K_TOR; j++)
		{
			candidate[j] = rand() % N + 1;
			complete = 0;

			while (!complete)
			{
				for (k=0; k<j; k++)
				{
					if (candidate[j] == candidate[k])
						candidate[j] = rand() % N + 1;
				}

				if (k==j)
					complete = 1;
			}
		}

		
		while (num_of_candidates > 1)
		{
			num_of_candidates /= 2;
			int tmp[2];

			for (j=0; j<num_of_candidates; j++)
			{
				r = (double)rand() / RAND_MAX;

				// tmp[0] is smaller one, tmp[1] is bigger one.
				if (population[candidate[2*j]]->cost > population[candidate[2*j+1]]->cost)
				{
					tmp[0] = candidate[2*j+1];
					tmp[1] = candidate[2*j];
				}
				else
				{
					tmp[0] = candidate[2*j];
					tmp[1] = candidate[2*j+1];
				}

				if (r < T)
					candidate[j] = tmp[1];
				else
					candidate[j] = tmp[0];
			}					
		}

		num[i] = candidate[0];
	}

	*p1 = num[0];
	*p2 = num[1];

	return 1;
}
