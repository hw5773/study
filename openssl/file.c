#include <stdio.h>
#include <stdlib.h>

int main()
{
	fprintf(stdout, "test: %s, line: %d\n", __FILE__, __LINE__); 
	fprintf(stdout, "test: %s, line: %d\n", __FILE__, __LINE__); 

	return 0;
}
