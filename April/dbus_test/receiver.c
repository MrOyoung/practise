#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "msgid.h"

int main(int argc,char** argv)
{
    (void)argc;(void)argv;

    DBusConnection* dbus = get_dbus( "com.saic.AppCluster" );

    if( dbus_add_match( dbus , "type='signal',path='/com/saic/AppCluster'" ) < 0 ){
        printf( "add match error!" );
        exit( 1 );
    }
    
    unsigned int array[8];
    int  size;
    
    int empty = 0;
    while( 1 ){
        if( dbus_get_message_timeout( dbus , array, &size , (empty>5)?-1:0 ) >= 0 ){
        
           
           unsigned long send_tick = (unsigned long)array[0];
           
           printf("cost ticks: %ld - index :%d\n", get_tick_count() - send_tick , array[1] );
           
           

            empty = 0;
        }else{
            empty++;
        }
    }

    return 0;
}
