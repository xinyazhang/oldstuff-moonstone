#include "stmt_sqlite_impl.h"
#include "sqlite3pal.h"
#include <stdio.h>

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

void stmt_sqlite_impl::bind(int c, uint64_t idx)
{
	sqlite3_bind_int64(stmt_, c, (int64_t)idx);
}

void stmt_sqlite_impl::bind(int c, const unistr& str)
{
	int ret = sqlite3_bind_text_native(stmt_, c, str.native(), -1, SQLITE_TRANSIENT);
}

void stmt_sqlite_impl::bind(int c, void* ptr, int64_t size)
{
	int ret = sqlite3_bind_blob(stmt_, c, ptr, size, NULL);
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
	else {
#ifndef NDEBUG
		fwprintf(stderr, L"ERROR CODE: %s\n", (wchar_t*)sqlite3_errmsg16(db_));
#endif
		return sqlite3_errcode(db_);
	}
}

bool stmt_sqlite_impl::step()
{
	int ret = sqlite3_step(stmt_);
#ifndef NDEBUG
	if (SQLITE_ROW != ret)
		fwprintf(stderr, L"ERROR CODE: %s\n", (wchar_t*)sqlite3_errmsg16(db_));
#endif
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

void stmt_sqlite_impl::col(int c, uint64_t& value)
{
	value = (uint64_t)sqlite3_column_int64(stmt_, c - 1);
}