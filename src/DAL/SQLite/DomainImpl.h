#ifndef _REMUS_DAL_SQLITE_DOMAIN_IMPL_
#define _REMUS_DAL_SQLITE_DOMAIN_IMPL_

#include "../domain.h"

template<typename T>
class SQLiteRefCounter;

class SQLiteTag
	:public Tag
{
	friend class SQLiteTagDeleter;
public:
	SQLiteTag(SQLiteRefCounter<SQLiteTag>* rc);
	typedef SQLiteTagDeleter Deleter;
private:
	SQLiteRefCounter<SQLiteTag>* rc_;
};

class SQLiteTagDeleter
{
public:
	void operator()(SQLiteTag* p);
};

#endif
