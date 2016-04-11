#include "genetic.h"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: ./maxcut <input data path> <output data path>\n");
		exit(-1);
	}

	in 		= fopen(argv[1], "r");
	out 	= fopen(argv[2], "w");
	int num_of_vertex, num_of_edge;
	int i, v1, v2, w;	// (v1, v2) is the vertex and w is the weight

	fscanf(in, "%d %d\n", &num_of_vertex, &num_of_edge);
	
	int edge[num_of_vertex+1][num_of_vertex+1];
	memset(edge, 0, sizeof(edge));
	
	while (fscanf(in, "%d %d %d\n", &v1, &v2, &w) != EOF)
	{
		edge[v1][v2] = w;
		edge[v2][v1] = w;
	}
	
	init_population();

	while (!(stop_condition()))
	{
		for (i=1; i<=K; i++)
		{
			int *p1, *p2;
			init_chromosome(&offsprings[i], SIZE);
			selection(&p1, &p2);
			crossover(i, p1, p2);
			mutation(i);
		}

		replacement();

		for (i=1; i<=K; i++)
		{
			free_chromosome(&offsprings[i]);
		}
	}

	free_population();

	fclose(in);
	fclose(out);

	return 0;
}


