#include "ipc.h"
#include <string.h>

//get a dbus handle
IpcObject_t* IPCOBJ_connect(const char* well_known_name , const char* listen_rules );
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
    if( NULL == connection )  
        return NULL;  

    ret = dbus_bus_request_name(connection, well_known_name ,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);  
    if(dbus_error_is_set(&err)){  
        fprintf(stderr,"Name Err :%s\n",err.message);  
        dbus_error_free(&err);  
        goto __failed_get;
    }  

    if(ret !=DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)  
        goto __failed_get;


	dbus_bus_add_match(dbus, listen_rules , &err);  
    if(dbus_error_is_set(&err)){  
        fprintf(stderr,"Match Error%s\n",err.message);  
        dbus_error_free(&err);  
        goto __failed_get;
    }
    
    dbus_connection_flush(dbus); 
	
    return (IpcObject_t*)connection;

__failed_get:
    dbus_connection_unref( connection );
    return NULL;
}

//free the handle
inline int IPCOBJ_disconnect(IpcObject_t*  obj );
{
	dbus_connection_unref( obj );
	return 0;
}


//set the send function parameter
inline int IPCMQ_init_for_send(struct IpcMessageQueue* queue , const char* path , const char* interface);
{
    DBusMessage* msg = dbus_message_new_signal( path , interface , "d" );
    if( !msg )
        return -1;

    dbus_message_iter_init_append(  msg , &queue->it );
    queue->m = msg;
    queue->cnt = 0;
    return 0;
}

//
inline int IPCMQ_put_message( struct IpcMessageQueue* queue, const void* message , int size );
{
    DBusMessageIter sub;
    if( FALSE == dbus_message_iter_open_container( 
                &queue->it ,
               DBUS_TYPE_ARRAY,
               DBUS_TYPE_BYTE_AS_STRING,
             &sub ) )
       return -1;
    
    if( FALSE == dbus_message_iter_append_fixed_array( &sub , DBUS_TYPE_BYTE , &message , size ) )
        return -1;

    if( FALSE == dbus_message_iter_close_container( &queue->it , &sub ) )
        return -1;
        
    queue->cnt++;

    return 0;
}



inline int IPCMQ_send( struct IpcMessageQueue* queue , IpcObject_t* __obj )
{
	dbus_connection_send(  __obj , queue->m , 0 );
    dbus_connection_flush( __obj );
    return 0;
}


inline int IPCMQ_init_and_recv(struct IpcMessageQueue* queue ,IpcObject_t* __obj);
{
    DBusMessage* msg;

    msg =dbus_connection_pop_message( __obj );  
    if( !msg )
        return -1;

    if( DBUS_MESSAGE_TYPE_SIGNAL != dbus_message_get_type(msg) ){
        dbus_message_unref(msg);  
        return -1;
    }
    
    queue->m = msg;
    dbus_message_iter_init( msg ,  &queue->it );

    return 0;
}


inline int IPCMQ_get_message(struct IpcMessageQueue* queue , void** message , int* size);
{
    DBusMessageIter sub;

    if( DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type( & queue->it ) ){
        return 0;
    }
    
    dbus_message_iter_recurse( &queue->it , &sub );
    dbus_message_iter_get_fixed_array( &sub , message , size );
    dbus_message_iter_next( &queue->it );

    return 1;
}


inline int IPCMQ_free( struct  IpcMessageQueue* queue )
{
	dbus_message_unref( queue.m );
	queue.cnt = 0;
	
	return 0;
}

//loop -- get the dbus data
int IPCMQ_dispatch( IpcObject_t* __obj , IPCMSG_handler_t __handler, int __timeout );
{
	int ret;						/*处理了多少个消息*/
	unsigned int* msg_buffer_ptr;	/*指向msg消息的缓冲区指针*/
	int msg_buffer_size;			/*指向msg消息的缓冲区字节大小*/
	struct IpcMessageQueue queue;		/*消息结构体*/
	
	ret = 0;
	
	if( 0 != __timeout ){
		/*超时等待*/
		dbus_connection_read_write( __obj , __timeout );
	}
	
	if( IPCMQ_init_and_recv( __obj , &queue ) < 0 ){
		/*没有消息读出*/
		return -1;
	}
		
	while( 0 != IPCMQ_get_message( &queue , &msg_buffer_ptr , &msg_buffer_size ) ){
		/*调用传入的msg处理函数*/
		__handler(  
			__obj ,										/*ipc dbus 通信对象*/    
			msg_buffer_ptr[0] ,								/*msgid*/
			&(msg_buffer_ptr[1]) ,							/*除去msgid后的其他数据*/
			msg_buffer_size - sizeof(msg_buffer_ptr[0])		/*除去msgid后的其他数据的大小，以字节为单位*/ 
			);
			
		ret++;/*处理的消息数量++*/
	}
	
	/*释放这个消息*/
	IPCMQ_free( &queue );
	
	return ret;
}
