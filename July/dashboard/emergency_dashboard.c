#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <zre/libhealth_mgr/health_mgr.h>
#include <zre/libserial/serial_lib.h>
#include <zre/libvideo/zre_video.h>
#include <pthread-zre.h>

#define HELPER_ID    1

#define IDLE 1
#define RUN 2
void *serial_dev = NULL;
static int should_stop = 1;

static void * helper_func(void *args)
{
	unsigned int width, height;
	unsigned char *pic = getRGB24BMP("rom/1.bmp", &width, &height);
	
	while(1) 
	{
		char read_buffer[512] = {0};
		int i = 0, ret = 0;
		asm volatile("":::"memory");
		if(!should_stop){//主线程收到健康管理模块的异常消息
	
			ret = uart_read(serial_dev, read_buffer,sizeof (read_buffer));
			if(ret > 0){		
				for(i = 0; i < ret; i++)
					printf("%x ", read_buffer[i]);
				printf("\n");
			}			

			display_RGB24BMP(args,1920, 720, pic, width, height, 100, 100);
		}
		sleep(1);
	}

	free(pic);

	return NULL;
}

static void helper_main(unsigned long vram_virt)
{
	pthread_t th;
	long ret;
	int msg;

	printf("helper vram_virt is 0x%lx\n", vram_virt);

	//将自身注册为健康管理模块的helper应用
	ret = health_mgr_register_helper(HELPER_ID);
	if(ret < 0)
	{
		printf("register helper failed\n");
		return;
	}
	
	pthread_create(&th, NULL, helper_func, (void *)vram_virt);
	
	while(1)
	{
		//等待接收健康管理模块的消息，并根据消息类型决定子线程是否工作
		msg = health_mgr_get_msg();

		if(msg < 0)
			continue;		
		if(msg == RUN)
		{
			printf("helper recve mesg to run\n");
			should_stop = 0;
		}
		else if(msg == IDLE)
		{
			printf("helper recve mesg to stop\n");
			should_stop = 1;
		}
		asm volatile("":::"memory");
	}
}


int
main(void)
{
	unsigned long vram_virt=0;
	printf("begin emergy_dash\n");
	
	vram_virt = vram_init();//初始化显示及串口
	if(!vram_virt)
	{
		printf("map vram failed\n");
		return 0;
	}
	
	serial_dev = uart_open_raw(1);
	if(!serial_dev)
	{
		printf("open raw uart failed 2\n");
		return 0;
	}

	
	helper_main(vram_virt);
	return 0;
}
