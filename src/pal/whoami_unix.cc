#include "whoami.h"
#include <unistd.h>

unistr whoami()
{
	unistr ret(getlogin());
	return ret;
}
