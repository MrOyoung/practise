#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <time.h>
#include <unistd.h>


int main( int argc , char** argv)
{
    (void)argc;(void)argv;

    DBusConnection* dbus = get_dbus( "com.saic.ServiceDiData" );
    
    struct big_dbus_msg msg;
    
    int array[2];
    int index = 0;
    
	int count = 400;
    
    if( argc > 1 )
 		count = atoi( argv[1] );
 		
 	printf("this is powerful sender!!!!!!!\n");
 	printf("send count %d\n", count );
 	
 	unsigned long cur = get_tick_count();
	printf("current ticks: %lu \n", cur );
    while( index < count ){
        big_msg_write_init( &msg , "/com/saic/AppCluster" , "ServiceDiData.signal.type");

        
        int sub = 50; 
        while( (sub--) && (index < count) ) {
        
        	array[0] = (int)get_tick_count();
            array[1] = index++;
        
        
            if( big_msg_append_array(  &msg , array , sizeof(array ) ) < 0 ){
                printf("big msg append array error\n!!");
                exit(1);
            }
        }

        dbus_connection_send(  dbus , msg.m , 0 );
        dbus_connection_flush( dbus );
        dbus_message_unref( msg.m );
    }

	printf("send ok , cost time:%lu \n" , (get_tick_count() - cur) );
    
    return 0;
}
