#include "genetic.h"

int num_of_vertex, num_of_edge;
unsigned long start_time;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: ./maxcut <input data path> <output data path>\n");
		exit(-1);
	}

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
	init_fitness(edge);
	sort_population();

//	int stop = 0; // test variable for the stop_condition
//	while (stop < 2)
	while (!(stop_condition()))
	{
		for (i=1; i<=K; i++)
		{
			int p1, p2;
			selection(&p1, &p2);
			crossover(i, p1, p2);
			mutation(i);
		}

		replacement(edge);

		sort_population();

		// test variable for the stop_condition
//		stop++;
//		stop_condition(edge);
	}

	for (i=0; i<=SIZE; i++)
	{
		if (population[N]->ch[i] == 1)
			fprintf(out, "%d ", i+1);
	}

	free_offsprings();
	free_population();

	fclose(in);
	fclose(out);

	return 0;
}
