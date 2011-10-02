#include "../priv.h"
#include "../packet_handler.h"
#include <pal/ipc.h>
#include <Windows.h>

static int serv_volume_handle(ipc_packet* packet, ph_cookie cookie)
{
	native_fd fd = (native_fd)cookie;
	std::wstring uuid_string((wchar_t*)packet->payload, packet->header.payload_size);
	std::wstring volname(L"\\\\?\\Volume{");
	volname += uuid_string;
	volname += L"}";

	HANDLE volume_handle = CreateFileW(volname.c_str(),
			GENERIC_READ, 
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);
	if (volume_handle == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		ipc_direct_write_packet(fd, PT_PRIV_WIN32_VOLUME_HANDLE_ERR, 
				&err, sizeof(err));
		return 0;
	}
	ipc_direct_write_packet(fd, PT_PRIV_WIN32_VOLUME_HANDLE_REP, 
		packet->payload, packet->header.payload_size );
	ipc_send_fd(fd, volume_handle);
	CloseHandle(volume_handle);
	return 0;
}

void priv_init()
{
	ph_register(PT_PRIV_WIN32_VOLUME_HANDLE_REQ, serv_volume_handle);
}

void priv_clean()
{
}
