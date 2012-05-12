#ifndef FDPOOL_WIN32_H
#define FDPOOL_WIN32_H

#include "fdpool.h"
#include "threadpool.h"
#include <pal/stdtype.h>

class fdpool_win32
	:public fdpool_t, public threadpool_worker_t
{
public:
	fdpool_win32();
	~fdpool_win32();

	bool async_attach(pollablefd_t*);
	virtual int tp_working();
private:
	native_fd iocp_;
	threadgroup_t* tg_;
};


#endif
