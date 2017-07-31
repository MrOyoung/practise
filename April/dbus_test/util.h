#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdio.h>
#include <dbus/dbus-glib.h>  
#include <dbus/dbus.h>  


typedef unsigned char byte;

unsigned long get_tick_count();

DBusConnection* get_dbus(const char* well_known_name);

int dbus_add_match(DBusConnection* dbus, const char* rules );

int dbus_send(DBusConnection* dbus,void* array,int size,const char* path,const char*interface );

int dbus_send_noblocking( DBusConnection* dbus, void* array,int size , const char* path, char*interface   );

int dbus_get_message(DBusConnection* dbus, void* message, int* length);

int dbus_get_message_timeout(DBusConnection* dbus, void* message, int* length, int ms_timeout );

struct big_dbus_msg{
	DBusMessage* m;
	DBusMessageIter it;
	int total;
};

int big_msg_write_init(struct big_dbus_msg* msg , const char* path , const char* interface);

int big_msg_append_array( struct big_dbus_msg* big, const void* array , int size );

int big_msg_get_array(struct big_dbus_msg* big , void** array , int* size);

int big_msg_read(DBusConnection* dbus , struct big_dbus_msg* big );

typedef void (*ipc_msg_handler)( DBusConnection* __dbus,unsigned int __msgid , void* __data , int __size );

int IPC_msg_dispatch( DBusConnection* __dbus , ipc_msg_handler __handler, int __timeout );

#endif
