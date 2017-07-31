/************************************************************
 **filename:syslog_main.c
 **autor:Rocky
 **date:2017/05/24
 **description:syslog main
 ************************************************************/
#include "include/syslog_mgr.h"
#include "include/syslog_monitor.h"
#include "include/get_syslog.h"
#include "include/ipc.h"

//#define MODULE_LOG_DIR		"/home/root/log"
#define MODULE_LOG_DIR		"./log/log"

#define _DBUS_IN_MAIN		0
#define _NEW_DBUS_IN_MAIN		1

int main(int argc, char *argv[])
{
	int ret, main_running = 1;

	pthread_t monitor_pth; 
	pthread_t record_pth;
	pthread_t xitoslog_pth;

#if _NEW_DBUS_IN_MAIN

	/* create a dbus handler */	
	IpcObject_t *dbus = IPCOBJ_connect("com.saic.SyslogModule", MAKE_RULES_STR("/com/saic/SyslogModule"));
	

#endif


	while (main_running)
	{
#if 1
		ret = pthread_create(&record_pth, NULL, SM_start_to_record, dbus);
		if (0 != ret) 
			errExit("pthread_create record_pth error\n");
		else
			printf("pthread_create record_pth success\n");
#endif

#if 0
		ret = pthread_create(&xitoslog_pth, NULL, start_monitor_kmsg, NULL);
		if (0 != ret) 
			errExit("pthread_create xitoslog_pth error\n");
		else
			printf("pthread_create xitoslog_pth success\n");

#endif

#if 0
		ret = pthread_create(&monitor_pth, NULL, start_monitor_logdir, MODULE_LOG_DIR);
		if (0 != ret)
			errExit("pthread_create monitor_pth error\n");
		else
			printf("pthread_create monitor_pth success\n");

#endif


#if 1
		ret = pthread_join(record_pth, NULL);
		if (0 != ret)
			errExit("pthread_join record_pth error\n");
		else
		{
			printf("record_pth exit\n");
			goto _label;
		}
#endif

#if 0
		ret = pthread_join(monitor_pth, NULL);
		if (0 != ret)
			errExit("pthread_join monitor_pth error\n");
		else
		{
			printf("monitor_pth exit\n");
			goto _label;
		}
#endif

#if 0
		ret = pthread_join(xitoslog_pth, NULL);
		if (0 != ret)
			errExit("pthread_join xitoslog_pth error\n");
		else
		{
			printf("xitoslog_pth exit\n");
			goto _label;
		}
#endif

_label:
		pthread_cancel(monitor_pth);
		pthread_cancel(record_pth);
		pthread_cancel(xitoslog_pth);
	}/* end of while() */


	return 0;
}
