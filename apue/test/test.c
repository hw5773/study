#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd = 1;
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	if (fd > 2)
		close(fd);

	return 0;
}
