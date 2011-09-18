#ifndef IPC_PACKET_TYPE_H
#define IPC_PACKET_TYPE_H

enum IPC_PACKET_TYPE
{
	PT_WIN32_HANDLE = 0xED320001, /* rEserveD win32 */
	PT_LINUX_FD	= 0xED110001,	/* rEserveD llnux */
	PT_DEFINED_COUNT
};

#endif
