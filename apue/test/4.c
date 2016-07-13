#include <stdio.h>
#include <sys/stat.h>

int main()
{
	struct stat buf;

	stat("~/study/apue/test/tmp", &buf);

	printf("ino: %lu\n", buf.st_ino);
//	printf("uid: %s\n", buf.st_uid);
//	printf("gid: %s\n", buf.st_gid);
	printf("link?: %d\n", S_ISLNK(buf.st_mode));

	lstat("~/study/apue/test/tmp", &buf);

	printf("ino: %lu\n", buf.st_ino);
//	printf("uid: %s\n", buf.st_uid);
//	printf("gid: %s\n", buf.st_gid);
	printf("link?: %d\n", S_ISLNK(buf.st_mode));

	return 0;
}
