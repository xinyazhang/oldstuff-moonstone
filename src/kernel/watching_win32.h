#ifndef WATCHING_WIN32_H
#define WATCHING_WIN32_H

#include <windows.h>
#include "watching.h"

class watching_win32
	:public watching_t
{
public:
	watching_win32(const struct volume& vol, class Database* db);
	~watching_win32();

	bool check();
	int init();
	int dispach_read();

	native_fd fd() const;
	int process(const ioinfo_t&);
	int status()const;
protected:
	native_fd fd_;

	uint64_t lastjid;
	uint64_t lastusn;
	/* Asynchronized processing staff */
	USN_JOURNAL_DATA usn_meta;
	READ_USN_JOURNAL_DATA usn_param;
	OVERLAPPED overlap;
	shared_array<char> usn_buffer;
	DWORD usn_read_bytes;

	bool recheck;
	int st;
	bool first_blood_;
};

#endif
