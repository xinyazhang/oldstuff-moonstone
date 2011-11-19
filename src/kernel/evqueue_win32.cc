#include "evqueue.h"
#include <Windows.h>

evqueue::evqueue()
{
	CreatePipe(&readfd_, &writefd_, NULL, 0);
}

evqueue::~evqueue()
{
	CloseHandle(readfd_);
	CloseHandle(writefd_);
}

bool evqueue::enqueue(struct event_t* ev)
{
	char arb;
	boost::mutex::scoped_lock lock(queue_lock_);
	queue_.push(ev);
	WriteFile(writefd_,
			&arb,
			1,
			NULL,
			NULL);
}

/*
 * Inherited from pollablefd_t
 */
bool evqueue::fd() const
{
	return readfd_;
}

bool evqueue::terminated() const
{
	return false;
}
