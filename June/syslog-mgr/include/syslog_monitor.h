/************************************************************
**Filename:syslog_monitor.h
**Description:monitor log file
**Autor:Rocky
**Date:2017/05/25
************************************************************/

#ifndef _SYSLOG_MONITOR_H
#define _SYSLOG_MONITOR_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "Debug.h"

#define KBSIZE (1024)
#define MBSIZE (1024 * 1024)
#define GBSIZE (1024 * 1024 * 1024)

#define FILENO_MAX		10
//#define DIRSIZE_MAX		4 * (MBSIZE)
#define DIRSIZE_MAX		2 * (KBSIZE)

/* keep the message of each module-log directory */
struct LOG_DIR_UNIT
{
	int		file_size_sum;
	int		file_num_sum;
	char	file_path[PATH_MAX];
	char	oldest_file[PATH_MAX];
};


/* referenced by function - main, start the thread */
void *start_monitor_logdir(void *argv);

/*********************************************************************************
 **Function		: _do_scan_logdir
 **Descriptor	: a call-back function
 **		Use the func readdir() to scan the parameter(dirpath)
 **			if the member is a directory, reference this func;
 **			if the member is a directory, record the variable among struct LOG_DIR_UNIT
 **		After readdir(), do check the size of the file in the directory
 **Autor		: Rocky
 **Date			: 2017-05-26
 **Version		: log_mgr V0.1
 **********************************************************************************/
static int _do_scan_logdir(const char *dirpath);

#endif //_SYSLOG_MONITOR_H
