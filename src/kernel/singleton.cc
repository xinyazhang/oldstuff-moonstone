#include "singleton.h"
#include "eventd.h"
#include "feedback.h"
#include "threadpool.h"

void cleanup_singletons()
{
	delete devd();
	delete devfp();
	delete &procd();
	delete &proc();
	delete &log();
	delete globaltp();
}
