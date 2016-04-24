#include "genetic.h"

int num_of_vertex, num_of_edge;
unsigned long start_time;
FILE *log_file;

int main(int argc, char *argv[])
{
	// Need to change to 3. need to remove the logging.
	if (argc != 4)
	{
		printf("Usage: ./maxcut <input data path> <output data path> <log file>\n");
		exit(-1);
	}
	
	printf("N: %d\n", N);
	printf("K: %d\n", K);

	// Need to remove when submitting.
	log_file = fopen(argv[3], "w");
	fprintf(log_file, "rate, elasped time (s), max val, avg val\n");

	start_time = get_seconds();

	in 		= fopen(argv[1], "r");
	out 	= fopen(argv[2], "w");
	int i, j, v1, v2, w;	// (v1, v2) is the vertex and w is the weight

	fscanf(in, "%d %d\n", &num_of_vertex, &num_of_edge);
	
	int edge[num_of_vertex+1][num_of_vertex+1];
	
	for (i=0; i<=SIZE; i++)
		for (j=0; j<=SIZE; j++)
			edge[i][j] = 0;

	while (fscanf(in, "%d %d %d\n", &v1, &v2, &w) != EOF)
	{
		edge[v1][v2] = w;
		edge[v2][v1] = w;
	}
	
	init_population();
	init_offsprings();
	init_cost(edge);
	sort_population();
	init_crossover();

	int p1, p2;

	while (!(stop_condition()))
	{
		for (i=1; i<=K; i++)
		{
			selection(&p1, &p2);
			crossover(i, p1, p2);
			mutation(i);
		}

		replacement(edge);

		sort_population();
	}

	for (i=1; i<=SIZE; i++)
	{
		if (population[N]->ch[i] == 1)
			fprintf(out, "%d ", i);
	}

	free_population();

	fclose(in);
	fclose(out);

	printf("N: %d, K: %d, S_RATE: %lf, M_THRE: %lf, P0: %lf, POINTS: %d, K_FIT: %d, T: %lf\n", N, K, S_RATE, M_THRE, P0, POINTS, K_FIT, T);

	return 0;
}
