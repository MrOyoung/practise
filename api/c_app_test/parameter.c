/************************************************************
**filename:parameter.c
**autor:Rocky
**date:2017/04/11
**description:practice the parameter
************************************************************/
#include "common.h"

int strstr_test();

int parameter_specify(int len)
{
	//char buf[1024] = {'a','d','d'};
	//printf("sizeof(buf) = %lu %d\n", sizeof(buf), (int)strlen(buf));
	
	char *p = NULL;
	p = (char *)malloc(1024);
	//printf("sizeof(p) = %d\n", (int)sizeof(p));
	free(p);
	
	len += 1;
	return len;
}

int parameter(int len)
{
	strstr_test();

	return parameter_specify(len);
}


/************************************************************************
**函数名：strstr
**头文件：#include <strings.h>
**函数原型：char *strstr(const char *haystack, const char *needle);
**描述：在haystack字符串中查找needle第一次出现的地方
**返回值：第一次出现该字符串处的指针，或null（未发现needle）
************************************************************************
**函数名：index
**头文件：#include <strings.h>
**函数原型：char *index(const char *s, int c);
**描述：在字符串s中查找字符c第一次出现的地方
**返回值：第一次出现该字符处的指针，或null（未发现needle）
************************************************************************
**函数名：strtoul
**头文件：#include <stdlib.h>
**函数原型：unsigned long int strtoul(const char *nptr, char **endptr, int base);
**描述：函数会将参数nptr字符串根据参数base来转换成无符号的长整型数。参数base范围从2至36,或0。
		参数	base代表采用的进制方式，如base值为10则采用10进制，若base值为16则采用16进制数等。当
		base值为0时会根据情况选择用哪种进制;如果第一个字符是‘0’，就判断第二个字符如果是‘x’则用16
		进制，否则用8进制;第一个字符不是‘0’，则用10进制。一开始strtoul()会扫描参数nptr字符串，跳
		过前面的空格字符串，直到遇上数字或者正负符号才开始做转换，再遇到非数字或字符串结束时(")结束
		结束转换，并将结果返回。若参数endptr不为NULL，则会将遇到不合条件而终止的nptr中的字符指针由
		endptr返回。
**返回值：返回转换后的长整型数，否则返回ERANGE并将错误代码存入errno中。
************************************************************************
**作者：rocky
**日期：2017.05.09
************************************************************************/

int strstr_test(void)
{
	char buf[1024] = "These functions return a pointer to the beginning of the  located  sub‐string, 0x12345678 or NULL if the substring is not found.";
	char *tmp, *rev, *rev_ret;
	unsigned long sys_rev;
	
	//memcpy(buf, example, sizeof(example));
	
	tmp = strstr(buf, "functions");
	if (tmp)//若不加tmp的非空判断，返回为null时直接打印会出现段错误
	{
		printf("%s\n", tmp);
		rev = index(tmp, ',');
		if (rev)
		{
			printf("%s\n", rev);
			rev++;
			sys_rev = strtoul(rev, &rev_ret, 0);
			printf("sys_rev = %x\n", (unsigned int)sys_rev);
			printf("rev_ret is%s\n", rev_ret);
		}
		
	}

	return 0;
}
