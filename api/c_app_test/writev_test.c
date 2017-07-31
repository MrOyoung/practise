#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include "common.h"

int writev_test(void)
{
	char *str0 = "hello ";
	char *str1 = "world\n";
	struct iovec iov[2];
	ssize_t nwriten;

	iov[0].iov_base = str0;
	iov[0].iov_len = strlen(str0);
	iov[1].iov_base = str1;
	iov[1].iov_len = strlen(str1);
	
	nwriten = writev(STDOUT_FILENO, iov, 2);
	printf("nwriten = %ld\n", nwriten);

	return 0;
}
