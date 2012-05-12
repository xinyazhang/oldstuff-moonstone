#include "privilege.h"
#include <windows.h>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <pal/stdtype.h>

native_fd privilege::open_volume(const uuids& uuid)
{
	unistr volname(UT("\\\\?\\Volume{"));
	volname += uuid2unistr(uuid);
	volname += UT("}");

	native_fd fd = CreateFileW(volname.native(),
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);
	return fd;
}
