/************************************************************
**filename:str_test.c
**autor:Rocky
**date:2017/05/19
**description:deal with string
************************************************************/
#include <sys/time.h>
#include "common.h"

#define hex_to_string( hex )

typedef enum
{
	true = 0,
	false = 1,
} BOOL;


unsigned long get_clocktick()
{
	struct timeval val;
	
	gettimeofday(&val, NULL);

	return (val.tv_sec * 1000000 + val.tv_usec);
}

/************************************************************
**头文件：#include <stdlib.h>
**函数原型：int atoi(const char *str)
**函数描述：扫描参数str字符串，跳过前面的空白字符（例如空格，tab缩进等，
	可以通过isspace()函数来检测），直到遇上数字或正负符号才开始做转换，
	而再遇到非数字或者字符串结束时（'\0'）才结束转换，并将结果返回
**返回值：返回转换后的整型数，如果str不能转换成int或者str为空字符串，则返回0
************************************************************/
int my_atoi(char *pstr)
{
	int val = 0;
	int flag = 1;

	//判断指针是否为空
	if (NULL == pstr)
	{
		printf("pointer is NULL\n");
		return 0;
	}

	//跳过前面的空格
	if (' ' == *pstr) pstr++;

	//判断是否为负数
	if ('-' == *pstr) flag = -1;
	if (('-' == *pstr) || ('+' == *pstr)) pstr++; 

	//转换数字部分
	while((*pstr >= '0') && (*pstr <= '9'))
	{
		val = val * 10 + (*pstr - '0');
		pstr++;
	}
	
	return val * flag;
}


void atoi_test(void)
{
	char num_str[16] = "-123456789-10";

	printf("%d\n", my_atoi(num_str));
}

static char hex2str(char tmp)
{
	static char convert[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f',
	};

	return convert[tmp];
}


/*****************************
**功能：将十六进制的字符转换成字符形式
**对比：使用sprintf()标准函数和自定义的字符串转换函数完成上述功能，使用get_clocktick()函数测试转换所需时间
**测试结果：使用sprintf()函数耗时在8～30us之间，而使用自定义的函数接口，耗时基本为0us
**结论：在效率上，使用自定义的函数而不是标准函数，能减少CPU资源的占用
*****************************/
void hex2str_test()
{
	int index = 0;
	unsigned long start_time;
	char arr[16] = {0x12, 0x23, 0x34, 0x45, 0x56};
	char str[64] = {0};

	start_time = get_clocktick();

	for (int i = 0; i < sizeof(arr); i++)
	{
		str[index] = hex2str(arr[i] >> 4); index++;
		str[index] = hex2str(arr[i] & 0x0f); index++;
	}

	//sprintf(str, "%x", arr[0]);

	printf("%lu\n", get_clocktick() - start_time);
	printf("%s\n", str);
}


int str_test(void)
{
	//hex2str_test();
	atoi_test();
	return 0;
}

