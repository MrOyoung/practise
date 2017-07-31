/************************************************************
**filename:debug.c
**autor:Rocky
**date:2017/05/10
**description:practice the usage of va_start
************************************************************/
#include "c_app_test.h"
#include <stdarg.h>
#include <unistd.h>


char *itoa(const int itemp, char *stemp)
{
	char int_str[16] = {0};
	
	sprintf(int_str, "%d", itemp);
	strncpy(stemp, int_str, strlen(int_str));
	
	return stemp;
}



/************************************************************************
**函数名：va_start
**头文件：#include <stdarg.h>
**函数原型：void va_start(va_list ap, last);
	ap		： _arg_out，用于保存函数参数列表中可变参数的首指针（即，可变参数列表）
	last	： _arg_in，为函数参数列表中最后一个固定参数
**描述：获取函数参数列表中可变参数的首指针（获取函数可变参数列表）
	va_list	：表示可变参数列表类型，实际上就是一个char指针
**返回值：void
************************************************************************
**函数名：va_arg
**函数原型：void va_arg(va_list ap, type);
	ap		： _arg_in，可变参数列表，指向当前正要处理的可变参数
	type	： _arg_in，正要处理的可变参数的类型
**描述：获取当前ap所指向的可变参数，并将ap指针移向下一可变参数
**返回值：当前可变参数的值
************************************************************************
**函数名：va_end
**函数原型：void va_end(va_list ap);
	ap		： _arg_in，可变参数列表，指向当前正要处理的可变参数
**描述：结束对可变参数的处理。实际上va_end被定义为空，它只是为实现与va_start配对
	（实现代码对称和“代码自动注释”功能）
**返回值：void
************************************************************************
**作者：rocky
**日期：2017/05/11
************************************************************************/
void foo(char *str, char *fmt, ...)
{
	va_list ap;
	int d, pos = 0;
	char c, *s;
	char *buf = NULL;
	//char int_buf[32] = {0}; 
	
	va_start(ap, fmt);
	while(*fmt)
	{
		if ('%' !=  *fmt)
		{
			//putchar(*fmt);
			sprintf(str + pos, "%c", *fmt); pos++;
			fmt++; continue;
		}
		++fmt;
		switch (*fmt)
		{
			case '%':
				str[pos] = '%';pos++;
				break;
			case 's':			/* string */
				s = va_arg(ap, char *);
				sprintf(str + pos, "%s", s); pos+=strlen(s);
				break;
			case 'd':			/* int */
				d = va_arg(ap, int);
				
				//memset(int_buf, 0, sizeof(int_buf));
				//sprintf(int_buf, "%d", d);
		
				//sprintf(str + pos, "%s", int_buf); pos+=strlen(int_buf);
				
				buf = itoa(d, str + pos); pos += strlen(buf);
				
				
				break;
			case 'c':			/* char */
				c = (char) va_arg(ap, int);
				sprintf(str + pos, "%c", *fmt); pos+=sizeof(char);
				break;
			default:
				break;		
		}
		++fmt;
	}
	va_end(ap);
	
	printf("str is %s\n", str);

}

void multi_para()
{
#if 1
	char a = 'a';
	int b = 64;
	char c[64] = "string";
	
	char debug_buf[128] = {0};
	
	foo(debug_buf, "char is %c, int is %d%%, string is %s", a, b, c);
#endif

}

