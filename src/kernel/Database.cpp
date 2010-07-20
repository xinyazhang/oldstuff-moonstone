#include "Database.h"
#include "common.h"
#include "../dal/DatabaseInterface.h"
//#include <boost/static_assert.hpp>

#define TABLE_NUMBER 3

const unistr Database::table_name_postfix_[] =
{
	"tnode",
	"tag",
	"tagtag_relation"
};

Database::Database(DatabaseInterface* i)
	:db_(i), prefix_(i->prefix()), nest_(0)
{
	tagman_ = new TagMan(this);
	tnodeman_ = new TnodeMan(this);
	relman_ = new RelationMan(this);

	for(int i = 0; i < TABLE_NUMBER; i++)
		table_name_.push_back(prefix_ + table_name_postfix_[i]);
}

Database::~Database()
{
	delete tagman_;
	delete tnodeman_;
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

static unistr insert_content[] =
{
	UT("(refc, mastername, comment)"),
	UT("(name, tnode)")
};

sql_stmt Database::create_insert_stmt(TableSelector ts, int col_number)
{
	unistr sql("insert into ");
	sql += table(ts);
	sql += insert_content[ts];
	sql += " values($1";
	for(int i = 2; i <= col_number; i++)
	{
		sql += ", $";
		sql += unistr::number(i);
	}
	sql += ");";
	return create_stmt_ex(sql);
}

sql_stmt Database::create_simsel_stmt(TableSelector s, const unistr& locator, const unistr& selcontent)
{
	unistr sql("SELECT ");
	sql += selcontent;
	sql += " FROM ";
	sql += table(s);
	sql += " WHERE ";
	sql += locator;
	sql += "=$1;";
	return create_stmt_ex(sql);
}

sql_stmt Database::create_update_stmt(TableSelector s, const unistr& locator, int updatecoln, const char* updatecols[])
{
	unistr sql("UPDATE ");
	sql += table(s);
	sql += " SET ";
	sql += updatecols[0];
	sql += "=$2";
	for(int i = 1; i < updatecoln; i++)
	{
		sql += ", ";
		sql += updatecols[i];
		sql += "=$";
		sql += unistr::number(i+2);
	}
	sql += " WHERE ";
	sql += locator;
	sql += "=$1;";

	return create_stmt_ex(sql);
}

sql_stmt Database::create_del_stmt(TableSelector s, int locatorn, const char* locators[])
{
	unistr sql("DELETE FROM ");
	sql += table(s);
	sql += " WHERE ";
	sql += locators[0];
	sql += "=$1";
	for(int i = 2; i <= locatorn; i++)
	{
		sql += ",";
		sql += locators[i-1];
		sql += "=$";
		sql += unistr::number(i);
	}
	sql += ";";

	return create_stmt_ex(sql);
}

sql_stmt Database::create_stmt_ex(const unistr& sql)
{
	return sql_stmt(db_->create_stmt(sql));
}

unistr Database::table(TableSelector selector)
{
	return table_name_[selector];
}

TagMan* Database::tagman()
{
	return tagman_;
}

TnodeMan* Database::tnodeman()
{
	return tnodeman_;
}

idx_t Database::last_serial()
{
	return db_->last_insert();
}
