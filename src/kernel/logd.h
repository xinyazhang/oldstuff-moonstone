/*
 * Log ``daemon'' which stores message of ring buffer(maybe)
 * to file
 */
#ifndef LOGD_H
#define LOGD_H

#include <stdio.h>
#include <pal/int_type.h>
#include "threadpool.h"

class EXPORT_TO_DLL logd
	:public threadpool_worker_t
{
public:
	logd();
	~logd();

	int tp_working();
private:
	threadgroup_t* tg_;
	FILE* journal;
};

#endif
