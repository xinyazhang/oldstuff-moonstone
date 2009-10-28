#ifndef _KERNEL_INDEXED_OBJECT_
#define _KERNEL_INDEXED_OBJECT_

#include "../core/int_type.h"
#include "../core/UniStr.h"
#include "Declares.h"

class IndexedObject
{
protected:
	IndexedObject(index_t idx, Category* cat, Database* db)
		:idx_(idx), db_(db), cat_(cat){}
	virtual ~IndexedObject() {};
public:
	index_t idx() { return idx_; }
protected:
	int64_t idx_;
	Database* db_;
	Category* cat_;
};


#endif
