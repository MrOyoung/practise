/************************************************************
** filename:env_api.c
** autor:Rocky
** date:2017/04/27
** description:package env api
************************************************************/
#include "env_api.h"

_ENV_UNIT tmp[ARRAY_MAX_MEMBER_NUM];
int num = 0;

int env_test(_arg_in void* msg_data, _arg_in unsigned int msg_size)
{
	int index = -1;
	unsigned int msg_ack_size = 0;
	unsigned char msg_ack_buf[BUFFER_SIZE] = {0};
	_ENV_UNIT *msg_mem;
	MSG_ID msg_id, msg_id_ack;
	
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

	msg_mem = unpack_msg(msg_data, msg_size, &msg_id);
	
	printf("msg id = %d\nenv_name is %s, env_value is %s\n", msg_id, msg_mem->env_name, msg_mem->env_value);
	
	
	index = is_env_exist(msg_mem->env_name, tmp, num);
	if (ENV_SERVICE_GET == msg_id)
	{
		printf("GET: index = %d\n", index);
		if (-1 == index)//the env is not exist
			msg_id_ack = ENV_SERVICE_NOT_EXIST;
		else
			msg_id_ack = ENV_SERVICE_VALUE;
		
		msg_ack_size = pack_msg(msg_id_ack, tmp[index], msg_ack_buf);
		
		printf("msg_ack_size = %d msg_ack_buf is %s\n", msg_ack_size, msg_ack_buf + sizeof(MSG_ID));
	}
	else if (ENV_SERVICE_SET == msg_id)
	{
		printf("SET: index = %d\n", index);
		
		if (-1 == index)//the env is not exist
		{
			tmp[num] = *msg_mem; num++;
		}
		else
			tmp[index] = *msg_mem;
		
		msg_id_ack = ENV_SERVICE_OK;
		msg_ack_size = pack_msg(msg_id_ack, tmp[index], msg_ack_buf);
		
		printf("msg_ack_size = %d\n", msg_ack_size);
	}
	
	
	//printf("%d\n", is_env_exist("PATH", tmp, num));
	
	//array_member_free(tmp, num);
	
	return 0;
}


/******************************************************************
**传参：
	msg_data ： env相关的数据帧内容
	msg_size ： env相关的数据帧长度
	id		 ： 获取数据帧类型
**返回值： _ENV_UNIT *型，返回消息中的name和value数据
**描述：
	解析消息，获取msg_id/name和value，若为env查询帧，则返回的_ENV_UNIT *数据中
	value成员则为null
**作者：Rocky
**日期：2017/04/27
******************************************************************/
_ENV_UNIT *unpack_msg(void* msg_data , unsigned int msg_size, _arg_out MSG_ID *id)
{
	*id = get_msg_id(msg_data, msg_size);
	
	return get_name_and_value(msg_data, msg_size);
}


/******************************************************************
**传参：
	frame_buf	： 	其他模块发来的env相关的数据帧
	size		：	数据帧的大小
**返回值： MSG_ID型
**描述：
	获得并返回数据帧中的MSG_ID值
**作者：Rocky
**日期：2017/04/28
******************************************************************/
MSG_ID get_msg_id(const unsigned char *frame_buf, unsigned int size)
{
	if (size < sizeof(MSG_ID)) return ENV_SERVICE_FUNC_ERROR;
	
	MSG_ID tmp;
	
	memcpy(&tmp, frame_buf, sizeof(MSG_ID));
	
	return tmp;
}


/******************************************************************
**传参：
	frame_buf	： 	其他模块发来的env相关的数据帧
	size		：	数据帧的大小
**返回值： _ENV_UNIT *型
**描述：
	解析数据帧，分别获得其中的name和vlaue，将其放在_ENV_UNIT *的动态内存中，
	返回该内存的首地址。
**作者：Rocky
**日期：2017/04/28
******************************************************************/
_ENV_UNIT *get_name_and_value(const unsigned char *frame_buf, unsigned int size)
{
	if (size < sizeof(MSG_ID)) return NULL;
	
	char name[64] = {0};
	char value[256] = {0};
	
	_ENV_UNIT *tmp = (_ENV_UNIT *)malloc_rebuild(sizeof(_ENV_UNIT));
	
	sscanf((const char *)frame_buf + sizeof(MSG_ID), "%[^=]=%s", name, value);
	
	tmp->env_name = (char *)malloc_rebuild(strlen(name) + 1);
	tmp->env_value = (char *)malloc_rebuild(strlen(value) + 1);

	strncpy(tmp->env_name, name, strlen(name));
	strncpy(tmp->env_value, value, strlen(value));
	
	return tmp;
}

void array_member_free(_ENV_UNIT array[], unsigned int member_num)
{
	for (int i = 0; i < member_num; i++)
	{
		free_rebuild(array[i].env_name);
		free_rebuild(array[i].env_value);
	}
}


