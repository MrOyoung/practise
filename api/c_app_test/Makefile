//CC=gcc
CC=arm-poky-linux-gnueabi-gcc

CFLAGS:=
CFLAGS+=-lpthread -Wall

CSOURCE:=main.c
CSOURCE+=exchange_value.c
CSOURCE+=file_auto_print.c
CSOURCE+=mutex_test.c
CSOURCE+=parameter.c
CSOURCE+=writev_test.c
CSOURCE+=structure.c
CSOURCE+=stat_test.c
CSOURCE+=env_api.c
CSOURCE+=debug.c
CSOURCE+=str_test.c
CSOURCE+=vsnprintf/vsnprintf.c
CSOURCE+=cmdline_read.c

target=main

all::$(CSOURCE)
	$(CC) -o $(target) $^ $(CFLAGS)

clean:
	rm $(target) 
