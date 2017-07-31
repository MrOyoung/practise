#include <stdio.h>
#include <stdlib.h>

const char* ltoa(long l )
{
	static char buffer[256] = {0};

	char* p = buffer + 255;

	int neg = 0;
	if( l < 0 ){
		neg = 1;
		l = -l;
	}

	if( 0 == l )
		return "0";

	while( l ){
		*(--p) = (l % 10) + '0';
		l /= 10;
	}

	if( neg )
		*(--p) = '-';

	return p;
}


int main(int argc,char** argv)
{
	if( argc < 2 ){
		printf("error arg\n");
		exit(1);
	}

	long l = atol( argv[1] );

	printf(" %s \n" , ltoa(l) );

	return 0;
}
