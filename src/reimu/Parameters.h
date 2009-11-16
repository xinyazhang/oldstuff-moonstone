#ifndef _KERNEL_PARAMETERS_
#define _KERNEL_PARAMETERS_

#include "Declares.h"
#include <vector>

class Parameters
{
protected:
	Parameters();
	~Parameters();
	
	void push(const UniStr& col, const UniStr& val = UniStr());
	virtual void assembly(SqlQueryRef ) const;

	std::vector<UniStr> condi_;
	std::vector<UniStr> value_;
};

#endif
