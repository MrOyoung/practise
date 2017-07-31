#include "include/syslog_monitor.h"


#define LOG_DIR		"./log"

int main(int argc, char *argv[])
{

	start_monitor_logdir(LOG_DIR);
	
	return 0;
}
