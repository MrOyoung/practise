#ifndef _DBUSCLIENT_H_
#define _DBUSCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
 *注意添加这两个头文件
 * */
#include <dbus/dbus-glib.h>  
#include <dbus/dbus.h>  

DBusConnection* get_dbus(const char* well_known_name)
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

    ret =dbus_bus_request_name(connection, well_known_name ,DBUS_NAME_FLAG_REPLACE_EXISTING,&err);  
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

int dbus_send(DBusConnection* dbus,void* array,int size,const char* path,const char*interface )
{
    int ret = 0;
    dbus_uint32_t serial;
    void *v_ARRAY = array;

    DBusMessage* msg =dbus_message_new_signal( path ,interface ,"data");
    if( !msg )
        return -1;

    if( ! dbus_message_append_args(msg, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &v_ARRAY, size , DBUS_TYPE_INVALID) ){
        ret = -1;
        goto __unref_msg;
    }

    if( !dbus_connection_send(dbus,msg,&serial)){  
        fprintf(stderr,"Out of Memory!\n");  
        ret = -1;
        goto __unref_msg;
    }  

    dbus_connection_flush(dbus); 

__unref_msg:
    dbus_message_unref( msg );
    return ret;
}


int dbus_get_message(DBusConnection* dbus, int* messageid , int* data)
{
    DBusMessage* msg;
    int ret = 0;

    int* array;
    int len;

    DBusError err;
    dbus_error_init(&err);  

    if( FALSE == dbus_connection_read_write(dbus,0) )
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

    if( (sizeof(int)*2) != len  ){
        ret = -1;
        goto __unref_msg;
    }
    
    *messageid = array[0];
    *data      = array[1];

__unref_msg:
    dbus_message_unref(msg);  
    return ret;
}


int dbus_add_match(DBusConnection* dbus, const char* rules )
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

class DBusClient{
public:
    DBusClient(){
        m_pConnection = NULL;
        m_pInterface = NULL;
    }

    ~DBusClient(){
        Destroy();
    }

/*获取消息*/
    bool GetMessage(int* pMessageID, int* pData ){
        return 
            (0 == dbus_get_message( m_pConnection , pMessageID , pData  ));
    }
    
/*发送消息*/
    bool Send(void* pData, int size , const char* pObjectPath ){
        return 
            (0 == dbus_send(m_pConnection , pData , size , pObjectPath , m_pInterface ));
    }

    bool AddMatch(const char* pRules){
        return 
            (0 == dbus_add_match(m_pConnection, pRules));
    }

    bool Create(const char* pName , const char* pInterface){
        m_pConnection = get_dbus( pName );
        if( !m_pConnection )
            return false;
        
        m_pInterface = new char[ ( (strlen(pInterface) + 15 + 1) & (~15) ) ];
        strcpy( m_pInterface , pInterface  );
        return true;
    }

    void Destroy(){
        if( m_pConnection ){
            dbus_connection_unref(m_pConnection);
            m_pConnection = NULL;
        }

        if( m_pInterface ){
            delete[] m_pInterface;
            m_pInterface = NULL;
        }
    }

private:
    char* m_pInterface;
    DBusConnection* m_pConnection;
};

#endif
