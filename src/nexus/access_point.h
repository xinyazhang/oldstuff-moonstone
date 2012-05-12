#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include "machine.h"

class EXPORT_TO_DLL access_point {
public:
	machine_t* phost;
	idx_t host;
	idx_t id;
	unistr url;
	unistr comment;

	bool valid() const;
	bool completed() const;
	access_point(machine_t* );
private:
	access_point();
};

#endif
