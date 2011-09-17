#include "ipc.h"
#include <windows.h>

#define IPC_PIPE_BUFFER_SIZE 4096

static const unistr path_base(L"\\\\.\\pipe\\");

native_fd ipc_create_sp(const unistr& ipcname)
{
	unistr pipename(path_base);
	pipename += ipcname;

	native_fd handle = CreateNamedPipeW(pipename.native(),
			PIPE_ACCESS_DUPLEX,
			0, /* BYTE, READ_BYTE, WAIT */
			PIPE_UNLIMITED_INSTANCES,
			IPC_PIPE_BUFFER_SIZE,
			IPC_PIPE_BUFFER_SIZE,
			0,
			NULL);
	return handle;
}

native_fd ipc_connect_to_sp(const unistr& ipcname)
{
	unistr pipename(path_base);
	pipename += ipcname;

	native_fd handle = CreateFileW(pipename.native(),
			GENERIC_READ | GENERIC_WRITE, 
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

	return handle;
}

native_fd ipc_accept(native_fd fd)
{
	bool connected = ConnectNamedPipe(fd, NULL) ? 
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	if (connected)
		return fd;
	else
		return INVALID_HANDLE_VALUE;
}

void ipc_close_sp(native_fd fd)
{
	CloseHandle(fd);
}

void ipc_close_connect(native_fd)
{
	/* Blank, as Named pipe do not support multiple connection */
}

int ipc_write(native_fd fd, const void* data, size_t bytes)
{
	DWORD written = 0;
	WriteFile(fd, data, (DWORD)bytes, &written, NULL);
	return written;
}

int ipc_read(native_fd fd, void* data, size_t bytes)
{
	DWORD read = 0;
	ReadFile(fd, data, (DWORD)bytes, &read, NULL);
	return read;
}

bool ipc_valid_native_fd(native_fd fd)
{
	return fd == INVALID_HANDLE_VALUE;
}

bool ipc_multi_support()
{
	return false;
}
