#ifndef MACHINE_H
#define MACHINE_H

#include <pal/stdtype.h>

struct machine_t {
	int64_t id;
	unistr name;
	unistr comment;
};

#endif
