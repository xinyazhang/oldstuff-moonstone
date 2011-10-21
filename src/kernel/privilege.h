#ifndef PRIVILEGE_H
#define PRIVILEGE_H

#include <pal/stdtype.h>

namespace privilege
{
	native_fd open_volume(const uuids&);
};

#endif
