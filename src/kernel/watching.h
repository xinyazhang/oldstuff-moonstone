#ifndef WATCHING_H
#define WATCHING_H

#if 0
#include <windows.h>
#include <pal/stdtype.h>
#include <pal/volume.h>

struct watching_t
{
	native_fd fd;
	/* the two should be loaded from db */
	volume vol;
	uint64_t lastjid;
	uint64_t lastusn;
	/* Asynchronized processing staff */
	USN_JOURNAL_DATA usn_meta;
	READ_USN_JOURNAL_DATA usn_param;
	OVERLAPPED overlap;
	shared_ptr<char> usn_buffer;
	DWORD usn_read_bytes;
	/* State */
	bool recheck;

	static watching_t create(class Database*, const struct volume&);
	static void close(watching_t&);

	bool check();
	void init(class Database*);
	void dispach_read();
	void dump(class Database*);
	native_fd waitobj();
};

inline bool operator==(const watching_t& w, const volume& v) { return w.vol == v;}
#endif

#include "pollable.h"
#include <pal/stdtype.h>
#include <pal/volume.h>

class watching_t
	:public pollablefd_t
{
	friend inline bool operator==(const watching_t* wat, const volume& vol);
public:
	watching_t(const struct volume& _vol, class Database* db);

	virtual bool check() = 0;
	virtual int init() = 0;
	virtual int dispach_read() = 0;
protected:
	class Database* dbmgr;
	volume vol;
};

inline bool operator==(const watching_t* wat, const volume& vol)
{
	return wat->vol == vol;
}

#endif
