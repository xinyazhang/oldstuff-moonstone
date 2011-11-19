#include "threadpool.h"

/*
 * Singleton
 */
class threadpool_t* globaltp()
{
	static threadpool_t tp;
	return &tp;
}

/*
 * Thread Pool
 * 	ctor: nothing
 * 	dtor: delete all tp_thread_t
 *
 * 	alloc: pick n th_thread_t and use them to create a threadgroup_t
 * 	free: called by dtor of threadgroup_t, store the threads to cans_
 * 		(i.e. CANdidate threadS)
 *
 */

threadpool_t::threadpool_t()
{
}

threadpool_t::~threadpool_t()
{
	for(std::vector<tp_thread_t*>::iterator iter = all_.begin();
			iter != all_.end();
			iter++) {
		delete *iter;
	}
}

threadgroup_t* threadpool_t::allocate_threads(int n, class threadpool_worker* worker)
{
	manlock.lock();
	if (cans_.size() < n) {
		inter_alloc(n - cans_.size());
	}
	std::vector<boost::thread*> alloc;
	while (alloc.size() < n && !cans_.empty()) {
		boost::thread* t = cans_.back();
		cans_.pop_back();
		alloc.push_back(t);
	}
	manlock.unlock();

	return new threadgroup_t(this, alloc, worker);
}

void threadpool_t::recycle_threads(const std::vector<tp_thread_t*>& tps)
{
	manlock.lock();
	cans_.insert(cans_.end(), tps.begin(), tps.end());
	manlock.unlock();
}

void threadpool::inter_alloc(int n)
{
	for(int i = 0; i < n; i++) {
		tp_thread_t* tpt = new tp_thread_t
		cans_.push_back(tpt);
		all_.push_back(tpt);
	}
}

/*
 * Thread Group, a bunch of threads share a same worker
 * 	
 * 	ctor: accept a bunch of tp_thread_t, and wake them with worker
 * 	dtor: call its threadpool_t and send the tp_thread_ts back to it
 *
 */

threadgroup_t::threadgroup_t(class threadpool_t* tp,
		const std::vector<tp_thread_t*>& cans, 
		class threadpool_worker_t* worker)
	:tp_(tp), runs_(cans)
{
	for(std::vector<tp_thread_t*>::iterator iter = runs_.begin();
			iter != runs_.end();
			iter++) {
		(*iter)->wake(worker);
	}
}

threadgroup_t::~threadgroup_t()
{
	tp_->recycle_threads(runs_);
}
