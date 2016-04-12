#include "genetic.h"

int num_of_vertex, num_of_edge;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: ./maxcut <input data path> <output data path>\n");
		exit(-1);
	}

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
	
	for (i=0; i<=SIZE; i++)
		for (j=0; j<=SIZE; j++)
			printf("edge[%d][%d] = %d\n", i, j, edge[i][j]);


	init_population();
	init_offsprings();
	init_fitness();

	int stop = 0; // test variable for the stop_condition
	while (stop < 2)
//	while (!(stop_condition(edge)))
	{
		for (i=1; i<=K; i++)
		{
			int p1, p2;
			selection(&p1, &p2);
			crossover(i, p1, p2);
			mutation(i);
		}

		replacement();

		// test variable for the stop_condition
		stop++;
		stop_condition(edge);
	}

	free_offsprings();
	free_population();

	fclose(in);
	fclose(out);

	return 0;
}
