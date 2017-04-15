#include <stdio.h>
#include <stdlib.h>

typedef struct tmp
{
	unsigned char *str;
	unsigned int length;
} TMP;

int main()
{
	TMP *t;
	t = (TMP *)malloc(sizeof(TMP));
	t->str = "hello";
	t->length = 5;
	printf("str: %s\n", t->str);
	printf("length: %d\n", t->length);

	return 0;
}
