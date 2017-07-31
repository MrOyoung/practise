/************************************************************
**filename:structure.c
**autor:Rocky
**date:2017/04/17
**description:practice the usage of structure
************************************************************/
#include "common.h"

typedef struct
{
	long c;
	short d;
	int b;
	char a[17];
	//char a;
} test;

int structure(void)
{
	test one;
	
	printf("%ld %ld %ld\n", sizeof(test), sizeof(one.d), sizeof(long));

	return 0;
}

