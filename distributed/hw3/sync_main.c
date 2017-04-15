#ifndef __INCREMENT_H__
#define __INCREMENT_H__
#include "increment.h"
#endif

void sync_main(void *p)
{
	printf("PROGRESS: Start sync_main() by %d\n", syscall(SYS_gettid));
	ARGS *args = (ARGS *)p;
	LOCK *sync = args->sync;

	while (count < COUNT)
	{
		while(sync->lock());
		if (count < COUNT)
			count++;
		sync->unlock();
	}
}

void faa_main(void *p)
{
	printf("PROGRESS: Start faa_main() by %d\n", syscall(SYS_gettid));
	int i;
	ARGS *args = (ARGS *)p;
	int num_of_threads = args->num_of_threads;

	for (i=0; i<COUNT/num_of_threads; i++)
		__sync_fetch_and_add(&count, 1);
}
