#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

int main(int argc, char *argv[])
{
  int n;
  char *s1, *s2;
  s1 = argv[1];
  s2 = argv[2];
  n = atoi(argv[3]);

  printf("s1: %s / s2: %s / n: %d\n", s1, s2, n);

  printf("result of strcasecmp: %d\n", strcasecmp(s1, s2));
  printf("result of strncasecmp: %d\n", strncasecmp(s1, s2, n));

  return 0;
}

int strcasecmp(const char *s1, const char *s2)
{
  char a, b;
  int i, ret;
  size_t n1, n2, n;

  n1 = strlen(s1);
  n2 = strlen(s2);

  if (n1 > n2)
  {
    n = n2;
    a = s1[n];

    if (a >= 0x41 && a <= 0x5A)
      a += 32;

    ret = -1 * a;
  }
  else if (n1 < n2)
  {
    n = n1;
    b = s2[n];

    if (b >= 0x41 && b <= 0x5A)
      b += 32;

    ret = b;
  }
  else
  {
    n = n1;
    ret = 0;
  }

  for (i=0; i<n; i++)
  {
    a = s1[i];
    b = s2[i];

    if (a == b)
      continue;

    if (a >= 0x41 && a <= 0x5A)
      a += 32;

    if (b >= 0x41 && b <= 0x5A)
      b += 32;

    if (a < b)
    {
      ret = b;
      break;
    }
    else if (a > b)
    {
      ret = -1 * a;
      break;
    }
  }

  return ret;
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
  char a, b;
  int i, ret;

  ret = 0;
  for (i = 0; i < n; i++)
  {
    a = s1[i];
    b = s2[i];

    if (a == b)
      continue;

    if (a >= 0x41 && a <= 0x5A)
      a += 32;

    if (b >= 0x41 && b <= 0x5A)
      b += 32;

    if (a < b)
    {
      ret = b;
      break;
    }
    else if (a > b)
    {
      ret = -1 * a;
      break;
    }
  }

  return ret;
}
