#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include "common.h"

#define EXIT_FAILED -1
//#define EXIT_SUCCESS 1

void show_file_info(char *filename, struct stat *info_p);
void dostat(char *filename);
void do_ls(char diename[]);
char *gid_to_name(gid_t gid);
char *uid_to_name(uid_t uid);
void mode_to_letters(int mode, char str[]);
//int stat_test(const char *file_path);

/*
int stat_test(const char *file_path)
{
	struct stat sb;
	
	if (NULL == file_path)
	{
		fprintf(stderr, "pathname error\n");
		return EXIT_FAILED;
	}
	
	if (stat(file_path, &sb))
	{
		perror("stat");
		return EXIT_FAILED;
	}
	
	printf("File type:	");
	switch (sb.st_mode & S_IFMT)
	{
		case S_IFBLK:	 printf("block device\n");		break;
		case S_IFCHR:	 printf("character device\n");	break;
		case S_IFDIR:	 printf("directory\n");			break;
		case S_IFIFO:	 printf("FIFO / pipe\n");		break;
		case S_IFLNK:	 printf("symlink\n");			break;
		case S_IFREG:	 printf("regular file\n");		break;
		case S_IFSOCK:	 printf("socket\n");			break;
		default:		 printf("unknown?\n");
	}
	
	printf("I-node number:				%ld\n", (long)sb.st_ino);
	printf("Mode:						%lo (octal)\n", (unsigned long)sb.st_mode);
	printf("Link count:					%ld\n", (long)sb.st_nlink);
	printf("Ownership:					UID=%ld GID=%ld\n", (long)sb.st_uid, (long)sb.st_gid);
	printf("Preferred I/O block size: 	%ld bytes\n", (long)sb.st_blksize);
	printf("File size:					%lld bytes\n", (long long)sb.st_size);
	printf("Blocks allocated:			%lld\n", (long long)sb.st_blocks);
	
	printf("Last status change:			%s", ctime(&sb.st_ctime));
	printf("Last file access:			%s", ctime(&sb.st_atime));
	printf("Last file modification:		%s", ctime(&sb.st_mtime));
	
	return EXIT_SUCCESS;
}
*/

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");
	
	if (S_ISDIR(mode))			str[0] = 'd';
	else if (S_ISCHR(mode))		str[0] = 'c';
	else if (S_ISBLK(mode))		str[0] = 'b';
	else if (S_ISLNK(mode))		str[0] = 'l';
	else if (S_ISSOCK(mode))	str[0] = 'b';

	if ((mode & S_IRUSR))		str[1] = 'r';
	if ((mode & S_IWUSR))		str[2] = 'w';
	if ((mode & S_IXUSR))		str[3] = 'x';

	if (mode & S_IRGRP)		str[4] = 'r';
	if (mode & S_IWGRP)		str[5] = 'w';
	if (mode & S_IXGRP)		str[6] = 'x';
	
	if (mode & S_IROTH)		str[7] = 'r';
	if (mode & S_IWOTH)		str[8] = 'w';
	if (mode & S_IXOTH)		str[9] = 'x';
}

char *uid_to_name(uid_t uid)
{
	struct passwd *pw_ptr;
	static char numstr[10];
	
	if ((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
	struct group *grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL)
	{
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

void do_ls(char dirname[])
{
	DIR *dir_ptr;
	struct dirent *direntp;
	
	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls2 : cannot open %s \n", dirname);
	else
	{
		while ((direntp = readdir(dir_ptr)) != NULL)
			dostat(direntp->d_name);
		//close(dir_ptr);
	}
}

void dostat(char *filename)
{
	struct stat info;
	
	if (stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char *filename, struct stat *info_p)
{
	//char *uid_to_name(), *ctime(), *gid_to_name(), *filemode;
	char modestr[11];
	
	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf(" %2d", (int)info_p->st_nlink);
	printf(" %-8s", uid_to_name(info_p->st_uid));
	printf(" %-8s", gid_to_name(info_p->st_gid));
	printf(" %8ld", (long)info_p->st_size);
	printf(" %.12s", 4 + ctime(&info_p->st_mtime));
	printf(" %s\n", filename);
}

int do_scan(int ac, char *av[])
{
	if (1 == ac)
		do_ls(".");
	else
	{
		while (--ac)
		{
			printf("%s: \n", *(++av));
			
			do_ls(*av);
		}
	}

	return 0;
}

