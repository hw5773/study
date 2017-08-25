#ifndef __INCREMENT_H__
#define __INCREMENT_H__
#include "increment.h"
#endif

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		printf("Please insert the result file name\n");
		printf("USAGE: %s <result file name> <# of threads> <synchronization operation> <# of rounds>\n", argv[0]);
		printf("SYNC: Test and Set (0), Test and Test and Set (1), Compare and Exchange (2), Fetch and Add (3)\n");
		exit(1);
	}

	int num_of_threads;
	int sync_operation;
	int num_of_rounds;
	LOCK *sync;
	ARGS *args;
	int l = 0, t, rc, round = 0;
	void *status;
	unsigned long start, end;

	num_of_threads = atoi(argv[2]);
	sync_operation = check_operation(atoi(argv[3]));
	num_of_rounds = atoi(argv[4]);

	if (sync_operation < 0)
	{
		printf("ERROR: Please input the right operation\n");
		printf("SYNC: Test and Set (0), Test and Test and Set (1), Compare and Exchange (2), Fetch and Add (3)\n");
		return 1;
	}

	sync = (LOCK *)malloc(sizeof(LOCK));
	args = (ARGS *)malloc(sizeof(ARGS));
	args->sync = sync;
	args->num_of_threads = num_of_threads;

	switch (sync_operation)
	{
	case 0:
		printf("PROGRESS: Test and Set operation is selected\n");
		sync->lock = tas_lock;
		sync->unlock = tas_unlock;
		sync->sync_main = (void *)sync_main;
		sync->operation_name = "Test and Set";
		break;
	case 1:
		printf("PROGRESS: Test and Test and Set operation is selected\n");
		sync->lock = ttas_lock;
		sync->unlock = ttas_unlock;
		sync->sync_main = (void *)sync_main;
		sync->operation_name = "Test and Test and Set";
		break;
	case 2:
		printf("PROGRESS: Compare and Swap operation is selected\n");
		sync->lock = cas_lock;
		sync->unlock = cas_unlock;
		sync->sync_main = (void *)sync_main;
		sync->operation_name = "Compare and Swap";
		break;
	case 3:
		printf("PROGRESS: Fetch and Add operation is selected\n");
		sync->lock = faa_lock;
		sync->unlock = faa_unlock;
		sync->sync_main = (void *)sync_main;
		sync->operation_name = "Fetch and Add";
		break;
	}

	printf("PROGRESS: %s Operation set complete\n", sync->operation_name);

	fp = fopen(argv[1], "w");
	fprintf(fp, "Round, Value, Time\n");

	pthread_t thread[num_of_threads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	printf("PROGRESS: Start the %s experiment with %d threads in %d rounds\n", sync->operation_name, num_of_threads, num_of_rounds);

	while (round < num_of_rounds)
	{
		start = get_current_microseconds();
		for (t=0; t<num_of_threads; t++)
		{
			rc = pthread_create(&thread[t], &attr, (*(sync->sync_main)), args);

			if (rc)
			{
				printf("ERROR: return code from pthread_create: %d\n", rc);
				return 1;
			}
		}

		pthread_attr_destroy(&attr);
		for (t=0; t<num_of_threads; t++)
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
	free(args);

	return 0;

err:
	free(args);
	free(sync);

	return 1;
}
