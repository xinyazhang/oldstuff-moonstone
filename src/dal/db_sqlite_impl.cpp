#include "db_sqlite_impl.h"
#include "stmt_sqlite_impl.h"
#include "sqlite3pal.h"
#include <boost/algorithm/string/replace.hpp>

#ifndef NDEBUG
#include <stdio.h>
#endif

db_sqlite_impl::db_sqlite_impl(const unistr& filename, const unistr& prefix)
	:filename_(filename), DatabaseInterface(prefix)
{
}

void db_sqlite_impl::begin_transaction()
{
	DatabaseInterface::begin_transaction();
}

void db_sqlite_impl::final_transaction()
{
	DatabaseInterface::final_transaction();
}

void db_sqlite_impl::abort_transaction()
{
	DatabaseInterface::final_transaction();
}

db_sqlite_impl::~db_sqlite_impl()
{
	sqlite3_close(handle_);
}

bool db_sqlite_impl::connect()
{
	bool ret = SQLITE_OK == sqlite3_open_native(filename_.native(), &handle_);
	if ( ret )
		check_version();
	return ret;
}

sql_stmt_interface* db_sqlite_impl::create_stmt(const unistr& sqlraw)
{
	sqlite3_stmt *stmt;
	unistr sql = sqlraw;
	boost::replace_all(sql, UT("$"), UT("?"));
#ifndef NDEBUG
	wprintf(L"SQL: %s\n", sql.native());
#endif
	int ret = sqlite3_prepare_native(handle_, sql.native(), sql.size() * sizeof(unichar), &stmt, NULL);
	if ( SQLITE_OK != ret )
	{
		return new stmt_sqlite_impl(NULL, NULL);
	} else
		return new stmt_sqlite_impl(stmt, handle_);
}

unistr db_sqlite_impl::initialize_sqls(int i) const
{
	return sqls_[i];
}

void db_sqlite_impl::check_version() // would modify initialized_
{
	unistr sql(UT("SELECT VERSION FROM $1META;"));
	// ugly patch
	//std::wstring tmp = sql.toStdWString();
	boost::replace_first(sql, UT("$1"), prefix_);
	//sql = QString::fromStdWString(tmp);
	boost::shared_ptr<sql_stmt_interface> stmt_ptr(create_stmt(sql));
	if ( stmt_ptr.get() )
	{
		if ( stmt_ptr->step() )
		{
			initialized_ = true;
		}
	}
	if ( !initialized_ )
	{
		build_sqls();
	}
}

static const unistr sql_template[] =
{
	UT("PRAGMA encoding = \"UTF-16\";"), 
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_META(version INTEGER);"),
	UT("INSERT INTO $PREFIX_META VALUES(1);"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tnode(idx INTEGER PRIMARY KEY ASC AUTOINCREMENT, refc INTEGER, mastername TEXT, comment TEXT);"),
	UT("INSERT INTO $PREFIX_tnode VALUES(0, 0, '', '');"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tag(name TEXT, tnode INTEGER, PRIMARY KEY(name, tnode));")
};

int db_sqlite_impl::initialize_sql_number() const
{
	return 6; // magical number, but it doesn't matter
}

void db_sqlite_impl::build_sqls()
{
	for(int i = 0; i < sizeof(sql_template)/sizeof(unistr); i++)
	{
		unistr str(sql_template[i]);
		boost::replace_first(str, UT("$PREFIX_"), prefix_);
		sqls_.push_back(str);
	}
}

void db_sqlite_impl::complete_initialize()
{
	sqls_.clear(); // release memory
}

idx_t db_sqlite_impl::last_insert()
{
	return sqlite3_last_insert_rowid(handle_);
}
