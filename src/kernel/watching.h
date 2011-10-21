#ifndef WATCHING_H
#define WATCHING_H

#include <pal/stdtype.h>
#include <windows.h>

struct watching_t
{
	native_fd fd;
	/* the two should be loaded from db */
	int64_t kpi;
	uint64_t lastjid;
	uint64_t lastusn;
	/* Asynchronized processing staff */
	USN_JOURNAL_DATA usn_meta;
	READ_USN_JOURNAL_DATA usn_param;
	OVERLAPPED overlap;
	shared_ptr<char> usn_buffer;
	DWORND usn_read_bytes;
	/* State */
	bool recheck;

	static watching_t create(Database*, const struct volume&);
	static void close(watching_t&);

	bool check();
	void init(class Database*);
	void dispach_read();
	void dump(class Database*);
	native_fd waitobj();
};

#endif
