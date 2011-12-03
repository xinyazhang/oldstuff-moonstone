#include "tp_thread.h"

tp_thread_t::tp_thread_t()
	:quit(false)
{
	thread_ = new boost::thread(&tp_thread_t::master, this);
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
			boost::mutex::scoped_lock lock(can_lock_);
			while (candidate_ == NULL && !quit) {
				cond_.wait(lock);
			}
			if (quit || !candidate_)
				return ;
			next = candidate_;
			candidate_ = NULL;
		}
		{
			boost::mutex::scoped_lock lock(done_lock_);
			done = false;
			next->tp_working();
			done = true;
			done_cond_.notify_one();
		}
	}
}

void tp_thread_t::run(tp_thread_t* tpt)
{
	tpt->master();
}

void tp_thread_t::wake(class threadpool_worker_t* worker)
{
	boost::mutex::scoped_lock lock(can_lock_);
	candidate_ = worker;
	cond_.notify_one();
}

void tp_thread_t::wait()
{
	boost::mutex::scoped_lock lock(done_lock_);
	while (!done) {
		done_cond_.wait(lock);
	}
}