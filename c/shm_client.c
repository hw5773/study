#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ 30

int main()
{
  int shmid;
  key_t key;
  char *shm, *s;

  key = 9778;

  if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
  {
    perror("shmget");
    exit(1);
  }

  if (!(shm = shmat(shmid, NULL, 0)))
  {
    perror("shmat");
    exit(1);
  }

  for (s=shm; *s!='\t'; s++)
    putchar((char)*s);
  putchar((char)'\n');

  *shm = '*';

  return 0;
}
