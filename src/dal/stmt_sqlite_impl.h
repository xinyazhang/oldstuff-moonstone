#ifndef DATABASE_ABSTRACT_LAYER_STATEMENT_SQLITE_IMPLEMENTATION_H
#define DATABASE_ABSTRACT_LAYER_STATEMENT_SQLITE_IMPLEMENTATION_H

#include "sql_stmt_interface.h"

class sqlite3_stmt;

class stmt_sqlite_impl
	:public sql_stmt_interface
{
public:
	stmt_sqlite_impl(sqlite3_stmt* ,sqlite3* );
	~stmt_sqlite_impl();
	virtual void bind(int c, idx_t );
	virtual void bind(int c, const unistr&);
	virtual int execute();
	virtual bool step();
	virtual void col(int c, unistr& );
	virtual void col(int c, idx_t& );
private:
	sqlite3_stmt* stmt_;
	sqlite3* db_;
};

#endif
