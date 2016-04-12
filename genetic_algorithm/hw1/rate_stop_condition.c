#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int stop_condition()
{
	int i=0, max_val=0, count=0;
	double rate = 0.0;

	for (i=1; i<=N; i++)
	{
		if (population[i]->fitness > max_val)
		{
			max_val = population[i]->fitness;
			count = 1;
		}
		else if (population[i]->fitness == max_val)
			count++;
		else
			continue;
	}

	rate = ((double)count) / N;

	printf("rate: %lf\n", rate);
	
	if (rate >= S_RATE)
		return 1;
	else
		return 0;
}	
