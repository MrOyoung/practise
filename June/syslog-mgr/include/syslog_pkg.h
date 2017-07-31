/************************************************************
**filename:syslog_pkg.h
**autor:Rocky
**date:2017/05/26
**description:syslog msg pkg (head file)
************************************************************/

#ifndef _SYSLOG_PKG_H
#define _SYSLOG_PKG_H

#include <stdio.h>
#include "Module_Def.h"
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define _arg_in
#define _arg_out

#define TIMEVAL_SIZE 32 //时间标签的长度
#define LEVELVAL_SIZE 16 //level标签的长度

enum _log_level
{
	info,
	warn,
	error,
	emerg
};


//int syslog_pkg_log(enum _log_level level, char *log_msg, _arg_out char *msg);

unsigned int syslog_msg_pkg(_arg_out char *pMsg, _MODULE_NAME name, enum _log_level level, char *fmt, ...);
#endif //_SYSLOG_PKG_H