void *malloc_rebuild(unsigned int size)
{
	void *tmp = malloc(size);
	memset(tmp, 0, size);
	
	return tmp;
}

void free_rebuild(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}


/******************************************************************
**传参：
	env_name_ptr： 	指向环境变量名称name的指针
	msg_ptr		：	结构体数组，数组的成员为_ENV_UNIT类型的结构体
	size		：	指针数组的成员个数
**返回值： unsigned int 型，返回该环境变量成员在数组中对应的下标号，-1 ： 不存在
**描述：
	将传入的环境变量名称和模块中的环境变量存储数据进行比对，判断此环境变量是否存在，
	若存在，则返回该成员在数组中的下标
**作者：Rocky
**日期：2017/04/28
******************************************************************/
int is_env_exist(const char *env_name_ptr, _ENV_UNIT msg_ptr[], unsigned int member_total_num)
{
	if (!env_name_ptr) return RESULT_ERR;

	for (int i = 0; i < member_total_num; i++)
	{
		if (!strcmp(env_name_ptr, msg_ptr[i].env_name)) return i;
	}
	
	return RESULT_ERR;
}


/******************************************************************
**传参：
	env_name_ptr： 	指向环境变量名称name的指针
	msg_ptr		：	指针数组，数组的成员为_ENV_UNIT *类型的指针
	size		：	指针数组的成员个数
**返回值： unsigned int 型，返回该环境变量成员在数组中对应的下标号，-1 ： 不存在
**描述：
	将传入的环境变量名称和模块中的环境变量存储数据进行比对，判断此环境变量是否存在，
	若存在，则返回该成员在数组中的下标
**作者：Rocky
**日期：2017/04/28
******************************************************************/
unsigned int pack_msg(const MSG_ID msg_id, const _ENV_UNIT env_unit_ptr, void  *msg_ptr)
{
	if (!msg_ptr) return RESULT_ERR;
	unsigned int str_len = 0;

	memcpy(msg_ptr, &msg_id, sizeof(MSG_ID));
	
	if (ENV_SERVICE_VALUE == msg_id)
	{
		sprintf((char *)msg_ptr + sizeof(MSG_ID), "%s=%s", env_unit_ptr.env_name, env_unit_ptr.env_value);
		str_len = strlen(env_unit_ptr.env_name) + strlen(env_unit_ptr.env_value) + 1;
	}
	
	return sizeof(MSG_ID) + str_len;
}


/****************************************************************
**传参： 
	name : 环境变量名称，类型：const char *
**返回值： char * : 环境变量内容
**描述：传入环境变量的名称，成功返回环境变量的值，失败返回null
**作者：Rocky
**日期：2017/04/27
******************************************************************/
char *get_env(const char *name)
{

	return getenv(name);	
}


/******************************************************************
**传参：
	name ： 	添加的环境变量的名称
	value ： 添加的环境变量的值
	overwrite ： 标签
**返回值： int型，成功返回0，失败返回-1
**描述：
	overwrite=0，不会修改name对应的值
	overwrite=1，若name不存在，则新增并设置值为value，若name存在，则对应的值改为value
**作者：Rocky
**日期：2017/04/27
******************************************************************/
int set_env(const char *name, const char *value, int overwrite)
{
	int ret = 0;
	//char com[64] = {};
	
	ret = setenv(name, value, overwrite);
	if (-1 == ret)
		perror("setenv error");
	
	return ret;
}


void malloc_test()
{

	//测试证明malloc返回的地址之前是有数据的
	char *ptr = NULL;
	//int count = 0;
	
	ptr = (char *)malloc(100);
	
	for (int i = 0; i < 100; i++)
	{
		printf("%02x ", *(ptr + i));
	
		if ((0 == ((i+1) % 10))) printf("\n");
	}
	
/*	
	while(ptr)
	{
		printf("%c\n", *ptr);
		ptr--;
		count++;
		//printf("count = %d\n", count);
	}

	printf("count = %d\n", count);
*/
	free(ptr);
	ptr = NULL;


	//MSG_ID a = ENV_SERVICE_SET;
	//printf("a = %d, sizeof(a) = %d\n", a, sizeof(a));

//测试使用sscanf解析字符串
/*
	char *ip_msg = "192.168.1.1";
	char part[4][8] = {0};

	sscanf(ip_msg, "%[^.].%[^.].%[^.].%s", part[0], part[1], part[2], part[3]);
	printf("%s\n%s\n%s\n%s\n", part[0], part[1], part[2], part[3]);
	

	char *env_msg = "192=168";
	sscanf(env_msg, "%[^=]=%s", part[0], part[1]);
	printf("%s\n%s\n", part[0], part[1]);
*/

}

