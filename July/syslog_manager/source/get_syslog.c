/************************************************************
 **filename:get_syslog.c
 **autor:Rocky
 **date:2017/05/24
 **description:get the message from file /proc/kmsg
 ************************************************************/

#include "../include/get_syslog.h"
#include "../include/get_time_str.h"

static FILE *dst_fp;

static void record_kmsg(char *msg, int size);

void *start_monitor_kmsg(void *arg)
{
	FILE *src_fp = NULL;
	char msg_buf[BUFFER_SIZE + 1] = {0};
	size_t ret_size = 0;

	while ( 1 )
	{
		printf("begin to work\n");

		src_fp = fopen(ZTE_SYSLOG_DIR_FROM, "r");
		if (NULL == src_fp)
		{
			perror("fopen src_fp error");
			return NULL;
		}
		
		dst_fp = fopen(ZTE_SYSLOG_DIR_TO, "a+");
		if (NULL == dst_fp)
		{
			perror("fopen dst_fp error");
			return NULL;
		}
		
		while ((ret_size = fread(msg_buf + 20, 1, BUFFER_SIZE - 20, src_fp)) > 0)
		{
			printf("begin to record\n");
			
			record_kmsg(msg_buf, ret_size);	
		}
		
		fclose(src_fp);
		fclose(dst_fp);
		
		sleep(1);
	}/* end of while() */
	
	return NULL;
}

static void record_kmsg(char *msg, int size)
{
	size_t ret_size;
	size_t writen_size;
	
	make_current_time_str(msg);
	
	if ((size + 21) > BUFFER_SIZE)
	{
		msg[BUFFER_SIZE + 1] = '\0';
		writen_size = BUFFER_SIZE;
	}
	
	msg[size + 20] = '\n';
	msg[size + 21] = '\0';
	writen_size = size + 21;
	
	ret_size = fwrite(msg, 1, writen_size, dst_fp);
	//printf("msg is %s, time_size = %d, ret_size = %d\n", msg, writen_size, ret_size);
	if (writen_size != ret_size)
	{
		perror("fwrite error");
		return;
	}
	
	fflush(dst_fp);
	
	return ;
}


