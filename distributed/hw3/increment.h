#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <syscall.h>
#define COUNT 1000000000

extern volatile int l;
unsigned long get_current_microseconds();
int check_operation(int);

FILE *fp;
unsigned long count;

typedef struct locks
{
	int (*lock)(void);
	void (*unlock)(void);
	void * (*sync_main)(void *);
	const char *operation_name;
} LOCK;

typedef struct thread_data
{
	LOCK *sync;
	int num_of_threads;
} ARGS;

void sync_main();

int tas_lock();
void tas_unlock();

int ttas_lock();
void ttas_unlock();

int cas_lock();
void cas_unlock();

int faa_lock();
void faa_unlock();
void faa_main(void *);
