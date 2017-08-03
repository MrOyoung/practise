#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thread1.h"
#include "thread2.h"

int main(void)
{
	pthread_t th1, th2;

	int num = 3;

	if (-1 == pthread_create(&th1, NULL, thread1, (void *)&num))
	{
		perror("thread1 create error");
		return -1;
	}

	sleep(1);

	if (-1 == pthread_create(&th2, NULL, thread2, (void *)&num))
	{
		perror("thread2 create error");
		return -1;
	}

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	return 0;
}
