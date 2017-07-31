#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILE_SIZE 1024

int main(int argc, char *argv[])
{
	int fd;
	char *map_ptr;
	char buf[FILE_SIZE] = {0};
	
	fd = open("data.txt", O_CREAT | O_RDWR, 00600);
	if (-1 == fd)
	{
		perror("open error");
		return -1;
	}

	//memcpy(buf, "1", FILE_SIZE);
	//write(fd, buf, FILE_SIZE);
	//fsync(fd);
	
	ftruncate(fd, FILE_SIZE);
	
	map_ptr = mmap(NULL, FILE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if (-1 == *map_ptr)
	{
		perror("mmap error");
		return -1;
	}

	int pos = 10;
	int i  = 0;
	for( ; i < FILE_SIZE ; i++ )
	{	
		map_ptr[i] = pos + '0';
		pos = (pos + 1) % 10;
	}
	


	ftruncate(fd, 10);
	munmap(map_ptr, FILE_SIZE);
	close(fd);

	return 0;
}
