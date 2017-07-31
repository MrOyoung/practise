#ifndef _UART_H_
#define _UART_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "aio_read.h"

#define UART_LOG printf
#define LOG printf

int open_uart(const char* port);

int uart_setup(int fd,int speed,int flow_ctrl,int databits, int stopbits,int parity);

#endif
