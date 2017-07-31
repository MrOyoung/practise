#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


static void *handle_thread(void *args)
{
	int *running = (int *)args;
	int pos = 0;

	while(*running)
	{
		fprintf(stdout, "%d\n", pos++);
		sleep(1);
	}

	pthread_exit("handle_pthread exit!!\n");
	
	return NULL;
}


int main(int argc, char *argv[])
{
	int ret;
	int run_flag = 1;
	char *str = NULL; 
	pthread_t pth;	
	
	ret = pthread_create(&pth, NULL, handle_thread, &run_flag);
	if (0 != ret)
	{
		perror("pthread create error\n");
		return -1;
	}

	sleep(5);

	run_flag = 0;

	ret = pthread_join(pth, (void **)&str);
	if (0 != ret)
	{
		perror("pthread join error");
		return -1;
	}

	printf("str is %s\n", str);
	
	return 0;
}
