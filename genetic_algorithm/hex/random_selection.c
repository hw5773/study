#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <stdlib.h>
#include <time.h>

int selection(int *p1, int *p2)
{
	
	unsigned long seed = get_nano_seconds();
	srand(seed);

	*p1 = rand()%N + 1;

	do {
		*p2 = rand()%N + 1;
	} while (*p1 == *p2);

	return 1;
}
