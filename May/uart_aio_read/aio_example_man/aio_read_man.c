/************************************************************
 **autor:Rocky
 **date:2017/05/22
 **description:practice the usage of aio_read() -man aio
 ************************************************************/
#include "aio_read_man.h"

int main(int argc, char *argv[])
{
	struct ioRequest *ioList;
	struct aiocb *aiocbList;
	struct sigaction sa;
	int s, j;
	int numReqs;	/* total number of queued I/O requests */
	int openReqs;	/* number of I/O requests still in progress */

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <pathname> <pathname>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	numReqs = argc - 1;

	/* allocate our arrays */
	ioList = calloc(numReqs, sizeof(struct ioRequest));
	if (ioList == NULL) errExit("calloc");

	aiocbList = calloc(numReqs, sizeof(struct aiocb));
	if (aiocbList == NULL) errExit("calloc");

	/* establish handlers for SIGQUIT and the I/O completion signal */
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	sa.sa_handler = quitHandler;
	if (sigaction(SIGQUIT, &sa, NULL) == -1) errExit("sigaction");

	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = aioSigHandler;
	if (sigaction(IO_SIGNAL, &sa, NULL) == -1) errExit("sigaction");

	/* open each file specified on the command line, and queue a read request on the resulting file descriptor */
	for (j = 0; j < numReqs; j++)
	{
		ioList[j].reqNum = j;
		ioList[j].status = EINPROGRESS;
		ioList[j].aiocbp = &aiocbList[j];

		ioList[j].aiocbp->aio_fildes = open(argv[j + 1], O_RDONLY);
		if (ioList[j].aiocbp->aio_fildes == -1) errExit("open");
		printf("opened %s on descriptor %d\n", argv[j + 1], ioList[j].aiocbp->aio_fildes);

		ioList[j].aiocbp->aio_buf = malloc(BUF_SIZE);
		if (ioList[j].aiocbp->aio_buf == NULL) errExit("malloc");

		ioList[j].aiocbp->aio_nbytes = BUF_SIZE;
		ioList[j].aiocbp->aio_reqprio = 0;
		ioList[j].aiocbp->aio_offset = 0;
		ioList[j].aiocbp->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
		ioList[j].aiocbp->aio_sigevent.sigev_signo = IO_SIGNAL;
		ioList[j].aiocbp->aio_sigevent.sigev_value.sival_ptr = &ioList[j];

		s = aio_read(ioList[j].aiocbp);
		if (s == -1) errExit("aio_read");
	}

	openReqs = numReqs;

	/* loop, monitoring status of I/O requests */
	while (openReqs > 0)
	{
		sleep(3); /* delay between each monitoring step */

		if (gotSIGQUIT)
		{
			/* on receipt of SIGQOUT, attempt to cancel each of the outstanding I/O requests, and display status returned from the cancellation requests */
			printf("got SIGQUIT; cancelling I/O requests: \n");
			for (j = 0; j < numReqs; j++)
			{
				if (ioList[j].status == EINPROGRESS)
				{
					printf("Request %d on descriptor %d:", j, ioList[j].aiocbp->aio_fildes);
					s = aio_cancel(ioList[j].aiocbp->aio_fildes, ioList[j].aiocbp);
					if (s == AIO_CANCELED) printf("I/O canceled\n");
					else if (s == AIO_NOTCANCELED) printf("I/O not canceled\n");
					else if (s == AIO_ALLDONE) printf("I/O all done");
					else errMsg("aio_cancel");
				}

			}

			gotSIGQUIT = 0;
		}

		/* check the status of each I/O request that is still in progress */
		printf("aio_error():\n");
		for (j = 0; j < numReqs; j++)
		{
			if (ioList[j].status == EINPROGRESS)
			{
				printf("for request %d (descriptor %d): ", j, ioList[j].aiocbp->aio_fildes);
				ioList[j].status = aio_error(ioList[j].aiocbp);

				switch (ioList[j].status)
				{
					case 0:
						printf("I/O succeeded\n");
						break;
					case EINPROGRESS:
						printf("In progress\n");
						break;
					case ECANCELED:
						printf("Canceled\n");
						break;
					default:
						errMsg("aio_error");
						break;
				}

				if (ioList[j].status != EINPROGRESS) openReqs--;
			}
		}

	}

	printf("All I/O requests completed\n");

	/* check status return of all I/O requests */
	printf("aio_return():\n");
	for (j = 0; j < numReqs; j++)
	{
		ssize_t s;

		s = aio_return(ioList[j].aiocbp);
		printf(" for request %d (descriptor %d): %zd\n", j, ioList[j].aiocbp->aio_fildes, s);
	}

	exit(EXIT_SUCCESS);
}

