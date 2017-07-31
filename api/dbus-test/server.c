/************************************************************
 *filename		:	server.c
 *description	:	dbus dbus
 *author	 	:	ouyanghan
 *date			:	2017/07/26
 *version		:	v1.0
 ************************************************************/

#include <stdio.h>
#include <dbus/dbus.h>
#include "dbus_low_level.h"


int main(int argc, char *argv[])
{
	DBusConnection *con;
	
	con = open_dbus_connection("com.saic.server");

	add_dbus_match(	con,
		"type='signal', interface='com.signal.src'");	
	
	recv_dbus_message(	con,
						"com.signal.src",
						"test",
						1000);

	close_dbus_connection(con);

	return 0;
}
