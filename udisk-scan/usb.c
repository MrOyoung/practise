/********************************************
 *filename		: udisk.c
 *description	: functiong about udisk
 *author			: Rocky
 *date			: 2017/08/08
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>


#include "debug.h"
#include "config.h"
#include "common.h"

static const int recv_buffer_size = 1024;

static inline int udisk_create_socket_fd(void)
{
	int sockfd;
	struct sockaddr_nl snl;

	memset(&snl, 0, sizeof(struct sockaddr_nl));	
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();	/* program ID */
	snl.nl_groups = 1;		/* 多组播掩码 */	

	/* create socket fd */
	if (-1 == (sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT)))
	{
		prtExit(-1, "socket error %s\n", strerror(errno));
	}

	/* set the size of receive buffer */
	setsockopt(sockfd, 
			SOL_SOCKET,
			SO_RCVBUF, 
			(const void *)&recv_buffer_size,
			sizeof(BUFFER_SIZE));	

	if (-1 == bind(sockfd, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl)))
	{
		prtExit(-1, "bind error %s\n", strerror(errno));
	}

	return sockfd;
}


int udisk_monitor(void *arg)
{
	int retval;
	fd_set read_set;
	struct timeval tm;
	char uevent_buf[BUFFER_SIZE] = {0};

	int udisk_fd = udisk_create_socket_fd();
	int maxfd = udisk_fd + 1;
	(void)maxfd;

	prtMsg("socket fd = %d\n", udisk_fd);

	while(1)
	{

		tm.tv_sec	= 2;
		tm.tv_usec	= 0;

		FD_ZERO(&read_set);
		FD_SET(udisk_fd, &read_set);	

		retval = select((udisk_fd + 1),
				&read_set,
				NULL,
				NULL,
				NULL);
		if (-1 == retval)
		{
			prtMsg("select error %s\n", strerror(errno));
		}
		else if (0 == retval)
		{
			prtMsg("select timeout\n");	
			continue;
		}
		else if (FD_ISSET(udisk_fd, &read_set))
		{
			bzero((void *)uevent_buf, BUFFER_SIZE);
			retval = read(udisk_fd, uevent_buf, BUFFER_SIZE);
			if (-1 == retval)
			{
				prtExit(-1, "read error %s\n", strerror(errno));
			}
			else
			{
				prtMsg("recv msg : %s\n", uevent_buf);	
			}
		}//end of if()
	}//end of while(1)
}//end of func()

