#ifndef PAL_IPC_H
#define PAL_IPC_H

#include "stdtype.h"

const static int IPC_HEADER_SIZE = sizeof(uint32_t)+sizeof(uint32_t);

#if defined(_MSC_VER)
#pragma warning(disable : 4200) // Disable C4200 as it's useful
#endif

struct ipc_packet_header
{
	uint32_t type;
	uint32_t payload_size;
};

struct ipc_packet
{
	ipc_packet_header header;
	char payload[];
};

/*
 * Use case:
 * [server]
 * 		create_sp
 * 		accept
 * 		read/write
 * 		...
 * 		close_connect
 * 		close_sp
 * [client]
 * 		connect_to_sp
 * 		read/write
 * 		...
 * 		close_connect
 * 		close_sp
 *
 * For win32: accepted handle is the same as the create_sp returned
 * 		and close_connect and close_sp call DisconnectNamedPipe
 * 		and CloseHandle seperately.
 * 	For linux: accepted fd is a new fd, close_connect and close_sp 
 * 		both calls close
 */
EXPORT_TO_DLL native_fd ipc_create_sp(const unistr&); /* used in server */
EXPORT_TO_DLL native_fd ipc_accept(native_fd);
EXPORT_TO_DLL native_fd ipc_connect_to_sp(const unistr&); /* used in client */
EXPORT_TO_DLL void ipc_close_connect(native_fd);
EXPORT_TO_DLL void ipc_close_sp(native_fd);

EXPORT_TO_DLL int ipc_write(native_fd, const void*, size_t);
EXPORT_TO_DLL int ipc_read(native_fd, void*, size_t);
              
EXPORT_TO_DLL int ipc_write_packet(native_fd, ipc_packet* );
EXPORT_TO_DLL int ipc_direct_write_packet(native_fd,
	   	uint32_t type, uint32_t payload_size, const char* payload);
EXPORT_TO_DLL ipc_packet* ipc_read_packet(native_fd);

EXPORT_TO_DLL ipc_packet* ipc_allocate_packet(size_t payload);
EXPORT_TO_DLL void ipc_recycle_packet(ipc_packet*);
              
EXPORT_TO_DLL bool ipc_valid_native_fd(native_fd);
EXPORT_TO_DLL bool ipc_multiple_sp_support();

/*
 * fd transmission routines
 * NOTE: 
 * 	1. Unix Socket do not require an additional ''Process Handle''
 */
EXPORT_TO_DLL void ipc_send_fd(native_fd target, native_fd sent);
EXPORT_TO_DLL native_fd ipc_recv_fd(native_fd source);

#endif
