#include "util.h"
#include <string.h>



inline unsigned long get_tick_count()
{  
    struct timespec ts;  
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}


inline DBusConnection* get_dbus(const char* well_known_name)
{
    int ret;
    DBusError err;  
    DBusConnection * connection;

    dbus_error_init(&err);  

    connection = dbus_bus_get(DBUS_BUS_SESSION ,&err );  
    if(dbus_error_is_set(&err)){  
        fprintf(stderr,"ConnectionErr : %s\n",err.message);  
        dbus_error_free(&err);  
        return NULL;
    }  
    if(connection == NULL)  
        return NULL;  

    ret = dbus_bus_request_name(connection, well_known_name ,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);  
    if(dbus_error_is_set(&err)){  
        fprintf(stderr,"Name Err :%s\n",err.message);  
        dbus_error_free(&err);  
        goto __failed_get;
    }  

    if(ret !=DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)  
        goto __failed_get;

    return connection;

__failed_get:
    dbus_connection_unref( connection );
    return NULL;
}



inline int dbus_send(DBusConnection* dbus,void* array,int size,const char* path,const char*interface )
{
    int ret = 0;
    void *v_ARRAY = array;

    DBusMessage* msg =dbus_message_new_signal( path ,interface ,"d");
    if( !msg )
        return -1;

    if( ! dbus_message_append_args(msg, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &v_ARRAY, size , DBUS_TYPE_INVALID ) ){
        ret = -1;
        goto __unref_msg;
    }

    if( !dbus_connection_send(dbus,msg, 0 )){  
        fprintf(stderr,"Out of Memory!\n");  
        ret = -1;
        goto __unref_msg;
    }  

    dbus_connection_flush(dbus); 

__unref_msg:
    dbus_message_unref( msg );
    return ret;
}

inline int dbus_send_noblocking( DBusConnection* dbus, void* array,int size , const char* path, char*interface   )
{
    int ret = 0;
    void *v_ARRAY = array;

    DBusMessage* msg =dbus_message_new_signal( path ,interface ,"d");
    if( !msg )
        return -1;

    if( ! dbus_message_append_args(msg, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &v_ARRAY, size , DBUS_TYPE_INVALID) ){
        ret = -1;
        goto __unref_msg;
    }

    if( !dbus_connection_send(dbus,msg,0)){  
        fprintf(stderr,"Out of Memory!\n");  
        ret = -1;
        goto __unref_msg;
    }  

__unref_msg:
    dbus_message_unref( msg );
    return ret;
    
}

inline int dbus_get_message_timeout(DBusConnection* dbus, void* message, int* length, int ms_timeout )
{
    DBusMessage* msg;
    int ret = 0;

    void* array;
    int len;

    DBusError err;
    dbus_error_init(&err);  

    if( ms_timeout != 0 )
        if( FALSE == dbus_connection_read_write(dbus,ms_timeout) )
            return -1;

    msg =dbus_connection_pop_message (dbus);  
    if( !msg )
        return -1;

    if( DBUS_MESSAGE_TYPE_SIGNAL != dbus_message_get_type(msg) ){
        ret = -1;
        goto __unref_msg;
    }
    
    if(  FALSE == 
            dbus_message_get_args( msg, &err ,DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &array, &len, DBUS_TYPE_INVALID  ) ){
        dbus_error_free( &err );
        ret = -1;
        goto __unref_msg;
    }
    
    *length = len;
    memcpy( message , array , len );


__unref_msg:
    dbus_message_unref(msg);  
    return ret;
}

inline int dbus_get_message(DBusConnection* dbus, void* message, int* length)
{
    DBusMessage* msg;
    int ret = 0;

    void* array;
    int len;

    DBusError err;
    dbus_error_init(&err);  

    msg =dbus_connection_pop_message (dbus);  
    if( !msg )
        return -1;

    if( DBUS_MESSAGE_TYPE_SIGNAL != dbus_message_get_type(msg) ){
        ret = -1;
        goto __unref_msg;
    }
    
    if(  FALSE == 
            dbus_message_get_args( msg, &err ,DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &array, &len, DBUS_TYPE_INVALID  ) ){
        dbus_error_free( &err );
        ret = -1;
        goto __unref_msg;
    }
    
    *length = len;
    memcpy( message , array , len );


__unref_msg:
    dbus_message_unref(msg);  
    return ret;
}

