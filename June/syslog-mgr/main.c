/************************************************************
 **filename:syslog_main.c
 **autor:Rocky
 **date:2017/05/24
 **description:syslog main
 ************************************************************/
#include "include/syslog_mgr.h"
#include "include/syslog_monitor.h"
#include "include/get_syslog.h"
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>

int main(int argc, char *argv[])
{
	int ret, main_running = 1;
	DBusConnection *bus;
	
	pthread_t monitor_pth; 
	pthread_t record_pth;
	pthread_t xitoslog_pth;

	/*create a dbus handler*/
	if (!(bus = get_dbus("com.saic.SyslogModule"))) {
		LOG("dbus get error\n");
		return -1;
	}

	if (dbus_add_match(bus, "type='signal',path='/com/saic/SyslogModule'") < 0){
		LOG("dbus add match error!!!\n");
		return -1;
	}

	while (main_running) {
#if 1
		ret = pthread_create(&monitor_pth, NULL, logfile_monitor, NULL);
		if (0 != ret)
			errExit("pthread_create monitor_pth error\n");
		else
			printf("pthread_create monitor_pth success\n");
#endif
		ret = pthread_create(&record_pth, NULL, logfile_record, (void *)bus);
		if (0 != ret) 
			errExit("pthread_create record_pth error\n");
		else
			printf("pthread_create record_pth success\n");

		ret = pthread_create(&xitoslog_pth, NULL, start_monitor_kmsg, (void *)bus);
		if (0 != ret) 
			errExit("pthread_create xitoslog_pth error\n");
		else
			printf("pthread_create xitoslog_pth success\n");
#if 1
		ret = pthread_join(monitor_pth, NULL);
		if (0 != ret)
			errExit("pthread_join monitor_pth error\n");
		else
		{
			printf("monitor_pth exit\n");
			goto _label;
		}
#endif
		ret = pthread_join(record_pth, NULL);
		if (0 != ret)
			errExit("pthread_join record_pth error\n");
		else
		{
			printf("record_pth exit\n");
			goto _label;
		}

		ret = pthread_join(xitoslog_pth, NULL);
		if (0 != ret)
			errExit("pthread_join xitoslog_pth error\n");
		else
		{
			printf("xitoslog_pth exit\n");
			goto _label;
}

_label:
	pthread_cancel(monitor_pth);
	pthread_cancel(record_pth);
	pthread_cancel(xitoslog_pth);
	}


	return 0;
}
