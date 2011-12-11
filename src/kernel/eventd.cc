#include <stdio.h>
#include <pal/xtdio.h>
#include "eventd.h"

eventd::eventd(feedback_port& p)
	:fb_(p)
{
	tg_ = globaltp()->allocate_threads(1, this);
}

eventd::~eventd()
{
	fb_.printf(TASKLET_QUIT, UT(""));
	delete tg_;
}

const unichar EVENTD_REG_FMT[] = UT("%p %p %p\n");

int eventd::tp_working()
{
	while(true) {
		feedback_event le = fb_.next();
		if (le.evid == TASKLET_QUIT)
			return 0;
		if (le.evid == EVENTD_REG) {
			eventd *target;
			event_cb ecb;
			xsscanf((unichar*)le.payload.get(), EVENTD_REG_FMT, 
					&target, &ecb.func, &ecb.cookie);
			if (target == this) {
				callbacks_.push_back(ecb);
			}
		}
		for(std::vector<event_cb>::iterator iter = callbacks_.begin();
				iter != callbacks_.end();
				iter++)
			iter->func(iter->cookie, le);
	}
}

int eventd::register_receiver(eventd_cb_func_t cb, void* cookie)
{
	fb_.printf(EVENTD_REG, EVENTD_REG_FMT, this, cb, cookie);
	return 0;
}
