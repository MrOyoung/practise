#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#if _YDP_

sem_t g_sem;
volatile int running;
volatile int thread_exit = 0;

static void* other_thread(void* argv)
{
	while(1) {

		sem_wait( &g_sem );

		if( thread_exit )
			break;

		while( running ){
		/*main loop begin*/


		/*main loop end*/
		}
	
	}

}

#endif

#if _PTHREAD_CANCEL_

static void *handler(void *arg)
{
	int s;

	s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (s != 0)
	{
		perror("pthread setcancelstate error");
		return NULL;
	}

	printf("pthread cancelltion disable!!\n");
	sleep(5);
	printf("abort to enable cancelltion!\n");

	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (s != 0)
	{
		perror("pthread setcancelstate error");
		return NULL;
	}

	printf("thread cancelled!!\n");
	sleep(2);

	printf("thread not cancelled!!\n");
}


int main(int argc, char *argv[])
{
	void *ret_str;
	pthread_t pth;

	int ret = pthread_create(&pth, NULL, handler, NULL);
	if (0 != ret)
	{
		perror("pthread create error");
		return -1;
	}
	
	sleep(5);

	ret = pthread_cancel(pth);
	if (0 != ret)
	{
		perror("pthread cancel error");
		return -1;
	}

	ret = pthread_join(pth, &ret_str);
	if (0 != ret)
	{
		perror("pthread join error");
		return -1;
	}

	if (ret_str == PTHREAD_CANCELED)
		printf("main func has cancel the pthread!!!\n");
	else
		printf("main func has not cancel the pthread!!\n");
		
	return 0;
}

#endif

static void *handler(void *arg)
{
	unsigned int *is_running = (unsigned int *)arg;
	int pos = 0;

	while(*is_running)
	{
		printf("pos = %d\n", pos++);	
		sleep(1);
	}

	printf("thread exit!!\n");

	pthread_exit("exit");
	
	return NULL;
}


int main(int argc, char *argv[])
{
	void *ret_str;
	int running = 1;	
	pthread_t pth;

	int ret = pthread_create(&pth, NULL, handler, (void *)&running);
	if (0 != ret)
	{
		perror("pthread create error");
		return -1;
	}
	
	sleep(5);

	running = 0;

	ret = pthread_join(pth, &ret_str);
	if (0 != ret)
	{
		perror("pthread join error");
		return -1;
	}

	printf("receive from thread %s!!!\n", (char *)ret_str);
	
	return 0;
}
