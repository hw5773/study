#include <stdio.h>
#include <stdint.h>
#include "macro.h"

DEFINE_STRUCT(test);

int main()
{

  struct test_st t;
  t.a = 10;
  printf("t.a: %d\n", t.a);

  uint8_t a = 5;
  PRINT_TOKEN(a);

  return 1;
}
