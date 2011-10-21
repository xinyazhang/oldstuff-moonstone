#include "Database.h"
#include "filemgr.h"
#include "volmgr.h"
#include <dal/DatabaseInterface.h>

const unistr Database::table_name_postfix_[] =
{
	"tnode",
	"tag",
	"tagtag_relation",
	"fso",
	"ft"
};

Database::Database(DatabaseInterface* i)
	:db_(i), prefix_(i->prefix()), nest_(0)
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
	}
	nest_++;
	mutex_.unlock();
}

void Database::abort_transaction()
{
	mutex_.lock();
	nest_--;
	breaked_ = true;
	if ( nest_ == 0 )
		db_->abort_transaction();
	mutex_.unlock();
}

void Database::final_transaction()
{
	mutex_.lock();
	nest_--;
	if ( nest_ == 0 )
	{
		if ( breaked_ )
			db_->abort_transaction();
		else 
			db_->final_transaction();
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
