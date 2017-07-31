/************************************************************
 *filename		:	dbus_api.c
 *description	:	dbus low level api - get/send/recive
 *author	 	:	ouyanghan
 *date			:	2017/07/26
 *version		:	v1.0
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include "dbus_low_level.h"

/************************** declaration *********************/
static inline void init_dbus_error(DBusError *err);
static inline void free_dbus_error(DBusError *err);
static inline void handle_dbus_error(DBusError *err, char *msg);

DBusConnection *open_dbus_connection(const char *name)
{
	DBusError err;
	static DBusConnection *conn;
	int ret;

	/* initialise the errors */
	init_dbus_error(&err);

	/* connect to the bus */
	conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
	handle_dbus_error(&err, "Connection Error");

	if (NULL == conn)
	{
		exit(-1);
	}

	/* request a name on the bus */
	ret = dbus_bus_request_name(conn, 
			name, 
			DBUS_NAME_FLAG_REPLACE_EXISTING, 
			&err);
	handle_dbus_error(&err, "Name Error");

	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret)
	{
		exit(-1);
	}
	free_dbus_error(&err);

	return conn;
}

void close_dbus_connection(DBusConnection *conn)
{
	return dbus_connection_close(conn);
}


static inline void init_dbus_error(DBusError *err)
{
	return dbus_error_init(err);
}

/* free error handler */
static inline void free_dbus_error(DBusError *err)
{
	return dbus_error_free(err);
}


static inline void handle_dbus_error(DBusError *err, char *msg)
{
	/* prototype : dbus_bool_t dbus_error_is_set(const DBusError *error) */
	if (dbus_error_is_set(err))
	{
		fprintf(stderr, "%s (%s)\n", msg, err->message);
		free_dbus_error(err);
		exit(-1);
	}
}

/* signal infomation */
struct send_info
{
	char *sig_path;
	char *sig_iface;
	char *sig_name;
};


uint send_dbus_message(DBusConnection *conn,
							  char *path,			//the path to the object emitting the signal
							  char *iface,			//the interface the signal is emitted from
							  char *name			//name of the signal
							  )
{
	/* unique number to associate replies with requests */
	dbus_uint32_t serial = 0;
	DBusMessage *msg;
	DBusMessageIter args;
	char *sigvalue = "hello reciver!!";

	/* create a signal and check for errors */
	msg = dbus_message_new_signal(path, iface, name);
	if (NULL == msg)
	{
		fprintf(stderr, "Message null\n");
		return -1;
	}

	/* append arguments onto signal */
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &sigvalue))
	{
		fprintf(stderr, "Out of Memery!\n");
		return -1;
	}

	/* send the message and flush the connection */
	if (!dbus_connection_send(conn, msg, &serial))
	{
		fprintf(stderr, "Out of Memory!\n");
		return -1;
	}

	dbus_connection_flush(conn);

	/* free the message */
	dbus_message_unref(msg);

	return 0;
}


/* see signals from the given interface */
uint add_dbus_match(DBusConnection *conn, char *rules)
{
	DBusError err;

	/* initialise the errors */
	init_dbus_error(&err);

	/* add a rule for which messages we want to see */
	dbus_bus_add_match(conn, rules, &err);
	dbus_connection_flush(conn);

	handle_dbus_error(&err, "Match Error");

	free_dbus_error(&err);

	return 0;
}


/* recv the msg according to the rules */
uint recv_dbus_message(DBusConnection *conn, 
							  char *iface,
							  char *name, 
							  int timeout_ms //timeout milliseconds
							  )
{
	DBusMessage *msg;
	DBusMessageIter args;
	char sigvalue[1024] = {0};

	/* loop listening for signals being emmited */	
	while(1)
	{
		/* non blocking read of the next available message */
		dbus_connection_read_write(conn, 0);

		msg = dbus_connection_pop_message(conn);
		if (NULL == msg)
		{
			sleep(1);
			continue;
		}

		/* check whether the message is a signal with the given interface and member fields */
		if (dbus_message_is_signal(msg, iface, name))
		{
			/* read the parameters */	
			if (!dbus_message_iter_init(msg, &args))
			{
				fprintf(stderr, "Message has no arguments!\n");	
			}
			else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
			{
				fprintf(stderr, "Arguments is not string!\n");
			}
			else
			{
				dbus_message_iter_get_basic(&args, &sigvalue);
				printf("Got Signal with value %s\n", sigvalue);
			}
		}
	}

	/* free the message */
	dbus_message_unref(msg);

	return 0;
}

