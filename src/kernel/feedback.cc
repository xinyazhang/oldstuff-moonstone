#include "feedback.h"

#define BUFLIMIT (1<<16)

int feedback_port::printf(EVENT_ID evid, const unichar* fmt, ...)
{
	va_list args;
	unichar buf[BUFLIMIT+1];
	int charc = xvsnprintf(buf, BUFLIMIT, fmt, args);
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
		le.payload = boost::shared_ptr<char>(real_buf);
	}

	lock_.lock();
	queue_.push(le);
	lock_.unlock();
	cond_.notify_one();
}

feedback_event feedback_port::next()
{
	boost::mutex::scoped_lock lock(lock_);
	while (queue_.empty()) {
		cond_.wait(lock);
	}

	feedback_event le = evid_queue_.front();
	/* Put this before pop makes one TASKLET_QUIT event exits all the
	 * threads */
	if (le.evid != TASKLET_QUIT)
		queue_.pop();
	return le;
}
