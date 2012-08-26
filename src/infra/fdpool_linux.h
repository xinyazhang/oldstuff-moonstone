#ifndef FDPOOL_WIN32_H
#define FDPOOL_WIN32_H

#include "fdpool.h"
#include "threadpool.h"
#include <pal/stdtype.h>

class fdpool_linux
	:public fdpool_t, public threadpool_worker_t
{
public:
	fdpool_linux();
	~fdpool_linux();

	bool async_attach(pollablefd_t*);
	virtual int tp_working();
private:
	native_fd ep_;
	native_fd epev_;
	threadgroup_t* tg_;
};


#endif
