#ifndef _REMUS_DAL_SQLITE_TAGITER_
#define _REMUS_DAL_SQLITE_TAGITER_

#include "DataBase.h"
#include "DomainImpl.h"

class SQLite;
struct sqlite3_stmt;

class SQLiteTagIterator
	:public DataBase::TagIterator
{
	friend class SQLite;
private:
	SQLiteTagIterator(SQLite*, SQLiteRefCounter<SQLiteTag>* tagrc);
	~SQLiteTagIterator(); // remember to call finalize

	TagRef read_tag();
	SQLite* db_;
	SQLiteRefCounter<SQLiteTag>* tagrc_;
	sqlite3_stmt* stmt_;
};

#endif
