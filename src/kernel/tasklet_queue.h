/*
 * evqueue: base class to emulate a pollable file descriptor for event
 * processing
 */

#ifndef KERNEL_EVQUEUE_H
#define KERNEL_EVQUEUE_H

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include "threadpool.h"

class tasklet_queue_t
	:public threadpool_worker
{
public:
	tasklet_queue_t(int = 0);
	virtual ~tasklet_queue_t(); // close all the threads

	bool enqueue(int evid);
	int tp_working();
private:
	std::queue<class tasklet_t*> evid_queue_; // id only, subclasses
	boost::condition_variable queue_cond_;
	boost::mutex queue_lock_;
	thread_group* tg_;
};

const int TASKLET_QUIT = 0;

#endif
