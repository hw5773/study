#ifndef __GENETIC_H__
#define __GENETIC_H__
#include "genetic.h"
#endif

#include <time.h>

unsigned long get_seconds(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec;
}

unsigned long get_nano_seconds(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return 1000000000*(ts.tv_sec) + ts.tv_nsec;
}
