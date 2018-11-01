#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DELIMITER "\r\n"
#define DELIMITER_LEN 2

int main(int argc, char *argv[])
{
  uint8_t *nptr, *cptr;
  uint8_t *msg = "Helloworld\r\nTest\r\n\r\nApplication";
  printf("Input (%lu bytes): %s\n", strlen(msg), msg);

  cptr = msg;
  while ((nptr = strstr(cptr, DELIMITER)))
  {
    printf("cptr: %p\n", cptr);
    printf("nptr: %p\n", nptr);
    printf("len: %ld\n", nptr - cptr);
    cptr = nptr + DELIMITER_LEN;
  }
  printf("cptr: %s\n", cptr);
  return 0;
}
