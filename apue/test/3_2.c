#include "apue.h"
#include <fcntl.h>

char	buf1[]	=	"abcdefghij";
char	buf2[]	=	"ABCDEFGHIJ";

int main(void)
{
	int	fd;
	if ((fd = creat("file.hole", FILE_MODE)) < 0)
		printf("creat error");

	if (write(fd, buf1, 10) != 10)
		printf("buf1 write error");

	if (lseek(fd, 16384, SEEK_SET) == -1)
		printf("lseek error");

	if (write(fd, buf2, 10) != 10)
		printf("buf2 write error");

	close(fd);

	if ((fd = creat("file.nohole", FILE_MODE)) < 0)
		printf("creat error");

	if (write(fd, buf1, 10) != 10)
		printf("buf1 write error");

	char buf = '\0';

	int i;

	for (i=0; i<16374; i++)
		write(fd, &buf, 1);

	if (write(fd, buf2, 10) != 10)
		printf("buf2 write error");

	close(fd);

	exit(0);
}
