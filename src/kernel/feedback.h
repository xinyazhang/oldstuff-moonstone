#ifndef LOG_H
#define LOG_H

/*
 * Strategy: 
 * 	Frontend: a pair of unnamed pipe
 * 	Backend: a ring buffer
 */

#include "evid.h"
#include <pal/stdtype.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>

struct feedback_event
{
	uint32_t evid;
	uint32_t pl_len;
	shared_array<char> payload;
};

class EXPORT_TO_DLL feedback_port
{
public:
	/* Log usage */
	int printf(EVENT_ID evid, const unichar* fmt, ...);
	/* Out of band information */
	int oob(EVENT_ID evid, const unichar* fmt, ...);

	feedback_event next();
private:
	std::queue<feedback_event> queue_;
	std::queue<feedback_event> oob_queue_;
	boost::condition_variable cond_;
	boost::mutex lock_;
};

extern feedback_port& log();
extern feedback_port& proc();

#endif
