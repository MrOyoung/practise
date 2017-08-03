#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thread2.h"

static pthread_key_t key;

void func2(void)
{
	int *num = (int *)pthread_getspecific(key);

	*num += 1; 

	return;
}

void *thread2(void *arg)
{
	pthread_key_create(&key, NULL);

	pthread_setspecific(key, arg);
	printf("thread id = %lu...num = %d\n", pthread_self(), *(int *)pthread_getspecific(key));

	func2();
	printf("thread id = %lu...num = %d\n", pthread_self(), *(int *)pthread_getspecific(key));


	pthread_key_delete(key);
	return NULL;
}

