/************************************************************
 *filename		:	client.c
 *description	:	dbus client
 *author	 	:	ouyanghan
 *date			:	2017/07/26
 *version		:	v1.0
 ************************************************************/

#include <stdio.h>
#include <dbus/dbus.h>
#include "dbus_low_level.h"
#include "../include/debug.h"

int main(int argc, char *argv[])
{
	DBusConnection *con;
	uint ret;
	
	con = open_dbus_connection("com.saic.client");

	ret = send_dbus_message(con,
							"/com/saic/server",
							"com.signal.src",
							"test");	
	if (0 == ret)
	{
		printf("signal send success\n");
	}

	close_dbus_connection(con);
	return 0;
}
