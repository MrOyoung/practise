/************************************************************
 **filename:syslog_mgr.c
 **autor:Rocky
 **date:2017/05/24
 **description:receive msg from dbus, then write the msg
 ************************************************************/
#include <fcntl.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>
#include "../include/syslog_mgr.h"

#define LOG_BUF_SIZE 1024

static int SM_is_logfile_exist(const char *pathname);
static int SM_log_record(_MODULE_NAME name, unsigned char *log_msg, unsigned int log_size);
static void SM_handler(IpcObject_t *__obj, unsigned int __msgid, void *__data, int __size);

/* reference by main */
void *SM_start_to_record(void *arg)
{
	IpcObject_t *dbus;
	char *dbus_msg = NULL;/*接受消息的缓冲区*/

	dbus_msg = (char *)calloc(1, LOG_BUF_SIZE);
	if (NULL == dbus_msg)
	{
		perRtn("calloc error", NULL);
	}

	dbus = (IpcObject_t *)arg;

	if (-1 == SM_is_logfile_exist(SM_logdir_path))
	{
		exit(-1);
	}

	while (1)
	{
		/*get msg from dbus*/
		IPCMQ_dispatch(dbus, SM_handler, -1);

		usleep(100000);
	}

	free(dbus_msg);
}


static int SM_log_record(_MODULE_NAME name, unsigned char *log_msg, unsigned int log_size);


/* call-back func,the parameter of func:IPCMQ_dispatch() */
static void SM_handler(IpcObject_t *__obj, unsigned int __msgid, void *__data, int __size)
{
	/* check id */
	

	SM_log_record(__msgid, 
			(unsigned char *)(__data), 
			__size
			);

#if 0
	/* record */
	SM_log_record(*((int*)__data), 
			(unsigned char *)(__data + sizeof(_MODULE_NAME) ), 
			__size - sizeof(_MODULE_NAME)
			);
#endif
}


static int SM_check_module_info(_MODULE_NAME name);
static int SM_write_log(_MODULE_NAME name, unsigned char *log_msg, unsigned int log_size);

/* record the log msg */
static int SM_log_record(_MODULE_NAME name, unsigned char *log_msg, unsigned int log_size)
{
	/* check the parameter */
	if ((name < 0) || (name > MODULE_SUM))
	{
		errRtn("module name error", -1);	
	}

	/* get the member from array */
	if (-1 == SM_check_module_info(name))
	{
		return -1;
	}

	return SM_write_log(name, log_msg, log_size);
}


static int SM_write_log(_MODULE_NAME name, unsigned char *log_msg, unsigned int log_size)
{
	size_t writen_size = fwrite(log_msg, log_size, 1, module_arr[name].fp);
	if (writen_size != 1)
	{
		errRtn("fwrite error\n", -1);
	}

	printf("write success %s\n", log_msg);

	/* execv fflush() every n times ? */
	fflush(module_arr[name].fp);

	return 0;
}

static int SM_flush_module_info(const struct tm *tv, _MODULE_NAME name);

/* judge that the day changes or not */
static int SM_check_module_info(_MODULE_NAME name)
{
	time_t tm;
	struct tm *tv;

	tm = time(NULL);
	tv = localtime(&tm);

	if ((tv->tm_year == module_arr[name].tv.tm_year) && 
			(tv->tm_mon == module_arr[name].tv.tm_mon) &&
			(tv->tm_mday == module_arr[name].tv.tm_mday))
	{
		printf("the same day\n");
		return 1;
	}

	memcpy(&module_arr[name].tv, tv, sizeof(struct tm));

	return SM_flush_module_info(tv, name);
}


static inline int SM_get_filename(const struct tm *tv, _arg_out char *pstr);
static char *SM_get_module_name(const _MODULE_NAME module_num);

static int SM_flush_module_info(const struct tm *tv, _MODULE_NAME name)
{
	/* get the newer file log name,format : xxxx-xx-xx.log */
	SM_get_filename(tv, module_arr[name].name);
	printf("filename is %s\n", module_arr[name].name);

	/* if module dir does not exist, create it, format : "log/WindowManager" eg.*/

	sprintf(module_arr[name].path, "%s/%s%c", 
			SM_logdir_path, 
			SM_get_module_name(name),
			'\0');

	
	SM_is_logfile_exist(module_arr[name].path);	

	/* get the absolute path of today's log file */
	sprintf(module_arr[name].path, "%s/%s/%s%c",
			SM_logdir_path,
			SM_get_module_name(name),
			module_arr[name].name,
			'\0');
	printf("path is %s\n", module_arr[name].path);

	/* create the new file streamer */
	if (module_arr[name].fp)
	{
		fclose(module_arr[name].fp);
	}

	module_arr[name].fp = fopen(module_arr[name].path, "a");
	if (NULL == module_arr[name].fp)
	{
		perRtn("fopen error", -1);
	}

	return 0;
}


static inline int SM_get_filename(const struct tm *tv, _arg_out char *pstr)
{
	char *p = pstr;

	/* year */
	DIGITAL4_INC(p, tv->tm_year + 1900);
	*p++ = '-';

	/* mon */
	DIGITAL2_INC(p, tv->tm_mon + 1);
	*p++ = '-';

	/* day */
	DIGITAL2_INC(p, tv->tm_mday);
	*p++ = '.';
	*p++ = 'l';
	*p++ = 'o';
	*p++ = 'g';
	*p   = '\0';

	return p - pstr;
}

/* use func access() to check pathname */
static int SM_is_logfile_exist(const char *pathname)
{
	if ((access(pathname, W_OK | F_OK) != 0) && (ENOENT == errno))
	{
		/* the path does not exist */
		if (-1 == mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
		{
			perRtn("mkdir", -1);
		}
	}

	return 0;
}

/* get module name according to module num */
static char *SM_get_module_name(const _MODULE_NAME module_num)
{
	static char *num2name[] = {
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

	return num2name[module_num];	
}

