#include <vector>
#include <boost/thread.hpp>
#include <pal/ipc.h>
#include <pal/proc.h>
#include "sp.h"
#include "auth.h"
#include "packet_handler.h"

static boost:thread* sp_thread = NULL;
static const unistr sp_name(UT("MOONSTONE"));
static native_fd sp_fd;
static native_fd term_fd;

static void sp_accepting();
static void sp_working(native_fd fd);

void sp_init()
{
	sp_fd = ipc_create_sp(sp_name);
	if (!ipc_valid_native_fd(sp_fd))
		return ;

	sp_thread = new boost::thread(&sp_accepting);
}

void sp_close()
{
	sp_thread->detach(); // not interesting any more
	delete sp_thread;
	ipc_close_sp(sp_fd);
}

void sp_accepting()
{
	do
	{
		native_fd fd = ipc_accept(sp_fd);
		if (!auth_ipc_client(fd))
		{
			ipc_close_connect(fd);
			continue;
		}

		if (!ipc_valid_native_fd(fd))
			break;

		boost::thread thread(boost::bind(&sp_working, fd));
		thread.detach(); // not interesting any more, it has become a standaloe procedure
	}while (true);

	ipc_close_connect(fd);
}

static void sp_working(native_fd fd)
{
	do
	{
		ipc_packet* packet = ipc_read_packet(fd);
		if (packet->header.type == PT_FIN)
		{
			ipc_recycle_packet(packet);
			break; 
		}
		ph_process(packet, fd);
		ipc_recycle_packet(packet);
	} while (true);
}
