#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int fd;
	int val;
	fd = open("exec_copy.txt", O_CREAT | O_EXCL);

	val = fcntl(fd, F_GETFD, 0);
	if (val & FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");

	val |= FD_CLOEXEC;
	if (val & FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");
	fcntl(fd, F_SETFD, val);

	execl("~/study/apue/text/loop", "./loop", 0);

	return 0;
}
