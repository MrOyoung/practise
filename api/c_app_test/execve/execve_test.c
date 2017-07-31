#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *arg[] = {NULL, "hello", "world", NULL};
	char *envp[] = {"name=Rokcy", "path=/home/rocky/private", NULL};

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
		return -1;
	}

	arg[0] = argv[1];

	execve(argv[1], arg, envp);
	perror("execv error");

	return 0;
}
