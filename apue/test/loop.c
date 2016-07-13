#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigint_handler(int signum)
{
	printf("interrupt! - %d\n", signum);
	exit(0);
}

int main()
{
	signal(SIGINT, sigint_handler);

	while (1) {}

	return 0;
}
