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
#define DIRSIZE_MAX		9 * (MBSIZE)
//#define DIRSIZE_MAX		2 * (KBSIZE)

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


#endif //_SYSLOG_MONITOR_H
