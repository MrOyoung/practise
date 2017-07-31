#include "common.h"

/*exchange the value of three variable*/
void exchange_value(void)
{
/*
	if (argc < 4)
	{
		printf("Usage:%s value1 value2 value3\n", argv[0]);
		return -1;
	}
*/
	int a, b, c;
	
	printf("please input three nums:");
	scanf("%d %d %d", &a, &b, &c);
	
	a = a + b + c;
	b = a - b;
	b = b - c;
	c = a - b - c;
	a = a - b - c;

	printf("%d %d %d\n", a, b, c);
	
	return;
}
