#ifndef _REMUS_DAL_SQLITE_TAGITER_
#define _REMUS_DAL_SQLITE_TAGITER_

#include "../DAL/DataBase.h"
#include "Declares.h"

class TagIterator
{
public:
	TagIterator(Category* cat, Database* db);
	TagRef read();
	bool done();
private:
	SqlQueryRef query;
	Category* cat_;
	DataBase* db_;
};

#endif
