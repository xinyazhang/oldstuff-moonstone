#ifndef LS_CONF_H
#define LS_CONF_H

#include "../version.h"
#include "serialization.h"
void platform_depedent_init();
void platform_depedent_load(barc_i& );
void platform_depedent_save(barc_o& );

#endif
