#include "access_point.h"

bool access_point::valid() const
{
	if (host < 0 || id < 0 || url.empty())
		return false;
	return true;
}

bool access_point::completed() const
{
	return !phost && valid();
}

access_point::access_point(machine_t* mac)
	:phost(mac), host(-1), id(-1)
{
}
