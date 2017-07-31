/************************************************************
**filename:syslog_mgr.h
**autor:Rocky
**date:2017/05/24
**description:syslog manager
************************************************************/

#ifndef _SYSLOG_MANAGER_H
#define _SYSLOG_MANAGER_H

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include "Module_Def.h"
#include "Debug.h"
#include "get_time_str.h"
#include "ipc.h"


#define _arg_in
#define _arg_out


/* structure : module dir info */
struct MODULE_LOG_INFO {
	FILE 		*fp;
	int  		num;
	char 		name[NAME_MAX];
	char 		path[PATH_MAX];
	struct tm 	tv;
};

/****************** gloable varaiable ****************/

/* gloable varaible each module has it's member, containing the basal info */
static struct MODULE_LOG_INFO module_arr[MODULE_SUM];


/****************** syslog_mgr ****************/

void *SM_start_to_record(void *argv);


#endif //_SYSLOG_SERVICE_H
