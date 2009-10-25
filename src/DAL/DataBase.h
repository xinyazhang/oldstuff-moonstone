#ifndef _REMUS_DAL_DATABASE_
#define _REMUS_DAL_DATABASE_

#include "../core/UniStr.h"
#include <boost/shared_ptr.hpp>

class SqlQuery;
typedef boost::shared_ptr<SqlQuery> SqlQueryRef;

class Database
{
public:
	struct DBConnFalied {};
public:
	Database();
	virtual ~Database();

	virtual SqlQueryRef create_query() = 0;
	virtual int exec(SqlQueryRef ) = 0;
	virtual void close_query(SqlQueryRef&) = 0;
};

class SqlQuery
{
	friend class Database;
pubcli:
	struct SqlOperateSetFailed {};
	enum SqlOperate
	{
		unknown,
		search,
		remove,
		update,
		insert,
		query_max
	};
private:
	virtual int exec() = 0;
public:
	virtual void set_operate(SqlOperate) = 0;
	virtual void app_table(const char*) = 0;
	
	virtual void app_target(const char*, const UniStr&) = 0; // select(or delete) Value from Table where PARA1=PARA2
	virtual void app_target(const char*, int64_t ) = 0; // select(or delete) Value from Table where PARA1=PARA2
	virtual void app_target(const char* ) = 0; // select max(TARGET) from Table;

	virtual void app_value(const char*) = 0; // select PARA1 from Table where Target, or Insert/Delete PARA1 into Table
	virtual void app_value(const UniStr&) = 0;
	virtual void app_value(int64_t ) = 0;
	virtual void app_value(const char*, const UniStr&) = 0; // update set PARA1=PARA2 where Target
	virtual void app_value(const char*, int64_t) = 0;

	virtual void col(int idx, UniStr* ) = 0;
	virtual void col(int idx, int64_t* ) = 0;
	virtual void col(int idx, int* ) = 0;

	virtual void next_row() = 0;

	UniStr sql() const = 0;

	int rows() const = 0;
};

#endif
