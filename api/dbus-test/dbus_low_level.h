/************************************************************
 *filename	 :	dbus_api.c
 *description :	dbus low level api - get/send/recive
 *author	 	 :	ouyanghan
 *date		 :	2017/07/26
 *version	 :	v1.0
 ************************************************************/

#ifndef _DBUS_LOW_LEVEL_H
#define _DBUS_LOW_LEVEL_H

#include "../include/debug.h"

DBusConnection *open_dbus_connection(const char *name);
void close_dbus_connection(DBusConnection *conn);
uint add_dbus_match(DBusConnection *conn, char *rules);
uint send_dbus_message(DBusConnection *conn, char *path, char *iface, char *name);

uint recv_dbus_message(DBusConnection *conn, char *iface, char *name, int timeout_ms);

#endif //_DBUS_LOW_LEVEL_H
