#ifndef AUTH_H
#define AUTH_H

#include <pal/ipc.h>

/*
 * Linux: TODO
 * Win32: Only Administrators could connect
 */
bool auth_ipc_client(native_fd);

#endif
