#include "vc_log.h"
#include "vc_log_cfg.h"

int main(int argc, char *argv[])
{
	DBusConnection *bus;
	
	/*create a dbus handler*/
	if (!(bus = get_dbus("com.saic.WindowsManager"))){
		return -1;
	}

	if (dbus_add_match(bus, "type='signal',path='/com/saic/WindowsManager'") < 0){
		return -1;
	}

	LOG_INFO(bus, "info");
	sleep(1);

	LOG_WARN(bus, "warn");
	sleep(1);

	LOG_ERROR(bus, "error");
	sleep(1);

	LOG_EMERG(bus, "emerg");
	sleep(1);

	return 0;
}
