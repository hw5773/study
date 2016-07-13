#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int fd;
	umask(S_IWUSR | S_IWGRP | S_IWOTH);

	fd = open("test.txt", O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IRWXO);

	if (fd < 0)
	{
		printf("open failed\n");
		exit(0);
	}

	return 0;
}
