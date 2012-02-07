#include "Database.h"
#include "filemgr.h"
#include "volmgr.h"
#include <kernel/dal/DatabaseInterface.h>
#include "sql_stmt.h"
#include "feedback.h"

Database::Database(DatabaseInterface* i)
	:db_(i), nest_(0)
{
	filemgr_ = new filemgr_t(this);
	volmgr_ = new volmgr_t(this);
}

Database::~Database()
{
	delete volmgr_;
	delete filemgr_;
}

bool Database::initialized() const
{
	return db_->initialized();
}

bool Database::initialize()
{
	db_->begin_transaction();
	int sqln = db_->initialize_sql_number();
	for(int i = 0; i < sqln; i++)
	{
		sql_stmt stmt = db_->create_stmt(db_->initialize_sqls(i));
		if ( stmt.execute() != 0 )
		{
			db_->abort_transaction();
			return false;
		}
	}
	db_->final_transaction();
	return true;
}

void Database::begin_transaction()
{
	mutex_.lock();
	if ( nest_ == 0 )
	{
		breaked_ = false;
		db_->begin_transaction(); // default: blocked if have another transaction...
		log().printf(LOG_DEBUG, UT("Begin transaction\n"));
	}
	nest_++;
	mutex_.unlock();
}

void Database::abort_transaction()
{
	mutex_.lock();
	nest_--;
	breaked_ = true;
	if ( nest_ == 0 ) {
		db_->abort_transaction();
		log().printf(LOG_DEBUG, UT("Abort transaction\n"));
	}
	mutex_.unlock();
}

void Database::final_transaction()
{
	mutex_.lock();
	nest_--;
	if ( nest_ == 0 )
	{
		if ( breaked_ ) {
			db_->abort_transaction();
			log().printf(LOG_DEBUG, UT("Abort transaction\n"));
		} else {
			db_->final_transaction();
			log().printf(LOG_DEBUG, UT("Final transaction\n"));
		}
	}
	mutex_.unlock();
}

sql_stmt Database::create_stmt_ex(const unistr& sql)
{
	return sql_stmt(db_->create_stmt(sql));
}

filemgr_t* Database::filemgr()
{
	return filemgr_;
}

volmgr_t* Database::volmgr()
{
	return volmgr_;
}
