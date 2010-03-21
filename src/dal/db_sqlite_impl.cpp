#include "db_sqlite_impl.h"
#include "sqlite3pal.h"
#include <boost/algorithm/string/replace.hpp>

db_sqlite_impl::db_sqlite_impl(sqlite3* handle, const unistr& prefix)
	:handle_(handle), DatabaseInterface(prefix)
{
	check_version();
}

db_sqlite_impl::~db_sqlite_impl()
{
	sqlite3_close(handle_);
}

sql_stmt_interface* create_stmt(const unistr& sql)
{
	sqlite3_stmt *stmt;
	if ( SQLITE_OK != sqlite3_prepare_native(handle_, sql.native(), -1, &stmt, NULL) )
	{
		return new stmt_sqlite_impl(NULL, NULL);
	} else
		return new stmt_sqlite_impl(stmt, handle_);
}

unistr db_sqlite_impl::initialize_sqls(int i) const
{
	return sqls_[i];
}

int db_sqlite_impl::initialize_sql_number() const
{
	return 3; // magical number, but it doesn't matter
}

void db_sqlite_impl::check_version() // would modify initialized_
{
	unistr sql(UT("SELECT VERSION FROM $1META"));
	boost::replace_first(sql, UT("$1"), prefix_);
	boost::shared_ptr stmt_ptr(create_stmt(sql));
	if ( stmt_ptr.get() )
	{
		if ( stmt_ptr.step() )
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
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tnode(idx INTEGER PRIMARY KEY ASC AUTOINCREMENT, mastername TEXT, comment TEXT);"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tag(name TEXT, tnode INTEGER, PRIMARY KEY(name, tnode));")
};

void db_sqlite_impl::build_sqls()
{
	for(int i = 0; i < sizeof(sql_template)/sizeof(unistr); i++)
	{
		unistr str(sql_template[i]);
		boost::replace_first(str, UT("$PREFIX_"), prefix);
		sqls.push_back(str);
	}
}

void db_sqlite_impl::complete_initialize()
{
	sqls_.clear(); // release memory
}
