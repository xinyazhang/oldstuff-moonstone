#ifndef _REMUS_DAL_SQLITE_TAGITER_
#define _REMUS_DAL_SQLITE_TAGITER_

#include "DataBase.h"

class SQLite;
struct sqlite3_stmt;

class SQLiteTagIterator
	:public DataBase::TagIterator
{
	friend class SQLite;
private:
	SQLiteTagIterator(SQLite* );
	~SQLiteTagIterator();

	sqlite3_stmt* stmt_;
};

#endif
