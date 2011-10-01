#include <pal/ipc.h>
#include <pal/ipc_packet_type.h>
#include "packet_handler.h"
#include "partition.h"
#include <file_struct.h>
#include <serialization.h>

/* Write Single StringStream Packet ...*/
static int ipc_write_sssp(native_fd fd, uint32_t type, native_sstream& ss)
{
	ipc_direct_write_packet(fd, 
			type, 
			(ss.str().size()+1) * sizeof(unichar),
			ss.str().c_str());
}

static int serv_list_vol(ipc_packet* /* NOBODY */, ph_cookie cookie)
{
	native_sstream ss;
	barc_o arc(ss);
	arc << known_partitions << online_partitions;
	ipc_write_sssp((native_fd)cookie, PT_PRIV_VOL_LIST_REP, ss);
	return 0;
}

static int serv_list_tracing(ipc_packet* /* NOBODY */, ph_cookie cookie)
{
	native_sstream ss;
	barc_o arc(ss);
	arc << tracing_paths ;
	ipc_write_sssp((native_fd)cookie, PT_PRIV_TRACING_LIST_REP, ss);
	return 0;
}

void gs_init()
{
	ph_register(PT_PRIV_VOL_LIST_REQ, serv_list_vol);
	ph_register(PT_PRIV_TRACING_LIST_REQ, serv_list_tracing);
}

void gs_clean()
{
}
