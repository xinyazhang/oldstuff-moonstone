#include "tp_thread.h"

tp_thread_t::tp_thread_t()
	:quit(false)
{
	thread_ = new thread(master, this);
}

tp_thread_t::~tp_thread_t()
{
	quit = true;
	cond_.notify_one();
	thread_->join();
	delete thread_;
}

void tp_thread_t::master()
{
	while(true) {
		threadpool_worker_t* next;
		{
			boost::thread::scoped_lock lock(can_lock_);
			while (candidate_ == NULL && !quit) {
				cond_.wait(can_lock_);
			}
			if (quit)
				return ;
			next = candidate_;
		}
		next->tp_working();
	}
}

void tp_thread_t::run(tp_thread_t* tpt)
{
	tpt->master();
}

void tp_thread_t::wake(class threadpool_worker_t* worker)
{
	boost::thread::scoped_lock lock(can_lock_);
	candidate_ = worker;
	cond_.notify_one();
}
