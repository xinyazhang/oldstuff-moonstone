/*
 * fdpool_t: class to epoll/iocp on file descriptors for event based
 * processing, typical workflow:
 * 1. ctor: create epoll/iocp fd, an evqueue (fdpoolmgr_t) for add/remove 
 * 	and working threads if necessary
 * 2. add: stack the pollablefd by enqueu a event in to the internal evqueue.
 * 	This would make the evqueue readable and a working thread would be 
 * 	waked up and call the fdpoolmgr_t::process, this would add the event
 * 	to the epoll/iocp fd
 * 3. [event] the fd became readable, the virtual pollablefd_t::process is
 * 	called by the thread waken up, to process some things like receive
 * 	packets and read files.
 * 4. detach: enqueue a detach event to the internal evqueue
 * 5. clean: delete the pollablefd
 * 6. dtor: delete data structures constructed
 *
 */

#ifndef KENREL_FD_POOL_H
#define KENREL_FD_POOL_H

#include "pollablefd.h"

class fdpool_t
{
public:
	virtual ~fdpool_t() {}

	virtual bool async_attach(pollablefd_t*) = 0;
};

#endif
