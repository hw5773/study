#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int stop_condition()
{
	int i=0, max_val=0, count=0;
	double rate = 0.0, avg_val = 0.0;

	for (i=1; i<=N; i++)
	{
		avg_val += population[i]->cost;
		if (population[i]->cost > max_val)
		{
			max_val = population[i]->cost;
			count = 1;
		}
		else if (population[i]->cost == max_val)
			count++;
		else
			continue;
	}

	avg_val = avg_val / N;
	rate = ((double)count) / N;
	unsigned long curr_time = get_seconds();

	if (rate >= S_RATE || ((curr_time - start_time) >= 170))
		return 1;
	else
		return 0;
}	
