#include "stmt_sqlite_impl.h"
#include "sqlite3pal.h"

stmt_sqlite_impl::stmt_sqlite_impl(sqlite3_stmt* stmt, sqlite3* db)
	:stmt_(stmt), db_(db)
{
}

stmt_sqlite_impl::~stmt_sqlite_impl()
{
	sqlite3_finalize(stmt_);
}

void stmt_sqlite_impl::bind(int c)
{
	sqlite3_bind_null(stmt_, c);
}

void stmt_sqlite_impl::bind(int c, idx_t idx)
{
	sqlite3_bind_idx(stmt_, c, idx);
}

void stmt_sqlite_impl::bind(int c, const unistr& str)
{
	int ret = sqlite3_bind_text_native(stmt_, c, str.native(), -1, SQLITE_TRANSIENT);
}

int stmt_sqlite_impl::execute()
{
	int ret = sqlite3_step(stmt_);
	while (SQLITE_DONE != ret && SQLITE_ROW == ret)
	{
		ret = sqlite3_step(stmt_);
	}
	if ( ret == SQLITE_DONE )
		return 0;
	else
		return sqlite3_errcode(db_);
}

bool stmt_sqlite_impl::step()
{
	int ret = sqlite3_step(stmt_);
	return SQLITE_ROW == ret;
}

void stmt_sqlite_impl::col(int c, unistr& value)
{
	value = (unichar*)sqlite3_column_native(stmt_, c - 1);
}

void stmt_sqlite_impl::col(int c, idx_t& value)
{
	value = sqlite3_column_idx(stmt_, c - 1);
}
