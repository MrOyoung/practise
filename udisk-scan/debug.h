/********************************************
*filename		: debug.h
*description	: debug macro 
*author			: Rocky
*date			: 2017/08/08
********************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <unistd.h>


#define prtRtn(val, fmt, ...) \
	do { printf(fmt, ##__VA_ARGS__); return (val); } while(0)

#define prtExit(val, fmt, ...) \
	do { printf(fmt, ##__VA_ARGS__); exit(val); } while(0)

#define prtMsg(fmt, ...) \
	printf("[file:%s] [func:%s] [line:%d] "fmt" ",\
			__FILE__, __FUNCTION__, __LINE__, \
			 ##__VA_ARGS__)


#endif //_DEBUG_H_
