#include "singleton.h"
#include "eventd.h"
#include "feedback.h"
#include "threadpool.h"

void cleanup_singletons()
{
	delete globaltp();
	delete &procd();
	delete &proc();
	delete &log();
}
