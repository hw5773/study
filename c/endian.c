#include <stdio.h>

const int i = 1;
#define is_bigendian() ( (*(char *)&i) == 0)

int main(void)
{
  int val;
  char *ptr;
  ptr = (char *) &val;
  val = 0x12345678;

  if (is_bigendian())
  {
    printf("big endian\n");
    printf("%X.%X.%X.%X\n", ptr[0], ptr[1], ptr[2], ptr[3]);
  }
  else
  {
    printf("little endian\n");
    printf("%X.%X.%X.%X\n", ptr[3], ptr[2], ptr[1], ptr[0]);
  }

  printf("result: %X\n", val & 0xff);
  printf("result: %X\n", (val >> 24) & 0xff);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  printf("machine is little endian\n");
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  printf("machine is big endian\n");
#else
  printf("error\n");
#endif

  return 0;
}
