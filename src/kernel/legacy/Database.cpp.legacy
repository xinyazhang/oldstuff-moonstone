#include "Database.h"
#include "common.h"
#include "../dal/DatabaseInterface.h"
//#include <boost/static_assert.hpp>

#define TABLE_NUMBER 5

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
	tagman_ = new TagMan(this);
	tnodeman_ = new TnodeMan(this);
	relman_ = new RelationMan(this);
	fsodbman_ = new FsodbMan(this);
	ss_ = new snapshotter(this);
	ftman_ = new ftman_t(this);

	for(int i = 0; i < TABLE_NUMBER; i++)
		table_name_.push_back(prefix_ + table_name_postfix_[i]);
}

Database::~Database()
{
	delete ftman_;
	delete ss_;
	delete tagman_;
	delete tnodeman_;
	delete relman_;
	delete fsodbman_;
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
	UT("(name, tnode)"),
	UT("(tagger, taggee)"),
	UT("(parentid, name, size, fs_date, recusive_date, hash_algo, hash)"),
	UT("(fsoid, tnode)")
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

sql_stmt Database::create_selall_stmt(TableSelector s, int locatorn, const char* locators[])
{
	unistr sql("SELECT * FROM ");
	sql += table(s);
	sql += " WHERE ";
	for(int i = 0; i < locatorn - 1; i++)
	{
		sql += locators[i];
		sql += "=$";
		sql += unistr::number(i+1);
		sql += " AND ";
	}
	sql += locators[locatorn - 1];
	sql += "=$";
	sql += unistr::number(locatorn);

	return create_stmt_ex(sql);
}

sql_stmt Database::create_list_stmt(TableSelector s, 
		int restrictn, const char* restrictions[],
		int ordern, const char* order_cols[], const bool asc[])
{
	unistr sql("SELECT * FROM ");
	sql += table(s);
	sql += " WHERE ";
	for(int i = 0; i < restrictn - 1; i++)
	{
		sql += restrictions[i];
		sql += " $";
		sql += unistr::number(i + 1);
		sql += " AND ";
	}
	sql += restrictions[restrictn - 1];
	sql += " $";
	sql += unistr::number(restrictn);

	sql += " ORDER BY ";
	for(int i = 0; i < ordern - 1; i++)
	{
		sql += order_cols[i];
		if ( asc[i] )
			sql += " ASC, ";
		else
			sql += " DESC, ";
	}
	sql += order_cols[ordern - 1];
	if ( asc[ordern - 1] )
		sql += " ASC";
	else
		sql += " DESC";

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
		sql += " AND ";
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

RelationMan* Database::relman()
{
	return relman_;
}

FsodbMan* Database::fsodbman()
{
	return fsodbman_;
}

snapshotter* Database::ss()
{
	return ss_;
}

ftman_t* Database::ftman()
{
	return ftman_;
}

idx_t Database::last_serial()
{
	return db_->last_insert();
}
