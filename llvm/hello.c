#include <stdio.h>

void hello();
void test();

int main()
{
  printf("hello world\n");
  hello();
  return 0;
}

void test()
{
  printf("hello world in test()\n");
}

void hello()
{
  printf("hello world in hello()\n");
}

