#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("uid=%d euid=%d\n", (int) getuid(), (int) geteuid());
	printf("gid=%d egid=%d\n", (int) getgid(), (int) getegid());
	return 0;
}
