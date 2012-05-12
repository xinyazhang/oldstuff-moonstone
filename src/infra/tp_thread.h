/*
 * WARNING: THIS FILE IS NOT INTENDED TO BE USED OUTSIDE kernel MODULE
 */

#ifndef THREADPOOL_THREAD_H
#define THREADPOOL_THREAD_H

#include <boost/thread.hpp>
#include "threadpool.h"

class tp_thread_t
{
public:
	tp_thread_t();
	~tp_thread_t();

	void master();
	static void run(tp_thread_t*);
	void wake(class threadpool_worker_t* worker);
	void wait();
private:
	boost::thread* thread_;
	boost::mutex can_lock_, done_lock_;
	boost::condition_variable cond_, done_cond_;
	class threadpool_worker_t* candidate_;
	bool quit;
	bool done;
};

#endif
