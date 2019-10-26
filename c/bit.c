#include <stdio.h>

int main()
{
  int a = 254;
  printf ("%d\n", a & 0xf);
  printf ("%d\n", (a >> 4) & 0xf);

  int b = 15;
  printf ("%d\n", b & 0xf);
  printf ("%d\n", (b >> 4) & 0xf);

  return 0;
}
