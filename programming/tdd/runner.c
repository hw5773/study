#include <stdio.h>
#include "sut.h"

int tests_run = 0;

#define FAIL() printf("\nFailure in %s() line %d\n", __func__, __LINE__)
#define _assert(test) do { if (!(test)) { FAIL(); return 1; } } while(0)
#define _verify(test) do { int r=test(); tests_run++; if(r) return r; } while(0)
