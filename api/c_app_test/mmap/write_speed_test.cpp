/****************************************************************
 * 说明: 用不同的方法，测试写文件的速度，测试结果表明
 *       改变COUNT的值，将改变写入文件的大小
 *      测试CPU使用率方法： /usr/bin/time -f "CPU: %P" ./a.out 1 
 * ************************************************************/
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <malloc.h>
#include <sys/time.h>

#define WRITE_FILE        /*定义这个宏，则把数据写到文件，否则不写，用于测试不写文件用的时间*/
const unsigned short usPageSize = 4*1024;            /*4K*/
const unsigned long STR_LEN = (usPageSize*1024) +1;      /*每次缓存4M有效数据写入文件 ，最后一个字节保证 null-terminated ，不写入文件*/
const unsigned long COUNT = 256;         /*改变COUNT的值，将改变写入文件的大小 256 --->1G  16 ---> 64M 请问 为什么512 2G出现总线错误？  */

char src[1024 + 1];

void fwrite_write2();
void mmap_write5_1();

int main(int argc, char *argv[])
{
	int iFlowLen = 0;
	int iNum = 1;
	
	if(1==argc || 0==strcmp("--help", argv[1]))
	{
		printf("\nusage:"
				"\n\t run all :              %s "
				"\n\t run fwrite_write2():   %s 1"  
				"\n\t run mmap_write5_1():   %s 2\n",
				argv[0], argv[0], argv[0]
			  );
		exit(0); 
	} 
	if(3==argc)
	{
		iNum = atoi(argv[2]);
	}

	for (iFlowLen = 0; iFlowLen < 1024; iFlowLen++)
	{
		*(src + iFlowLen) = '0';
	}
	
	while(iNum--)
	{
		if(1==argc)
		{
			fwrite_write2();
			sleep(2);
			mmap_write5_1();
		}
		else if(1 == atof(argv[1]))
		{
			fwrite_write2();   
		}
		else if(2 == atof(argv[1]))
		{
			mmap_write5_1();   
		}
	}

}

 
unsigned long get_tick_count()
{
	struct timeval tm;

	gettimeofday(&tm, NULL);

	return (tm.tv_sec * 1000000 + tm.tv_usec);
}


/****************************************************************
 * 功能: 用fwrite，（使用系统缓存）测试写文件速度
 * ************************************************************/
void fwrite_write2()
{   
	int i = 0;
	FILE *fp = NULL;
	unsigned long  start_time;

	char strLeftData[2048] = {0};   
	unsigned short strSize = sizeof(strLeftData);

	start_time = get_tick_count();

	fp = fopen("data.txt", "w+");

	for (i=0; i < 4*1024; i++)
	{
		fwrite(src, 1024, 1, fp);
	}
	int fd = fileno(fp);
	fsync(fd);

	fclose(fp);
	printf("fwrite_write2 all time diff:   %lu\n\n", get_tick_count() - start_time);
}

/****************************************************************
 * 功能: 用mmap每次一次性映射全部文件，测试写文件速度
 * ************************************************************/
void mmap_write5_1()
{    
	int i = 0, j = 0;
	char *pMap = NULL;
	int fd;
	unsigned long  start_time;

	start_time = get_tick_count();

	fd = open("data.txt",O_RDWR|O_CREAT,00600);
	pMap = (char *)mmap(0, 4 * 1024 * 1024, PROT_WRITE, MAP_SHARED, fd, 0); // 一次全部打开文件映射（内存要求）
	ftruncate(fd,  (4 * 1024 * 1024) );
	
	for (j=0;  j< 4 * 1024; j++)
	{        
		memcpy(pMap + j * 1024, src, 1024);
	}
	
	msync(pMap, (4 * 1024 * 1024), MS_ASYNC);
	munmap(pMap, (4 * 1024 * 1024));
	close(fd);

	printf("mmap_write5_1 all time diff:   %lu\n\n", get_tick_count() - start_time);
}
