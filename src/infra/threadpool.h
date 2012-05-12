#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <boost/thread.hpp>

class tp_thread_t;
class threadpool_t* globaltp();

class threadpool_t
{
public:
	threadpool_t();
	~threadpool_t();

	class threadgroup_t* allocate_threads(int n, class threadpool_worker_t*);
	void recycle_threads(const std::vector<tp_thread_t*>&);
private:
	std::vector<tp_thread_t*> cans_, all_;
	boost::mutex manlock;

	void inter_alloc(int n);
};

class threadpool_worker_t
{
public:
	virtual ~threadpool_worker_t() {}
	virtual int tp_working() = 0;
};

class threadgroup_t
{
public:
	threadgroup_t(class threadpool_t*,
			const std::vector<tp_thread_t*>&, 
			class threadpool_worker_t*);
	~threadgroup_t();
	int size() const;
private:
	std::vector<tp_thread_t*> runs_;
	class threadpool_t* tp_;
};

#endif
