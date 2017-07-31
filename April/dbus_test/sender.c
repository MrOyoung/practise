#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <time.h>
#include <unistd.h>


int main( int argc , char** argv)
{
	int count = 400;
	
    DBusConnection* dbus = get_dbus( "com.saic.ServiceDiData" );
    
 
 	if( argc > 1 )
 		count = atoi( argv[1] );
 		
 	printf("send count %d\n", count );
 		   
 

	unsigned long cur = get_tick_count();
	printf("current ticks: %lu \n", cur );
    
    
    int array[2];


    int index = 0;

	
    while( index < count ){

        array[0] = (int)get_tick_count();
        array[1] = (int)index++;

        dbus_send( dbus , array , sizeof(array) ,"/com/saic/AppCluster" , "ServiceDiData.signal.type");

    }
    
    printf("send ok , cost time:%lu \n" , (get_tick_count() - cur) );

    
    return 0;
}
