#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd = open("~/study/apue/test/test_dir/test.txt", O_RDONLY);

	printf("fd: %d\n", fd);

	return 0;
}
