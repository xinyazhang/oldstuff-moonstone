#ifndef KERNEL_ASYNCHRONIZE_WORKING_QUEUE_H
#define KERNEL_ASYNCHRONIZE_WORKING_QUEUE_H

#include "common_declare.h"

/* This class is used internally. */
class AsyncWQ
{
public:
	AsyncWQ(Database* );
	~AsyncWQ();

	wq_t invoke_cp(const unistr& path, const unistr& pathnew);
	wq_t invoke_mv(const unistr& path, const unistr& pathnew);
	wq_t invoke_rm(const unistr& path);

	int eno();
private:
	Database* db_;
};

#endif
