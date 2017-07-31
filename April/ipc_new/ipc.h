#ifndef _IPC_H_
#define _IPC_H_
#include <stdio.h>
#include <dbus/dbus-glib.h>  
#include <dbus/dbus.h>  



/*DBusConnection的别名，用于封装dbus*/
typedef DBusConnection IpcObject_t;
typedef DBusMessage    IpcMessage;
typedef DBusMessageIter IpcMessageIter;



/*封装用于dbus通信的结构体*/
struct IpcMessageQueue{
	/*dbus消息*/
	IpcMessage* 	m;
	/*dbus消息的迭代器*/
	IpcMessageIter it;
	/*记录当前该结构体已经添加了多少个消息*/
	int 			cnt;
};



/*
*
*构造一个用于IPCOBJ_connect第二个参数的字符串
* 参数path必须是一个常量字符串
* example:
*		MAKE_RULES_STR( "/com/saic/IpcUartData" )
*/
#define MAKE_RULES_STR( path ) ("type='signal',path='" path "'")


/*===========================================================================*\
 * FUNCTION: IPCOBJ_connect
 *===========================================================================
 * PARAMETERS:
 * well_known_name 	- name
 * listen_rules   - 监听规则，可以使用MAKE_RULES_STR宏来定义
 *
 * RETURN VALUE:
 * 返回一个ipc通信对象，如果失败返回一个NULL
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 该函数获取一个ipc通信对象,IPC通信时必须需要一个这样的对象
 * example:
 * IpcObject_t* ipc;
 * ipc = IPCOBJ_connect("com.saic.IpcUartData" , MAKE_RULES_STR("/com/saic/IpcUartData") );
\*===========================================================================*/
IpcObject_t* IPCOBJ_connect(const char* well_known_name , const char* listen_rules );


/*===========================================================================*\
 * FUNCTION: IPCOBJ_disconnect
 *===========================================================================
 * PARAMETERS:
 * obj 	- 一个有效的IpcObject_t对象的指针
 *
 *
 * RETURN VALUE:
 * 0  - disconnect sucessed
 * -1 - 参数为无效参数
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 当ipc对象不再需要时，使用该函数关闭连接，并且销毁该对象所占据的内存空间。
 * example:
 * pcObject_t* ipc;
 * ipc = IPCOBJ_connect("com.saic.IpcUartData" , MAKE_RULES_STR("/com/saic/IpcUartData") );
 * //do something.....
 * IPCOBJ_disconnect( ipc ); //free ipc object
\*===========================================================================*/
int IPCOBJ_disconnect(IpcObject_t*  obj );


/*===========================================================================*\
 * FUNCTION: IPCMQ_init_for_send
 *===========================================================================
 * PARAMETERS:
 * queue 	- 一个没有使用过，或者一个使用过但是free过的IpcMessageQueue结构体指针
 * path   -   要发送只的路径
 * interface   - 发送的模块的接口
 *
 * RETURN VALUE:
 * 0  - 初始化成功
 * -1 - 初始化失败，没有足够的内存空间
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 初始化一个IpcMessageQueue结构体，该结构体用于向path发送，在调用IPCMQ_put_message
 * 前，必须调用该函数用于初始化一个用于发送的消息队列结构体，之后便可以调用函数
 * IPCMQ_put_message用于添加消息。
 *
\*===========================================================================*/
int IPCMQ_init_for_send(struct IpcMessageQueue* queue , const char* path , const char* interface);


/*===========================================================================*\
 * FUNCTION: IPCMQ_put_message
 *===========================================================================
 * PARAMETERS:
 * queue 	- IpcMessageQueue结构体指针
 * message   - 消息数据的首地址
 * size   - 消息的大小，以字节为单位
 *
 * RETURN VALUE:
 * 0  - 添加消息成功
 * -1 - 添加消息失败，没有足够的内存空间
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 将消息数据添加到消息队列中，该消息队列必须之前调用过IPCMQ_init_for_send函数，
 * 否则后果为定义。
\*===========================================================================*/
int IPCMQ_put_message( struct IpcMessageQueue* queue, const void* message , int size );


/*===========================================================================*\
 * FUNCTION: dbus_get_message_timeout
 *===========================================================================
 * PARAMETERS:
 * queue 	- IpcMessageQueue结构体指针
 * __obj   - 一个有效的IpcObject_t对象的指针
 *
 * RETURN VALUE:
 * 0  - send sucessed
 * -1 - send failed
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 将消息队列queue，通过__obj对象发送出去，该queue指向的结构体必须调用函数 
 * IPCMQ_init_for_send初始化过，并且调用函数IPCMQ_put_message添加了消息数据。
 *
\*===========================================================================*/
int IPCMQ_send( struct IpcMessageQueue* queue , IpcObject_t* __obj );


/*===========================================================================*\
 * FUNCTION: IPCMQ_get_message
 *===========================================================================
 * PARAMETERS:
 * 
 *
 * RETURN VALUE:
 * 0  - receive sucessed
 * -1 - receive failed,no message received
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 
\*===========================================================================*/
int IPCMQ_get_message(struct IpcMessageQueue* queue , void** message , int* size);



/*===========================================================================*\
 * FUNCTION: dbus_get_message_timeout
 *===========================================================================
 * PARAMETERS:
 * 
 *
 * RETURN VALUE:
 * 
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 
\*===========================================================================*/
int IPCMQ_init_and_recv(struct IpcMessageQueue* queue ,IpcObject_t* __obj);


/*===========================================================================*\
 * FUNCTION: IPCMQ_free
 *===========================================================================
 * PARAMETERS:
 * queue 	- IpcMessageQueue结构体指针
 *
 * RETURN VALUE:
 * 0  - free sucessed
 * -1 - free failed
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 该函数用以释放调用过IPCMQ_init_and_recv，IPCMQ_init_for_send这两个函数的
 * IpcMessageQueue结构体.
 * 共有两种情况必须调用该函数：
 * 1.调用完IPCMQ_init_and_recv，然后读取其中的数据之后,必须调用该函数以释放空间。
 * 2.调用完IPCMQ_init_for_send ，将该消息队列发送出去后，必须调用该函数以释放空间。
 * If the data does not arrive, it returns immediately
\*===========================================================================*/
int IPCMQ_free(struct IpcMessageQueue* queue );


typedef void (*IPCMSG_handler_t)( IpcObject_t* __obj ,unsigned int __msgid , void* __data , int __size );

/*===========================================================================*\
 * FUNCTION: dbus_get_message_timeout
 *===========================================================================
 * PARAMETERS:
 * __obj 	- Must be a valid pointer to the DBusConnection object
 * 
 *
 * RETURN VALUE:
 * 
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * 
\*===========================================================================*/
int IPCMQ_dispatch( IpcObject_t* __obj , IPCMSG_handler_t __handler, int __timeout );

#endif
