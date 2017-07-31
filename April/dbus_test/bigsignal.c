
   
   
   
int main(int argc,char** argv)
{
	DBusError err;
   DBusConnection* conn;
   int ret;
   // initialise the errors
   dbus_error_init(&err);

// connect to the bus
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (NULL == conn) { 
      exit(1); 
   }

// request a name on the bus
   ret = dbus_bus_request_name(conn, "test.method.server", 
         DBUS_NAME_FLAG_REPLACE_EXISTING 
         , &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Name Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
      exit(1);
   }




   dbus_uint32_t serial = 0; // unique number to associate replies with requests
   DBusMessage* msg;
   DBusMessageIter args;
      
   // create a signal and check for errors 
   msg = dbus_message_new_signal("/test/signal/Object", // object name of the signal
         "test.signal.Type", // interface name of the signal
         "Test"); // name of the signal
   if (NULL == msg) 
   { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
   }

   // append arguments onto signal
   dbus_message_iter_init_append(msg, &args);
   
   
   
   
   
   

   // send the message and flush the connection
   if (!dbus_connection_send(conn, msg, &serial)) { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }
   dbus_connection_flush(conn);
   
   // free the message 
   dbus_message_unref(msg);
	return 0;
}
