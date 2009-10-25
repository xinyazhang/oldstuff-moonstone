#ifndef _REMUS_DAL_SQLITE_
#define _REMUS_DAL_SQLITE_

#include "../Database.h"
#include <vector>

struct sqlite3_stmt;
struct sqlite3;

class SQLite
	:public Database
{
public:
	SQLite(const UniStr& filename);
	~SQLite();
	void initialize();

	SqlQueryRef create_query();
	int exec(SqlQueryRef);
	void close_query(SqlQueryRef& );
private:
	sqlite3* db_;
};

class SQLiteQuery
	:public SqlQuery
{
	friend class SQLite;
private:
	SQLiteQuery();
	SQLiteQuery(sqlite3* db);
	int exec();
public:
	virtual void set_operate(SqlOperate);
	virtual void app_table(const char*);
	
	virtual void app_target(const char*, const UniStr&); // select(or delete) Value from Table where PARA1=PARA2
	virtual void app_target(const char*, int64_t ); // select(or delete) Value from Table where PARA1=PARA2
	virtual void app_target(const char* ); // select max(TARGET) from Table;

	virtual void app_value(const char*); // select PARA1 from Table where Target, or Insert/Delete PARA1 into Table
	virtual void app_value(const UniStr&);
	virtual void app_value(int64_t );
	virtual void app_value(const char*, const UniStr&); // update set PARA1=PARA2 where Target
	virtual void app_value(const char*, int64_t);

	virtual void col(int idx, UniStr* );
	virtual void col(int idx, int64_t* );
	virtual void col(int idx, int* );

	UniStr sql() const;

	int rows() const;

private:
	SqlOperate op_;
	sqlite3_stmt* stmt_;
	sqlite3* db_;
	UniStr sql_;
	int rows_;

	std::vector<UniStr> table_, target_, value_;

private:
	UniStr escape(const UniStr&);
	void compose(UniStr&, const std::vector<UniStr>);

	void sql_select();
	void sql_remove();
	void sql_update();
	void sql_insert();
	void sql_max();
};

#endif
