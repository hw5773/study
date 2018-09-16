#include <time.h>
#include <stdio.h>

long long get_time_diff(unsigned int n_flag);

int main()
{
  time_t t;
  int i;
  t = time(NULL);

  printf("t: %ld\n", t);
  printf("sizeof(t): %ld\n", sizeof(time_t));

  get_time_diff(0);
  for (i=0; i<10000000; i++)
  {}
  printf("%lld\n", get_time_diff(1));

  return 0;
}

long long get_time_diff(unsigned int n_flag)
{
  const long long NANOS = 1000000000LL;
  static struct timespec start_ts, end_ts;
  static long long ret_diff = 0;

  if (n_flag == 0)
  {
    ret_diff = 0;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_ts) == -1)
      printf("failed to call clock_gettime\n");
  }
  else
  {
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_ts) == -1)
      printf("failed to call clock_gettime\n");
    ret_diff = NANOS * (end_ts.tv_sec - start_ts.tv_sec) + (end_ts.tv_nsec - start_ts.tv_nsec);
  }

  return ret_diff / 1000000;
}
