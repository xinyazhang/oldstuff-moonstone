#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <vector>
#include <pal/stdtype.h>

class notifier
{
public:
	void register_callback(cb_func_t, void* cookie);
	void broadcast(int type, void* data = NULL);
private:
	struct fnc
	{
		cb_func_t cb_func;
		void* cb_cookie;
	};
	std::vector<fnc> registered_;
};

#endif