inline int dbus_add_match(DBusConnection* dbus, const char* rules )
{
    DBusError err;
    dbus_error_init(&err);  
    dbus_bus_add_match(dbus, rules , &err);  
    if(dbus_error_is_set(&err)){  
        fprintf(stderr,"Match Error%s\n",err.message);  
        dbus_error_free(&err);  
        return -1;
    }

    dbus_connection_flush(dbus);  
    return 0;
}

inline int big_msg_write_init(struct big_dbus_msg* big , const char* path , const char* interface)
{
    DBusMessage* msg = dbus_message_new_signal( path , interface , "d" );
    if( !msg )
        return -1;

    dbus_message_iter_init_append(  msg , &big->it );
    big->m = msg;
    big->total = 0;
    return 0;
}

inline int big_msg_append_array( struct big_dbus_msg* big, const void* array , int size )
{
    DBusMessageIter sub;
    if( FALSE == dbus_message_iter_open_container( 
                &big->it ,
               DBUS_TYPE_ARRAY,
               DBUS_TYPE_BYTE_AS_STRING,
             &sub ) )
       return -1;
    
    if( FALSE == dbus_message_iter_append_fixed_array( &sub , DBUS_TYPE_BYTE , &array , size ) )
        return -1;

    if( FALSE == dbus_message_iter_close_container( &big->it , &sub ) )
        return -1;
        
    big->total++;

    return 0;
}

inline int big_msg_get_array(struct big_dbus_msg* big , void** array , int* size)
{
    DBusMessageIter sub;

    if( DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type( & big->it ) ){
        return 0;
    }
    
    dbus_message_iter_recurse( &big->it , &sub );
    dbus_message_iter_get_fixed_array( &sub , array , size );
    dbus_message_iter_next( &big->it );

    return 1;
}

inline int big_msg_read(DBusConnection* dbus , struct big_dbus_msg* big )
{
    DBusMessage* msg;

    msg =dbus_connection_pop_message( dbus );  
    if( !msg )
        return -1;

    if( DBUS_MESSAGE_TYPE_SIGNAL != dbus_message_get_type(msg) ){
        dbus_message_unref(msg);  
        return -1;
    }
    
    big->m = msg;
    dbus_message_iter_init( msg ,  &big->it );

    return 0;
}


int IPC_msg_dispatch( DBusConnection* __dbus , ipc_msg_handler __handler, int __timeout )
{
	int ret;						/*处理了多少个消息*/
	unsigned int* msg_buffer_ptr;	/*指向msg消息的缓冲区指针*/
	int msg_buffer_size;			/*指向msg消息的缓冲区字节大小*/
	struct big_dbus_msg msg;		/*消息结构体*/
	
	ret = 0;
	
	if( 0 != __timeout ){
		/*超时等待*/
		dbus_connection_read_write( __dbus , __timeout );
	}
	
	if( big_msg_read( __dbus , &msg ) < 0 ){
		/*没有消息读出*/
		return -1;
	}
		
	while( 0 != big_msg_get_array( &msg , &msg_buffer_ptr , &msg_buffer_size ) ){
		/*调用传入的msg处理函数*/
		__handler(  
			__dbus ,										/*ipc dbus 通信对象*/    
			msg_buffer_ptr[0] ,								/*msgid*/
			&(msg_buffer_ptr[1]) ,							/*除去msgid后的其他数据*/
			msg_buffer_size - sizeof(msg_buffer_ptr[0])		/*除去msgid后的其他数据的大小，以字节为单位*/ 
			);
			
		ret++;/*处理的消息数量++*/
	}
	
	/*释放这个消息*/
	dbus_message_unref( msg.m );
	
	return ret;
}










