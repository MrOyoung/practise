#include "vc_log_cfg.h"
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>

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

	VC_set_dbus_object(bus);
	
	VC_LOG_INFO("info");
	sleep(1);

	VC_LOG_WARN("warn");
	sleep(1);

	VC_LOG_ERROR("error");
	sleep(1);

	VC_LOG_EMERG("emerg");
	sleep(1);

	return 0;
}
