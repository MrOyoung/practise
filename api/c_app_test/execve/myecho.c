#include <stdio.h>

int main(int argc, char *argv[], char *envp[] )
{
	int i = 0;

	for (i; i < argc; i++)
	{
		printf("argv[%d] is %s\n", i, argv[i]);
	}

	i = 0;
	while(envp[i])
	{
		printf("envp[%d] is %s\n", i, envp[i++]);	
	}

	return 0;
}
