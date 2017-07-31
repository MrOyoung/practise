/*how mant usecs will the sprintf() take*/

#include "vsnprintf.h"

unsigned long getclick(void)
{
	struct timeval tm;

	gettimeofday(&tm, NULL);

	return (tm.tv_sec * 1000000 + tm.tv_usec);
}

static int my_vsnprintf(char *ptr, char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	
	ret = vsnprintf(ptr, 1024, fmt, ap);
	if (ret <= 1024)
	{
		return ret;
	}

	va_end(ap);

	return ret;
}


void sprintf_test(void)
{
	unsigned long start_time;
	char buf[1024] = {0};
	
	start_time = getclick();

	my_vsnprintf(buf, "%s/%s", "log", "main");
	
	printf("time = %lu\n", getclick() - start_time);
	printf("ptr1 is %s\n", buf);
	
	start_time = getclick();

	sprintf(buf, "%s/%s", "log", "main");
	
	printf("time = %lu\n", getclick() - start_time);
	printf("ptr2 is %s\n", buf);

}
