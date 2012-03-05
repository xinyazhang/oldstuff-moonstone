#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include "machine.h"

class access_point {
public:
	idx_t host;
	idx_t id;
	unistr url;
	unistr comment;

	bool valid() const;
	access_point();
};

#endif
