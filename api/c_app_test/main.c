#include "common.h"


int main(int argc, char *argv[])
{
	//exchange_value();	
	
	//file_auto_print("/home/rocky/private/api/c_app_test/a.txt");

	//mutex_test();
#if 0
	int length = 4;
	printf("%d %d\n", parameter(length), length);
#endif
	//测试writev的效果
	//writev_test();
	
	//测试结构体字节对奇
	//structure();
	
	/*测试stat()函数*/
	/*if (argc != 2)
	{
		printf("Usage : %s <pathname>\n", argv[0]);
		return -1;
	}
	
	printf("ret = %d\n", stat_test(argv[1]));
	*/

#ifdef _MY_LS_
	/*实现ls -l的查询效果*/
	do_scan(argc, argv);
#endif	

	/*测试环境变量相关API*/
	//printf("ret = %d\n", set_env("ENV_TEST", "meet you in the most beautiful time", 1));
	//printf("env is %s\n", get_env("ENV_TEST"));
	
	//malloc_test();
	
#if 0	
	unsigned char tmp[128] = {0};
	MSG_ID a = ENV_SERVICE_SET;
	
	memcpy(tmp, &a, sizeof(a));
	memcpy(tmp + sizeof(a), "PATH=/usr/bin", strlen("USER=/usr/bin") + 1);
	
	env_test(tmp, sizeof(a) + strlen("USER=/usr/bin"));
	
	memset(tmp, 0, 128);
	a = ENV_SERVICE_GET;
	
	memcpy(tmp, &a, sizeof(a));
	memcpy(tmp + sizeof(a), "PAOH=/usr/bin", strlen("USER=/usr/bin") + 1);
	
	env_test(tmp, sizeof(a) + strlen("USER=/usr/bin"));
#endif

	//测试函数传参可变，va_start()/va_arg()/va_end()
	//multi_para();

	//测试自定义的字符串操作函数
	//str_test();

#ifdef _MY_SPRINTF_

	sprintf_test();	
	
#endif


	is_xitos_crash();	

	return 0;
}
