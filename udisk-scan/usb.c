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
	if (-1 == (sockfd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)))
	{
		prtExit(-1, "socket error %s\n", strerror(errno));
	}

	/* set the size of receive buffer */
	setsockopt(sockfd, 
			SOL_SOCKET,
			SO_RCVBUF, 
			(const void *)&recv_buffer_size,
			sizeof(recv_buffer_size));	

	if (-1 == bind(sockfd, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl)))
	{
		prtExit(-1, "bind error %s\n", strerror(errno));
	}

	return sockfd;
}

const char *add_str = "add@/devices";
const char *rmv_str = "remove@/devices";

enum
{
	ADD,
	REMOVE
};

char dev_name[32] = {0};

unsigned int udisk_get_name(char *buf, int flag)
{
	char *tmp = strstr(buf, "block/sdb/");
	if (!tmp)
		return -1;

	tmp = strrchr(tmp, '/');

	bzero(dev_name, sizeof(dev_name));
	strcat(dev_name, "/dev");
	strcat(dev_name, tmp);

	if (ADD == flag)
		prtMsg("[ add ] %s\n", dev_name);
	else if (REMOVE == flag)
		prtMsg("[ remove ] %s\n", dev_name);

	return 0;
}

unsigned int udisk_parse(char *buf)
{
	char *tmp = buf;
	int plug_flag;

	if (!strncmp(tmp, add_str, strlen(add_str)))
		plug_flag = ADD;
	else if (!strncmp(tmp, rmv_str, strlen(rmv_str)))
		plug_flag = REMOVE;

	udisk_get_name(tmp, plug_flag);	

	return 0;
}

void udisk_recv(int sockfd)
{
	int retval;
	fd_set read_set;

	char uevent_buf[BUFFER_SIZE * 4] = {0};
	while(1)
	{
		FD_ZERO(&read_set);
		FD_SET(sockfd, &read_set);	

		/* block */
		retval = select((sockfd + 1), &read_set, NULL, NULL, NULL);
		if (-1 == retval)
		{
			prtMsg("select error %s\n", strerror(errno));
		}

		if (FD_ISSET(sockfd, &read_set))
		{
			bzero(uevent_buf, sizeof(uevent_buf));
			retval = recv(sockfd, &uevent_buf, sizeof(uevent_buf), 0);
			if (-1 == retval)
			{
				prtMsg("read error %s\n", strerror(errno));
				continue;
			}
			else
			{
				prtMsg("recv msg : %s\n", uevent_buf);

				udisk_parse(uevent_buf);
			}
		}//end of if()
	}//end of while(1)
}

int udisk_work(void *arg)
{
	int udisk_fd = udisk_create_socket_fd();
	prtMsg("socket fd = %d\n", udisk_fd);

	udisk_recv(udisk_fd);

	return 0;
}

