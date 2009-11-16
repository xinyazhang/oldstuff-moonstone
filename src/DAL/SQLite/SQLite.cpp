#include "SQLite.h"
#include "sqlite3.h"
#include "../../core/unicode.h"
#include "SQL.h"
#include "SQLiteEasy.h"

SQLite::SQLite(const UniStr& filename)
{
	int ret = sqlite3_open_u(filename.c_str(), &db_);
	if ( ret != SQLITE_OK )
		throw DBConnFalied();
}

SQLite::~SQLite()
{
	sqlite3_close(db_);
}

void SQLite::initialize()
{
	easyquery(db_, sql_unicode);
	easyquery(db_, sql_create_tag_table);
	easyquery(db_, sql_initial_tag_table);
	easyquery(db_, sql_create_family);
	easyquery(db_, sql_initial_family);
}

SqlQueryRef SQLite::create_query()
{
	SqlQueryRef ref(new SQLiteQuery(db_));
	return ref;
}

/*
int SQLite::exec(SqlQueryRef ref)
{
	ref->call_exec();
}
*/

void SQLite::close_query(SqlQueryRef& ref)
{
	ref.reset();
}

SQLiteQuery::SQLiteQuery(sqlite3* db)
	:db_(db), op_(unknown), stmt_(0), done(false)
{
}

int SQLiteQuery::exec()
{
	sqlite3_prepare_u(db_, sql().c_str(), -1, &stmt_, NULL);
	int ret = sqlite3_step(stmt_);
	rows_ = sqlite3_column_count(stmt_);
	check_done(ret);
	return ret;
}

SQLiteQuery::~SQLiteQuery()
{
	if ( stmt_ )
		sqlite3_finalize(stmt_);
}

void SQLiteQuery::set_operate(SqlOperate op)
{
	if ( op == unknown )
		throw SqlOperateSetFailed();
	op_ = op;
}

void SQLiteQuery::reset_query()
{
	if ( stmt_ )
		sqlite3_reset(stmt_);
	table_.clear();
	target_.clear();
	value_.clear();
	rows_ = 0;
	done = false;
}

void SQLiteQuery::app_table(const char* table)
{
	table_.push_back(UniStr(table));
}

void SQLiteQuery::app_target(const char* para1, const UniStr& para2)
{
	target_.push_back(UniStr(para1)+UT("=")+escape(para2));
}

void SQLiteQuery::app_target(const UniStr& para1, const UniStr& para2)
{
	target_.push_back(para1+UT("=")+escape(para2));
}

void SQLiteQuery::app_target(const char* para1, int64_t value)
{
	target_.push_back(UniStr(para1)+UT("=")+UniStr(value));
}

void SQLiteQuery::app_target(const char* para)
{
	target_.push_back(UT("MAX(")+UniStr(para)+UT(")"));
}

void SQLiteQuery::app_target(const UniStr& para)
{
	target_.push_back(para);
}

void SQLiteQuery::app_value(const char* para)
{
	value_.push_back(UniStr(para));
}

void SQLiteQuery::app_value(const UniStr& para)
{
	value_.push_back(para);
}

void SQLiteQuery::app_value(int64_t para)
{
	value_.push_back(UniStr(para));
}

void SQLiteQuery::app_value(const char* para1, const UniStr& para2)
{
	target_.push_back(UniStr(para1)+UT("=")+escape(para2));
}

void SQLiteQuery::app_value(const char* para1, int64_t para2)
{
	target_.push_back(UniStr(para1)+UT("=")+UniStr(para2));
}

void SQLiteQuery::col(int idx, UniStr* val)
{
	*val = UniStr((const wchar_t*)sqlite3_column_text16(stmt_, idx));
}

void SQLiteQuery::col(int idx, int64_t* val)
{
	*val = sqlite3_column_int64(stmt_, idx);
}

void SQLiteQuery::col(int idx, int* val)
{
	*val = sqlite3_column_int(stmt_, idx);
}

void SQLiteQuery::next_row()
{
	check_done(sqlite3_step(stmt_));
}

UniStr SQLiteQuery::sql() const
{
	switch (op_)
	{
		case search:
			return sql_select();
			break;
		case remove:
			return sql_remove();
			break;
		case update:
			return sql_update();
			break;
		case insert:
			return sql_insert();
			break;
		case query_max:
			return sql_max();
		default:
			return UniStr();
			break;
	}
}

int SQLiteQuery::rows() const
{
	return rows_;
}

UniStr SQLiteQuery::escape(const UniStr& org)
{
	UniStr ret;
	for ( int i = 0; i < org.size(); i++ )
	{
		if ( org[i] == UT('\'') )
			ret += UT("''");
		else
			ret += org[i];
	}
	return ret;
}

void SQLiteQuery::compose(UniStr& str, const std::vector<UniStr> pairs) const
{
	str += UT(' ');

	for( std::vector<UniStr>::const_iterator iter = pairs.begin();
			(iter+1) != pairs.end();
			iter++)
	{
		str += *iter;
		str += UT(',');
	}
	
	if ( !pairs.empty() )
		str += pairs.back();

	str += UT(' ');
}

UniStr SQLiteQuery::sql_select() const
{
	UniStr ret("SELECT");
	compose(ret, value_);
	if ( value_.empty() )
		ret += UT("*");
	ret += UT("FROM");
	compose(ret, table_);
	if ( !target_.empty() )
	{
		ret += UT("WHERE");
		compose(ret, target_);
	}
	
	return ret;
}

UniStr SQLiteQuery::sql_remove() const
{
	UniStr ret("DELETE FROM");
	compose(ret, table_);
	ret += UT("WHERE");
	compose(ret, target_);
	
	return ret;
}

UniStr SQLiteQuery::sql_update() const
{
	UniStr ret("UPDATE");
	compose(ret, table_);
	ret += UT("SET");
	compose(ret, value_);
	ret += UT("WHERE");
	compose(ret, target_);

	return ret;
}

UniStr SQLiteQuery::sql_insert() const
{
	UniStr ret("INSERT");
	compose(ret, value_);
	ret += UT("INTO");
	compose(ret, target_);

	return ret;
}

UniStr SQLiteQuery::sql_max() const
{
	UniStr ret("SELECT MAX(");
	compose(ret, target_);
	ret += UT(") FROM");
	compose(ret, table_);

	return ret;
}
