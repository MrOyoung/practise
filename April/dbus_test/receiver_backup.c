#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"




const char* ultoa( unsigned long l )
{
	static char buffer[256] = {0};
	
	
	char* p = buffer + 255;
	
	if( 0 == l )
		return "0";
		
	while( l ){
		*(--p) = (l % 10) + '0';
		l /= 10;
	}
	
	return p;
}




int main(int argc,char** argv)
{
    (void)argc;(void)argv;

    struct big_dbus_msg msg;

    DBusConnection* dbus = get_dbus( "com.saic.AppCluster" );


    if( dbus_add_match( dbus , "type='signal',path='/com/saic/AppCluster'" ) < 0 ){
        printf( "add match error!" );
        exit( 1 );
    }
   
    int ret;
    int* array;
    int  size;

	char buffer[256] = "cost time:";
	
	unsigned long max_tick = 0;
	unsigned long min_tick = 1000000;
	int data_size = 0;
	
	int print_count = 1000;
	if( argc > 1 ){
		print_count = atoi( argv[1] );
		if( 0 == print_count )
			print_count = 1000;
	}
		
	


    int no_msg_cnt = 0;
    while( 1 ){
    	
    	if( data_size >= print_count ){
        			printf("receive data:%lu - max const time:%lu - min cost time:%lu \n",data_size , max_tick , min_tick );
        			data_size = 0;
        			max_tick = 0;
        			min_tick = 1000000;
        }
    	
    	
    	
    	
    	dbus_connection_read_write( dbus ,(no_msg_cnt>20)?-1:0 );
    	
        ret = big_msg_read( dbus , &msg );
        if( ret < 0 ){
        	no_msg_cnt++;   	
        	continue;
        }
        no_msg_cnt = 0;


		
		
        while( big_msg_get_array( &msg , (void**)&array , &size )  ){
        	unsigned long send_tick = get_tick_count() - (unsigned long)array[0];
        	
        	
        	if( send_tick > max_tick )
        		max_tick = send_tick;
        	else if( send_tick < min_tick )
        		min_tick = send_tick;
       
       
       
       		data_size++;
       		/*
            strcpy( buffer + 11 , ultoa( get_tick_count() - send_tick )  );
            strcat( buffer , "  index: " );
            strcat( buffer , ultoa( (unsigned long)array[1] )  );
          
          
          	
          
          
            puts( buffer );
            */
            
            
            
            
            
        }
        
        
     
        dbus_message_unref( msg.m );
    }

    return 0;
}
