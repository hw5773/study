#ifndef __INCREMENT_H__
#define __INCREMENT_H__
#include "increment.h"
#endif

volatile int l;

int check_operation(int n)
{
	if ((n < 0) || (n >= 4))
		return -1;
	else
		return n;
}

unsigned long get_current_microseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int tas_lock()
{
#ifdef DEBUG
	printf("INVOKE: tas_lock() by %d\n", syscall(SYS_gettid)); 
#endif
	return __sync_lock_test_and_set(&l, 1);
}

void tas_unlock()
{
#ifdef DEBUG
	printf("INVOKE: tas_unlock() by %d\n", syscall(SYS_gettid));
#endif
	l = 0;
}

int ttas_lock()
{
#ifdef DEBUG
	printf("INVOKE: ttas_lock() by %d\n", syscall(SYS_gettid)); 
#endif

	while (l == 1);

	return tas_lock();
}

void ttas_unlock()
{
#ifdef DEBUG
	printf("INVOKE: ttas_unlock() by %d\n", syscall(SYS_gettid)); 
#endif
	l = 0;
}

int cas_lock()
{
#ifdef DEBUG
	printf("INVOKE: cas_lock() by %d\n", syscall(SYS_gettid)); 
#endif
	return !(__sync_bool_compare_and_swap(&l, 0, 1));
}

void cas_unlock()
{
#ifdef DEBUG
	printf("INVOKE: cas_unlock() by %d\n", syscall(SYS_gettid)); 
#endif
	l = 0;
}

int faa_lock()
{
#ifdef DEBUG
	printf("INVOKE: faa_lock() by %d\n", syscall(SYS_gettid)); 
#endif
	return 1;
}

void faa_unlock()
{
#ifdef DEBUG
	printf("INVOKE: faa_unlock() by %d\n", syscall(SYS_gettid)); 
#endif
}
