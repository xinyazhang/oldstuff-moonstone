#ifndef NEXUS_FD_H
#define NEXUS_FD_H

#include <infra/pollable.h>
#include <infra/fdpool.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.h>

class nexus_fd
	:public pollablefd_t
{
public:
	nexus_fd(native_fd fd);
	~nexus_fd();

	native_fd fd() const;
	int process(const ioinfo_t&);
	int status() const;

	void join(fdpoll_t* fdp);
	void wait_terminate();
private:
	fdpoll_t *fdp_;
	native_fd *fd_;

	boost::condition_variable cond_;
	boost::mutex lock_;
}

#endif
