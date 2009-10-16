#include "TagIterator.h"
#include "sqlite3.h"
#include "../../core/UniStr.h"
#include "DataBase.h"
#include "SQLiteEasy.h"

SQLiteTagIterator::SQLiteTagIterator(SQLite* db, SQLiteRefCounter<SQLiteTag>* tagrc)
	:db_(db), tagrc_(tagrc)
{
	sqlite3_prepare_u(db_->db_, UT("SELECT * FROM remus_tag WHERE idx > 0;"), -1, &stmt_, NULL);
}

SQLiteTagIterator::~SQLiteTagIterator() // remember to call finalize
{
	sqlite3_finalize(stmt_);
}

TagRef SQLiteTagIterator::read_tag()
{
	int result = sqlite3_step(stmt_);
	if ( result == SQLITE_ROW )
	{
		TagRef ret = tagrc_->create(split_idx(stmt_));
		return ret;
	} else if ( result == SQLITE_DONE )
		return TagRef();
}
