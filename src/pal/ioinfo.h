#ifndef IOINFO_H
#define IOINFO_H

#ifdef _WIN32
struct ioinfo_t {
	unsigned long size;
	struct _OVERLAPPED* poverlapped;
};
#else
typedef struct epoll_event ioinfo_t;
#endif

#endif
