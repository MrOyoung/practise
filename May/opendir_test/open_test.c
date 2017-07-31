#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#define filename "a.txt"
#define msg "hello\n"


void open_test(const char *log_path, unsigned char *log_msg, unsigned int log_size)
{
	int ret, fd;
	fd = open(log_path, O_CREAT | O_APPEND | O_RDWR);
	if (-1 == fd) perror("open error");

	printf("fd = %d\n", fd);
	ret = write(fd, log_msg, log_size);

	close(fd);

}

void *pthread_test(void *arg)
{

	while(1)
	{
		open_test(filename, msg, strlen(msg));
		sleep(1);
	}
}

int main()
{
	int ret;
	pthread_t pth;

	ret = pthread_create(&pth, NULL, pthread_test, NULL);
	if (ret)
	{
		perror("pthread_create error");
		return -1;
	}

	pthread_join(pth, NULL);

	return 0;
}
