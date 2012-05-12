#ifndef MACHINE_H
#define MACHINE_H

#include <pal/stdtype.h>

class EXPORT_TO_DLL machine_t {
public:
	int64_t id;
	unistr name;
	unistr comment;
	
	bool valid() const;
	machine_t();
};

#endif
