/************************************************************
 **filename:syslog_monitor.c
 **autor:Rocky
 **date:2017/05/24
 **description:遍历模块目录及模块目录下的log日志文件，\
 当log日志文件数量或者总的log日志文件相加大小超出设定范围,\
 删除创建最早的log文件
 ************************************************************/
#include "../include/syslog_monitor.h"


/* start to monitor log directory */
void *start_monitor_logdir(void *argv)
{
	int		running = 1;
	char 	*log_dir = NULL;

	log_dir = (char *)argv;

	while (running)
	{	
		/* loop */
		_do_scan_logdir(log_dir);

		sleep(5);
	}

	return NULL;
}

static int _get_file_size(const char *dirpath, const char *filename);
static int _get_oldest_file(char *older_file ,const char *filename);
static int _remove_oldest_file(const char *filename);

/* reference to head file */
static int _do_scan_logdir(const char *dirpath)
{
	if (!dirpath)
	{
		printf("dirpath is empty\n");
		return -1;
	}

	DIR 				*dir_ptr;
	char 				deeper_dir[PATH_MAX] = {0};
	struct dirent		*direntp;
	struct LOG_DIR_UNIT mod_dir_unit;

	bzero(&mod_dir_unit, sizeof(struct LOG_DIR_UNIT));

	if (NULL == (dir_ptr = opendir((char *)dirpath)))
	{
		perror("opendir error");
		return -1;
	}

	while ((direntp = readdir(dir_ptr)) != NULL)
	{
		if ((!strcmp(direntp->d_name, ".")) || (!strcmp(direntp->d_name, "..")))
		{
			continue;
		}

		sprintf(deeper_dir, "%s/%s", dirpath, direntp->d_name);
		if (DT_DIR == direntp->d_type)//directory
		{
			//printf("directory : %s \n", direntp->d_name);

			_do_scan_logdir(deeper_dir);
		}
		else if (DT_REG == direntp->d_type)//regular file
		{
			printf("file : %s \n", direntp->d_name);
			mod_dir_unit.file_size_sum += _get_file_size(dirpath, direntp->d_name); 
			mod_dir_unit.file_num_sum++;

			_get_oldest_file(mod_dir_unit.oldest_file, deeper_dir);//保存时间戳最旧的log文件

		}

	}/* end of while() */

	printf("dir size = %d\noldest_file is %s\nfile_num = %d\n", 
			mod_dir_unit.file_size_sum, 
			mod_dir_unit.oldest_file, 
			mod_dir_unit.file_num_sum);

	/* After scaning a module directory, delete the oldest file 
	 *	if the sum size of the log-file is bigger than DIRSIZE_MAX 
	 *	or the num of the log-file is bigger than FILENO_MAX 
	 */
	//if ((mod_dir_unit.file_size_sum > DIRSIZE_MAX) || (mod_dir_unit.file_num_sum > FILENO_MAX))

	/* After scaning a module directory, delete the oldest file
	 *	if the sum size of the log-file is bigger than DIRSIZE_MAX  
	 */
	if ((mod_dir_unit.file_size_sum > DIRSIZE_MAX))
	{
		_remove_oldest_file(mod_dir_unit.oldest_file);
	}

	closedir(dir_ptr);

	return 0;
}


/* get the size of the file */
static int _get_file_size(const char *dirpath, const char *filename)
{
	char		logfile_ptr[PATH_MAX] = {0};
	struct stat info;

	sprintf(logfile_ptr, "%s/%s", dirpath, filename);

	if (-1 == stat(logfile_ptr, &info))
	{
		perror("stat error");
		return 0;
	}

	return info.st_size;
}


/* if older_file is empty or newer than filename, replace it by filename */
static int _get_oldest_file(char *older_file ,const char *filename)
{
	if ((0 == strlen(older_file)) || (strcmp(older_file, filename) > 0))
	{
		//bzero(older_file, strlen(older_file));
		strncpy(older_file, filename, strlen(filename));
	}

	return 0;
}


static int _remove_oldest_file(const char *filename)
{
	if (strlen(filename))
	{
		if (-1 == remove(filename))
			perRtn("remove error", -1);

		printf("remove %s success\n", filename);
	}

	return 0;
}

