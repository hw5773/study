#include "increment.h"

FILE *fp;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Single thread increment\n");
		printf("Please insert the output file name\n");
		printf("USAGE: ./single <result file name>\n");
		exit(1);
	}
	printf("Start the single thread increment without any synchronization method\n");

	fp = fopen(argv[1], "w");
	fprintf(fp, "Round, Value, Time\n");

	unsigned long count=0, start, end;
	int i, round=0;

	while (round < 100)
	{
		start = get_current_microseconds();

		for (i=0; i<COUNT; i++)
			count++;
		end = get_current_microseconds();
		
		printf("Round %d\n", round);
		printf("Elapsed Time: %ld us\n", end - start);
		printf("Final Value: %ld\n", count);
		fprintf(fp, "%d, %ld, %ld\n", round, count, end - start);
		round++;
		count = 0;
	}
	close(fp);

	return 0;
}
