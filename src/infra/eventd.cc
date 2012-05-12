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
	fb_.oob(TASKLET_QUIT, UT(""));
	delete tg_;
}

const unichar EVENTD_REG_FMT[] = UT("%p %p %p\n");

void eventd::process_oob(feedback_event& le)
{
	if (le.evid == EVENTD_REG || le.evid == EVENTD_SUPRESS) {
		eventd *target;
		event_cb ecb;
		xsscanf((unichar*)le.payload.get(), EVENTD_REG_FMT, 
				&target, &ecb.func, &ecb.cookie);
		if (target != this)
			return ;
		if (le.evid == EVENTD_REG) {
			callbacks_.push_back(ecb);
			return ;
		}
		for (std::vector<event_cb>::iterator iter 
				= callbacks_.begin();
			iter != callbacks_.end();
			iter++)
			if (iter->func == ecb.func &&
			    iter->cookie == ecb.cookie) {
				callbacks_.erase(iter);
				return ;
			}
	}
}

int eventd::tp_working()
{
	while(true) {
		feedback_event le = fb_.next();
		if (le.evid == TASKLET_QUIT)
			return 0;
		switch (le.evid) {
			case EVENTD_REG:
			case EVENTD_SUPRESS:
				process_oob(le);
				break;
			default:
				for(std::vector<event_cb>::iterator iter 
					= callbacks_.begin();
					iter != callbacks_.end();
					iter++)
				iter->func(iter->cookie, le);
				break;
		}
	}
}

int eventd::register_receiver(eventd_cb_func_t cb, void* cookie)
{
	fb_.oob(EVENTD_REG, EVENTD_REG_FMT, this, cb, cookie);
	return 0;
}

int eventd::supress_receiver(eventd_cb_func_t cb, void* cookie)
{
	fb_.oob(EVENTD_SUPRESS, EVENTD_REG_FMT, this, cb, cookie);
	return 0;
}

eventd& procd()
{
	static eventd* e = new eventd(proc());
	return *e;
}