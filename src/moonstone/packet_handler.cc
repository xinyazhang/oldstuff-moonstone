#include <hash_map>
#include "packet_handler.h"

using std::hash_map;

hash_map<IPC_PACKET_TYPE, ph_t> ph_map;

void ph_init()
{
}

void ph_clean()
{
}

void ph_register(IPC_PACKET_TYPE pt, ph_t handler)
{
	ph_map[pt] = handler;
}

ph_cookie ph_cookie_create()
{
	return NULL;
}

void ph_cookie_destroy(ph_cookie)
{
}

void ph_process(ipc_packet* packet, ph_cookie cookie)
{
	hash_map<IPC_PACKET_TYPE, ph_t>::iterator iter = ph_map.find((IPC_PACKET_TYPE)packet->header.type);
	if (iter != ph_map.end())
	{
		(*iter->second)(packet, cookie);
	}
}
