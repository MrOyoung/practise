/************************************************************
**filename:aio_read.h
**autor:Rocky
**date:2017/05/22
**description:practice the usage of aio_read() - main aio
************************************************************/
#ifndef _AIO_READ_H
#define _AIO_READ_H

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <aio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
//#include "c_app_test.h"

#ifdef _AIO_MAN

#define BUF_SIZE 20 /*size of buffers for read operations*/
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define errMsg(msg) do { perror(msg); } while (0)
struct ioRequest
{
	int reqNum;
	int status;
	struct aiocb *aiocp;
}

static volatile sig_atomic_t gotSIGQUIT = 0;

#endif

#define BUFFER_SIZE 4096

typedef struct mcuDataBuf
{
	int size;
	unsigned char buf[BUFFER_SIZE];
} _DATA_BUFFER;
//int MAX_LIST = 2;

extern int myAioRead(int fileFd, char *tmpBuf);

#endif //_AIO_READ_H
