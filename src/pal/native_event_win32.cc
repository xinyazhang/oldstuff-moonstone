#include "native_event.h"
#include <Windows.h> 

native_fd create_event(bool initvalue)
{
	return CreateEvent(NULL, FALSE, initvalue, NULL);
}

void close_event(native_fd fd)
{
	CloseHandle(fd);
}

void wait_event(native_fd fd)
{
	WaitForSingleObject(fd, INFINITE);
}

void flag_event(native_fd fd)
{
	SetEvent(fd);
}
