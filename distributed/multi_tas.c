#include "increment.h"

FILE *fp;
unsigned long count;
volatile int lock = 0;

void *increment(void *);

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Multi thread increment with the synchronization method\n");
		printf("Please insert the result file name\n");
		printf("USAGE: %s <result file name>\n", argv[0]);
		exit(1);
	}
	
	printf("Start the multi thread increment with the compare and swap\n");

	fp = fopen(argv[1], "w");
	fprintf(fp, "Round, Value, Time\n");

	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int t, rc, round = 0;
	void *status;
	unsigned long start, end;

	while (round < 100)
	{
		start = get_current_microseconds();
		for (t=0; t<NUM_THREADS; t++)
		{
			rc = pthread_create(&thread[t], &attr, increment, NULL);

			if (rc)
			{
				printf("ERROR: return code from pthread_create: %d\n", rc);
				return 1;
			}
		}

		pthread_attr_destroy(&attr);
		for (t=0; t<NUM_THREADS; t++)
		{
			rc = pthread_join(thread[t], &status);
			if (rc)
			{
				printf("ERROR: return code from pthread_join: %d\n", rc);
				return 1;
			}
		}
		end = get_current_microseconds();

		fprintf(fp, "%d, %ld, %ld\n", round, end - start, count);
		printf("Round %d\n", round);
		printf("Elapsed Time: %ld us\n", end - start);
		printf("Final Value: %ld\n", count);

		round++;
		count = 0;
	}
	pthread_exit(NULL);
	close(fp);

	return 0;
}

static inline int fetch_and_add(unsigned long *ptr, unsigned long val)
{
	__asm__ volatile("lock; xadd %0, %1"
		: "+r" (val), "+m" (*ptr)
		: 
		: "memory"
	);
	return val;
}

static inline unsigned long compare_and_swap(unsigned long *ptr, unsigned long old_val, unsigned long new_val)
{
	unsigned long ret = 1;

	__asm__ volatile(
		"movq %2, %%rax\n\t"
		"movq %3, %%rbx\n\t"
		"movq %0, %%rcx\n\t"
		"lock\n\t"
		"cmpxchg %%rbx, (%%rcx)\n\t"
		"jz done\n\t"
		"movq $0, %1\n\t"
		"done: \n\t"
		:"=m"(ptr), "=g"(ret)
		:"g"(old_val), "g"(new_val), "m"(ptr)
		:"%rax", "%rbx", "rcx", "cc"
	);

	if (ret) 
		return 0;
}

void *increment(void *p)
{
	unsigned long tmp;
	int lock;
	while (count < COUNT)
	{
		while(__sync_lock_test_and_set(&lock, 1))
		{
			if (count < COUNT)
			{
				count++;
				__sync_lock_release(&lock);
			}
		}
	}
}
