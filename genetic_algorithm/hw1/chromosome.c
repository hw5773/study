#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

int init_chromosome(Chromosome **c, int size)
{
	*c = (Chromosome *)malloc(sizeof(Chromosome));
	(*c)->size = size;
	(*c)->ch = (unsigned char *)malloc(size);

	return 1;
}

int free_chromosome(Chromosome **c)
{
	free((*c)->ch);
	free(*c);
}
