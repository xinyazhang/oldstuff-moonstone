#include "ipc.h"
#include "ipc_packet_type.h"
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

void ipc_close_connect(native_fd fd)
{
	/* Blank, as Named pipe do not support multiple connection */
	DisconnectNamedPipe(fd);
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

void ipc_send_fd(native_fd target, native_fd sent)
{
	ULONG client_pid;
	GetNamedPipeClientProcessId(target, &client_pid);
	DWORD cpid = client_pid;
	HANDLE proc = OpenProcess(PROCESS_DUP_HANDLE, FALSE, cpid);
	HANDLE sent_handle = 0;
	uint64_t sent_value = 0;
	DuplicateHandle(GetCurrentProcess(),
			sent,
			proc,
			&sent_handle,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
	sent_value = (uint64_t)sent_handle;
	ipc_packet* packet = ipc_allocate_packet(sizeof(sent_value));
	memcpy(packet->payload, &sent_value, sizeof(sent_value));
	packet->type = PT_WIN32_HANDLE;
	ipc_write_packet(target, packet);

	ipc_recycle_packet(packet);
}

native_fd ipc_recv_fd(native_fd source)
{
	ipc_packet* packet = ipc_read_packet(source);
	if (packet->type != PT_WIN32_HANDLE)
		return INVALID_HANDLE_VALUE;
	uint64_t value;
	memcpy(&value, packet->payload, sizeof(value));
	ipc_recycle_packet(packet);

	return (native_fd)value;
}
