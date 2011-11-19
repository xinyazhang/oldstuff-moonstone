#ifndef IOINFO_H
#define IOINFO_H

#ifdef _WIN32
struct ioinfo_t {
	unsigned long size;
	struct OVERLAPPED* poverlapped;
};
#else
#error "NOT FULLY SUPPORTED PLATFORM: LACKING ioinfo_t"
#endif

#endif
