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
#include <unistd.h>
//#include "c_app_test.h"

#ifdef _AIO_MAN

#define BUF_SIZE 20 /* size of buffers for read operations */
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define errMsg(msg) do { perror(msg); } while (0)
struct ioRequest /* application-defined structure for tracking I/O requests */
{
	int reqNum;
	int status;
	struct aiocb *aiocbp;
};

static volatile sig_atomic_t gotSIGQUIT = 0;
/* on delivery of SIGQUIT, we attempt to cancel all outstanding I/O requests */

static void quitHandler(int sig) /* handler for SIGQUIT */
{
	gotSIGQUIT = 1;
}

#define IO_SIGNAL SIGUSR1 /* signal used to notify I/O completion */

static void aioSigHandler(int sig, siginfo_t *si, void *ucontext)
{
	write(STDOUT_FILENO, "I/O completion signal received\n", 31);

	/* the corresponding ioRequest structure would be available as 
		struct ioRequest *ioReq = si->si_value.sival_ptr;
	and the file descriptor would then be available via
		ioReq->aiocbp->aio_fildes */
}
#endif

#endif //_AIO_READ_H
