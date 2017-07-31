#ifndef _LOCALTIME_H_
#define _LOCALTIME_H_

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


void *localtime_pth1(void *arg);

void *localtime_pth2(void *arg);

#endif // _LOCALTIME_H_
