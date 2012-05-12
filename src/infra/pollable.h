#ifndef KERNEL_POLLABLE_H
#define KERNEL_POLLABLE_H

#include <pal/stdtype.h>

class pollablefd_t
{
public:
	virtual ~pollablefd_t() {};
	virtual native_fd fd() const = 0;
	virtual int process(const ioinfo_t&) = 0;
	virtual int status() const = 0;

	enum fd_status {
		terminated = 0,
		last
	};
};

#endif
