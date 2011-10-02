#include "../lsconf.h"
#include "logusn.h"

void platform_depedent_init()
{
	logusn_init();
}

void platform_depedent_load(barc_i& ar)
{
	logusn_load(ar);
}

void platform_depedent_save(barc_o& ar)
{
	logusn_save(ar);
}
