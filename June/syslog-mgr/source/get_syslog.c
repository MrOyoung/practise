/************************************************************
 **filename:get_syslog.c
 **autor:Rocky
 **date:2017/05/24
 **description:get the message from file /proc/kmsg
 ************************************************************/

#include "../include/get_syslog.h"
#include "../include/get_time_str.h"
//#include "../pkg_log_msg/vc_log.h"
#include "../pkg_log_msg/vc_log_cfg.h"

void record_kmsg(const char *msg, int size);

void *start_monitor_kmsg(void *arg)
{
	int iswatching = 1, startwatching = 1;
	FILE *fp = NULL;
	char msg_buf[BUFFER_SIZE + 1] = {0};
	size_t ret_size = 0;

	while (startwatching)
	{
		printf("begin to work\n");
		iswatching = 1;

		fp = fopen(ZTE_SYSLOG_DIR_FROM, "r");
		if (NULL == fp)
		{
			perror("fopen fp error");
			return NULL;
		}
	
		while (iswatching)
		{
			/* wait for system log */
			ret_size = fread(msg_buf, 1, BUFFER_SIZE, fp);
			if (ret_size > 0)
			{
				printf("begin to record\n");
				record_kmsg(msg_buf, ret_size);
			}
			else
			{
				printf("read nothing\n");
				iswatching = 0;

				fclose(fp);
			}
		}
		sleep(1);
	}

}


void record_kmsg(const char *msg, int size)
{
	int		pos = 0;
	FILE 	*local_fp = NULL;
	char	msg_arr[BUFFER_SIZE] = {0};
	size_t 	time_size = 0, ret_size = 0;

	local_fp = fopen(ZTE_SYSLOG_DIR_TO, "a+");
	if (NULL == local_fp)
	{
		perror("fopen local_fp error");
		return;
	}

	msg_arr[0] = '\n';
	pos += 1;
	
	time_size = make_current_time_str(msg_arr + pos);
	pos += time_size;

	strncat(msg_arr + pos, " : ", strlen(" : "));
	pos += strlen(" : "); 
	if ((size + pos) > BUFFER_SIZE)
		size = BUFFER_SIZE - pos;
	
	strncat(msg_arr + pos, msg, size);
	pos += size; 

	ret_size = fwrite(msg_arr, 1, pos, local_fp);

	//printf("msg is %s, time_size = %d, ret_size = %d\n", msg_arr, time_size, ret_size);
	if (pos != ret_size)
	{
		perror("fwrite error");
		return;
	}

	fclose(local_fp);
}

