/* None Thread Safe : func localtime() */
#include "localtime.h"

void *localtime_pth1(void *arg)
{
	printf("int pth1\n");
	
	time_t		tm0;
	struct tm 	*tm1;

	tm0 = time(NULL);
	if (-1 == tm0)
		perror("time error");

	sleep(2);
	//tm1 = localtime(&tm0); //NTF
	localtime_r(&tm0, tm1);
	//if (NULL == tm1)
	//	perror("localtime error");


	printf("pth1 : %04d-%02d-%02d %02d:%02d:%02d\n",
			tm1->tm_year + 1900,
			tm1->tm_mon + 1,
			tm1->tm_mday,
			tm1->tm_hour,
			tm1->tm_min,
			tm1->tm_sec);

}

void *localtime_pth2(void *arg)
{
	printf("int pth2\n");
	
	time_t 		tm0;
	struct tm 	*tm2;

	tm0 = time(NULL);
	if (-1 == tm0)
		perror("time error");

	//tm2 = localtime(&tm0);//NTF 
	localtime_r(&tm0, tm2); 
	if (NULL == tm2)
		perror("localtime error");

/*	printf("pth2 : %04d-%02d-%02d %02d:%02d:%02d\n",
			tm2->tm_year + 1900,
			tm2->tm_mon + 1,
			tm2->tm_mday,
			tm2->tm_hour,
			tm2->tm_min,
	 		tm2->tm_sec);
*/			
}

