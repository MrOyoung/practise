/************************************************************
**filename:mutex_test.c
**autor:Rocky
**date:2017/04/11
**description:practice the usage of mutex
************************************************************/
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include "common.h"

void *fun1(void *arg);
void *fun2(void *arg);
void *fun3(void *arg);

int buffer = 0;
pthread_mutex_t mutex;
int running = 1;

int mutex_test(void)
{
	pthread_t pth1;
	pthread_t pth2;
	pthread_t pth3;

	//init mutex
	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&pth1, NULL, fun1, (void *)&running);
	pthread_create(&pth2, NULL, fun2, (void *)&running);
	pthread_create(&pth3, NULL, fun3, (void *)&running);
	
	usleep(1000);
	running = 0;
	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);
	pthread_join(pth3, NULL);
	//free mutex
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *fun1(void *arg)
{
	printf("fun1\n");
	while(*(int *)arg)
	{
		pthread_mutex_lock(&mutex);
		printf("in fun1 before add, buffer is : %d\n", buffer);
		usleep(2);
		buffer++;
		printf("in fun1 after sleep and add one, now buffer is : %d\n", buffer);
		pthread_mutex_unlock(&mutex);
		usleep(2);
	}
	
	return NULL;
}

void *fun2(void *arg)
{
	printf("fun2\n");
	while(*(int *)arg)
	{
		pthread_mutex_lock(&mutex);
		printf("in fun2 before add, buffer is : %d\n", buffer);
		usleep(2);
		buffer++;
		printf("in fun2 after sleep and add one, now buffer is : %d\n", buffer);
		pthread_mutex_unlock(&mutex);
		usleep(2);
	}
	
	return NULL;
}

//the following is the result without mutex
/*
in fun2 before add, buffer is : 0
in fun1 before add, buffer is : 0
in fun2 after sleep and add one, now buffer is 1
in fun1 after sleep and add one, now buffer is : 2
in fun2 before add, buffer is : 2
in fun1 before add, buffer is : 2
in fun2 after sleep and add one, now buffer is 3
in fun1 after sleep and add one, now buffer is : 4
in fun2 before add, buffer is : 4
in fun1 before add, buffer is : 4
in fun2 after sleep and add one, now buffer is 5
in fun1 after sleep and add one, now buffer is : 6
in fun2 before add, buffer is : 6
in fun1 before add, buffer is : 6
in fun2 after sleep and add one, now buffer is 7
in fun1 after sleep and add one, now buffer is : 8
in fun2 before add, buffer is : 8
in fun1 before add, buffer is : 8
in fun2 after sleep and add one, now buffer is 9
in fun1 after sleep and add one, now buffer is : 10
in fun2 before add, buffer is : 10
in fun1 before add, buffer is : 10
in fun2 after sleep and add one, now buffer is 11
in fun1 after sleep and add one, now buffer is : 12
in fun2 before add, buffer is : 12
in fun1 before add, buffer is : 12
in fun2 after sleep and add one, now buffer is 13
in fun1 after sleep and add one, now buffer is : 14
in fun2 before add, buffer is : 14
in fun1 before add, buffer is : 14
in fun2 after sleep and add one, now buffer is 15
in fun1 after sleep and add one, now buffer is : 16
in fun2 before add, buffer is : 16
in fun1 before add, buffer is : 16
in fun2 after sleep and add one, now buffer is 17
in fun1 after sleep and add one, now buffer is : 18
*/

//the following is the result with mutex
/*
in fun1 before add, buffer is : 0
in fun1 after sleep and add one, now buffer is : 1
in fun2 before add, buffer is : 1
in fun2 after sleep and add one, now buffer is : 2
in fun1 before add, buffer is : 2
in fun1 after sleep and add one, now buffer is : 3
in fun2 before add, buffer is : 3
in fun2 after sleep and add one, now buffer is : 4
in fun1 before add, buffer is : 4
in fun1 after sleep and add one, now buffer is : 5
in fun2 before add, buffer is : 5
in fun2 after sleep and add one, now buffer is : 6
in fun1 before add, buffer is : 6
in fun1 after sleep and add one, now buffer is : 7
in fun2 before add, buffer is : 7
in fun2 after sleep and add one, now buffer is : 8
in fun1 before add, buffer is : 8
in fun1 after sleep and add one, now buffer is : 9
*/


void *fun3(void *arg)
{
	printf("fun3\n");
	while(*(int *)arg)
	{
		pthread_mutex_lock(&mutex);
		printf("in fun3 before add, buffer is : %d\n", buffer);
		usleep(2);
		buffer++;
		printf("in fun3 after sleep and add one, now buffer is : %d\n", buffer);
		pthread_mutex_unlock(&mutex);
		usleep(2);
	}
	return NULL;
}

