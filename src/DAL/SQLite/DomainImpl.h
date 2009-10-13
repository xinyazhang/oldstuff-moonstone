#ifndef _REMUS_DAL_SQLITE_DOMAIN_IMPL_
#define _REMUS_DAL_SQLITE_DOMAIN_IMPL_

#include "../domain.h"

class SQLiteRefCounter;

class SQLiteTag
{
	friend class SQLiteTagDeleter;
public:
	SQLiteTag(SQLiteRefCounter* rc);
	typedef SQLiteTagDeleter Deleter;
private:
	SQLiteRefCounter* rc_;
};

struct SQLiteTagDeleter
{
	void operator(SQLiteTag* p);
};

#endif
