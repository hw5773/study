#include "increment.h"

FILE *fp;
unsigned long count;
void *increment(void *);
int i;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Multi thread increment without any synchronization method\n");
		printf("Please insert the result file name\n");
		printf("USAGE: %s <result file name>\n", argv[0]);
		exit(1);
	}
	printf("Start the multi thread increment without any synchronization method\n");

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

		fprintf(fp, "%d, %ld, %ld\n", round, count, end - start);
		printf("Round %d\n", round);
		printf("Elapsed Time: %ld ms\n", end - start);
		printf("Final Value: %ld\n", count);

		round++;
		count = 0;
	}
	pthread_exit(NULL);
	close(fp);

	return 0;
}

void *increment(void *p)
{
	while (count < COUNT)
		count++;
}
	
