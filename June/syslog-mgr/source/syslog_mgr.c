/************************************************************
 **filename:syslog_mgr.c
 **autor:Rocky
 **date:2017/05/24
 **description:receive msg from dbus, then write the msg
 ************************************************************/
#include "../include/syslog_mgr.h"
#include <fcntl.h>

#define LOG_BUF_SIZE 1024

//arg : dbus handler
void *logfile_record(void *arg)
{
	int len = 0;
	int size, debug_level = 0, module_count = 0;
	DBusConnection *bus = (DBusConnection *)arg;
	char message[LOG_BUF_SIZE] = {0};/*接受消息的缓冲区*/

	printf("work_path is %s\nlog_dir_path is %s\n", work_path, log_dir_path);

	/*检查log目录是否存在*/
	is_log_file_exist((const char *)log_dir_path);
	
	while (1)
	{
		/*get msg from dbus*/
		if (dbus_get_message_timeout(bus, message, &len, 0) >= 0){
			LOG("recv msg : %s\n", message + sizeof(_MODULE_NAME));

		record_module_log(*((int*)message), 
						(unsigned char *)(message + sizeof(_MODULE_NAME) ), 
						len - sizeof(_MODULE_NAME)
						);

		}

		usleep(100000);
	}
}

//判断目录是否存在，不存在则创建
int is_log_file_exist(const char *module_name)
{
	int ret;
	DIR *dirptr;

	dirptr = opendir(module_name);
	if (!dirptr && (ENOENT == errno))//目录不存在
	{
		ret = mkdir(module_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == ret) perRtn("mkdir", -1);
	}

	closedir(dirptr);

	return 0;	
}


//路径拼接,path/name
int merge_path(const char *name, _arg_out char *path)
{
	if (!path) errRtn("filepath ptr is empty\n", -1);

	strcat(path, "/");
	strcat(path, name);

	return 0;
}

//以日期命名的日志文件名，如2017-05-26.log
int get_logfile_name(char *filename)
{
	if (!filename) errRtn("filename ptr is empty\n", -1);

	time_t tv;
	struct tm *filetime;

	tv = time(NULL);
	filetime = localtime(&tv);

	sprintf(filename, "%04d-%02d-%02d.log", filetime->tm_year + 1900, filetime->tm_mon + 1, filetime->tm_mday);

	return 0;
}

/*
**描述：
	获取日志文件的路径
**全局变量:
	log_dir_path : log目录的绝对路径名
*/	
int get_logfile_path(_MODULE_NAME module_num, _arg_out char *pathname)
{
	if (!pathname)	errRtn("module name or pathname is empty\n", -1);

	char filename[NAME_MAX] = {0};
	char module_name[NAME_MAX] = {0};

	//由module_num获取模块名
	get_module_name(module_num, module_name);

	//获取模块日志目录的路径
	memcpy(pathname, log_dir_path, strlen(log_dir_path));
	merge_path(module_name, pathname);

	//检查模块的日志目录是否存在
	is_log_file_exist(pathname);

	//获取以当前时间命名的日志文件名，格式为:2017-05-24.log
	get_logfile_name(filename);

	merge_path((const char *)filename, pathname);

	return 0;
}

#ifdef _YDP_
struct MODULE_LOG_INFO{
	FILE* 			fp;
	_MODULE_NAME 	module_name;
	const char* 	name;
	char 			path[256];
};
#endif //_YDP_


//打开形参中的文件，写文件，内容为log_msg,大小为log_size
bool write_log(const char *log_path, unsigned char *log_msg, unsigned int log_size)
{
	FILE *fp = NULL;
	ssize_t writen_size;

	fp = fopen(log_path, "a");
	if (NULL == fp) perRtn("fopen error", -1);

	printf("log_msg is %s, log_size = %d\n", log_msg, log_size);
	writen_size = fwrite(log_msg, log_size, 1, fp);
	if (writen_size != 1)	errRtn("fwrite error\n", -1);

	fclose(fp);

	return true;
}

//获得今日日志文件的绝对路径,调用write_log()函数写日志
unsigned int record_module_log(_MODULE_NAME module_num, unsigned char *log_msg, unsigned int log_size)
{
	char log_path[PATH_MAX] = {0};

	get_logfile_path(module_num, log_path);
	write_log((const char *)log_path, log_msg, log_size);

	return 0;
}

#define _MODULE_SUM 25

//存储模块名
static const char *num2name[] = {
	/* background service */
	SERVICE_CONTENTPROVIDER_NICK, SERVICE_MUTIMEDIA_NICK, SERVICE_SYSLOG_NICK,
	SERVICE_WINDOW_NICK, SERVICE_ENVIRONMENT_NICK, SERVICE_POWER_NICK,
	SERVICE_UPGRADE_NICK, 
	
	/* application */
	ACTIVITY_CLUSTER_NICK, ACTIVITY_LVDS_NICK,
	
	/* background service manager */
	SERVICEMGR_CONTENTPROVIDER_NICK, SERVICEMGR_MUTIMEDIA_NICK, SERVICEMGR_SYSLOG_NICK,
	SERVICEMGR_WINDOW_NICK, SERVICEMGR_ENVIRONMENT_NICK, SERVICEMGR_POWER_NICK,
	SERVICEMGR_UPGRADE_NICK, 

	/* application manager */
	ACTIVITYMGR_CLUSTER_NICK, ACTIVITYMGR_LVDS_NICK,

	/* hardware manager */
	HARDWAREMGR_BUS_NICK, HARDWAREMGR_LCD_NICK, HARDWAREMGR_POWER_NICK,
	HARDWAREMGR_STORAGE_NICK, HARDWAREMGR_CORE_NICK, HAREWAREMGR_LVDS_NICK,
	HAREWAREMGR_USBSTATE_NICK,
	
	/*kanzi applications*/
	KANZIAPP_SPEED_NICK, KANZIAPP_TACH_NICK, KANZIAPP_POPUP_NICK, KANZIAPP_TABICON_NICK,
	KANZIAPP_BACKGROUND_NICK
};

//根据模块id获取模块名
void get_module_name(const _MODULE_NAME module_num, _arg_out char *module_name_ptr)
{
	if (!module_name_ptr) errRtn("str is empty\n", );
	
	strncpy(module_name_ptr, num2name[module_num], strlen(num2name[module_num]));	
	
}

