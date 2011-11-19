/*
 * evqueue: base class to emulate a pollable file descriptor for event
 * processing
 */

#ifndef KERNEL_EVQUEUE_H
#define KERNEL_EVQUEUE_H

#include "pollable.h"
#include <pal/asio_type.h>
#include <boost/thread/mutex.hpp>

class evqueue 
	:public pollablefd_t
{
public:
	evqueue();
	virtual ~evqueue();
	bool enqueue(struct event_t*);

	/* Inherited */
	virtual native_fd fd() const;
	virtual void process(const ioinfo_t*);
	virtual bool terminated() const;
private:
	virtual int process(struct event_t*) = 0;

	std::queue<event_t> queue_;
	boost::mutex queue_lock_;

	native_fd readfd_;
	native_fd writefd_;
};

#endif
