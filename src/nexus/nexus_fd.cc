#include "nexus_fd.h"

void nexus_fd::join(fdpoll_t *fdp)
{
	fdp->async_attach(this);
	fdp_ = fdp;
}

void nexus_fd::wait_terminate()
{
	boost::mutex::scoped_lock lock(lock_);
	while (status() != pollablefd_t::terminated)
		cond_.wait(lock);
}
