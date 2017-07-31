/************************************************************
 **filename:slist.h
 **autor:Rocky
 **date:2017/07/03
 **description:head file,reference the function of list
 ************************************************************/

struct MODULE_LOG_INFO
{
	FILE 	*fp;
	int 	module_name;
	char 	*name;
	char 	*path;
}


struct slist;

struct slist
{
	struct MODULE_LOG_INFO info;
	struct slist *next;
}





