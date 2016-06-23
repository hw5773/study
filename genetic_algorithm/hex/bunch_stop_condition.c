#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int stop_condition(int edge[][SIZE+1])
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

	if ((curr_time - start_time) >= 170)
	{
		printf("rate: %lf / elapsed time: %lu / max val: %d / avg val: %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);
		printf("elapsed time: %lu s\n", curr_time - start_time);
		printf("max val: %d\n", population[N]->cost);
		fprintf(log_file, "%lf, %lu, %d, %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);
		return 1;
	}
	else if (rate >= S_RATE)
	{
		printf("rate: %lf / elapsed time: %lu s / curr max val: %d / avg val: %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);
		printf("replace all the generation except the maximum\n");
		Chromosome *max;
		init_chromosome(&max);
		memcpy(max, population[N], sizeof(Chromosome));
		free_population();
		init_population(edge);
		init_cost(edge);
		population[N] = max;	
		fprintf(log_file, "%lf, %lu, %d, %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);
		return 0;
	}
	else
	{
		printf("rate: %lf / elapsed time: %lu s / curr max val: %d / avg val: %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);

		fprintf(log_file, "%lf, %lu, %d, %.2lf\n", rate, curr_time - start_time, population[N]->cost, avg_val);
		return 0;
	}
}	