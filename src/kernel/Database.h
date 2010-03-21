/*
 * Database class
 * note: 
 * 	this class is used as intermediate layer between real Database's interfaces 
 * 	and kernel operators
 */

#ifndef KERNEL_DATABASE_H
#define KERNEL_DATABASE_H

#include "common_declare.h"
class DatabaseInterface;

class Database
{
public:
	Database(DatabaseInterface* , const unistr& prefix);
	~Database();
public:
	void begin_transaction();
	void final_transaction();
	sql_stmt create_insert_stmt(TableSelector, int col_number);
	sql_stmt create_simsel_stmt(TableSelector, const unistr& locator, const unistr& selcontent);
	sql_stmt create_update_stmt(TableSelector, const unistr& locator, int updatecoln, const char* updatecols[]);
	sql_stmt create_del_stmt(TableSelector, int locatorn, const char* locators[]);
	sql_stmt create_stmt_ex(const unistr& );
	unistr table(TableSelector);

	TagMan* tagman();
	TnodeMan* tnodeman();

	enum TableSelector
	{
		TnodeTable = 0,
		TagTable = 1
	};
private:
	unistr prefix_;
	static const unistr table_name_postfix_[]; // it seems we must use this
	std::vector<unistr> table_name_;
	DatabaseInterface *db_;
	TagMan* tagman_;
	TnodeMan* tnodeman_;

	int nest_;
	boost::mutex mutex_;
};

#endif
