/************************************************************
 **autor:Rocky
 **date:2017/05/22
 **description:practice the usage of aio_read() -man aio
 ************************************************************/
#include "aio_read.h"

char bytelow4bit_to_hex( unsigned char c )
{
	static  char hexs[] = { '0' , '1' , '2' , '3' ,
		'4' , '5' , '6' , '7' ,
		'8' , '9' , 'A' , 'B' ,
		'C' , 'D' , 'E' , 'F' };		
	return hexs[ c & 0xf ];
}

unsigned long get_clocktick()
{
	struct timeval val;

	gettimeofday(&val, NULL);

	return (val.tv_sec * 1000000 + val.tv_usec);
}


void print_buffer_hex( unsigned char* buf , int read_size )
{
	int i, line_size, index = 0;
	char print_buffer[BUFFER_SIZE];
	for (i = 0; i < read_size; i++){
		print_buffer[index++] = '0';
		print_buffer[index++] = 'x';
		print_buffer[index++] = bytelow4bit_to_hex( (buf[i]) >> 4 ) ;
		print_buffer[index++] = bytelow4bit_to_hex( (buf[i]) >> 0 );
		print_buffer[index++] = ' ';

		if( ++line_size ==  16 ){
			print_buffer[index++] = '\n';
			line_size = 0;
		}	
	}
	print_buffer[index++] = '\0';
	printf("%s\n", print_buffer );
}

int myAioRead(int fileFd, char *tmpBuf)
{
	struct aiocb rd;
	int i, fd, ret = 0, couter = 5, line_size;
	int read_size = 0, index = 0;
	unsigned long startTime;
	_DATA_BUFFER mcuDataBuf[2]; //设置交叉缓冲区，交叉存放mcu数据和printf打印

	memset(mcuDataBuf, 0, sizeof(mcuDataBuf));
	memset(&rd, 0, sizeof(rd));

	if (NULL == tmpBuf)
	{
		printf("tmpBuf is BULL\n");
		return -1;
	}
	//volatile void*类型 
	//rd.aio_buf = tmpBuf;
	//填充rd结构体
	rd.aio_fildes = fileFd;
	rd.aio_nbytes = 212;
	rd.aio_offset = 0;

	startTime = get_clocktick();

	while (1)
	{
		//volatile void*类型 
		rd.aio_buf = mcuDataBuf[index].buf;
		
		//进行异步读写
		ret = aio_read(&rd);
		if (ret < 0)
		{
			perror("aio_read");
			exit(1);
		}

		if( mcuDataBuf[index ^ 1].size > 0)
			print_buffer_hex(mcuDataBuf[index ^ 1].buf, mcuDataBuf[index ^ 1].size);

		struct aiocb* aio_arrya[] = {
			&rd,
		};

		//阻塞
		ret = aio_suspend(aio_arrya, 1, NULL);

		//获取异步读返回值
		ret = aio_return(&rd);
		printf("\n\nreturn value = %d\n", ret);

		mcuDataBuf[index ^ 1].size = ret;
		index ^= 1;
	}

	return 0;
}
