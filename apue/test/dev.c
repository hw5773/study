#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	int i;

	for (i=1; i<argc; i++)
	{
		lstat(argv[i], &buf);

		printf(" dev = %d/%d", major(buf.st_dev), minor(buf.st_dev) );

		if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode))
		{
			printf(" (%s) rdev = %d/%d", (S_ISCHR(buf.st_mode))? "character" : "block", major(buf.st_rdev), minor(buf.st_rdev));
		}
		printf("\n");
	}

	exit(0);
}

