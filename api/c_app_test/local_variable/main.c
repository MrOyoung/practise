#include <stdio.h>


static char *ret_str()
{
	static char *p = "hello c!\n";
	
	return p;
}

static char *static_ret()
{
	char *tmp = NULL;

	tmp = ret_str();

	return tmp;
}


int main(int argc, char *argv[])
{
	char *p = static_ret();

	printf("p is %s\n", p);

	return 0;
}
