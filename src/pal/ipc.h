#ifndef PAL_IPC_H
#define PAL_IPC_H

#include "stdtype.h"

const static int IPC_HEADER_SIZE = sizeof(uint32_t)+sizeof(uint32_t);

struct ipc_packet
{
	uint32_t type;
	uint32_t payload_size;
	char payload[1];
};

EXPORT_TO_DLL native_fd ipc_create_sp(const unistr&); /* used in server */
EXPORT_TO_DLL native_fd ipc_accept(native_fd);
EXPORT_TO_DLL native_fd ipc_connect_to_sp(const unistr&); /* used in client */
EXPORT_TO_DLL void ipc_close_sp(native_fd);
EXPORT_TO_DLL void ipc_close_connect(native_fd);

EXPORT_TO_DLL int ipc_write(native_fd, const void*, size_t);
EXPORT_TO_DLL int ipc_read(native_fd, void*, size_t);
              
EXPORT_TO_DLL int ipc_write_packet(native_fd, ipc_packet* );
EXPORT_TO_DLL ipc_packet* ipc_read_packet(native_fd);
EXPORT_TO_DLL ipc_packet* ipc_allocate_packet(size_t payload);
EXPORT_TO_DLL void ipc_recycle_packet(ipc_packet*);
              
EXPORT_TO_DLL bool ipc_valid_native_fd(native_fd);
EXPORT_TO_DLL bool ipc_multiple_sp_support();

#endif
