#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#define ZPVIO 0xAE

#define	ZPV_REGISTER_MON				_IOW(ZPVIO,  0x00, struct zpv_mon_info)//注册
#define	ZPV_KEEP_ALIVE					_IO(ZPVIO,   0x01)//发送保活信息
#define	ZPV_UNREGISTER_MON				_IO(ZPVIO,   0x02)//注销


enum EXC_ACTION{
  Nothing = 0,
  CALL_HELPER = 0x1,
  KILL = 0x2,
  RESTART = 0x4,
  KEEP_HWD = 0x1000,
};


struct zpv_mon_info{
	int alive_time;//超时时间
	int action;//
	int helper_id;//注册ID
};


#define ZPV_MISC_DEV "/dev/zpv"//设备节点


int main(int argc, char *argv[])
{
	struct zpv_mon_info minfo;
	int fd, ret;
	int reg = 1;
	if(argc == 2)
	  reg = atoi(argv[1]);
	
	printf("dashboard is running\n");

	minfo.alive_time = 5;
	minfo.action = KILL | RESTART | KEEP_HWD;
	minfo.helper_id = 1;

	fd = open(ZPV_MISC_DEV, O_RDONLY);
	if(fd < 0)
	{
		printf("open zpv misc dev failed, errno %d\n", errno);
		return 0;
	}
	
	if(!reg)
	{
		ret = ioctl(fd, ZPV_UNREGISTER_MON);
		printf("unregister monitor , ret %d\n", ret);
		close(fd);
		return 0;
	}

	ret = ioctl(fd, ZPV_REGISTER_MON, &minfo);//注册监控
	if(ret < 0)
	{
		printf("register zpv monitor failed, errno %d\n", errno);
		close(fd);
		return 0;
	}
	
	while(1) 
	{
		printf("dash app process...\n");
		ret = ioctl(fd, ZPV_KEEP_ALIVE);//发送保活消息
		if(ret < 0)
		{
			printf("send alive msg failed, errno %d\n", errno);
			close(fd);
			return 0;
		}
		sleep(1);
	}
	
	close(fd);
	return 0;
}
