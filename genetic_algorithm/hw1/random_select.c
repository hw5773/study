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

	*p1 = rand()%SIZE + 1;
	*p2 = 0;

	while (*p1 == *p2)
		*p2 = rand()%SIZE + 1;

	printf("p1: %d, p2: %d\n", *p1, *p2);

	return 1;
}
