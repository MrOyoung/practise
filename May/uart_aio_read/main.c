#include "uart.h"

#define UART_DEV "/dev/ttymxc1"



char print_buffer[2048];


int main(int argc, char *argv[])
{
	int i, fd, size, line_size;
	int index = 0;
	unsigned char buf[256];

	fd = open_uart( UART_DEV );
    if( fd < 0 ){
        LOG( "open_uart error\n" );
        return 1;
    }
	printf("fd = %d\n", fd);
	
	myAioRead(fd, print_buffer);
	
	

#if 0
    while (is_running)
    {
    	int line_size = 0;
    	index = 0;
    	size = read(fd, buf, sizeof(buf));
    	
    	for (i = 0; i < size; i++){
    		print_buffer[index++] = '0';
    		print_buffer[index++] = 'x';
    		print_buffer[index++] = bytelow4bit_to_hex( buf[i] >> 4 ) ;
    		print_buffer[index++] = bytelow4bit_to_hex( buf[i] >> 0 ) ;
    		print_buffer[index++] = ' ';
    		
    		if( ++line_size ==  16 ){
    			print_buffer[index++] = '\n';
    			line_size = 0;
    		}	
    	}
    	
    	print_buffer[index++] = '\0';
    	puts( print_buffer );
    }
#endif
    return 1;
}
