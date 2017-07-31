/************************************************************
** filename:env_main.c
** autor:Rocky
** date:2017/05/03
** description:env service module
************************************************************/
#include "env_api.h"

void env_handler( IpcObject_t* __obj ,unsigned int __msgid , void* __data , int __size )
{
	struct timeval end_time;
	struct timeval *send_time = (struct timeval *)__data;
	
	gettimeofday(&end_time, NULL);
	//printf("message id:%d --- size:%d \n" , __msgid , __size );
	
	//printf("time = %ld\n", (end_time.tv_sec - send_time->tv_sec) * 1000000 + (end_time.tv_usec - send_time->tv_usec));
	
#if 0	
	int num = 0;
	_ENV_UNIT tmp[ARRAY_MAX_MEMBER_NUM];
	
	tmp[num].env_name = (char *)malloc_rebuild(sizeof("USER") + 1);
	memcpy(tmp[num].env_name, "USER", sizeof("USER"));
	
	tmp[num].env_value = (char *)malloc_rebuild(sizeof("rocky") + 1);
	memcpy(tmp[num].env_value, "rocky", sizeof("rocky"));
	num++;
	
	tmp[num].env_name = (char *)malloc_rebuild(sizeof("HOME") + 1);
	memcpy(tmp[num].env_name, "HOME", sizeof("HOME"));
	
	tmp[num].env_value = (char *)malloc_rebuild(sizeof("/home/rocky") + 1);
	memcpy(tmp[num].env_value, "/home/rocky", sizeof("/home/rocky"));
	num++;
	
	//printf("member num = %d\n", is_env_exist("HOME", tmp, num));

	
	printf("%d\n", get_msg_id(msg_data, msg_size));
	
	(tmp[num]) = *get_name_and_value(msg_data, msg_size);
	
	//printf("env_name is %s, env_value is %s\n", tmp[num].env_name, tmp[num].env_value);
	num++;
	
	array_member_free(tmp, num);
#endif

	return;
}


int main(int argc, char *argv[])
{
	IpcObject_t* dbus = IPCOBJ_connect("com.saic.AppCluster" , MAKE_RULES_STR("/com/saic/AppCluster") );

	if( !dbus ){
		printf("cant not connect ipc\n");
		return 1;
	}


 	while(1){
 		IPCMQ_dispatch( dbus , env_handler  , -1 );
		
	}


    return 0;
}
