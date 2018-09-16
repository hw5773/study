#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ 4096

int main()
{
  char c;
  int shmid;
  key_t key;
  char *shm, *s;

  key = 9778;

  if ((shmid = shmget(key, SHMSZ, IPC_CREAT|0666)) < 0)
  {
    perror("shmget");
    exit(1);
  }

  if (!(shm = shmat(shmid, NULL, 0)))
  {
    perror("shmat");
    exit(1);
  }

  s = shm;

  for (c='a'; c<'z'; c++)
    *s++=c;
  *s = '\t';

  while (*shm != '*')
    sleep(1);

  return 0;
}
