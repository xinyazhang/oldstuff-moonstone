#ifndef EVENTD_H
#define EVENTD_H

#include <vector>
#include <pal/int_type.h>
#include "threadpool.h"
#include "feedback.h"

typedef int (*eventd_cb_func_t)(void* cookie, feedback_event ev);

class EXPORT_TO_DLL eventd
	:public threadpool_worker_t
{
public:
	eventd(feedback_port&);
	~eventd();

	int tp_working();
	int register_receiver(eventd_cb_func_t, void* cookie);
private:
	feedback_port& fb_;
	threadgroup_t* tg_;
	struct event_cb {
		eventd_cb_func_t func;
		void* cookie;
	};
	std::vector<event_cb> callbacks_;
};

EXPORT_TO_DLL extern eventd& procd();

enum EVENTD_RECEIVER
{
	FBEV_ACCEPT = 0,
	FBEV_IGNORE = 1,
	FBEV_SWALLOW = 2
};


#endif
