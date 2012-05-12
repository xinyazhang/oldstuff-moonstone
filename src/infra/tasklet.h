#ifndef TASKLET_H
#define TASKLET_H

class EXPORT_TO_DLL tasklet_t 
{
public:
	tasklet_t(int evid):evid_(evid) {}
	virtual ~tasklet_t() {};

	int evid() { return evid_; }
	virtual int doit() = 0;
private:
	int evid_;
};

#include "evid.h"

#endif
