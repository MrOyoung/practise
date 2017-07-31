#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
	if( argc < 2 ){
		printf("i dont know sleep how long!!!\n");
		exit(0);
	}

	usleep( (useconds_t)(atoi(argv[1]) * 1000) );
	return 0;
}

