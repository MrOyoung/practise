/************************************************************
**filename:get_syslog.h
**autor:Rocky
**date:2017/05/24
**description:get the message from file /proc/kmsg
************************************************************/

#ifndef _GET_SYSLOG_H
#define _GET_SYSLOG_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../include/get_time_str.h"

#define ZTE_SYSLOG_DIR_FROM		"/proc/kmsg"
#define ZTE_SYSLOG_DIR_TO		"/home/root/log/kmsg.txt"

#define BUFFER_SIZE 1024


void *start_monitor_kmsg(void *arg);

#endif //_GET_SYSLOG_H
