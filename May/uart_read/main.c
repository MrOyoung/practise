#include "uart.h"

#define UART_DEV "/dev/ttymxc1"



char print_buffer[2048];

char bytelow4bit_to_hex( unsigned char c )
{
	static  char hexs[] = { '0' , '1' , '2' , '3' ,
							'4' , '5' , '6' , '7' ,
							'8' , '9' , 'A' , 'B' ,
							'C' , 'D' , 'E' , 'F' };
							
	return hexs[ c & 0xf ];
}



int main(int argc, char *argv[])
{
	int i, fd, size;
	int index = 0;
	unsigned char buf[256];

	fd = open_uart( UART_DEV );
    if( fd < 0 ){
        LOG( "open_uart error\n" );
        return 1;
    }
    
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
    
    return 1;
}
