/*
 * note: creation is not sqlite_db's responsibility
 */

#ifndef DATABASE_ABSTRACT_LAYER_SQLITE_IMPLEMENTATION_H
#define DATABASE_ABSTRACT_LAYER_SQLITE_IMPLEMENTATION_H

#include "DatabaseInterface.h"
#include <vector>

struct sqlite3;

class db_sqlite_impl
	:public DatabaseInterface
{
public:
	db_sqlite_impl(const unistr& filename, const unistr& prefix);
	~db_sqlite_impl();
	bool connect();

	virtual void begin_transaction();
	virtual void final_transaction();
	virtual void abort_transaction();

	sql_stmt_interface* create_stmt(const unistr& );
	unistr initialize_sqls(int ) const;
	int initialize_sql_number() const;
	void complete_initialize();
	idx_t last_insert();
private:
	unistr filename_;
	sqlite3* handle_;

	void check_version(); 
	void build_sqls();
	unistr_list sqls_;
};

#endif
