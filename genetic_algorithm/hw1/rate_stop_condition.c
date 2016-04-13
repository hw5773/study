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
	unsigned long curr_time = get_seconds();

	if (rate >= S_RATE)
	{
		printf("rate: %lf / elapsed time: %lu / max val: %d\n", rate, curr_time - start_time, population[N]->fitness);
		printf("elapsed time: %lu us\n", curr_time - start_time);
		printf("max val: %d\n", population[N]->fitness);
		fprintf(log_file, "%lf, %lu, %d\n", rate, curr_time - start_time, population[N]->fitness);
		return 1;
	}
	else
	{
		printf("rate: %lf / elapsed time: %lu / curr max val: %d\n", rate, curr_time - start_time, population[N]->fitness);
		fprintf(log_file, "%lf, %lu, %d\n", rate, curr_time - start_time, population[N]->fitness);
		return 0;
	}
}	
