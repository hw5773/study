#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define COUNT 1000000000
//#define COUNT 1000000
#define NUM_THREADS 8

unsigned long get_current_microseconds()
{
	struct timeval curr;
	gettimeofday(&curr, NULL);

	return curr.tv_sec * 1000000 + curr.tv_usec;
}
