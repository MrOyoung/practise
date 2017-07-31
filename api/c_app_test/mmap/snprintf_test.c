#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int ret = 0;
	char str[1024] = {0};

	ret = snprintf(str, strlen("hello"), "%s", "hello");
	printf("ret = %d\n", ret);

	return 0;
}
