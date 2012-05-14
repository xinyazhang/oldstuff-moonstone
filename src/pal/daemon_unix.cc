#include <unistd.h>
#include "daemon.h"

int daemonize()
{
	return daemon(0, 0);
}
