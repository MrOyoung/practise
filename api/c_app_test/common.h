#ifndef _C_APP_TEST
#define _C_APP_TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vsnprintf/vsnprintf.h"
#include "cmdline_read.h"

#define BUFFER_SIZE 1024
typedef struct
{
	unsigned char msg_buf[BUFFER_SIZE];
	unsigned int size;
} _DATA_BUFFER;

typedef enum
{
	ENV_SERVICE_GET,			//0
	ENV_SERVICE_VALUE,			//1
	ENV_SERVICE_SET,			//2
	ENV_SERVICE_OK,
	ENV_SERVICE_FUNC_ERROR,
	ENV_SERVICE_NOT_EXIST
}MSG_ID;

typedef struct env_unit
{
	char *env_name;
	char *env_value;
} _ENV_UNIT;

extern void exchange_value(void);
extern int file_auto_print(const char *file_path);
extern int mutex_test(void);
extern int parameter(int len);
extern int writev_test(void);
extern int structure(void);
extern int stat_test(const char *file_path);
extern int do_scan(int ac, char *av[]);
extern char *get_env(const char *name);
extern int set_env(const char *name, const char *value, int overwrite);
extern void malloc_test();
extern int env_test(void* msg_data , unsigned int msg_size);
extern void multi_para();
extern void foo(char *str, char *fmt, ...);
extern unsigned long get_clocktick();
extern int str_test();

extern int is_xitos_crash(void);

#endif //_C_APP_TEST
