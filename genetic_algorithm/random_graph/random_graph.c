#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAX_VAL 5

FILE *out;

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Usage ./random_graph <filename> <# of nodes> <# of edges>\n");
		exit(-1);
	}

	int i, j, n1, n2, v, s, num=0;
	int nodes = atoi(argv[2]);
	int edges = atoi(argv[3]);
	int done[nodes+1][nodes+1];

	for (i=0; i<=nodes; i++)
	{
		for (j=0; j<=nodes; j++)
			done[i][j] = 0;
	}

	for (i=0; i<=nodes; i++)
		done[i][i] = 1;

	out = fopen(argv[1], "w");
	fprintf(out, "%d %d\n", nodes, edges);

	srand(time(NULL));

	while (num < edges)
	{
		n1 = rand() % nodes + 1;
		n2 = rand() % nodes + 1;
		v = rand() % MAX_VAL;
		s = rand() % 2;

		if (s==0)
			v = v*(-1);

		if (done[n1][n2] == 0)
		{
			if (n1 > n2)
				fprintf(out, "%d %d %d\n", n2, n1, v);
			else
				fprintf(out, "%d %d %d\n", n1, n2, v);
			done[n1][n2] = 1;
			done[n2][n1] = 1;
			num++;
		}
	}

	return 0;
}
