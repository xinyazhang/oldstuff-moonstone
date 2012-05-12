#include "daemon.h"

int daemonize()
{
	/* Win32 doesn't need daemonize */
	return 0;
}
