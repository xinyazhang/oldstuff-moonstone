#include "ipc.h"

int ipc_write_packet(native_fd fd, ipc_packet* packet)
{
	return ipc_write(fd, packet, packet->header.payload_size + IPC_HEADER_SIZE);
}

int ipc_direct_write_packet(native_fd fd,
	   	uint32_t type, const void* payload, uint32_t payload_size)
{
	ipc_packet_header header;
	header.type = type;
	header.payload_size = payload_size;
	return ipc_write(fd, &header, IPC_HEADER_SIZE) + 
		ipc_write(fd, payload, payload_size);
}

ipc_packet* ipc_read_packet(native_fd fd)
{
	uint32_t type, payload;
	ipc_packet* packet;
	/* HEADER */
	ipc_read(fd, &type, sizeof(packet->header.type));
	ipc_read(fd, &payload, sizeof(packet->header.payload_size));

	/* PAYLOAD */
	packet = ipc_allocate_packet(payload);
	packet->header.type = type;
	packet->header.payload_size = payload;
	ipc_read(fd, &packet->payload, packet->header.payload_size);

	return packet;
}

ipc_packet* ipc_allocate_packet(size_t payload)
{
	ipc_packet* packet = (ipc_packet*)malloc(payload + IPC_HEADER_SIZE);
	packet->header.payload_size = payload;
	return packet;
}

void ipc_recycle_packet(ipc_packet* packet)
{
	free(packet);
}
