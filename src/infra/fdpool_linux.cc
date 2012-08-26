#include <sys/epoll.h>
#include "fdpool_linux.h"
#include "threadpool.h"
#include "feedback.h"
#include <pal/arch_march.h>
#include <pal/ioinfo.h>

fdpool_linux::fdpool_linux()
{
	ep_ = epoll_create1(EPOLL_CLOEXEC);

	epev_ = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = 0;
	epoll_ctl(ep_, EPOLL_CTL_ADD, epev_, &ev);

	tg_ = globaltp()->allocate_threads(cpu_number(), this);
}

fdpool_linux::~fdpool_linux()
{
	u64 v = 1;
	write(epev_, &v, sizeof(v));
	delete tg_;
	close(epev_);
	close(ep_);
}

bool fdpool_linux::async_attach(pollablefd_t* pfd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN|EPOLLOUT|EPOLLONESHOT;
	ev.data.ptr = pfd;
	int r = epoll_ctl(ep_, EPOLL_CTL_ADD, pfd->fd(), &ev);
	if (r) {
		log().printf(LOG_ERROR, "Failed to add fd %d to epoll %d, error: %d\n",
			pfd->fd(), ep_, r);
		return false;
	}
	return true;
}

int fdpool_linux::tp_working()
{
	pollablefd_t* pfd;
	struct epoll_event ev;

	while (true) {
		int r = epoll_wait(ep_, &ev,
				1, -1);
		pfd = (pollablefd_t*)ev.data.ptr;
		if (!pfd) {
			return 0;
		}
		pfd->process(ev);
		if (pfd->status() == pollablefd_t::terminated) {
			/* detach is not needed */
			delete pfd;
		}
		/* rearming oneshoot */
		ev.events = EPOLLIN|EPOLLOUT|EPOLLONESHOT;
		epoll_ctl(ep_, EPOLL_CTL_MOD, pfd->fd(), &ev);
	}
	return 0;
}
