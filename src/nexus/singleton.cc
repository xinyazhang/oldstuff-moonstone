#include "singleton.h"
#include "eventd.h"
#include "feedback.h"
#include "threadpool.h"

void cleanup_singletons()
{
#if 0 // suppress temporary, because it is delayed.
	delete &devd();
	delete &devfp();
#endif
	delete &procd();
	delete &proc();
	delete &log();
	delete globaltp();
}
