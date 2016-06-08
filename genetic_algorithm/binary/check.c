#include "genetic.h"
int num_of_vertex;

int main(int argc, char *argnum_of_vertex[])
{
	FILE *graph = fopen(argnum_of_vertex[1], "r");
	FILE *result = fopen(argnum_of_vertex[2], "r");
	char buf[256];
	int i, j, v1, v2, w, e;

	fgets(buf, sizeof(buf), result);
	fscanf(graph, "%d %d\n", &num_of_vertex, &e);

	int edge[num_of_vertex+1][num_of_vertex+1];

	for (i=0; i<=num_of_vertex; i++)
	{
		for (j=0; j<=num_of_vertex; j++)
		{
			edge[i][j] = 0;
		}
	}

	while (fscanf(graph, "%d %d %d\n", &v1, &v2, &w) != EOF)
	{
		edge[v1][v2] = w;
		edge[v2][v1] = w;
	}

	Chromosome *c;
	init_chromosome(&c);

	for (i=0; i<=SIZE+1; i++)
	{
		c->ch[i]=0;
	}

	char *token = strtok(buf, " ");

	while (token)
	{
		printf("%d ", atoi(token));
		c->ch[atoi(token)] = 1;
		token = strtok(NULL, " ");
	}

	printf("\n");
	print_chromosome(c);
	
	int cost = calc_cost(c, edge);

	printf("\n\nedge: \n");
	for (i=21; i<=25; i++)
	{
		for (j=31; j<=40; j++)
			printf("%d ", edge[i][j]);
		printf("\n");
	}

	printf("cost: %d\n", cost);

	return 0;
}
