/*
 * note: creation is not sqlite_db's responsibility
 */

#ifndef DATABASE_ABSTRACT_LAYER_SQLITE_IMPLEMENTATION_H
#define DATABASE_ABSTRACT_LAYER_SQLITE_IMPLEMENTATION_H

#include "DatabaseInterface.h"
#include <vector>

class sqlite3;

class db_sqlite_impl
	:public DatabaseInterface
{
public:
	db_sqlite_impl(sqlite3* handle, const unistr& prefix);
	~db_sqlite_impl();
	sql_stmt_interface* create_stmt(const unistr& );
	unistr initialize_sqls() const;
	int initialize_sql_number() const;
	void complete_initialize();
private:
	sqlite3* handle_;

	void check_version(); 
	void build_sqls();
	std::vector<unistr> sqls_;
};
