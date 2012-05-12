#include "nexus.h"
#include <pal/ipc.h>
#include <infra/logd.h>
#include <infra/feedback.h>
#include <infra/factory.h>
#include <protocol/ipc_points.h>
#include "nexus_fd.h"

logd *log;
nexus_fd *nexus;
fdpoll_t *fdp;

int nexus_init(const char* pipename)
{
	/* Create log stuff */
	log = new logd;

	/* Create Named pipe(win32)/UNIX Socket(*nix) */
	native_fd point;
	unistr pn(pipename);
	pn = ipc_points_basic_to_full(pn);
	point = ipc_create_sp(pn);
	if (!ipc_valid_native_fd(point)) {
		log()->printf(LOG_FATAL,
			UT("Cannot create SP %s"), pn.native());
		return -1;
	}
	log()->printf(LOG_DEBUG, UT("Create SP %s"), pn.native());
	nexus = new nexus_fd(point);

	/* Create pollable fd */
	fdp = factory::fdpool();
	if (!fdp) {
		log()->printf(LOG_FATAL,
			UT("Cannot create fdpool"));
		delete nexus;
		return -1;
	}
	/** Don't use async_attach because nexus needs a fdpool **/
	nexus->join(fdp); 
}

int nexus_wait()
{
	nexus->wait_terminate();
}
