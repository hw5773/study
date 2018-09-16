#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char *start, *end, *buf;
  int size;

  if (argc != 2)
  {
    printf("You should insert the file name\n");
    exit(1);
  }

  printf("Input file name: %s\n", argv[1]);
  fp = fopen(argv[1], "rb");
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  printf("File size: %d\n", size);

  buf = (char *)malloc(size+1);
  fread(buf, sizeof(char), size, fp);
  buf[size] = '\0';
  printf("\n===== Read Content =====\n%s\n", buf);

  fclose(fp);
  return 0;
}
