#include "ipc.h"

int ipc_write_packet(native_fd fd, ipc_packet* packet)
{
	return ipc_write(fd, packet, packet->payload_size + IPC_HEADER_SIZE);
}

ipc_packet* ipc_read_packet(native_fd fd)
{
	uint32_t type, payload;
	ipc_packet* packet;
	/* HEADER */
	ipc_read(fd, &type, sizeof(packet->type));
	ipc_read(fd, &payload, sizeof(packet->payload_size));

	/* PAYLOAD */
	packet = ipc_allocate_packet(payload);
	packet->type = type;
	packet->payload_size = payload;
	ipc_read(fd, &packet->payload, packet->payload_size);

	return packet;
}

ipc_packet* ipc_allocate_packet(size_t payload)
{
	ipc_packet* packet = (ipc_packet*)malloc(payload + IPC_HEADER_SIZE);
	packet->payload_size = payload;
	return packet;
}

void ipc_recycle_packet(ipc_packet* packet)
{
	free(packet);
}
