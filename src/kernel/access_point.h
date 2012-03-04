#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include "machine.h"

struct access_point {
	idx_t host;
	idx_t id;
	unistr protocol;
	int port;
	unistr auth;
	unistr root;
	unistr url;
};

#endif
