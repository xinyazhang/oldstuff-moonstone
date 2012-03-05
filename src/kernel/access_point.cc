#include "access_point.h"

bool access_point::valid() const
{
	if (id < 0)
		return false;
	return true;
}

bool access_point::completed() const
{
	return phost;
}

access_point::access_point(machine_t* mac)
	:phost(mac), id(-1)
{
	if (phost)
		host = phost->id;
	else
		host = -1;
}
