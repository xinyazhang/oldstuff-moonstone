#include "SQLiteEasy.h"
#include "sqlite3.h"

void easyquery(sqlite3* db, const uchar* sql)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_u(db, sql, -1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void guarntee_query(sqlite3* db, const uchar* sql)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_u(db, sql, -1, &stmt, NULL);
	if ( SQLITE_DONE != sqlite3_step(stmt) )
	{
		throw("SQL execute error.");
	}
	sqlite3_finalize(stmt);
	//sqlite3_prepare_u(db, UT("INSERT INTO remus_tag (
}
