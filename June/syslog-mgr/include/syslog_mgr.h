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
#include "util.h"


#define _arg_in
#define _arg_out

#ifndef _BOOL_DEFINE
#define _BOOL_DEFINE

typedef enum
{
	false = 0,
	true = 1
} bool;

#endif //_BOOL_DEFINE

struct MODULE_LOG_INFO {
	FILE* fp;
	char *msg;
	char name[NAME_MAX];
	char path[PATH_MAX];
};

static char work_path[PATH_MAX] = "/home/root";
static char log_dir_path[PATH_MAX] = "/home/root/log";

//char work_path[PATH_MAX];
//char log_dir_path[PATH_MAX];

/****************** syslog_monitor ****************/
void *logfile_monitor(void *argv);

/****************** syslog_mgr ****************/
unsigned int record_module_log(_MODULE_NAME module_num, unsigned char *log_msg, unsigned int log_size);

void get_module_name(const _MODULE_NAME module_num, _arg_out char *module_name_ptr);

int is_log_file_exist(const char *module_name);

int merge_path(const char *name, char *path);

int get_logfile_name(char *filename);

int get_logfile_path(_MODULE_NAME module_num, char *pathname);

bool write_log(const char *log_path, unsigned char *log_msg, unsigned int log_size);

void *logfile_record(void *argv);

#endif //_SYSLOG_SERVICE_H
