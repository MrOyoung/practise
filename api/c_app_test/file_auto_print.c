#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

#define MAX_SIZE 1024

/*print the file*/
int file_auto_print(const char *file_path)
{
	assert(file_path != NULL);
	
	char buf_arr[MAX_SIZE] = {0};
	FILE *fp = NULL;
	
	fp = fopen(file_path, "r");
	if(NULL == fp)
	{
		perror("fopen a.txt error");
		return -1;
	}
	
	while(fgets(buf_arr, MAX_SIZE, fp))
	{
		printf("%s", buf_arr);
		memset(buf_arr, 0, MAX_SIZE);

		usleep(50000);
	}
	
	fclose(fp);	
	return 0;
}
