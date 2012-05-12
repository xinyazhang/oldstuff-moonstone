#include "notifier.h"

void notifier::register_callback(cb_func_t func, void* cookie)
{
	fnc f = {func, cookie};
	registered_.push_back(f);
}

void notifier::broadcast(int type, void* data)
{
	for(std::vector<fnc>::iterator iter = registered_.begin();
			iter != registered_.end();
			iter++)
		(*(iter->cb_func))(iter->cb_cookie, type, data);
}
