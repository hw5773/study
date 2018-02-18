#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  FILE *fp1 = fopen("t.txt", "w");
  FILE *fp2 = fopen("t.txt", "r");

  unsigned char *test = "test";
  unsigned char buf[1024];
  fprintf(fp1, "%s\n", test);
  fclose(fp1);
  fscanf(fp2, "%s\n", buf);
  fclose(fp2);
  printf("result: %s\n", buf);

  return 0;
}
