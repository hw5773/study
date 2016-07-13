#include "genetic.h"

int num_of_vertex, num_of_edge;
unsigned long start_time;
FILE *log_file;
int generation = 0;

int main(int argc, char *argv[])
{
	// Need to change to 3. need to remove the logging.
	if (argc != 4)
	{
		printf("Usage: ./maxcut <input data path> <output data path> <log file>\n");
		exit(-1);
	}
	
	// Need to remove when submitting.
	log_file = fopen(argv[3], "w");
	fprintf(log_file, "rate, elasped time (s), max val, avg val\n");

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

	preprocessing(edge);

	return 1;
}
