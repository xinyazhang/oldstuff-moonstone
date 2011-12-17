#include "feedback.h"
#include <pal/xtdio.h>

#define BUFLIMIT (1<<16)
#define ARRAYLIMIT (1024)

int feedback_port::printf(EVENT_ID evid, const unichar* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	unichar buf[BUFLIMIT+1];
	int charc = xvsnprintf(buf, BUFLIMIT, fmt, args);
	va_end(args);
	if (charc < 0)
		return -1;
	feedback_event le;
	le.evid = evid;
	le.pl_len = sizeof(unichar) * charc;
	buf[charc] = UT('\0');
	if (charc > 0) {
		long size = (charc+1) * sizeof(unichar);
		char* real_buf = new char[size];
		memcpy(real_buf, buf, size);
		le.payload = shared_array<char>(real_buf);
	}

	lock_.lock();
	queue_.push(le);
	if (queue_.size() > ARRAYLIMIT)
		queue_.pop();
	lock_.unlock();
	cond_.notify_one();
}

feedback_event feedback_port::next()
{
	boost::mutex::scoped_lock lock(lock_);
	while (queue_.empty()) {
		cond_.wait(lock);
	}

	feedback_event le = queue_.front();
	/* Put this before pop makes one TASKLET_QUIT event exits all the
	 * threads */
	if (le.evid != TASKLET_QUIT)
		queue_.pop();
	return le;
}

feedback_port& log()
{
	static feedback_port thelog;
	return thelog;
}

feedback_port& proc()
{
	static feedback_port theproc;
	return theproc;
}
