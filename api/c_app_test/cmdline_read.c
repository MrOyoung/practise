#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "cmdline_read.h"


/* whether the xitos is crashed */
int is_xitos_crash(void)
{
	int fd;
	int ret;
	char buf[256] = {0};

	fd = open("/proc/cmdline", O_RDONLY);
	if (-1 == fd)
	{
		perror("open error");
		return -1;
	}

	ret = read(fd, buf, sizeof(buf));
	if (-1 == ret)
	{
		printf("read error");
		ret = -1;
		goto cmdline_label;
	}

	printf("cmdline is %s", buf);

	char *str = strstr(buf, "zeosre");	
	if (NULL == str)
	{
		perror("strstr error");
		ret = -1;
		goto cmdline_label;
	}
	
	if (!strncmp(str, "zeosre=1", strlen("zeosre=1")))
	{
		printf("xitos has been crashed\n");
		ret = 1;
		goto cmdline_label;
	}

	return 0;

cmdline_label:
	close(fd);
	return ret;
}

