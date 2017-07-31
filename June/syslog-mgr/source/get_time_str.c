/************************************************************
 **filename:get_time_str.c
 **autor:YinDP
 **date:2017/06/23
 **description:get current time
 ************************************************************/

#include "../include/get_time_str.h"

#define DIG1 1
#define DIG2 10
#define DIG3 100
#define DIG4 1000

#define DIGITAL( num , dig ) ((((num) / DIG##dig) % 10) + '0' )

#define DIGITAL4_INC( __p , num ) \
	*__p++ = DIGITAL(num,4);\
	*__p++ = DIGITAL(num,3);\
	*__p++ = DIGITAL(num,2);\
	*__p++ = DIGITAL(num,1);\

#define DIGITAL2_INC( __p , num ) \
	*__p++ = DIGITAL(num,2);\
	*__p++ = DIGITAL(num,1);\


int make_current_time_str(char* buf)
{
	time_t t0;
	struct tm t1;
	char* p = buf;

	/*get current time*/
	time( &t0 );
	localtime_r( &t0, &t1 );

	/*year*/
	int year = t1.tm_year + 1900;
	DIGITAL4_INC( p , year );
	*p++ = '-';

	/*month*/
	DIGITAL2_INC( p , t1.tm_mon + 1);
	*p++ = '-';

	/*day*/
	DIGITAL2_INC( p , t1.tm_mday);
	*p++ = ' ';

	/*hour*/
	DIGITAL2_INC( p , t1.tm_hour);
	*p++ = ':';

	/*min*/
	DIGITAL2_INC( p , t1.tm_min);
	*p++ = ':';

	/*second*/
	DIGITAL2_INC( p , t1.tm_sec);
	*p++ = ' ';
	*p   = '\0';

	return (p - buf);
}

