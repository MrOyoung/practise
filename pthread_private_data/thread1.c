#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thread1.h"

static pthread_key_t key;

void func1(void)
{
	int *num = (int *)pthread_getspecific(key);

	*num += 1; 

	return;
}

void *thread1(void *arg)
{
	pthread_key_create(&key, NULL);

	pthread_setspecific(key, arg);
	printf("thread id = %lu...num = %d\n", pthread_self(), *(int *)pthread_getspecific(key));

	sleep(2);

	func1();
	printf("thread id = %lu...num = %d\n", pthread_self(), *(int *)pthread_getspecific(key));


	pthread_key_delete(key);
	return NULL;
}

