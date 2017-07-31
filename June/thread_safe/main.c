#include <pthread.h>
#include "localtime.h"

int main(int argc, char *argv[])
{
	pthread_t 	pth1;
	pthread_t 	pth2;
	int 		ret;

	ret = pthread_create(&pth1, NULL, &localtime_pth1, NULL);
	if (ret)
	{
		perror("pthread_create pth1 error");
	}
	else
	{
		printf("pthread_create pth1 success\n");
	}
#if 0
	ret = pthread_create(&pth2, NULL, &localtime_pth2, NULL);
	if (ret)
	{
		perror("pthread_create pth2 error");
	}
	else
	{
		printf("pthread_create pth2 success\n");
	}

	ret = pthread_join(pth2, NULL);
	if (ret)
		perror("pthread_join pth2 error");
#endif
	ret = pthread_join(pth1, NULL);
	if (ret)
		perror("pthread_join pth1 error");

	return 0;
}
