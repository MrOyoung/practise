#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "./project/include/Message_Def.h"
#include "./project/include/common.h"
#include <pthread.h>
#include "timer.h"

  
  
int main( int argc , char ** argv)
{  


 		if( argc != 2 ){
    	printf("please input sleep time!\n");
    	return 1;
    	}


	DBusConnection* dbus = get_dbus("com.saic.ServiceDiData");
	
                          
    
    unsigned int message1[2];
    unsigned int message2[2];
    
    int speed = 1;
    int tachometer = 50;
    int speed_inc = 1;
    int tachometer_inc = 50;
    
   	
    	
    int slp = atoi(argv[1]) * 1000;

    while( 1 ){

        message1[0] = SERVICE_DI_SPEED;
        message2[0] = SERVICE_DI_TACHOMETER;
        
        if( speed > 200 || speed <= 0 )
        	speed_inc = -speed_inc;
        
        if( tachometer > 7000 || tachometer <= 0 )
        	tachometer_inc = -tachometer_inc;
        	
        speed += speed_inc;
        tachometer += tachometer_inc; 
        
        message1[1] = speed;
        message2[1] = tachometer;

       // dbus_send(dbus, message1 , sizeof(message1) , "/com/saic/AppCluster" , "ServiceDiData.signal.type"  );
        dbus_send(dbus, message2 , sizeof(message2) , "/com/saic/AppCluster" , "ServiceDiData.signal.type" );
        
        
        usleep( slp );
    }

    return 0;  
} 
