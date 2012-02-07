#include "singleton.h"
#include "eventd.h"
#include "feedback.h"

void cleanup_singletons()
{
	delete procd();
	delete proc();
	delete log();
}
