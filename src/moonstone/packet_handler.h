#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <pal/ipc_packet_type.h>
#include <pal/ipc.h>

typedef void* ph_cookie;
typedef int (*ph_t)(ipc_packet*, ph_cookie);

void ph_init();
void ph_clean();
void ph_register(IPC_PACKET_TYPE, ph_t);
ph_cookie ph_cookie_create();
void ph_cookie_destroy(ph_cookie);

void ph_process(ipc_packet*, ph_cookie);

#endif
