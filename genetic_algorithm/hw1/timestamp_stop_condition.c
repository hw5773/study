#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int stop_condition(void)
{
	unsigned long curr_time = get_seconds();

	if (curr_time - start_time > 10)
		return 1;
	else
		return 0;
}
