/****************************************************************************
*描述：C语言带常量参数宏的编译时参数参数检查。关键点在于判断X是否为真，
*	当X=0时，DE_UNIQUE_NAME数据下标为-1,在编译阶段就会报错，从而到达检测的目的;
*	在宏定义实现时应使用两次展开，否则会宏定义的使用会直接报错
*日期：2017-06-05
****************************************************************************/
#include <stdio.h>

#define DE_STATIC_ASSERT(X)			typedef char DE_UNIQUE_NAME[(X) ? 1 : -1]
#define DE_UNIQUE_NAME				DE_MAKE_NAME(__LINE__, hoax)
#define DE_MAKE_NAME(line, token)	DE_MAKE_NAME2(line, token)
#define DE_MAKE_NAME2(line, token)	_static_assert_##line##_##token


int main(int argc,char** argv)
{
	DE_STATIC_ASSERT(1 == -1);
	printf("hello wrold\n");
	return 0;
}
